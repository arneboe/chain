#include "hsv2rgb.h"
#include <SPI.h>
#include "WS2801.h"
#include "Fade.h"
#include "FullStrobeWhite.h"
#include "HalfStrobeWhite.h"
#include "OneThirdStrobe.h"
#include "Equalizer.h"
#define PIN_MSG_OUT A5
#define PIN_MSG_STROBE 11
#define PIN_MSG_RESET 10
//dirty hack to get the equalizer in all relevant files :D
Equalizer eq(PIN_MSG_OUT, PIN_MSG_STROBE, PIN_MSG_RESET);
#include "MusicFade.h"
#include "Spectrum.h"
#include "Spectrum2.h"
#include "Spectrum3.h"
#include "ColorSelect.h"

#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>


#define LED_COUNT 20
#define PIN_POTI A4
#define PIN_BUTTON 6


void updateColors();
int readButton();

int dataPin = 8;
int clockPin = 9;
// Set the first variable to the NUMBER of pixels. 25 = 25 pixels in a row
WS2801 strip = WS2801(LED_COUNT, dataPin, clockPin);
const int numModes = 9;
typedef void (*initPtr)(void);

//returns message, int = poti value, chsv=color array, int= number of colors
typedef char* (*updatePtr)(int, CHSV*, int);
updatePtr update[numModes];
initPtr initt[numModes]; //stupid name because init() is already used by arduino.h
char* names[numModes];
int currentMode = 8;
CHSV colors[LED_COUNT];
Adafruit_PCD8544 display = Adafruit_PCD8544(7, A3, A2, A1, A0);

void setup() 
{
  strip.begin();
  Serial.begin(9600);
 
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  
  display.begin();
  display.setContrast(50);
  display.clearDisplay(); 
  display.setTextSize(1);
  display.setTextColor(BLACK);
  
  update[0] = fadeUpdate;
  initt[0] = fadeInit;
  names[0] = "Fade";
  
  update[1] = fullStrobeUpdate;
  initt[1] = fullStrobeInit;
  names[1] = "Strobe";
  
  update[2] = halfStrobeWhiteUpdate;
  initt[2] = halfStrobeWhiteInit;
  names[2] = "1/2 Strobe";
  
  update[3] = oneThirdStrobeUpdate;
  initt[3] = oneThirdStrobeInit;
  names[3] = "1/3 Strobe";

  update[4] = musicFadeUpdate;
  initt[4] = musicFadeInit;
  names[4] = "Bass Detect";

  update[5] = spectrumUpdate;
  initt[5] = spectrumInit;
  names[5] = "Spectrum";
  
  update[6] = spectrum2Update;
  initt[6] = spectrum2Init;
  names[6] = "Spectrum2";  
  
  update[7] = spectrum3Update;
  initt[7] = spectrum3Init;
  names[7] = "Spectrum3";  
  
  update[8] = colorSelectUpdate;
  initt[8] = colorSelectInit;
  names[8] = "Col Select";
  
  initt[currentMode]();
}


void loop() {
 // Serial.println(freeRam());
  const int buttonPressed = !readButton(); //negate because button is 0 when pressed
  if(buttonPressed)
  {
    currentMode = (currentMode + 1) % numModes;
    initt[currentMode]();
  }
  
  const int potiValue = analogRead(PIN_POTI);
  char* text = update[currentMode](potiValue, colors, LED_COUNT);
  
  updateColors();
  
  display.clearDisplay();
  display.println(names[currentMode]);
  display.println(text);
  display.display();
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
    //rgb.g = map(rgb.g, 0, 255, 0, 89);
    strip.setPixelColor(i, rgb.r, rgb.g, rgb.b);
  }
  strip.show();
}

//button is allowed to change state every 400 ms
int readButton()
{
  static unsigned long lastPressTime = 0;
  const unsigned long currentTime = millis();
  const unsigned long timeDiff = currentTime - lastPressTime;
  const int val = digitalRead(PIN_BUTTON);
  if(val == 0 && timeDiff > 400)
  {
    lastPressTime = currentTime;
    return 0;
  }
  return 1;
}

