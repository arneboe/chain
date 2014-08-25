#pragma once
#include "Mode.h"
#include "OneThirdLedSelector.h"
template <int NUM_LEDS>
class OneThirdStrobeWhite : public Mode 
{
public:

  OneThirdStrobeWhite(CHSV* colors) : Mode(colors, NUM_LEDS) 
  {
  }

  virtual void activate()
  {
    leds.init();
    off = true;
  }
  
  virtual void update(const int potiValue)
  {
    WAIT(map(potiValue, 0, 1023, 0, 1000));
    if(off)
    {
      off = false;
      leds.update();//select new leds
      //turn new leds on
      for(int i = 0; i < leds.onLedsSize; ++i)
      {
        colors[leds.onLeds[i]].v = 1; //this uses a HACK. v == 1 is a special case to create pure white.
      }               
    }
    else
    {
      off = true;
      //turn leds off
      for(int i = 0; i < leds.onLedsSize; ++i)
      {
        colors[leds.onLeds[i]].v = 0;
      }      
    }
  }
   
  virtual bool msgChanged()
  {
    return false;
  }
  
  virtual const char* getName() 
  {
    return "1/3 Strobe";
  }

private:
  OneThirdLedSelector<NUM_LEDS> leds;
  bool off; //currently on or off?
};
