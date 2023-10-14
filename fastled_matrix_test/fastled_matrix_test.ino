#include <FastLED_Matrix.h>

// Create a matrix using data pin 6, with 8 rows and 8 columns
LEDMatrix<6, 8, 8, GRB> matrix;

void setup() {
  Serial.begin(115200);
  matrix.begin();
}

void loop() {
  matrix.test();
  delay(1000);
}
