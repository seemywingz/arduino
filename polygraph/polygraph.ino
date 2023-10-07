#include <Pin.h>

int delayMS = 500;
Pin *poly = new Pin(A0);

void setup()
{
  Serial.begin(9600);
  poly->setPinMode(INPUT);
}

void loop()
{
  Serial.println(poly->readA());
  delay(delayMS);
}