// Ultrasonic - Version: Latest 
#include "../..lib/Ultrasonic.h"

Ultrasonic * u = new Ultrasonic(4, 5);
double dist;

void setup() {
  Serial.begin(9600);  
  Serial.println("--- Start Serial Monitor SEND_RCVE ---");
}

void loop() {
  dist = u->getDistanceCM();
  Serial.println(dist);
  delay(1000);
}
