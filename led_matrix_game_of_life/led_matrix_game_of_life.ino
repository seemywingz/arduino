#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <LED_Matrix.h>
#include <Pin.h>

int rows = 8;
int columns = 8;
int ledDataPIN = 6;
uint8_t matrixType =
    NEO_MATRIX_BOTTOM + NEO_MATRIX_RIGHT + NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG;
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(
    columns, rows, ledDataPIN, matrixType, NEO_GRB + NEO_KHZ800);

// Brightness Config
volatile int brightness = 3;
const int brightnessStep = 81;
const int maxBrightness = 255;

int colors[] = {MAGENTA, BLUE,   RED,  GREEN, CYAN,   YELLOW, WHITE, ORANGE,
                TEAL,    VIOLET, GOLD, CORAL, SALMON, ROSE,   PEACH};
volatile int currentColor = 0;
volatile int cellColor = colors[currentColor];

Pin *btn1 = new Pin(3, INPUT_PULLUP);
Pin *btn2 = new Pin(9, INPUT_PULLUP);

void setup() {
  matrix.begin();
  Serial.begin(9600);
  randomSeed(analogRead(0));
  matrix.setBrightness(brightness);
  attachInterrupt(digitalPinToInterrupt(btn1->pinNumber), btn1Press, RISING);
}

void loop() {
  gameOfLife();
  delay(100);
}

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

void gameOfLife() {
  int generations = 0;
  int cells[rows][columns];

  // testMatrix(*cells, rows, columns);

  initArray(*cells, rows, columns);
  randomizeCells(*cells, rows, columns);

  for (;;) {
    drawCells(*cells, rows, columns);
    applyRulesToCells(*cells, rows, columns);
    if (generations++ >= 60) {
      generations = 0;
      initArray(*cells, rows, columns);
      randomizeCells(*cells, rows, columns);
    }
    delay(100);
  }
}

void applyRulesToCells(int *cells, int rows, int columns) {
  int nextGenCells[rows][columns];
  initArray(*nextGenCells, rows, columns);

  for (int x = 0; x < columns; x++) {
    for (int y = 0; y < rows; y++) {
      int neighbors = countNeighbors(cells, rows, columns, x, y);
      // Serial.print("Cell X: ");
      // Serial.print(x);
      // Serial.print(" Y: ");
      // Serial.print(y);
      // Serial.print(" has ");
      // Serial.print(neighbors);
      // Serial.println(" neighbors");
      if (cells[x * columns + y] == 1 && (neighbors == 2 || neighbors == 3)) {
        nextGenCells[x][y] = 1;
      } else if (cells[x * columns + y] == 0 && neighbors == 3) {
        nextGenCells[x][y] = 1;
      } else {
        nextGenCells[x][y] = 0;
      }
    }
  }
  // copy nextGenCells to current cells
  for (int x = 0; x < columns; x++) {
    for (int y = 0; y < rows; y++) {
      cells[x * columns + y] = nextGenCells[x][y];
    }
  }
}

int countNeighbors(int *cells, int rows, int columns, int x, int y) {
  int neighbors = 0;
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      if (x + i < 0 || x + i >= columns || y + j < 0 || y + j >= rows) {
        continue;
      } else {
        neighbors += cells[(x + i) * columns + (y + j)];
      }
    }
  }
  return neighbors -= cells[x * columns + y];
}

void drawCells(int *cells, int rows, int columns) {
  matrix.fillScreen(0);
  for (int x = 0; x < columns; x++) {
    for (int y = 0; y < rows; y++) {
      if (cells[x * columns + y] == 1) {
        matrix.drawPixel(x, y, cellColor);
      }
    }
  }
  // Serial.println("Drawing Array");
  // printArr(cells, rows, columns);
  matrix.show();
}

void randomizeCells(int *cells, int rows, int columns) {
  for (int x = 0; x < columns; x++) {
    for (int y = 0; y < rows; y++) {
      int rand = random(0, 3000);
      if ((rand % 2) == 0) {
        cells[x * columns + y] = 1;
      }
    }
  }
}

void initArray(int *arr, int rows, int columns) {
  for (int x = 0; x < columns; x++) {
    for (int y = 0; y < rows; y++) {
      arr[x * columns + y] = 0;
    }
  }
}

void printArr(int *arr, int rows, int columns) {
  for (int x = 0; x < columns; x++) {
    for (int y = 0; y < rows; y++) {
      Serial.print("Cell X: ");
      Serial.print(x);
      Serial.print(" Y: ");
      Serial.print(y);
      Serial.print(" has Value: ");
      Serial.println(arr[x * columns + y]);
    }
  }
}

void testMatrix(int *cells, int rows, int columns) {
  Serial.println("Begin Testing LED Matrix");
  int initDelay = 1000;
  int color = BLUE;

  // bottom left
  matrix.fillScreen(0);
  matrix.drawPixel(0, 0, color);
  matrix.show();
  delay(initDelay);

  // bottom right
  matrix.fillScreen(0);
  matrix.drawPixel(columns - 1, 0, color);
  matrix.show();
  delay(initDelay);

  // top right
  matrix.fillScreen(0);
  matrix.drawPixel(columns - 1, rows - 1, color);
  matrix.show();
  delay(initDelay);

  // top left
  matrix.fillScreen(0);
  matrix.drawPixel(0, rows - 1, color);
  matrix.show();
  delay(initDelay);

  matrix.fillScreen(0);
  matrix.drawPixel(3, 4, color);
  matrix.drawPixel(3, 3, color);
  matrix.drawPixel(4, 4, color);
  matrix.drawPixel(4, 3, color);
  matrix.show();
  delay(initDelay);

  matrix.fillScreen(0);
  matrix.drawPixel(3, 4, color);
  matrix.drawPixel(2, 3, color);
  matrix.drawPixel(5, 4, color);
  matrix.drawPixel(4, 3, color);
  matrix.show();
  delay(initDelay);

  // loop through each pixel from bottom left to top right
  for (int x = 0; x < columns; x++) {
    for (int y = 0; y < rows; y++) {
      matrix.fillScreen(0);
      matrix.drawPixel(x, y, color);
      matrix.show();
      delay(50);
    }
  }

  Serial.println("Begin Testing Cell Data Array");
  // set all cells to 0  and test drawing from cell data
  initArray(cells, rows, columns);
  cells[1 * columns + 1] = 1;
  cells[6 * columns + 1] = 1;
  cells[1 * columns + 6] = 1;
  cells[6 * columns + 6] = 1;
  drawCells(cells, rows, columns);
  delay(initDelay);
}
