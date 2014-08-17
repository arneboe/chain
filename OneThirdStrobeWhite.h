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
  
  virtual void update()
  {
    if(off)
    {
      off = false;
      //turn leds off
      leds.update();//select new leds
      //turn new leds on
      for(int i = 0; i < leds.onLedsSize; ++i)
      {
        colors[leds.onLeds[i]].v = 1; //this uses a HACK. v == 1 is a special case to create pure white.
      }               
    }
    else
    {
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

private:
  String name; //the name of this mode (needs to be short to fit in one display line)  
  String msg; //some message, will be placed in the second line of the screen
  OneThirdLedSelector<NUM_LEDS> leds;
  bool off; //currently on or off?
};
