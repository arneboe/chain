#pragma once
#include "Time.h"


struct FullStrobeData
{
  bool off;
  char text[11];//Speed: XXX\0
};
static FullStrobeData fullStrobeData;

void fullStrobeInit()
{
  fullStrobeData.off = true;
}

char* fullStrobeUpdate(int potiValue, CHSV* colors, int colorSize)
{
  WAIT(map(potiValue, 0, 1023, 0, 1000), fullStrobeData.text);
  if(fullStrobeData.off)
  {
    fullStrobeData.off = false;
    //turn leds on
    for(int i = 0; i < colorSize; ++i)
    {
      colors[i].v = 1; //uses special case HACK
    }               
  }
  else
  {
    fullStrobeData.off = true;
    //turn leds off
    for(int i = 0; i < colorSize; ++i)
    {
      colors[i].v = 0;
    }      
  }
  sprintf(fullStrobeData.text, "Speed: %d", map(potiValue, 0, 1023, 100, 0));
  return fullStrobeData.text;  
}

