#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>

ESP8266WebServer server(80);  // create the webserver on port 80

// setup the wifi
char ssid[] = "connectedness";             // your network SSID (name)
char pass[] = "ReallyLongPassword123!@#";  // your network password

void handleRoot() { server.send(200, "text / plain", "Hello world"); }

void setup() {
  Serial.begin(115200);  // output to the serial terminal for debugging
  while (!Serial) {
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
  // this will handle the page 10.0.0.x/other
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