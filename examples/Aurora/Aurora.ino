
#include <RGBmatrixPanelGP.h>
#include <font/fontclock.h>
#include <DS1307ESP.h>

DS1307ESP rtc;
uint8_t sdapin = 3, sclpin = 22; 
//                          R1, G1, B1, R2, G2, B2  RGB Pins
//uint8_t listrgbpins[6] = {25, 26, 27, 12, 13, 14};

#define P_A    15
#define P_B    16
#define P_C    17
#define P_D    4
#define P_E    21
#define P_CLK  0 
#define P_LAT  32 
#define P_OE   33

RGBmatrixPanelGP matrix(P_A, P_B, P_C, P_D, P_E, P_CLK, P_LAT, P_OE, true, 64);

#define MATRIX_WIDTH 64
#define MATRIX_HEIGHT 64



unsigned long currentTime  = 0;
unsigned long lastTime = 0;
unsigned long maxTime = 45000; // 45 seconds
unsigned long prevTime = 0;


#include <FastLED.h> // Aurora needs fastled

#include "Effects.h"
Effects effects;

#include "Drawable.h"
#include "Playlist.h"

#include "Patterns.h"
Patterns patterns;

void setup() {

 Serial.begin(9600);
  rtc.begin(sdapin, sclpin); 
  matrix.begin();
  delay(3000);
  // setup the effects generator
  effects.Setup();
   delay(500);
  Serial.println("Effects being loaded: ");
  listPatterns();


  patterns.setPattern(0); //   // simple noise
  patterns.start();     

  Serial.print("Starting with pattern: ");
  Serial.println(patterns.getCurrentPatternName());

}

void loop()
{
    currentTime = millis();

  
    if ( (currentTime - lastTime) > maxTime ) 
    {      

       patterns.stop();  
       patterns.move(1);
       patterns.start(); 
 
       
       Serial.print("Changing pattern to:  ");
       Serial.println(patterns.getCurrentPatternName());
        
       lastTime = currentTime;
    }
 
    if ( prevTime < currentTime)
    {
      prevTime = patterns.drawFrame() + currentTime;
      matrix.swapBuffers(false);
    }
}

void listPatterns() {
  patterns.listPatterns();
}
