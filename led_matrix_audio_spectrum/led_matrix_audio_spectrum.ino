#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <IOPin.h>
#include <OneButton.h>
#include <arduinoFFT.h>

// Color definitions
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define ORANGE 0xFD20
#define TEAL 0x0410
#define VIOLET 0xEC1D
#define OLIVE 0x5345
#define GOLD 0xFEA0
#define SILVER 0xC618
#define DARK_GREEN 0x0320
#define FOREST_GREEN 0x2444
#define CORAL 0xFBEA
#define SALMON 0xFC0E
#define ROSE 0xF8A6
#define PEACH 0xFEA6

// IOPins
IOPin *ledDataPin = new IOPin(6);
IOPin *audioPin = new IOPin(A3, INPUT);
IOPin *btn1Pin = new IOPin(3, INPUT_PULLUP);
IOPin *btn2Pin = new IOPin(9, INPUT_PULLUP);

// LED Matrix Config
int ledRows = 8;
int ledColumns = 8;
uint8_t matrixType =
    NEO_MATRIX_BOTTOM + NEO_MATRIX_RIGHT + NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG;
Adafruit_NeoMatrix matrix =
    Adafruit_NeoMatrix(ledColumns, ledRows, ledDataPin->getPinNumber(),
                       matrixType, NEO_GRB + NEO_KHZ800);

// Brightness Config and Color Config
volatile int brightness = 3;
const int brightnessStep = 81;
const int maxBrightness = 255;
volatile int colorPalletsLength = 7;
volatile int currentPalette = 0;
uint32_t colorPallets[7][4] = {
    {GREEN, YELLOW, ORANGE, RED},       {BLUE, CYAN, CYAN, VIOLET},
    {MAGENTA, MAGENTA, VIOLET, VIOLET}, {CORAL, SALMON, SALMON, ROSE},
    {ROSE, ROSE, ROSE, ROSE},           {GREEN, GREEN, GREEN, GREEN},
    {WHITE, WHITE, WHITE, WHITE},
};

// Button Config
OneButton btn1(btn1Pin->getPinNumber(), true);

// Audio Config
arduinoFFT FFT = arduinoFFT();
int sensitivity = 20;
int maxSensitivity = 100;
int sensitivityStep = 20;
const uint16_t audioSamples = 64;
double vReal[audioSamples];
double vImage[audioSamples];

void setup() {
  matrix.begin();
  Serial.begin(115200);
  initButtonHandlers();
  matrix.setBrightness(brightness);
  // testMatrix();
}

void loop() {
  // volumeAnalyzer();
  btn1.tick();
  spectralAnalyzer();
}

void spectralAnalyzer() {
  for (int i = 0; i < audioSamples; i++) {
    vReal[i] = audioPin->readA() / sensitivity;
    vImage[i] = 0;
    // Serial.println(vReal[i]);
  }

  FFT.Windowing(vReal, audioSamples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImage, audioSamples, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImage, audioSamples);

  int spectralData[ledColumns] = {};
  int sum, sampleCount,
      spectralIndex = 0, maxInput = 80,
      usableSamples = (audioSamples / 2) - 12;  // high pass filter

  for (int i = 1; i < usableSamples; i++) {
    vReal[i] =
        constrain(vReal[i], 0, maxInput);  // set max value for input data

    vReal[i] = map(vReal[i], 0, maxInput, 0,
                   ledRows + 1);  // map data to fit our display

    sum += vReal[i];
    sampleCount++;
    if (i % (usableSamples / ledColumns) == 0) {
      int data = sum / sampleCount;
      data = (spectralIndex == 0 && data == 3) ? 0 : data;
      spectralData[spectralIndex++] = data;
      sum = 0;
      sampleCount = 0;
    }
  }
  // Serial.println("--------------------");

  matrix.fillScreen(0);
  for (int x = 0; x < ledColumns; x++) {
    // Serial.println(spectralData[i]);
    for (int y = 0; y < spectralData[x]; y++) {
      uint32_t pixelColor = colorPallets[currentPalette][0];
      pixelColor = (y > 2) ? colorPallets[currentPalette][1] : pixelColor;
      pixelColor = (y > 5) ? colorPallets[currentPalette][2] : pixelColor;
      pixelColor = (y > 6) ? colorPallets[currentPalette][3] : pixelColor;
      matrix.drawPixel(x, y, pixelColor);
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
//     pixelColor = (vol > 3) ? BLUE : GREEN;
//     pixelColor = (vol > 5) ? YELLOW : pixelColor;
//     pixelColor = (vol > 7) ? RED : pixelColor;
//     vol > i ? matrix.drawLine(i, 0, i, ledRows - 1, pixelColor)
//             : matrix.drawLine(i, 0, i, ledRows - 1, 0);
//   }
//   matrix.show();
// }

void initButtonHandlers() {
  // Button click handlers
  btn1.setClickMs(300);
  btn1.attachClick([]() {
    if (btn2Pin->readD() == LOW) {
      Serial.println("Changing sensitivity");
      sensitivity = (sensitivity + sensitivityStep > maxSensitivity)
                        ? 3
                        : sensitivity + sensitivityStep;
    } else {
      Serial.println("Changing brightness");
      brightness = (brightness + brightnessStep > maxBrightness)
                       ? 3
                       : brightness + brightnessStep;
      matrix.setBrightness(brightness);
    }
  });
  btn1.attachDoubleClick([]() {
    if (btn2Pin->readD() == LOW) {
      Serial.println("Changing Visualizer");
    } else {
      Serial.println("Changing color pallet");
      currentPalette = (currentPalette + 1) % colorPalletsLength;
    }
  });
}

void testMatrix() {
  Serial.println("Begin Testing LED Matrix");
  int testDelay = 1000;
  int pixelColor = WHITE;

  // bottom left
  Serial.println("Bottom Left");
  matrix.fillScreen(0);
  matrix.drawPixel(0, 0, pixelColor);
  matrix.show();
  delay(testDelay);

  // bottom right
  Serial.println("Bottom Right");
  matrix.fillScreen(0);
  matrix.drawPixel(ledColumns - 1, 0, pixelColor);
  matrix.show();
  delay(testDelay);

  // top right
  Serial.println("Top Right");
  matrix.fillScreen(0);
  matrix.drawPixel(ledColumns - 1, ledRows - 1, pixelColor);
  matrix.show();
  delay(testDelay);

  // top left
  Serial.println("Top Left");
  matrix.fillScreen(0);
  matrix.drawPixel(0, ledRows - 1, pixelColor);
  matrix.show();
  delay(testDelay);

  Serial.println("Drawing middle rectangle animation");
  matrix.fillScreen(0);
  matrix.drawPixel(3, 4, pixelColor);
  matrix.drawPixel(3, 3, pixelColor);
  matrix.drawPixel(4, 4, pixelColor);
  matrix.drawPixel(4, 3, pixelColor);
  matrix.show();
  delay(testDelay);

  matrix.fillScreen(0);
  matrix.drawPixel(3, 4, pixelColor);
  matrix.drawPixel(2, 3, pixelColor);
  matrix.drawPixel(5, 4, pixelColor);
  matrix.drawPixel(4, 3, pixelColor);
  matrix.show();
  delay(testDelay);

  // loop through each pixel from bottom left to top right
  Serial.println("Looping through each pixel from bottom left to top right");
  for (int x = 0; x < ledColumns; x++) {
    for (int y = 0; y < ledRows; y++) {
      matrix.fillScreen(0);
      matrix.drawPixel(x, y, pixelColor);
      matrix.show();
      delay(50);
    }
  }
}
