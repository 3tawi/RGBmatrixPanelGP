// REQUIRES the following Arduino libraries:
// - Lien vidéo: https://youtu.be/FJR3LrY4Vew
// - RGB matrix Panel Library: https://github.com/3tawi/RGBmatrixPanelGP
// - Adafruit_GFX Library: https://github.com/adafruit/Adafruit-GFX-Library
// - Adafruit_BusIO Library: https://github.com/adafruit/Adafruit_BusIO 
// - DS1307ESP Library: https://github.com/3tawi/DS1307ESP
// - RGB LED Matrix with an ESP32 - How to get started: https://youtu.be/Hndon1lIXL0
// - Getting Started ESP32 : https://www.youtube.com/watch?v=9b0Txt-yF7E 
// Find All "Great Projects" Videos : https://www.youtube.com/c/GreatProjects


#include <RGBmatrixPanelGP.h>
#include <DS1307ESP.h>
#include <font/fontclock.h>

DS1307ESP rtc;
uint8_t sdapin = 3, sclpin = 1; 
//                          R1, G1, B1, R2, G2, B2  RGB Pins
//uint8_t listrgbpins[6] = {25, 26, 27, 12, 13, 14};

#define P_A    15
#define P_B    17
#define P_C    16
#define P_D    4
#define P_E    21
#define P_CLK  0 
#define P_LAT  32 
#define P_OE   33

RGBmatrixPanelGP matrix(P_A, P_B, P_C, P_D, P_E, P_CLK, P_LAT, P_OE, true, 64);
//RGBmatrixPanelGP matrix(P_A, P_B, P_C, P_D, P_E, P_CLK, P_LAT, P_OE, true, 64, listrgbpins);

#define WIDTH 64
#define HEIGHT 64
const byte CENTRE_X = (WIDTH / 2) - 1;
const byte CENTRE_Y = (HEIGHT / 2) - 1;
#define R_HOUR 17
#define R_MIN  25
#define R_SEC   29
uint32_t lastTime; 

void drawtime() {

            double radians;
            int x;
            int y;
            int j = 0;
            rtc.DSread();
            uint8_t hour = rtc.hour;
            if (hour > 12)
                hour -= 12;
            uint16_t lineColor1 = matrix.ColorHSV((rtc.second + rtc.minute) * 30, 255, 255, true);
            uint16_t lineColor2 = matrix.ColorHSV(random(0, 1536), 255, 255, true);
            uint16_t lineColor3 = matrix.ColorHSV(random(0, 1536), 255, 255, true);
            uint16_t lineColor4 = matrix.ColorHSV(random(0, 1536), 255, 255, true);
            matrix.fillScreen(0);
            matrix.drawCircle(CENTRE_X,CENTRE_Y, 31, lineColor1);
            for (int i = 0; i < 60; i+=15) {
            radians = (90.0F - (i * 6.0F)) * M_PI / 180.0;
            x = round(CENTRE_X + 30 * cos(radians));
            y = round(CENTRE_Y - 30 * sin(radians));
            matrix.fillCircle(x,y, 2, lineColor2);
            }
            matrix.setFont(&fontclock);
            matrix.setTextColor(lineColor3);
            for (int i = 0; i < 60; i+=5) {
            radians = (90.0F - (i * 6.0F)) * M_PI / 180.0;
            x = round(CENTRE_X + 30 * cos(radians));
            y = round(CENTRE_Y - 30 * sin(radians));
            matrix.fillCircle(x,y, 1, lineColor2);
            x = round(CENTRE_X + 26 * cos(radians));
            y = round(CENTRE_Y - 26 * sin(radians));
            matrix.setCursor(x,y);
            matrix.print(char(j));
            j++;
            if (j == 10) { j = 11;}
            }
            matrix.setFont();
            // hour
            radians = (90.0F - ((hour * 30.0F) + ((rtc.minute * 29.0F) / 60.0F))) * M_PI / 180.0F;
            x = round(CENTRE_X + R_HOUR * cos(radians));
            y = round(CENTRE_Y - R_HOUR * sin(radians));
            matrix.drawLine(x,y, CENTRE_X, CENTRE_Y-1, 0xf000);
            matrix.drawLine(x,y, CENTRE_X, CENTRE_Y+1, 0xf000);
            matrix.drawLine(x,y, CENTRE_X-1, CENTRE_Y, 0xf000);
            matrix.drawLine(x,y, CENTRE_X+1, CENTRE_Y, 0xf000);

            // minute
            radians = (90.0F - (rtc.minute * 6.0F)) * M_PI / 180.0;
            x = round(CENTRE_X + R_MIN * cos(radians));
            y = round(CENTRE_Y - R_MIN * sin(radians));
            matrix.drawLine(x,y, CENTRE_X, CENTRE_Y-1, 0x0f00);
            matrix.drawLine(x,y, CENTRE_X, CENTRE_Y+1, 0x0f00);
            matrix.drawLine(x,y, CENTRE_X-1, CENTRE_Y, 0x0f00);
            matrix.drawLine(x,y, CENTRE_X+1, CENTRE_Y, 0x0f00);

            // second
            radians = (90.0F - ((float)rtc.second * 6.0F)) * M_PI / 180.0F;
            x = round(CENTRE_X + R_SEC * cos(radians));
            y = round(CENTRE_Y - R_SEC * sin(radians));
            matrix.drawLine(x,y, CENTRE_X-1, CENTRE_Y, lineColor4);
            matrix.drawLine(x,y, CENTRE_X+1, CENTRE_Y, lineColor4);
            matrix.drawLine(x,y, CENTRE_X, CENTRE_Y-1, lineColor4);
            matrix.drawLine(x,y, CENTRE_X, CENTRE_Y+1, lineColor4);
            matrix.fillCircle(x,y, 1, lineColor4);
    }


void setup() {
  rtc.begin(sdapin, sclpin);                  
  // rtc.DSadjust(0, 19, 21, 2022, 4, 5); // 00:19:21 April 5th, 2022
  matrix.begin();
  delay(1000);
}

void loop() {
  drawtime();
  matrix.swapBuffers(false);
  delay(1000);
}
