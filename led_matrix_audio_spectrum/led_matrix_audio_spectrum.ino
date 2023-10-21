// NodeMcu Mini Wireless D1 Module: ESP8266 ESP-12F

#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <IOPin.h>
#include <Servo.h>

ESP8266WebServer server(80);  // create the webserver on port 80

// setup the wifi
char ssid[] = "connectedness";             // your network SSID (name)
char pass[] = "ReallyLongPassword123!@#";  // your network password

IOPin led(LED_BUILTIN);
IOPin servoPin(0);  // D3 on NodeMCU

Servo servo;
const float servoSpeed = 0.30;
const int servoMax = 45;
const int servoMin = 0;
int servoPos = servoMin;

// this is the function that handles the root page
void handleRoot() { server.send(200, "text / plain", "Servo Server"); }

void setup() {
  WiFi.begin(ssid, pass);  // Connect to the WiFi network
  Serial.begin(115200);    // output to the serial terminal for debugging
  while (!Serial) {
  }  // wait for serial port to connect);

  Serial.print("\nConnecting to Network\n");
  while (WiFi.status() != WL_CONNECTED) {  // Wait for connection
    delay(3000);
  }
  Serial.print("Network Connected\n");

  server.begin();  // Start the server
  initHandlers();
  Serial.print("Server Started: \n");
  Serial.print("http://");
  Serial.println(WiFi.localIP());

  servo.attach(servoPin.pin());
  servo.write(servoMin);
}

void loop() {
  server.handleClient();  // Handling of incoming requests
}

void initHandlers() {
  server.on("/", handleRoot);  // Associate the handler function to the path

  server.on("/led/on", []() {
    led.setLow();
    server.send(200, "text/plain", "LED on");
    Serial.println("LED on");
  });

  server.on("/led/off", []() {
    led.setHigh();
    server.send(200, "text/plain", "LED off");
    Serial.println("LED off");
  });

  server.on("/servo/open", []() {
    servo.write(servoMin);
    server.send(200, "text/plain", "Servo open");
  });

  server.on("/servo/close", []() {
    servo.write(servoMax);
    server.send(200, "text/plain", "Servo close");
  });
}