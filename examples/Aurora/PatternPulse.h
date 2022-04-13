/*
 * Aurora: https://github.com/pixelmatix/aurora
 * Copyright (c) 2014 Jason Coon
 *
 * Based at least in part on someone else's work that I can no longer find.
 * Please let me know if you recognize any of this code!
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

#ifndef PatternPulse_H
#define PatternPulse_H

class PatternPulse : public Drawable {
  private:
    int hue, huea;
    int centerX = 0;
    int centerY = 0;
    int stepp = -1;
    int steppa = -1;
    int maxSteps = 20;
    float fadeRate = 0.8;
    int diff;
    CRGB color = effects.ColorFromCurrentPalette(hue, 255);
    CRGB colora = effects.ColorFromCurrentPalette(huea, 128);

  public:
    PatternPulse() {
      name = (char *)"Pulse";
    }

    unsigned int drawFrame() {
      effects.DimAll(235);

      if (stepp == -1) {
        centerX = random(64);
        centerY = random(64);
        hue = random(250); // 170;
        huea = random(250); // 170;
        stepp = 0;
        steppa = 1;
      }
      color = effects.ColorFromCurrentPalette(hue, 255);
      colora = effects.ColorFromCurrentPalette(huea, 255);
        hue++; // 170;
        huea++; // 170;

      if (stepp == 0) {
        effects.drawCircle(centerX, centerY, stepp, color);
        effects.ellipse(centerY, centerX, stepp, steppa, colora);
        stepp++;
      }
      else {
        if (stepp < maxSteps) {
          // initial pulse
          effects.drawCircle(centerX, centerY, stepp, color);
        effects.ellipse(centerY, centerX, stepp, steppa, colora);

          // secondary pulse
          if (stepp > 3) {
            effects.drawCircle(centerX, centerY, stepp - 3, color);
        effects.ellipse(centerY, centerX, stepp - 1, steppa - 3, colora);
          }
          stepp++;
          steppa++;
        }
        else {
          stepp = -1;
          steppa = -2;
        }
      }


      effects.ShowFrame();

      return 60;
    }
};

#endif
