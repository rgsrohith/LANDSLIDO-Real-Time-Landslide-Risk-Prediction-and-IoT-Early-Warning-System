/*
  LANDSLIDO ESP32 warning unit (student prototype)
  Every ~3 s it asks the laptop server for the latest result and shows it on the LCD, LEDs and buzzer.

  Libraries (Arduino IDE > Library Manager): "LiquidCrystal I2C" (Frank de Brabander), "ArduinoJson".
  Board: ESP32 Dev Module.  Not compiled or run on hardware by the author: test on your own board.
*/
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ================= CONFIGURATION: EDIT THESE =================
const char* WIFI_SSID   = "YOUR_WIFI_NAME";
const char* WIFI_PASS   = "YOUR_WIFI_PASSWORD";
const char* SERVER_URL  = "http://192.168.1.50:8000";   // printed by server.py ("Put this in the ESP32 code")
const char* DEVICE_TOKEN = "landslido-change-me";       // must match server.py
const char* DEVICE_ID   = "esp32-01";
const uint8_t LCD_ADDR = 0x27;                           // try 0x3F if the screen stays blank
const uint8_t LCD_COLS = 16, LCD_ROWS = 2;
const int PIN_RED = 25, PIN_YELLOW = 26, PIN_GREEN = 27, PIN_BUZZER = 14;  // example pins: match your wiring
// LCD wiring: SDA = GPIO21, SCL = GPIO22, VCC and GND (check if your LCD needs 5V)
// =============================================================

const unsigned long POLL_MS = 3000, LOST_MS = 15000, TEST_MS = 3000;
LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);

String curId = "", curRisk = "", curLoc = "", curMsg = "";
String testId = "", testTarget = "";
unsigned long lastPoll = 0, lastOk = 0, testUntil = 0;
bool everOk = false;
String shown1 = "~", shown2 = "~";

void show(String a, String b) {                 // only redraws when text changes
  if (a == shown1 && b == shown2) return;
  shown1 = a; shown2 = b;
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print(a.substring(0, LCD_COLS));
  lcd.setCursor(0, 1); lcd.print(b.substring(0, LCD_COLS));
}
void leds(bool r, bool y, bool g) {
  digitalWrite(PIN_RED, r); digitalWrite(PIN_YELLOW, y); digitalWrite(PIN_GREEN, g);
}
void buzz(bool on) { digitalWrite(PIN_BUZZER, on); }

void acknowledge(const String& id, bool isTest) {
  HTTPClient http;
  http.begin(String(SERVER_URL) + "/api/esp32/acknowledge");
  http.addHeader("Content-Type", "application/json");
  http.addHeader("X-Device-Token", DEVICE_TOKEN);
  StaticJsonDocument<200> d;
  d["id"] = id; d["device"] = DEVICE_ID; if (isTest) d["mode"] = "TEST";
  String body; serializeJson(d, body);
  http.POST(body);
  http.end();
}

void poll() {
  if (WiFi.status() != WL_CONNECTED) return;
  HTTPClient http;
  http.setTimeout(2500);
  http.begin(String(SERVER_URL) + "/api/esp32/poll");
  http.addHeader("X-Device-Token", DEVICE_TOKEN);
  int code = http.GET();
  if (code == 200) {
    StaticJsonDocument<512> d;
    if (!deserializeJson(d, http.getString())) {
      lastOk = millis(); everOk = true;
      String mode = d["mode"] | "NONE";
      if (mode == "TEST") {
        String tid = d["test_id"] | "";
        if (tid != testId) {                       // new test command
          testId = tid; testTarget = String((const char*)(d["target"] | ""));
          testUntil = millis() + TEST_MS;
          acknowledge(testId, true);
        }
      } else if (mode == "PREDICTION") {
        String id = d["id"] | "";
        String risk = d["risk"] | "";
        if (id != curId && (risk == "HIGH" || risk == "MODERATE" || risk == "LOW")) {  // only when ID changes
          curId = id; curRisk = risk;
          curLoc = String((const char*)(d["location"] | ""));
          curMsg = String((const char*)(d["message"] | ""));
          acknowledge(curId, false);
        }
      }
    }
  }
  http.end();
}

void applyOutputs() {
  unsigned long now = millis();
  // 1) Manual test (bounded to a few seconds)
  if (now < testUntil) {
    show("TEST MODE", testTarget);
    leds(testTarget == "red", testTarget == "yellow", testTarget == "green");
    buzz(testTarget == "buzzer" && (now / 200) % 2 == 0);
    return;
  }
  // 2) Connection lost: never show an old result as fresh, never assume LOW
  bool lost = !everOk ? (now > LOST_MS) : (now - lastOk > LOST_MS);
  if (WiFi.status() != WL_CONNECTED || lost) {
    show("CONNECTION LOST", "Reconnecting...");
    bool ph = (now / 400) % 2;
    leds(ph, !ph, ph);                              // alternating pattern = distinct warning
    buzz((now % 3000) < 80 || ((now % 3000) > 200 && (now % 3000) < 280));  // double chirp
    return;
  }
  if (!everOk) { show("Connecting...", ""); leds(0, 0, 0); buzz(false); return; }
  // 3) Real prediction
  if (curId == "") { show("Waiting for", "prediction"); leds(0, 0, 0); buzz(false); return; }
  show(curMsg, curLoc);
  if (curRisk == "HIGH") {
    leds(1, 0, 0); buzz((now / 150) % 2 == 0);              // rapid
  } else if (curRisk == "MODERATE") {
    leds(0, 1, 0); buzz((now % 2000) < 200);                // intermittent
  } else {
    leds(0, 0, 1); buzz((now % 10000) < 100);               // brief, occasional
  }
}

void setup() {
  pinMode(PIN_RED, OUTPUT); pinMode(PIN_YELLOW, OUTPUT); pinMode(PIN_GREEN, OUTPUT); pinMode(PIN_BUZZER, OUTPUT);
  leds(0, 0, 0); buzz(false);
  lcd.init(); lcd.backlight();
  show("LANDSLIDO", "Starting...");
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
}

void loop() {
  unsigned long now = millis();
  if (WiFi.status() != WL_CONNECTED && now % 5000 < 20) WiFi.reconnect();
  if (now - lastPoll >= POLL_MS) { lastPoll = now; poll(); }
  applyOutputs();
}
