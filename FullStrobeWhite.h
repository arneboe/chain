#pragma once
#include "Mode.h"
#include "Time.h"
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
  
  virtual void update(const int potiValue)
  {
    WAIT(map(potiValue, 0, 1023, 0, 1000));
    
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
  
  virtual const char* getName() 
  {
    return "Strobe";
  }

private:
  bool off; //currently on or off?
};
