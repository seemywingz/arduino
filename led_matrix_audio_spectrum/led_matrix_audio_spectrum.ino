#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <LED_matrix.h>
#include <Pin.h>
#include <arduinoFFT.h>

// LED Matrix Config
int ledRows = 8;
int ledColumns = 8;
int ledDataPIN = 6;
uint8_t matrixType =
    NEO_MATRIX_BOTTOM + NEO_MATRIX_RIGHT + NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG;
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(
    ledColumns, ledRows, ledDataPIN, matrixType, NEO_GRB + NEO_KHZ800);

// Brightness Config and Color Config
volatile int brightness = 3;
const int brightnessStep = 81;
const int maxBrightness = 255;
int colors[] = {MAGENTA, BLUE,   RED,  GREEN, CYAN,   YELLOW, WHITE, ORANGE,
                TEAL,    VIOLET, GOLD, CORAL, SALMON, ROSE,   PEACH};
volatile int currentColor = 0;
volatile int cellColor = colors[currentColor];

// Button Config
Pin *btn1 = new Pin(3, INPUT_PULLUP);
Pin *btn2 = new Pin(9, INPUT_PULLUP);

// Audio Config
int sensitivity = 1023;
arduinoFFT FFT = arduinoFFT();
const uint16_t audioSamples = 128;
Pin *audioPin = new Pin(A3, INPUT);
double vReal[audioSamples];
double vImage[audioSamples];
int spectralHeight[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};

void setup() {
  matrix.begin();
  Serial.begin(115200);
  matrix.setBrightness(brightness);
  attachInterrupt(digitalPinToInterrupt(btn1->pinNumber), btn1Press, RISING);
  // testMatrix();
}

void loop() {
  // volumeAnalyzer();
  spectralAnalyzer();
  // delay(1000);
}

void spectralAnalyzer() {
  for (int i = 0; i < audioSamples; i++) {
    vReal[i] = audioPin->readA();
    vImage[i] = 0;
  }

  FFT.Windowing(vReal, audioSamples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImage, audioSamples, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImage, audioSamples);

  int spectralData[ledColumns] = {};
  int sum, sampleCount, spectralIndex = 0, maxInput = 2047,
                        usableSamples = (audioSamples / 2) - 10;

  for (int i = 1; i < usableSamples; i++) {
    vReal[i] =
        constrain(vReal[i], 0, maxInput);  // set max value for input data

    vReal[i] = map(vReal[i], 0, maxInput, 0,
                   ledRows + 1);  // map data to fit our display

    sum += vReal[i];
    sampleCount++;
    if (i % (usableSamples / ledColumns) == 0) {
      spectralData[spectralIndex++] = sum / sampleCount;
      sum = 0;
      sampleCount = 0;
    }
  }
  // Serial.println("--------------------");

  matrix.fillScreen(0);
  for (int i = 0; i < ledColumns; i++) {
    // Serial.println(spectralData[i]);
    for (int j = 0; j < spectralData[i]; j++) {
      cellColor = (j > 2) ? YELLOW : GREEN;
      cellColor = (j > 5) ? ORANGE : cellColor;
      cellColor = (j > 6) ? RED : cellColor;
      matrix.drawPixel(i, j, cellColor);
    }
  }
  matrix.show();
}

// void volumeAnalyzer() {
//   int raw = audioPin->readA();
//   int vol = map(raw, 330, 650, 0, ledColumns + 1);
//   Serial.println(vol);
//   matrix.fillScreen(0);
//   for (int i = 0; i < ledColumns; i++) {
//     cellColor = (vol > 3) ? BLUE : GREEN;
//     cellColor = (vol > 5) ? YELLOW : cellColor;
//     cellColor = (vol > 7) ? RED : cellColor;
//     vol > i ? matrix.drawLine(i, 0, i, ledRows - 1, cellColor)
//             : matrix.drawLine(i, 0, i, ledRows - 1, 0);
//   }
//   matrix.show();
// }

void btn1Press() {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 300) {
    // change color if btn2 is low
    if (btn2->readD() == LOW) {
      currentColor = (currentColor + 1 >= 15) ? 0 : currentColor + 1;
      cellColor = colors[currentColor];
    } else {  // change brightness
      brightness = (brightness + brightnessStep > maxBrightness)
                       ? 3
                       : brightness + brightnessStep;
      matrix.setBrightness(brightness);
    }
  }
  last_interrupt_time = interrupt_time;
}

// void testMatrix() {
//   Serial.println("Begin Testing LED Matrix");
//   int testDelay = 1000;

//   // bottom left
//   Serial.println("Bottom Left");
//   matrix.fillScreen(0);
//   matrix.drawPixel(0, 0, cellColor);
//   matrix.show();
//   delay(testDelay);

//   // bottom right
//   Serial.println("Bottom Right");
//   matrix.fillScreen(0);
//   matrix.drawPixel(ledColumns - 1, 0, cellColor);
//   matrix.show();
//   delay(testDelay);

//   // top right
//   Serial.println("Top Right");
//   matrix.fillScreen(0);
//   matrix.drawPixel(ledColumns - 1, ledRows - 1, cellColor);
//   matrix.show();
//   delay(testDelay);

//   // top left
//   Serial.println("Top Left");
//   matrix.fillScreen(0);
//   matrix.drawPixel(0, ledRows - 1, cellColor);
//   matrix.show();
//   delay(testDelay);

//   Serial.println("Drawing middle rectangle animation");
//   matrix.fillScreen(0);
//   matrix.drawPixel(3, 4, cellColor);
//   matrix.drawPixel(3, 3, cellColor);
//   matrix.drawPixel(4, 4, cellColor);
//   matrix.drawPixel(4, 3, cellColor);
//   matrix.show();
//   delay(testDelay);

//   matrix.fillScreen(0);
//   matrix.drawPixel(3, 4, cellColor);
//   matrix.drawPixel(2, 3, cellColor);
//   matrix.drawPixel(5, 4, cellColor);
//   matrix.drawPixel(4, 3, cellColor);
//   matrix.show();
//   delay(testDelay);

//   // loop through each pixel from bottom left to top right
//   Serial.println("Looping through each pixel from bottom left to top right");
//   for (int x = 0; x < ledColumns; x++) {
//     for (int y = 0; y < ledRows; y++) {
//       matrix.fillScreen(0);
//       matrix.drawPixel(x, y, cellColor);
//       matrix.show();
//       delay(50);
//     }
//   }
// }
