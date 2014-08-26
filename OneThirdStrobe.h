#pragma once
#include "Mode.h"
#include "OneThirdLedSelector.h"
#include "Time.h"
template <int NUM_LEDS>
class OneThirdStrobe : public Mode 
{
public:

  OneThirdStrobe(CHSV* colors, OneThirdLedSelector<NUM_LEDS>* leds) : Mode(colors, NUM_LEDS), leds(leds)
  { }

  virtual void activate()
  {
    currentColor.h = 0;
    currentColor.s = 255;
    currentColor.v = 255;
    leds->init();
  }
  
  virtual void update(const int potiValue)
  {
    WAIT(map(potiValue, 0, 1023, 0, 1000));
    //calc new color
    currentColor.h = (currentColor.h + 129) % 256; //129 to move around in a slow circle
    leds->update();
    //turn old leds off
    for(int i = 0; i < leds->offLedsSize; ++i)
    {
      colors[leds->offLeds[i]].v = 0;
    }
    
    //turn new leds on
    for(int i = 0; i < leds->onLedsSize; ++i)
    {
      colors[leds->onLeds[i]] = currentColor;
    }
  }
   
  virtual bool msgChanged()
  {
    return false;
  }
  
  virtual const char* getName() 
  {
    return "1/3 Col-Strobe";
  }

private:
  CHSV currentColor;
};
