/*
* Aurora: https://github.com/pixelmatix/aurora
* Copyright (c) 2014 Jason Coon
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of
* this software and associated documentation files (the "Software"), to deal in
* the Software without restriction, including without limitation the rights to
* use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
* the Software, and to permit persons to whom the Software is furnished to do so,
* subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
* FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
* COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
* IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
* CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef PatternAnalogClock_H
class PatternAnalogClock : public Drawable {
private:
#define WIDTH 64
#define HEIGHT 64
const byte CENTRE_X = (WIDTH / 2) - 1;
const byte CENTRE_Y = (HEIGHT / 2) - 1;
#define R_HOUR 17
#define R_MIN  25
#define R_SEC   29

public:
    PatternAnalogClock() {
        name = (char *)"AnalogClock";
    }

    void start() {
    }

    unsigned int drawFrame() {
            matrix.fillScreen(0);
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
            delay(1000);
        return 0;
    }
};

#endif
