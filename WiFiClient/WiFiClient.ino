#include <Arduino.h>
#include <ESPWiFi.h>

#include "serverInfo.h"
ESPWiFi espWiFi(ssid, password);

void setup() {
  Serial.begin(115200);
  espWiFi.startAsClient();
}

void loop() {
  espWiFi.handleClient();
  Serial.println(espWiFi.infoString());
}
