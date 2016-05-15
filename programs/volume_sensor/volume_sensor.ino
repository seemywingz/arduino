#include <LED.h>

LED * led = new LED(7,6);
Pin * mic = new Pin(A1);
int maxVol = 230;
int mils = 1000;

void setup() {
  mic->setPinMode(INPUT);
  Serial.begin(9600);  
  Serial.println("--- Start Serial Monitor SEND_RCVE ---");
}

// the loop function runs over and over again forever
void loop() {
  int vol = mic->read();
//  Serial.println(vol);

  if(vol > maxVol){
    led->on();
    delay(mils);
  }
  led->off();
}
