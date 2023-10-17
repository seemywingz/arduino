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
const int ledRows = 8;
const int ledColumns = 8;
uint8_t matrixType = NEO_MATRIX_BOTTOM + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS +
                     NEO_MATRIX_ZIGZAG;
Adafruit_NeoMatrix matrix =
    Adafruit_NeoMatrix(ledColumns, ledRows, ledDataPin->getPinNumber(),
                       matrixType, NEO_GRB + NEO_KHZ800);

// Brightness and Color Config
const int minBrightness = 3;
volatile int brightness = minBrightness;
const int brightnessStep = 81;
const int maxBrightness = 255;
volatile int currentPalette = 0;
const int colorPalletsLength = 7;
uint32_t colorPallets[colorPalletsLength][4] = {
    {GREEN, YELLOW, ORANGE, RED},       {BLUE, CYAN, CYAN, VIOLET},
    {MAGENTA, MAGENTA, VIOLET, VIOLET}, {CORAL, SALMON, SALMON, ROSE},
    {ROSE, ROSE, ROSE, ROSE},           {GREEN, GREEN, GREEN, GREEN},
    {WHITE, WHITE, WHITE, WHITE},
};

// Audio Config
arduinoFFT FFT = arduinoFFT();
const int minSensitivity = 20;
const int maxSensitivity = 100;
const uint16_t audioSamples = 64;
int sensitivity = minSensitivity;
int sensitivityStep = minSensitivity;
const int usableSamples = (audioSamples / 2);
double vReal[audioSamples];
double vImage[audioSamples];

// Visualization Config
int visualization = 0;
int maxVisualization = 2;

// Button Config
OneButton btn1(btn1Pin->getPinNumber(), true);

void setup() {
  matrix.begin();
  Serial.begin(9600);
  initButtonHandlers();
  matrix.setBrightness(brightness);
}

void loop() {
  btn1.tick();
  // testMatrix();
  spectralAnalyzer();
}

void spectralAnalyzer() {
  // read audio data
  for (int i = 0; i < audioSamples; i++) {
    vReal[i] = audioPin->readA() / sensitivity;
    vImage[i] = 0;
  }

  int maxInput = 80;
  int spectralData[ledColumns] = {};
  int sum = 0, sampleCount = 0, spectralIndex = 0;

  FFT.Windowing(vReal, audioSamples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImage, audioSamples, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImage, audioSamples);

  for (int i = 2; i < usableSamples; i++) {
    vReal[i] =  // set max value for input data
        constrain(vReal[i], 0, maxInput);
    vReal[i] =  // map data to fit our display
        map(vReal[i], 0, maxInput, 0, ledRows + 1);

    if (usableSamples > ledColumns) {
      sum += vReal[i];
      sampleCount++;
      if (sampleCount == (usableSamples / ledColumns)) {
        spectralData[spectralIndex] = sum / sampleCount;
        spectralIndex++;
        sampleCount = 0;
        sum = 0;
      }
    } else  // we have less data than we can display so just copy the data into
            // the spectralData array
    {
      spectralData[i] = vReal[i];
    }
  }

  matrix.fillScreen(0);
  switch (visualization) {
    case 1:
      drawCircles(spectralData);
      break;
    default:
      drawBars(spectralData);
      break;
  }
  matrix.show();
}

void drawCircles(int *spectralData) {
  matrix.fillScreen(0);
  int circleRadius = spectralData[2];
  int circleColor = colorPallets[currentPalette][0];
  circleColor =
      (circleRadius > 2) ? colorPallets[currentPalette][1] : circleColor;
  circleColor =
      (circleRadius > 3) ? colorPallets[currentPalette][2] : circleColor;
  circleColor =
      (circleRadius > 5) ? colorPallets[currentPalette][3] : circleColor;
  matrix.drawCircle(3, 4, circleRadius, circleColor);
  matrix.show();
}

void drawBars(int *spectralData) {
  matrix.fillScreen(0);
  for (int x = 0; x < ledColumns; x++) {
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

void initButtonHandlers() {
  // Button click handlers
  btn1.setClickMs(450);
  btn1.attachClick([]() {
    if (btn2Pin->readD() == LOW) {
      Serial.println("Changing sensitivity");
      sensitivity = (sensitivity + sensitivityStep > maxSensitivity)
                        ? 20
                        : sensitivity + sensitivityStep;
    } else {
      Serial.println("Changing brightness");
      brightness = (brightness + brightnessStep > maxBrightness)
                       ? minBrightness
                       : brightness + brightnessStep;
      matrix.setBrightness(brightness);
    }
  });
  btn1.attachDoubleClick([]() {
    if (btn2Pin->readD() == LOW) {
      Serial.println("Changing Visualizer");
      visualization =
          (visualization + 1 > maxVisualization) ? 0 : visualization + 1;
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
