#include <FastLED_Matrix.h>
#include <IOPin.h>
// Create a FastLED Matrix:
// Using  data pin 6, 8 rows, 8 columns and color order of GRB
LEDMatrix<6, 8, 8, GRB> matrix;
IOPin audioPin(A3, INPUT);
IOPin btn1 = IOPin(3, INPUT_PULLUP);

volatile int brightness = 3;
const int brightnessStep = 81;
const int maxBrightness = 255;

void setup() {
  Serial.begin(115200);
  matrix.begin();
  attachInterrupt(digitalPinToInterrupt(btn1.pinNumber), btn1Press, RISING);
  // matrix.test();
}

void loop() { volumeAnalyzer(); }

void volumeAnalyzer() {
  // int raw = analogRead(audioPin);
  int raw = audioPin.readA();
  int vol = map(raw, 330, 800, 0, matrix.columns + 1);
  Serial.println(raw);
  Serial.println(vol);
  matrix.clear();
  CRGB cellColor = CRGB::Green;
  for (int i = 0; i < matrix.columns; i++) {
    cellColor = (vol > 3) ? CRGB::Blue : cellColor;
    cellColor = (vol > 5) ? CRGB::Yellow : cellColor;
    cellColor = (vol == 7) ? CRGB::Red : cellColor;
    matrix.setPixel(i, vol, cellColor);
  }
  matrix.show();
}

void btn1Press() {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 300) {
    brightness = (brightness + brightnessStep > maxBrightness)
                     ? 3
                     : brightness + brightnessStep;
    matrix.setBrightness(brightness);
  }
  last_interrupt_time = interrupt_time;
}