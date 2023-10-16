#include <FastLED_Matrix.h>

// Create a FastLED Matrix:
// Using  data pin 6, 8 rows, 8 columns and color order of GRB
LEDMatrix<6, 8, 8, GRB> matrix;

void setup() {
  Serial.begin(115200);
  matrix.begin();
}

void loop() {
  matrix.test();
  delay(1000);
}
