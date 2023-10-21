#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <IOPin.h>

ESP8266WebServer server(80);  // create the webserver on port 80

// setup the wifi
char ssid[] = "connectedness";             // your network SSID (name)
char pass[] = "ReallyLongPassword123!@#";  // your network password

IOPin led(2);

// this is the function that handles the root page
void handleRoot() { server.send(200, "text / plain", "Hello, World"); }

void setup() {
  Serial.begin(115200);  // output to the serial terminal for debugging
  while (!Serial) {
    led.on();
    delay(100);
    led.off();
    delay(100);
  }                        // wait for serial port to connect);
  WiFi.begin(ssid, pass);  // Connect to the WiFi network
  Serial.println("");      // print a new line
  Serial.print("Connecting .");
  while (WiFi.status() != WL_CONNECTED) {  // Wait for connection
    delay(500);
    Serial.print(".");
  }
  Serial.println("");  // print a new line

  server.on("/", handleRoot);  // Associate the handler function to the path

  server.on("/led/on", []() {
    led.on();
    server.send(200, "text/plain", "LED on");
  });

  server.on("/led/off", []() {
    led.off();
    server.send(200, "text/plain", "LED off");
  });

  server.on("/other", []() {  // Define the handling function for the path
    server.send(200, "text/plain", "Other URL");
  });

  server.begin();  // Start the server
  Serial.print("Server listening, Open ");
  Serial.print(WiFi.localIP());
  Serial.println(" in your browser.");
}

void loop() {
  server.handleClient();  // Handling of incoming requests
}