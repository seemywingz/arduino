
#include <LEDMatrix.h>
#include <Pin.h>
#include <arduinoFFT.h>

// LED Matrix Config
int ledRows = 8;
int ledColumns = 8;
int ledDataPIN = 6;
LEDMatrix matrix(ledRows, ledColumns, ledDataPIN);

// Brightness and Color Config
volatile int brightness = 3;
const int brightnessStep = 81;
const int maxBrightness = 255;
int pixelColor = MAGENTA;

// Button Config
Pin *btn1 = new Pin(3, INPUT_PULLUP);
Pin *btn2 = new Pin(9, INPUT_PULLUP);

// Audio Config
arduinoFFT FFT = arduinoFFT();
const uint16_t audioSamples = 128;
Pin *audioPin = new Pin(A3, INPUT);
double vReal[audioSamples];
double vImage[audioSamples];

void setup() {
  matrix.begin();
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(btn1->pinNumber), btn1Press, RISING);
  // testMatrix();
}

void loop() {
  // volumeAnalyzer();
  spectralAnalyzer();
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
                        usableSamples = (audioSamples / 2) - 12;

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

  matrix.clear();
  for (int i = 0; i < ledColumns; i++) {
    // Serial.println(spectralData[i]);
    for (int j = 0; j < spectralData[i]; j++) {
      pixelColor = (j > 2) ? YELLOW : GREEN;
      pixelColor = (j > 5) ? ORANGE : pixelColor;
      pixelColor = (j > 6) ? RED : pixelColor;
      matrix.drawPixel(i, j, pixelColor);
    }
  }
  matrix.show();
}

// void volumeAnalyzer() {
//   int raw = audioPin->readA();
//   int vol = map(raw, 330, 650, 0, ledColumns + 1);
//   Serial.println(vol);
//   matrix.clear();
//   for (int i = 0; i < ledColumns; i++) {
//     pixelColor = (vol > 3) ? BLUE : GREEN;
//     pixelColor = (vol > 5) ? YELLOW : pixelColor;
//     pixelColor = (vol > 7) ? RED : pixelColor;
//     vol > i ? matrix.drawLine(i, 0, i, ledRows - 1, pixelColor)
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
    brightness = (brightness + brightnessStep > maxBrightness)
                     ? 3
                     : brightness + brightnessStep;
    matrix.setBrightness(brightness);
  }
  last_interrupt_time = interrupt_time;
}

// void testMatrix() {
//   Serial.println("Begin Testing LED Matrix");
//   int testDelay = 1000;

//   // bottom left
//   Serial.println("Bottom Left");
//   matrix.clear();
//   matrix.drawPixel(0, 0, pixelColor);
//   matrix.show();
//   delay(testDelay);

//   // bottom right
//   Serial.println("Bottom Right");
//   matrix.clear();
//   matrix.drawPixel(ledColumns - 1, 0, pixelColor);
//   matrix.show();
//   delay(testDelay);

//   // top right
//   Serial.println("Top Right");
//   matrix.clear();
//   matrix.drawPixel(ledColumns - 1, ledRows - 1, pixelColor);
//   matrix.show();
//   delay(testDelay);

//   // top left
//   Serial.println("Top Left");
//   matrix.clear();
//   matrix.drawPixel(0, ledRows - 1, pixelColor);
//   matrix.show();
//   delay(testDelay);

//   Serial.println("Drawing middle rectangle animation");
//   matrix.clear();
//   matrix.drawPixel(3, 4, pixelColor);
//   matrix.drawPixel(3, 3, pixelColor);
//   matrix.drawPixel(4, 4, pixelColor);
//   matrix.drawPixel(4, 3, pixelColor);
//   matrix.show();
//   delay(testDelay);

//   matrix.clear();
//   matrix.drawPixel(3, 4, pixelColor);
//   matrix.drawPixel(2, 3, pixelColor);
//   matrix.drawPixel(5, 4, pixelColor);
//   matrix.drawPixel(4, 3, pixelColor);
//   matrix.show();
//   delay(testDelay);

//   // loop through each pixel from bottom left to top right
//   Serial.println("Looping through each pixel from bottom left to top right");
//   for (int x = 0; x < ledColumns; x++) {
//     for (int y = 0; y < ledRows; y++) {
//       matrix.clear();
//       matrix.drawPixel(x, y, pixelColor);
//       matrix.show();
//       delay(50);
//     }
//   }
// }
