#include <Pin.h>

Pin * mic = new Pin(A7);
Pin * light[10];

bool sendLog = true;

void setup() {
  mic->setPinMode(INPUT);
  for (int i = 0; i < 10; i++){
    light[i] = new Pin(i + 3);
  }
  if (sendLog)  {
    Serial.begin(9600);
    Serial.println("--- Start Serial Monitor SEND_RCVE ---");
  }
}

void loop() {

  int raw = mic->readA();
  int vol = map(raw, 330, 650, 0, 10);

  for (int i = 0; i < 10; i++){
    vol > i ? light[i]->on() : light[i]->off();
  }
  
  sendLog ? Serial.println("RAW: "+String(raw)+"   VOL: "+String(vol)) : true;
}
