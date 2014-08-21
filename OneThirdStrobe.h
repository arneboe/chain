#pragma once
#include "Mode.h"
#include "OneThirdLedSelector.h"
template <int NUM_LEDS>
class OneThirdStrobe : public Mode 
{
public:

  OneThirdStrobe(CHSV* colors) : Mode(colors, NUM_LEDS) 
  { }

  virtual void activate()
  {
    currentColor.h = 0;
    currentColor.s = 255;
    currentColor.v = 255;
    leds.init();
  }
  
  virtual void update()
  {
    //calc new color
    currentColor.h = (currentColor.h + 129) % 256; //129 to move around in a slow circle
    leds.update();
    //turn old leds off
    for(int i = 0; i < leds.offLedsSize; ++i)
    {
      colors[leds.offLeds[i]].v = 0;
    }
    
    //turn new leds on
    for(int i = 0; i < leds.onLedsSize; ++i)
    {
      colors[leds.onLeds[i]] = currentColor;
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
  OneThirdLedSelector<NUM_LEDS> leds;
  CHSV currentColor;
};
