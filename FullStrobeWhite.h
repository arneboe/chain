#pragma once
#include "Mode.h"
template <int NUM_LEDS>
class FullStrobeWhite : public Mode 
{
public:

  FullStrobeWhite(CHSV* colors) : Mode(colors, NUM_LEDS) 
  {
  }

  virtual void activate()
  {
    off = true;
  }
  
  virtual void update()
  {
    if(off)
    {
      off = false;
      //turn leds on
      for(int i = 0; i < NUM_LEDS; ++i)
      {
        colors[i].v = 1; //uses special case HACK
      }               
    }
    else
    {
      off = true;
      //turn leds off
      for(int i = 0; i < NUM_LEDS; ++i)
      {
        colors[i].v = 0;
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
  bool off; //currently on or off?
};
