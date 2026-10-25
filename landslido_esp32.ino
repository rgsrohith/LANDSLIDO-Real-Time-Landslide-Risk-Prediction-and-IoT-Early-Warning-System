/*
  LANDSLIDO ESP32 warning unit  (student prototype, not a validated public warning service)

  Libraries (Arduino Library Manager):
    - ArduinoJson by Benoit Blanchon, version 7.x
    - LiquidCrystal I2C by Frank de Brabander
  Board: "ESP32 Dev Module" (esp32 board package by Espressif)

  Backend contract this firmware expects (the backend must implement it):
    GET  /api/esp32/poll          header X-Device-Token
         -> {"prediction": null | {"prediction_id","risk_level","location","timestamp","message","mode":"PREDICTION"},
             "test":       null | {"test_id","target":"red|yellow|green|buzzer|lcd","mode":"TEST"}}
    POST /api/esp32/heartbeat     {"device_id","status":"ONLINE"}
    POST /api/esp32/acknowledge   {"device_id","command_id","kind":"PREDICTION|TEST","status":"RECEIVED"}
  RECEIVED means this firmware processed the command in software. It does not prove the
  LED, LCD or buzzer physically worked.
*/
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ======================= CONFIGURATION (edit this block) =======================
const char* WIFI_SSID     = "YOUR_WIFI_NAME";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";
const char* SERVER_URL    = "http://192.168.1.50:3000";   // laptop LAN IP printed by START_WEBSITE.bat, plus port
const char* DEVICE_TOKEN  = "CHANGE_ME_SAME_AS_BACKEND";   // must equal the backend's device token
const char* DEVICE_ID     = "landslido-esp32-01";

// LCD (I2C). Common addresses: 0x27 or 0x3F.
const uint8_t LCD_ADDR = 0x27;
const uint8_t LCD_COLS = 16;
const uint8_t LCD_ROWS = 2;
const int PIN_SDA = 21;           // example wiring, change to match yours
const int PIN_SCL = 22;

// Outputs (example pins, safe general-purpose GPIOs)
const int PIN_LED_RED    = 25;
const int PIN_LED_YELLOW = 26;
const int PIN_LED_GREEN  = 27;
const int PIN_BUZZER     = 14;    // drive an active buzzer through an NPN transistor, see wiring notes
// ===============================================================================

const uint32_t POLL_MS      = 2500;    // poll every 2.5 s
const uint32_t HEARTBEAT_MS = 7000;    // heartbeat every 7 s
const uint32_t LOST_MS      = 15000;   // no good poll for 15 s -> CONNECTION LOST
const uint32_t TEST_MS      = 4000;    // a manual test runs for 4 s, then ends by itself
const uint32_t WIFI_RETRY_MS = 6000;
const uint16_t HTTP_TIMEOUT_MS = 2000; // the only blocking part: one request waits at most this long

// Buzzer patterns: alternating ON, OFF durations in ms, starting with ON.
const uint16_t PAT_HIGH[] = {150, 150};                  // rapid, continuous
const uint16_t PAT_MOD[]  = {250, 1250};                 // intermittent
const uint16_t PAT_LOW[]  = {80, 9920};                  // brief, occasional
const uint16_t PAT_LOST[] = {120, 120, 120, 1640};       // double chirp, distinct from risk levels
const uint16_t PAT_TEST[] = {400, 400};

enum Risk  { R_NONE, R_LOW, R_MOD, R_HIGH, R_INVALID };
enum Mode  { M_BOOT, M_NO_PRED, M_LOW, M_MOD, M_HIGH, M_INVALID, M_LOST, M_TEST };

LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);

Risk     curRisk = R_NONE;
String   curLocation = "";
String   lastPredId = "";        // prediction already processed
String   lastTestId = "";        // test already processed
String   pendingAckId = "", pendingAckKind = "";
bool     everPolled = false;
uint32_t bootMs = 0, lastGoodPoll = 0, lastPoll = 0, lastBeat = 0, lastWifiTry = 0;
bool     testActive = false;
String   testTarget = "";
uint32_t testEnd = 0;
Mode     shownMode = M_BOOT;
String   shownL1 = "", shownL2 = "";

