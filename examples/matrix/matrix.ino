
// https://youtu.be/Hndon1lIXL0

#include <RGBmatrixPanel.h>
//                          R1, G1, B1, R2, G2, B2  RGB Pins
//uint8_t listrgbpins[6] = {25, 26, 27, 12, 13, 14};

#define A    15
#define B    17
#define C    16
#define D    4
#define E    21
#define CLK  0 
#define LAT  32 
#define OE   33

// RGBmatrixPanel matrix(A, B, C, D, E, CLK, LAT, OE, true, 64);
// RGBmatrixPanel matrix(A, B, C, D, E, CLK, LAT, OE, true, 128);
RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, true, 64);
// RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, true, 128, listrgbpins);

// Similar to F(), but for PROGMEM string pointers rather than literals
#define F2(progmem_ptr) (const __FlashStringHelper *)progmem_ptr

struct Text {
  char *text;
  uint16_t width, height;
  int16_t x, y;
  int16_t dx, dy;
} text = {"Hello", 0, 0, 0, 0, 1, 1};


const char str[] PROGMEM = "Adafruit 64x64 RGB LED Matrix";
int16_t    textX         = matrix.width(),
           textMin       = (int16_t)sizeof(str) * -8,
           hue           = 0;
int8_t ball[7][4] = {
  {  3,  0,  1,  1 }, // Initial X,Y pos & velocity for 3 bouncy balls
  { 17, 15,  1, -1 },
  { 27,  4, -1,  1 },
  { 37, 10, -1,  -1 },
  { 47, 20, 1,  -1 },
  { 17, 40, 1,  -1 },
  { 15, 6, 1,  1 }
};
static const uint16_t PROGMEM ballcolor[7] = {
  0xff0f, 0xf00f, 0x0f0f, 0xff00, 0x000f, 0x0f00, 0xf000
};

  uint16_t textColor;
  uint16_t cerColor;
  uint16_t lineColor;

void balls()
{
  //some basic gravity physics
  static float y[4] = {20, 20, 20, 20};
  static float x[4] = {20, 20, 20, 20};
  static float vx[4] = {.01, -0.07, .05, -.03};
  static float vy[4] = {0, 4, 8, 16};
  static unsigned long lastT = 0;
  unsigned long t = millis();
  float dt = (t - lastT) * 0.001f;
  lastT = t;
  const int rxx = 2, ryy = 5;
  for (int i = 0; i < 4; i++)
  {
    int rx = rxx;
    int ry = ryy;
    vy[i] += -9.81f * dt * 100;
    x[i] += vx[i];
    y[i] += vy[i] * dt;
    //check for boundaries and bounce back
    if (y[i] < ryy && vy[i] < 0)
    {
      vy[i] = 200 + i * 10;
      ry = y[i];
    }
    if (x[i] < rxx && vx[i] < 0)
    {
      vx[i] = -vx[i];
      rx = x[i];
    }
    if (x[i] >= matrix.width() - rxx && vx[i] > 0)
    {
      vx[i] = -vx[i];
      rx = matrix.width() - x[i];
    }
    //draw a filled ellipse
    matrix.fillEllipse(x[i], matrix.height() - y[i] - 1, rx, ry, cerColor);
    matrix.ellipse(x[i], matrix.height() - y[i] - 1, rx, ry, ballcolor[i]);
  }
}
int16_t x=0, dx=1;
void TextRo()
{
  if(x+dx>=matrix.width() || x+dx<0)
   dx=-dx;
  x+=dx;
  matrix.drawLine(x,0, matrix.width()-x-1, matrix.height()-1, lineColor);

  if(text.x+text.dx+text.width>=matrix.width() || text.x+text.dx<0)
    text.dx=-text.dx;
  if(text.y+text.dy+text.height>=matrix.height() || text.y+text.dy<0)
    text.dy=-text.dy;
  text.x+=text.dx;
  text.y+=text.dy;
  matrix.setTextColor(textColor);
  matrix.setCursor(text.x, text.y);
  matrix.print(text.text);
}

void setup() {
  matrix.begin();
  matrix.setTextWrap(false); // Allow text to run off right edge
  matrix.setFont(&atawi11x7g);
int16_t x1 = 0, y1 = 0;
  matrix.getTextBounds(text.text, 0, 0, &x1, &y1, &text.width, &text.height);
  text.width-=2;
  text.height-=2;
}

void loop() {
  byte i;
  textColor = matrix.ColorHSV(hue, 255, 255, true);
  cerColor = matrix.ColorHSV(hue*2, 255, 255, true);
  lineColor = matrix.ColorHSV(hue*3, 255, 255, true);

  // Clear background
  matrix.fillScreen(0);

  // Bounce three balls around
  for(i=0; i<7; i++) {
    // Draw 'ball'
   matrix.fillCircle(ball[i][0], ball[i][1], 3, pgm_read_word(&ballcolor[i]));
   matrix.drawCircle(ball[i][0], ball[i][1], 4, matrix.ColorHSV(hue*2, 255, 255, true));
    // Update X, Y position
    ball[i][0] += ball[i][2];
    ball[i][1] += ball[i][3];
    // Bounce off edges
    if((ball[i][0] == 0) || (ball[i][0] == (matrix.width() - 1)))
      ball[i][2] *= -1;
    if((ball[i][1] == 0) || (ball[i][1] == (matrix.height() - 1)))
      ball[i][3] *= -1;
  }


  // Draw big scrolly text on top
  matrix.setTextColor(matrix.ColorHSV(hue*4, 255, 255, true));
  matrix.setCursor(textX, 40);
  matrix.print(F2(str));

  // Move text left (w/wrap), increase hue
  if((--textX) < textMin) textX = matrix.width();
  hue ++;
  if(hue > 1536) hue -= 1536;

 balls();
 TextRo();
 matrix.swapBuffers(false);
 delay(30);
}
