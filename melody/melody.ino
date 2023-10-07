#include <StarWars.h>
#include <Melody.h>
#include <Nokia.h>
#include <TakeOnMe.h>
#include <ZeldaTheme.h>
#include <Pin.h>

int delayMS = 100;
Pin *buzzer = new Pin(9);
int *melody;
int numNotes = 0;
int tempo = 180;

void setup()
{
}

void loop()
{

  tempo = 110;
  melody = melody_zeldaTheme;
  numNotes = sizeof(melody_zeldaTheme) / sizeof(melody_zeldaTheme[0]);
  playMelody(buzzer->pinNumber, melody, numNotes, tempo);

  delay(delayMS);
}