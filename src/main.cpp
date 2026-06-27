#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "your-wifi";
const char* password = "your-password";

const int lightPin = 2;

WebServer server(80);

void handleRoot(){
  String html = R"rawliteral(
  <!DOCTYPE html>
  <html>
  <head>
    <title>ESP32 Light Control</title>
    <style>
      body { font-family: Arial; text-align: center; margin-top: 50px; }
      button { font-size: 30px; padding: 20px; margin: 10px; }
    </style>
  </head>
  <body>
    <h1>ESP32 Light Control</h1>
    <a href="/on"><button>ON</button></a>
    <a href="/off"><button>OFF</button></a>
  </body>
  </html>
  )rawliteral";
  server.send(200, "text/html", html);
}

void handleOn(){
  digitalWrite(lightPin, HIGH);
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleOff(){
  digitalWrite(lightPin, LOW);
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);
  pinMode(lightPin, OUTPUT);
  digitalWrite(lightPin, LOW);
  delay(1000);

  Serial.println();
  Serial.println("Starting ESP32 WiFi...");


  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  
  int attempts = 0;

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    attempts++;
  }
  
  Serial.printf("WiFi connected after %d attempts\n", attempts);

  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);

  server.begin();
}

void loop() {
  server.handleClient();
}