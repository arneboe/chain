#pragma once
#include "Mode.h"
#include <string.h> //for memcpy
template <int NUM_LEDS>
class OneThirdStrobe : public Mode 
{
public:

  OneThirdStrobe(

  virtual void activate()
  {
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
      colors[newOffLeds[i]].r = 0;
      colors[newOffLeds[i]].g = 0;
      colors[newOffLeds[i]].b = 0;
            
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
    onLedsSize = newOffLedsSize;
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
};
