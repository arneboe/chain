#pragma once
#include "Mode.h"
template <int NUM_LEDS>
class OneThirdStrobe : public Mode 
{
public:

  OneThirdStrobe(CHSV* colors) : Mode(colors, NUM_LEDS) 
  {
  }

  virtual void activate()
  {
    
    currentColor.h = 0;
    currentColor.s = 255;
    currentColor.v = 255;
    
    offLedsSize = 0;
    onLedsSize = 0;
    //initially 1/3 should be on, 2/3 should be off
    for(int i = 0; i < NUM_LEDS; ++i)
    {
      if(random(0,3) == 0)
      {
        onLeds[onLedsSize] = i;
        ++onLedsSize;
      }
      else
      {
        offLeds[offLedsSize] = i;
        ++offLedsSize;        
      }
    }
  }
  
  virtual void update()
  {
    int newOffLeds[NUM_LEDS];
    //first move all leds that are on to the new off led list
    //and turn them off at the same time
    for(int i = 0; i < onLedsSize; ++i)
    {
      newOffLeds[i] = onLeds[i];
      colors[newOffLeds[i]].v = 0;
      colors[newOffLeds[i]].s = 0;
            
    }
    int newOffLedsSize = onLedsSize;
    
    //now choose 1/2 of the leds that have been off before and turn them on, the other half remains off
    onLedsSize = 0;
    for(int i = 0; i < offLedsSize; ++i)
    {
      if(random(0,2) == 0)
      {
        onLeds[onLedsSize] = offLeds[i];
        ++onLedsSize; 
      }
      else
      {
        newOffLeds[newOffLedsSize] = offLeds[i];
        ++newOffLedsSize;
      }
    }
    //finally copy the new off leds to the off leds
    memcpy(offLeds, newOffLeds, sizeof(int) * newOffLedsSize);
    offLedsSize = newOffLedsSize;
    
    //calc new color
    currentColor.h = (currentColor.h + 128) % 256;
    
    //finally turn the new leds on
    for(int i = 0; i < onLedsSize; ++i)
    {
      const int colIndex = onLeds[i];
      colors[colIndex] = currentColor;
    }
  }
   
  virtual bool msgChanged()
  {
    return false;
  }

private:
  String name; //the name of this mode (needs to be short to fit in one display line)  
  String msg; //some message, will be placed in the second line of the screen
  int offLeds[NUM_LEDS];//contains the leds that are currently off
  int offLedsSize;
  int onLeds[NUM_LEDS];
  int onLedsSize;
  CHSV currentColor;
};
