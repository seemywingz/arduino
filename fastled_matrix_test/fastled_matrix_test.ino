#include <FastLED.h>

// Params for width and height
const uint8_t kMatrixWidth = 8;
const uint8_t kMatrixHeight = 8;
// Param for different pixel layouts
const bool kMatrixSerpentineLayout = true;
const bool kMatrixVertical = true;

#define ledDataPIN 6
#define ledRows 8
#define ledColumns 8
#define ledNumLeds ledColumns* ledRows
#define BRIGHTNESS 3
#define ledType WS2811
#define ledColorOrder GRB
CRGB leds[ledNumLeds];

void setup() {
  Serial.begin(115200);
  FastLED.addLeds<ledType, ledDataPIN, ledColorOrder>(leds, ledNumLeds)
      .setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  // testMatrix();
}

void loop() {
  testMatrix(leds, ledRows, ledColumns);
  delay(1000);
}

void testMatrix(CRGB leds[], int rows, int columns) {
  Serial.println("Begin Testing LED Matrix");
  int testDelay = 1000;
  CRGB pixelColor = CRGB::Red;

  // bottom left
  Serial.println("Bottom Left");
  FastLED.clear();
  leds[XY(0, 0)] = pixelColor;
  FastLED.show();
  delay(testDelay);

  // bottom right
  Serial.println("Bottom Right");
  FastLED.clear();
  leds[XY(columns - 1, 0)] = pixelColor;
  FastLED.show();
  delay(testDelay);

  // top right
  Serial.println("Top Right");
  FastLED.clear();
  leds[XY(columns - 1, rows - 1)] = pixelColor;
  FastLED.show();
  delay(testDelay);

  // top left
  Serial.println("Top Left");
  FastLED.clear();
  leds[XY(0, rows - 1)] = pixelColor;
  FastLED.show();
  delay(testDelay);

  // loop through each pixel from bottom left to top right
  Serial.println("Looping through each pixel from bottom left to top right");
  for (int x = 0; x < columns; x++) {
    for (int y = 0; y < rows; y++) {
      FastLED.clear();
      leds[XY(x, y)] = pixelColor;
      FastLED.show();
      delay(50);
    }
  }
}

uint16_t XY(uint8_t x, uint8_t y) {
  uint16_t i;

  if (kMatrixSerpentineLayout == false) {
    if (kMatrixVertical == false) {
      i = (y * kMatrixWidth) + x;
    } else {
      i = kMatrixHeight * (kMatrixWidth - (x + 1)) + y;
    }
  }

  if (kMatrixSerpentineLayout == true) {
    if (kMatrixVertical == false) {
      if (y & 0x01) {
        // Odd rows run backwards
        uint8_t reverseX = (kMatrixWidth - 1) - x;
        i = (y * kMatrixWidth) + reverseX;
      } else {
        // Even rows run forwards
        i = (y * kMatrixWidth) + x;
      }
    } else {  // vertical positioning
      if (x & 0x01) {
        i = kMatrixHeight * (kMatrixWidth - (x + 1)) + y;
      } else {
        i = kMatrixHeight * (kMatrixWidth - x) - (y + 1);
      }
    }
  }

  return i;
}
