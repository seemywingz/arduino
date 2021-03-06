#include <Ultrasonic.h>

Pin *buzzer = new Pin(A1);
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
    alarm();
  }

  delay(10);
}

void alarm(){
  int d = 500;
  light->on();
  buzzer->startTone(300);
  delay(d);
  buzzer->startTone(1000);
  delay(d);
  buzzer->stopTone();
  light->off();
  delay(200);
}