// NodeMcu Mini Wireless D1 Module: ESP8266 ESP-12F

#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <IOPin.h>

#include "indexHTML.h"

ESP8266WebServer server(80);  // create the webserver on port 80

// setup the wifi
char ssid[] = "connectedness";             // your network SSID (name)
char pass[] = "ReallyLongPassword123!@#";  // your network password

IOPin led(2);

void setup() {
  WiFi.begin(ssid, pass);  // Connect to the WiFi network
  Serial.begin(115200);    // output to the serial terminal for debugging
  Serial.print("\nConnecting to Network\n");
  while (WiFi.status() != WL_CONNECTED) {  // Wait for connection
    delay(500);
  }
  Serial.print("Network Connected: ");
  Serial.println(ssid);
  initHandlers();
  server.begin();  // Start the server
  Serial.print("Server Started: ");
  Serial.print("http://");
  Serial.println(WiFi.localIP());
  led.setHigh();
}

void loop() {
  server.handleClient();  // Handling of incoming requests
}

void toggleLED() {
  if (led.isHigh()) {
    led.setLow();
    server.send(200, "text/plain", "LED on");
  } else {
    led.setHigh();
    server.send(200, "text/plain", "LED off");
  }
  Serial.println("LED toggle");
}

void initHandlers() {
  server.on("/", HTTP_GET, []() { server.send(200, "text/html", indexHTML); });

  server.on("/info", []() { server.send(200, "text/html", boardInfo()); });

  server.on("/led/toggle", []() { toggleLED(); });

  server.on("/led/state", []() {
    server.send(200, "text/plain", led.isHigh() ? "LED off" : "LED on");
  });

  server.on("/led/on", []() {
    led.setLow();
    server.send(200, "text/plain", "LED on");
    Serial.println("LED on");
  });

  server.onNotFound([]() { server.send(404, "text/html", boardInfo()); });
}

String boardInfo() {
  String message = "<html><body>";
  message += "<h1>Board Information</h1>";
  message += "<p>URI: " + server.uri() + "</p>";
  message +=
      "<p>Method: " + String(server.method() == HTTP_GET ? "GET" : "POST") +
      "</p>";
  message += "<h2>Request Parameters</h2>";
  message += "<ul>";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += "<li>" + server.argName(i) + ": " + server.arg(i) + "</li>";
  }
  message += "</ul>";
  message += "<h2>Board Details</h2>";
  message += "<ul>";
  message +=
      "<li><strong>Board Type:</strong> NodeMcu Mini Wireless D1 Module "
      "(ESP8266 ESP-12F)</li>";
  message += "<li><strong>Firmware Version:</strong> " +
             String(ESP.getCoreVersion()) + "</li>";
  message += "<li><strong>SDK Version:</strong> " +
             String(ESP.getSdkVersion()) + "</li>";
  message += "<li><strong>Flash Chip ID:</strong> " +
             String(ESP.getFlashChipId(), HEX) + "</li>";
  message += "<li><strong>Flash Chip Size:</strong> " +
             String(ESP.getFlashChipSize() / (1024 * 1024)) + " MB</li>";
  message += "<li><strong>Free Heap:</strong> " + String(ESP.getFreeHeap()) +
             " bytes</li>";
  message +=
      "<li><strong>Chip ID:</strong> " + String(ESP.getChipId(), HEX) + "</li>";
  message += "<li><strong>CPU Frequency:</strong> " +
             String(ESP.getCpuFreqMHz()) + " MHz</li>";
  message += "<li><strong>Flash Frequency:</strong> " +
             String(ESP.getFlashChipSpeed() / 1000000) + " MHz</li>";
  message += "<li><strong>Flash Mode:</strong> " +
             String(ESP.getFlashChipMode()) + "</li>";
  message += "</ul>";
  message += "<h2>LED State</h2>";
  message += "<p>" + String(led.isHigh() ? "LED off" : "LED on") + "</p>";
  message += "</body></html>";
  return message;
}