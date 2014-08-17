#include "Color.h"
#include "SPI.h"
#include "WS2801.h"
#include "Mode.h"
#include "OneThirdStrobe.h"


#define LED_COUNT 20


void updateColors();

int dataPin = 7;
int clockPin = 6;
// Set the first variable to the NUMBER of pixels. 25 = 25 pixels in a row
WS2801 strip = WS2801(25, dataPin, clockPin);
Mode* modes[1];
int currentMode = 0;
int numModes = 1;
CHSV colors[LED_COUNT];
int h = 0;

void setup() {
  strip.begin();
  Serial.begin(9600);
  modes[0] = new OneThirdStrobe<LED_COUNT>(&colors[0]);
  modes[0]->activate();
}


void loop() {
  modes[currentMode]->update();
  updateColors();
  delay(200);
  //TODO check for mode select button
  //TODO update display text if the modes text has changed
}

void updateColors()
{
  CRGB rgb;
  for(int i = 0; i < LED_COUNT; ++i)
  {
    hsv2rgb_rainbow(colors[i], rgb);
    //scale green down to 35%
    //because green is much brighter
    //35% has been calculated from the datasheet lumen values (34.x% really)
    rgb.g = map(rgb.g, 0, 255, 0, 89);
    strip.setPixelColor(i, rgb.r, rgb.g, rgb.b);
  }
  strip.show();
}

