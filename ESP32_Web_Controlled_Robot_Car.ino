
#include <WiFi.h>
#include <WebServer.h>


// ---------- Wi-Fi ----------
const char* ssid = "ESP32_CAR";
const char* password = "12345678";

WebServer server(80);

// ---------- L298N pins ----------
// Motor enable pins
#define ENA 22
#define ENB 23

// Motor direction pins
#define IN1 26
#define IN2 25
#define IN3 33
#define IN4 32

// Stop motors if no command is received
const unsigned long STOP_TIMEOUT = 800;
unsigned long lastCommandTime = 0;

// ---------- Motor control ----------
void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void backward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void left() {
  // Left side stops; right side moves forward
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void right() {
  // Right side stops; left side moves forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// ---------- Webpage ----------
const char webpage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP32 Car Control</title>
  <style>
    body {
      background: #101820;
      color: white;
      font-family: Arial, sans-serif;
      text-align: center;
      margin: 0;
      padding: 20px;
    }
    h2 { margin-bottom: 8px; }
    p { color: #b9c5d0; }
    .controls {
      display: grid;
      grid-template-columns: 90px 90px 90px;
      gap: 12px;
      justify-content: center;
      margin-top: 35px;
    }
    button {
      height: 75px;
      border: none;
      border-radius: 15px;
      background: #1687e8;
      color: white;
      font-size: 25px;
      font-weight: bold;
      touch-action: none;
      user-select: none;
    }
    button:active { background: #075ca8; }
    .stop { background: #e53935; }
    .empty { visibility: hidden; }
  </style>
</head>
<body>
  <h2>ESP32 RC CAR</h2>
  <p>Press and hold a direction to move</p>

  <div class="controls">
    <div class="empty"></div>
    <button id="forward">▲</button>
    <div class="empty"></div>

    <button id="left">◀</button>
    <button class="stop" id="stop">STOP</button>
    <button id="right">▶</button>

    <div class="empty"></div>
    <button id="backward">▼</button>
    <div class="empty"></div>
  </div>

  <script>
    let timer = null;

    function sendCommand(command) {
      fetch("/" + command).catch(() => {});
    }

    function startMove(command) {
      clearInterval(timer);
      sendCommand(command);
      timer = setInterval(() => sendCommand(command), 200);
    }

    function stopMove() {
      clearInterval(timer);
      timer = null;
      sendCommand("stop");
    }

    ["forward", "backward", "left", "right"].forEach(command => {
      const button = document.getElementById(command);

      button.addEventListener("pointerdown", event => {
        event.preventDefault();
        startMove(command);
      });

      button.addEventListener("pointerup", stopMove);
      button.addEventListener("pointerleave", stopMove);
      button.addEventListener("pointercancel", stopMove);
    });

    document.getElementById("stop").addEventListener("click", stopMove);

    document.addEventListener("visibilitychange", () => {
      if (document.hidden) stopMove();
    });
  </script>
</body>
</html>
)rawliteral";

// ---------- Web routes ----------
void handleRoot() {
  server.send_P(200, "text/html", webpage);
}

void handleForward() {
  forward();
  lastCommandTime = millis();
  server.send(200, "text/plain", "FORWARD");
}

void handleBackward() {
  backward();
  lastCommandTime = millis();
  server.send(200, "text/plain", "BACKWARD");
}

void handleLeft() {
  left();
  lastCommandTime = millis();
  server.send(200, "text/plain", "LEFT");
}

void handleRight() {
  right();
  lastCommandTime = millis();
  server.send(200, "text/plain", "RIGHT");
}

void handleStop() {
  stopMotors();
  lastCommandTime = 0;
  server.send(200, "text/plain", "STOP");
}

// ---------- Setup ----------
void setup() {
  Serial.begin(115200);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Enable both L298N motor channels
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);

  stopMotors();

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  Serial.println();
  Serial.println("ESP32 Car Wi-Fi Started");
  Serial.print("Connect to Wi-Fi: ");
  Serial.println(ssid);
  Serial.print("Open this address: http://");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/forward", handleForward);
  server.on("/backward", handleBackward);
  server.on("/left", handleLeft);
  server.on("/right", handleRight);
  server.on("/stop", handleStop);

  server.begin();
  lastCommandTime = 0;
}

// ---------- Main loop ----------
void loop() {
  server.handleClient();

  if (lastCommandTime != 0 &&
      millis() - lastCommandTime > STOP_TIMEOUT) {
    stopMotors();
    lastCommandTime = 0;
    Serial.println("Safety timeout: motors stopped");
  }
}
