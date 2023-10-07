#include <Pin.h>
#include <Ultrasonic.h>

bool sendLog = true;

double dist;
double maxDist = 6;

int ms = 100;

Pin *light = new Pin(10);
Pin *buzzer = new Pin(9);
Ultrasonic *sensor = new Ultrasonic(3, 6);

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

  dist = sensor->getDistanceCM();
  sendLog ? Serial.println(dist) : true;

  if (dist < maxDist)
  {

    

    light->on();
    buzzer->on();
  }
  else
  {
    light->off();
    buzzer->off();
  }

  delay(ms);
}
