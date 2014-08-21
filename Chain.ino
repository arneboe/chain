#include "Color.h"
#include <SPI.h>
#include "WS2801.h"
#include "Mode.h"
#include "OneThirdStrobe.h"
#include "OneThirdStrobeWhite.h"
#include "FullStrobeWhite.h"
#include "MusicFade.h"
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>


#define LED_COUNT 20


void updateColors();

int dataPin = 8;
int clockPin = 9;
// Set the first variable to the NUMBER of pixels. 25 = 25 pixels in a row
WS2801 strip = WS2801(LED_COUNT, dataPin, clockPin);
const int numModes = 4;
Mode* modes[numModes];
int currentMode = 0;
CHSV colors[LED_COUNT];
int h = 0;
Adafruit_PCD8544 display = Adafruit_PCD8544(A4, A3, A2, A1, A0);

void setup() 
{
  modes[0] = new OneThirdStrobe<LED_COUNT>(&colors[0]);
  modes[1] = new OneThirdStrobeWhite<LED_COUNT>(&colors[0]);
  modes[2] = new FullStrobeWhite<LED_COUNT>(&colors[0]);
  modes[3] = new MusicFade<LED_COUNT>(&colors[0]);
  
  strip.begin();
  Serial.begin(9600);
  
  display.begin();
  display.setContrast(50);
  display.clearDisplay(); 
  display.setTextSize(2);
  display.setTextColor(BLACK);
  display.println(modes[currentMode]->getName());
  display.display();
  Serial.println(modes[currentMode]->getName());
  modes[currentMode]->activate();
}


void loop() {
  modes[currentMode]->update();
  updateColors();
  //TODO check for mode select button
  //TODO update display text if the modes text has changed
}

void updateColors()
{
  CRGB rgb;
  for(int i = 0; i < LED_COUNT; ++i)
  {
    if(colors[i].v == 1) //special case: white
    {
      rgb.r = 255;
      rgb.b = 255;
      rgb.g = 255;
    }
    else
    {
      hsv2rgb_rainbow(colors[i], rgb);
    }
    //scale green down to 35%
    //because green is much brighter
    //35% has been calculated from the datasheet lumen values (34.x% really)
   // rgb.g = map(rgb.g, 0, 255, 0, 89);
    strip.setPixelColor(i, rgb.r, rgb.g, rgb.b);
  }
  strip.show();
}