// ---------- helpers ----------
bool patternOn(const uint16_t* p, uint8_t n, uint32_t now) {
  if (!p) return false;
  uint32_t total = 0;
  for (uint8_t i = 0; i < n; i++) total += p[i];
  uint32_t t = now % total;
  for (uint8_t i = 0; i < n; i++) {
    if (t < p[i]) return i % 2 == 0;
    t -= p[i];
  }
  return false;
}

void setLeds(bool r, bool y, bool g) {
  digitalWrite(PIN_LED_RED, r);
  digitalWrite(PIN_LED_YELLOW, y);
  digitalWrite(PIN_LED_GREEN, g);
}

void showLcd(const String& a, const String& b) {
  if (a == shownL1 && b == shownL2) return;     // only redraw on change, avoids flicker
  shownL1 = a; shownL2 = b;
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print(a.substring(0, LCD_COLS));
  if (LCD_ROWS > 1) { lcd.setCursor(0, 1); lcd.print(b.substring(0, LCD_COLS)); }
}

Risk parseRisk(const char* s) {
  if (!s) return R_INVALID;
  if (!strcmp(s, "HIGH")) return R_HIGH;
  if (!strcmp(s, "MODERATE")) return R_MOD;
  if (!strcmp(s, "LOW")) return R_LOW;
  return R_INVALID;            // anything else is never treated as LOW
}

