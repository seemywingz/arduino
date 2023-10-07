#include <Pin.h>

float voltage = 0;
int delayMS = 500;
int analog_value = 0;
Pin *meter = new Pin(A0);

void setup()
{
  Serial.begin(9600);
  meter->setPinMode(INPUT);
}

void loop()
{
  analog_value = meter->readA();
  voltage = (analog_value * 5.0) / 1023.0;
  Serial.println(voltage);
  delay(delayMS);
}