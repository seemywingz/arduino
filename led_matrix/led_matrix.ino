// Adafruit_NeoMatrix example for single NeoPixel Shield.
// Scrolls 'Howdy' across the matrix in a portrait (vertical) orientation.

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#ifndef PSTR
#define PSTR // Make Arduino Due happy
#endif

// Color definitions
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

// MATRIX DECLARATION:
// Parameter 1 = width of NeoPixel matrix
// Parameter 2 = height of matrix
// Parameter 3 = pin number (most are valid)
// Parameter 4 = matrix layout flags, add together as needed:
//   NEO_MATRIX_TOP, NEO_MATRIX_BOTTOM, NEO_MATRIX_LEFT, NEO_MATRIX_RIGHT:
//     Position of the FIRST LED in the matrix; pick two, e.g.
//     NEO_MATRIX_TOP + NEO_MATRIX_LEFT for the top-left corner.
//   NEO_MATRIX_ROWS, NEO_MATRIX_COLUMNS: LEDs are arranged in horizontal
//     rows or in vertical columns, respectively; pick one or the other.
//   NEO_MATRIX_PROGRESSIVE, NEO_MATRIX_ZIGZAG: all rows/columns proceed
//     in the same order, or alternate lines reverse direction; pick one.
//   See example below for these values in action.
// Parameter 5 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_GRBW    Pixels are wired for GRBW bitstream (RGB+W NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)

// Example for NeoPixel Shield.  In this application we'd like to use it
// as a 5x8 tall matrix, with the USB port positioned at the top of the
// Arduino.  When held that way, the first pixel is at the top right, and
// lines are arranged in columns, progressive order.  The shield uses
// 800 KHz (v2) pixels that expect GRB color data.
int width = 8;
int height = 8;
int dataPIN = 6;
int brightness = 1;
uint8_t matrixType = NEO_MATRIX_BOTTOM + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG;
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(width, height, dataPIN, matrixType, NEO_GRB + NEO_KHZ800);

void setup()
{
  matrix.begin();
  Serial.begin(9600);
  matrix.setBrightness(brightness);
  randomSeed(analogRead(0));
}

void loop()
{
  gameOfLife();
  delay(100);
}

void gameOfLife()
{

  int rows = 8;
  int columns = 8;
  int cells[rows][columns];

  // testMatrix(*cells, rows, columns);
  initCells(*cells, rows, columns);
  randomizeCells(*cells, rows, columns);

  for (;;)
  {
    drawCells(*cells, rows, columns);
    delay(1000);
    applyRulesToCells(*cells, rows, columns);
  }
}

void applyRulesToCells(int *cells, int rows, int columns)
{
  int nextGenCells[rows][columns];
  initCells(*nextGenCells, rows, columns);

  // for (int x = 0; x < rows; x++)
  // {
  //   for (int y = 0; y < columns; y++)
  //   {
  //     int neighbors = countNeighbors(*cells, rows, columns, x, y);
  //     Serial.print("Neighbors count ");
  //     Serial.println(neighbors);
  //     if (cells[x * columns + y] == 1 && (neighbors == 2 || neighbors == 3))
  //     {
  //       nextGenCells[x][y] = 1;
  //     }
  //     else if (cells[x * columns + y] == 0 && neighbors == 3)
  //     {
  //       nextGenCells[x][y] = 1;
  //     }
  //     else
  //     {
  //       nextGenCells[x][y] = 0;
  //     }
  //   }
  // }

  randomizeCells(*nextGenCells, rows, columns);
  // copy nextGenCells to current cells
  for (int x = 0; x < rows; x++)
  {
    for (int y = 0; y < columns; y++)
    {
      cells[x * columns + y] = nextGenCells[x][y];
    }
  }
}

int countNeighbors(int *cells, int rows, int columns, int x, int y)
{
  int neighbors = 0;

  for (int i = x - 1; i < x + 2; i++)
  {
    for (int j = y - 1; j < y + 2; j++)
    {
      if (i >= 0 && i != x && i < columns && j >= 0 && j != y && j < rows)
      {
        if (cells[i * columns + j] == 1)
        {
          neighbors++;
        }
      }
    }
  }
  return neighbors;
}

void drawCells(int *cells, int rows, int columns)
{
  matrix.fillScreen(0);
  for (int x = 0; x < rows; x++)
  {
    for (int y = 0; y < columns; y++)
    {
      if (cells[x * columns + y] == 1)
      {
        matrix.drawPixel(x, y, MAGENTA);
      }
    }
  }
  matrix.show();
}

void randomizeCells(int *cells, int rows, int columns)
{
  for (int x = 0; x < rows; x++)
  {
    for (int y = 0; y < columns; y++)
    {
      int rand = random(0, 3000);
      if ((rand % 2) == 0)
      {
        cells[x * columns + y] = 1;
      }
    }
  }
}

void testMatrix(int *cells, int rows, int columns)
{

  Serial.println("Begin Testing LED Matrix");
  int initDelay = 1000;

  // bottom left
  matrix.fillScreen(0);
  matrix.drawPixel(0, 0, MAGENTA);
  matrix.show();
  delay(initDelay);

  // top left
  matrix.fillScreen(0);
  matrix.drawPixel(rows - 1, 0, MAGENTA);
  matrix.show();
  delay(initDelay);

  // top right
  matrix.fillScreen(0);
  matrix.drawPixel(rows - 1, columns - 1, MAGENTA);
  matrix.show();
  delay(initDelay);

  // bottom right
  matrix.fillScreen(0);
  matrix.drawPixel(0, columns - 1, MAGENTA);
  matrix.show();
  delay(initDelay);

  // loop through each pixel from bottom left to top right
  for (int x = 0; x < rows; x++)
  {
    for (int y = 0; y < columns; y++)
    {
      matrix.fillScreen(0);
      matrix.drawPixel(x, y, MAGENTA);
      matrix.show();
      delay(50);
    }
  }

  Serial.println("Begin Testing Cell Data Array");
  // set all cells to 0  and test drawing from cell data
  initCells(cells, rows, columns);
  cells[1 * columns + 1] = 1;
  cells[6 * columns + 1] = 1;
  cells[1 * columns + 6] = 1;
  cells[6 * columns + 6] = 1;
  drawCells(cells, rows, columns);
  delay(initDelay);
}

void initCells(int *cells, int rows, int columns)
{
  for (int x = 0; x < rows; x++)
  {
    for (int y = 0; y < columns; y++)
    {
      cells[x * columns + y] = 0;
    }
  }
}

// void bounce()
// {
//   int x = 0;
//   int y = 0;
//   int dx = 1;
//   int dy = 1;

//   for (;;)
//   {
//     matrix.fillScreen(0);
//     matrix.drawPixel(x, y, MAGENTA);
//     Serial.print("X: ");
//     Serial.print(x);
//     Serial.print(" Y: ");
//     Serial.println(y);
//     matrix.show();
//     x += dx;
//     y += dy;
//     dx = (x >= width || x <= 0) ? -dx : dx;
//     dy = (y >= height || y <= 0) ? -dy : dy;
//     delay(1000);
//   }
// }
