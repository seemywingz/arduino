#include <Arduino.h>
#include <ESPWiFi.h>

// WiFi credentials
const char* ssid = "ESPWiFi";
const char* password = "1234!@#$";

// Create an instance of ESPWiFi
ESPWiFi espWiFi(ssid, password);

void setup() {
  Serial.begin(115200);
  espWiFi.startAccessPoint();
}

void loop() { espWiFi.handleClient(); }
