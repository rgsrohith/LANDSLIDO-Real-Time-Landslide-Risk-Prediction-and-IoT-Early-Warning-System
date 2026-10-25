"""LANDSLIDO local server (student prototype).

Put this file in the same folder as landslido.html and run:   python server.py
It does three things:
  1. Serves the website (and your "background image" / "team photo" folders).
  2. Receives each prediction from the website.
  3. Lets the ESP32 fetch the latest prediction over Wi-Fi.
Run it only on a trusted home/college network.
"""
import json, os, socket, time
from http.server import ThreadingHTTPServer, SimpleHTTPRequestHandler
from pathlib import Path

ROOT = Path(__file__).parent.resolve()
PORT = int(os.environ.get("PORT", "8000"))
TOKEN = os.environ.get("LANDSLIDO_TOKEN", "landslido-change-me")   # same value goes in the ESP32 code
RISKS = {"HIGH": "LANDSLIDE DETECTED", "MODERATE": "MODERATE LANDSLIDE", "LOW": "LOW RISK"}
TESTS = {"red", "yellow", "green", "buzzer", "lcd"}
CONNECTED_WITHIN = 20  # seconds

state = {"pred": None, "last_seen": None, "acked": None, "test": None, "test_acked": None}


class Handler(SimpleHTTPRequestHandler):
    def __init__(self, *a, **k):
        super().__init__(*a, directory=str(ROOT), **k)

    def log_message(self, fmt, *args):
        pass

    def send_json(self, code, obj):
        data = json.dumps(obj).encode()
        self.send_response(code)
        self.send_header("Content-Type", "application/json")
        self.send_header("Content-Length", str(len(data)))
        self.send_header("Cache-Control", "no-store")
        self.end_headers()
        self.wfile.write(data)

    def read_json(self):
        try:
            n = int(self.headers.get("Content-Length", "0"))
            if n <= 0 or n > 4096:
                return None
            obj = json.loads(self.rfile.read(n))
            return obj if isinstance(obj, dict) else None
        except Exception:
            return None

    def device_ok(self):
        return self.headers.get("X-Device-Token") == TOKEN

    def status_text(self):
        if state["last_seen"] is None:
            return "WAITING", None
        age = time.time() - state["last_seen"]
        return ("CONNECTED" if age < CONNECTED_WITHIN else "DISCONNECTED"), round(age, 1)

    def do_GET(self):
        path = self.path.split("?")[0]
        if path == "/api/latest-prediction":
            if state["pred"] is None:
                return self.send_json(200, {"empty": True})   # empty is NOT low risk
            return self.send_json(200, state["pred"])
        if path == "/api/hardware-status":
            st, age = self.status_text()
            p = state["pred"]
            return self.send_json(200, {
                "state": st, "last_seen_age_s": age, "latest_prediction": p,
                "acked_id": state["acked"],
                "delivery": None if not p else ("RECEIVED" if state["acked"] == p["prediction_id"] else "PENDING"),
                "active_test": state["test"]["target"] if state["test"] else None})
        if path == "/api/esp32/poll":
            if not self.device_ok():
                return self.send_json(401, {"error": "bad or missing X-Device-Token"})
            state["last_seen"] = time.time()          # every poll counts as a heartbeat
            t = state["test"]
            if t and time.time() - t["created"] < 30 and state["test_acked"] != t["test_id"]:
                return self.send_json(200, {"mode": "TEST", "test_id": t["test_id"], "target": t["target"]})
            p = state["pred"]
            if p:
                return self.send_json(200, {"mode": "PREDICTION", "id": p["prediction_id"],
                                            "risk": p["risk_level"], "location": p["location"],
                                            "message": p["message"]})
            return self.send_json(200, {"mode": "NONE"})
        if path.endswith((".py", ".ino")):
            return self.send_error(404)
        return super().do_GET()

    def do_POST(self):
        path = self.path.split("?")[0]
        body = self.read_json()
        if path == "/api/prediction":                 # called by the website
            if not body or body.get("risk") not in RISKS:
                return self.send_json(400, {"error": "risk must be HIGH, MODERATE or LOW"})
            pid = str(body.get("id", ""))[:40]
            loc = str(body.get("location", ""))[:60]
            if not pid or not loc:
                return self.send_json(400, {"error": "id and location are required"})
            src = str(body.get("source", "unknown"))[:80]
            mode = body.get("mode") if body.get("mode") in ("SIMULATION", "PREDICTION") else "SIMULATION"
            state["pred"] = {"prediction_id": pid, "risk_level": body["risk"], "location": loc,
                             "timestamp": time.strftime("%Y-%m-%dT%H:%M:%S%z"),
                             "message": RISKS[body["risk"]], "source": src, "mode": mode}
            return self.send_json(200, {"ok": True})
        if path == "/api/hardware-test":              # called by the website
            target = None if body is None else body.get("target")
            if target == "":
                state["test"] = None
                return self.send_json(200, {"ok": True})
            if target not in TESTS:
                return self.send_json(400, {"error": "target must be red, yellow, green, buzzer or lcd"})
            state["test"] = {"test_id": "T" + str(int(time.time() * 1000)), "target": target, "created": time.time()}
            return self.send_json(200, {"ok": True})  # never touches state["pred"]
        if path == "/api/esp32/acknowledge":          # called by the ESP32
            if not self.device_ok():
                return self.send_json(401, {"error": "bad or missing X-Device-Token"})
            if not body or not isinstance(body.get("id"), str):
                return self.send_json(400, {"error": "id (string) required"})
            if body.get("mode") == "TEST":
                state["test_acked"] = body["id"]
            else:
                state["acked"] = body["id"]
            return self.send_json(200, {"ok": True})
        return self.send_json(404, {"error": "not found"})


def lan_ip():
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        s.connect(("10.255.255.255", 1))
        return s.getsockname()[0]
    except Exception:
        return "127.0.0.1"
    finally:
        s.close()


if __name__ == "__main__":
    ip = lan_ip()
    print(f"\nLANDSLIDO server running.\n  Website on this laptop : http://localhost:{PORT}/landslido.html")
    print(f"  Put this in the ESP32 code : http://{ip}:{PORT}")
    print(f"  Device token           : {TOKEN}   (change it: set LANDSLIDO_TOKEN)\nPress Ctrl+C to stop.\n")
    ThreadingHTTPServer(("0.0.0.0", PORT), Handler).serve_forever()
