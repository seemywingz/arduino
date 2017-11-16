#include <Ultrasonic.h>

Pin *light = new Pin(LED_BUILTIN);
Ultrasonic *u = new Ultrasonic(4, 5);

bool sendLog = true;
double dist;

void setup()
{
  if (sendLog)
  {
    Serial.begin(9600);
    Serial.println("--- Start Serial Monitor SEND_RCVE ---");
  }
}

void loop()
{
  
  dist = u->getDistanceCM();
  sendLog ? Serial.println(dist) : true;

  if (dist < 100)
  {
    light->on();
  }else{
    light->off();
  }

  delay(10);
}