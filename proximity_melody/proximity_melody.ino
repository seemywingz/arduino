#include <StarWars.h>
#include <Melody.h>
#include <Nokia.h>
#include <TakeOnMe.h>
#include <ZeldaTheme.h>

#include <Pin.h>
#include <Ultrasonic.h>

bool sendLog = true;

double dist;
double maxDist = 6;

int ms = 100;
int melodyPin = 2;
int maxMelodies = 4;
volatile int melodyNum = 2;

Pin *light = new Pin(10);
Pin *buzzer = new Pin(9);
Pin *button = new Pin(2);
Ultrasonic *sensor = new Ultrasonic(3, 6);

void setup()
{
  if (sendLog)
  {
    Serial.begin(9600);
    Serial.println("--- Start Serial Monitor SEND_RCVE ---");
  }
  pinMode(melodyPin, INPUT_PULLUP);
  // attachInterrupt(digitalPinToInterrupt(melodyPin), chngMelody, LOW);
}

void loop()
{

  dist = sensor->getDistanceCM();
  sendLog ? Serial.println(dist) : true;

  int *melody;
  int numNotes = 0;
  int tempo = 180;

  if (dist < maxDist)
  {

    switch (melodyNum)
    {
    case 1:
      melody = melody_nokia;
      numNotes = sizeof(melody_nokia) / sizeof(melody_nokia[0]);
      tempo = 180;
      break;
    case 2:
      tempo = 180;
      melody = melody_takeOnMe;
      numNotes = sizeof(melody_takeOnMe) / sizeof(melody_takeOnMe[0]);
      break;
    case 3:
      tempo = 110;
      melody = melody_zeldaTheme;
      numNotes = sizeof(melody_zeldaTheme) / sizeof(melody_zeldaTheme[0]);
      break;
    case 4:
      tempo = 118;
      melody = melody_starWarsTheme;
      numNotes = sizeof(melody_starWarsTheme) / sizeof(melody_starWarsTheme[0]);
      break;
    default:
      break;
    }

    light->on();
    playMelody(buzzer->pinNumber, melody, numNotes, tempo);
  }
  else
  {
    light->off();
  }

  delay(ms);
}

void chngMelody()
{
  melodyNum >= maxMelodies ? melodyNum = 1 : melodyNum++;
}