// ---------- network ----------
void wifiMaintain(uint32_t now) {
  if (WiFi.status() == WL_CONNECTED) return;
  if (now - lastWifiTry < WIFI_RETRY_MS) return;
  lastWifiTry = now;
  WiFi.disconnect();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

int httpRequest(const char* method, const String& path, const String& body, JsonDocument* out) {
  if (WiFi.status() != WL_CONNECTED) return -1;
  WiFiClient client;
  HTTPClient http;
  http.setTimeout(HTTP_TIMEOUT_MS);
  http.setConnectTimeout(HTTP_TIMEOUT_MS);
  if (!http.begin(client, String(SERVER_URL) + path)) return -1;
  http.addHeader("X-Device-Token", DEVICE_TOKEN);
  int code;
  if (!strcmp(method, "POST")) {
    http.addHeader("Content-Type", "application/json");
    code = http.POST(body);
  } else {
    code = http.GET();
  }
  if (code == 200 && out) {
    DeserializationError err = deserializeJson(*out, http.getString());
    if (err) code = -2;        // malformed JSON counts as a failed poll
  }
  http.end();
  return code;
}

void sendHeartbeat() {
  JsonDocument d;
  d["device_id"] = DEVICE_ID;
  d["status"] = "ONLINE";
  String body; serializeJson(d, body);
  httpRequest("POST", "/api/esp32/heartbeat", body, nullptr);
}

void trySendAck() {
  if (pendingAckId.isEmpty()) return;
  JsonDocument d;
  d["device_id"] = DEVICE_ID;
  d["command_id"] = pendingAckId;
  d["kind"] = pendingAckKind;
  d["status"] = "RECEIVED";
  String body; serializeJson(d, body);
  int code = httpRequest("POST", "/api/esp32/acknowledge", body, nullptr);
  if (code == 200 || code == 201 || code == 204) { pendingAckId = ""; pendingAckKind = ""; }
  // otherwise it stays pending and is retried on the next poll
}

void pollBackend(uint32_t now) {
  JsonDocument doc;
  int code = httpRequest("GET", "/api/esp32/poll", "", &doc);
  if (code != 200) return;                        // failure: lastGoodPoll is not refreshed
  lastGoodPoll = now;
  everPolled = true;

  // Genuine prediction (separate from tests)
  JsonObject p = doc["prediction"];
  if (p.isNull()) {
    curRisk = R_NONE; curLocation = ""; lastPredId = "";   // no data is never shown as LOW
  } else {
    curRisk = parseRisk(p["risk_level"] | (const char*)nullptr);
    curLocation = String((const char*)(p["location"] | ""));
    String id = String((const char*)(p["prediction_id"] | ""));
    if (id.length() && id != lastPredId) {         // act only when the ID changes
      lastPredId = id;
      if (curRisk != R_INVALID) { pendingAckId = id; pendingAckKind = "PREDICTION"; }
    }
  }

  // Manual test (bounded, one-shot)
  JsonObject t = doc["test"];
  if (!t.isNull()) {
    String tid = String((const char*)(t["test_id"] | ""));
    String target = String((const char*)(t["target"] | ""));
    bool known = target == "red" || target == "yellow" || target == "green" || target == "buzzer" || target == "lcd";
    if (tid.length() && tid != lastTestId && known) {
      lastTestId = tid;
      testTarget = target;
      testActive = true;
      testEnd = now + TEST_MS;
      pendingAckId = tid; pendingAckKind = "TEST";
    }
  }
  trySendAck();
}

// ---------- state and outputs ----------
Mode currentMode(uint32_t now) {
  if (testActive) {
    if ((int32_t)(now - testEnd) < 0) return M_TEST;
    testActive = false;
  }
  if (!everPolled) return (now - bootMs > LOST_MS) ? M_LOST : M_BOOT;
  if (now - lastGoodPoll > LOST_MS) return M_LOST;
  switch (curRisk) {
    case R_HIGH: return M_HIGH;
    case R_MOD:  return M_MOD;
    case R_LOW:  return M_LOW;
    case R_INVALID: return M_INVALID;
    default: return M_NO_PRED;
  }
}

void applyOutputs(uint32_t now) {
  Mode m = currentMode(now);
  bool r = false, y = false, g = false, buzz = false;
  String l1, l2;

  switch (m) {
    case M_BOOT:     l1 = "LANDSLIDO"; l2 = "Connecting..."; break;
    case M_NO_PRED:  l1 = "NO PREDICTION"; l2 = "Monitoring"; break;     // all LEDs off, no buzzer
    case M_INVALID:  l1 = "INVALID DATA"; l2 = "Check backend"; break;
    case M_HIGH:     r = true; buzz = patternOn(PAT_HIGH, 2, now); l1 = "LANDSLIDE"; l2 = "DETECTED"; break;
    case M_MOD:      y = true; buzz = patternOn(PAT_MOD, 2, now);  l1 = "MODERATE"; l2 = "LANDSLIDE"; break;
    case M_LOW:      g = true; buzz = patternOn(PAT_LOW, 2, now);  l1 = "LOW RISK"; l2 = curLocation; break;
    case M_LOST: {
      bool blink = (now / 500) % 2;                 // all three LEDs blink together
      r = y = g = blink;
      buzz = patternOn(PAT_LOST, 4, now);
      l1 = "CONNECTION LOST"; l2 = "Reconnecting...";
      break;
    }
    case M_TEST:
      l1 = "TEST MODE"; l2 = testTarget; l2.toUpperCase();
      if (testTarget == "red") r = true;
      else if (testTarget == "yellow") y = true;
      else if (testTarget == "green") g = true;
      else if (testTarget == "buzzer") buzz = patternOn(PAT_TEST, 2, now);
      else if (testTarget == "lcd") l2 = "LCD OK";
      break;
  }
  setLeds(r, y, g);
  digitalWrite(PIN_BUZZER, buzz);
  showLcd(l1, l2);
  shownMode = m;
}

// ---------- Arduino ----------
void setup() {
  pinMode(PIN_LED_RED, OUTPUT); pinMode(PIN_LED_YELLOW, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT); pinMode(PIN_BUZZER, OUTPUT);
  setLeds(false, false, false); digitalWrite(PIN_BUZZER, LOW);
  Wire.begin(PIN_SDA, PIN_SCL);
  lcd.init(); lcd.backlight();
  bootMs = millis();
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  lastWifiTry = bootMs;
  applyOutputs(bootMs);
}

void loop() {
  uint32_t now = millis();
  wifiMaintain(now);
  if (now - lastPoll >= POLL_MS) { lastPoll = now; pollBackend(now); }
  if (now - lastBeat >= HEARTBEAT_MS) { lastBeat = now; sendHeartbeat(); }
  applyOutputs(millis());       // pure millis() timing, no delay() anywhere
}
