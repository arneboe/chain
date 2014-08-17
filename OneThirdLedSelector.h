#pragma once
#include "Mode.h"
template <int NUM_LEDS>
//Randomly selects 1/3 of the leds from the set that has not been selected before
class OneThirdLedSelector
{
public:

  void init()
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
  
  void update()
  {
    int newOffLeds[NUM_LEDS];
    //first move all leds that are on to the new off led list
    //and turn them off at the same time
    for(int i = 0; i < onLedsSize; ++i)
    {
      newOffLeds[i] = onLeds[i];   
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
  }

  int offLeds[NUM_LEDS];//contains the leds that are currently off
  int offLedsSize;
  int onLeds[NUM_LEDS];
  int onLedsSize;
};
