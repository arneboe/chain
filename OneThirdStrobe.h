#pragma once
#include "Time.h"


struct OneThirdStrobeData
{
  char text[11];//Speed: XXX\0  
  unsigned char h;
  bool first;
  int reInitCounter;
};
OneThirdStrobeData otsData;


void oneThirdStrobeInit()
{
  otsData.h = 0;
  otsData.first = true;
  otsData.reInitCounter = 0;
}

char* oneThirdStrobeUpdate(int potiValue, CHSV* colors, int colorSize)
{
  WAIT(map(potiValue, 0, 1023, 0, 1000), otsData.text);
  
  //reinitialize every 1000 frames to avoid getting stuck
  if(otsData.first || otsData.reInitCounter > 1000)
  {
    otsData.first = false;
    otsData.reInitCounter = 0;
    for(int i = 0; i < colorSize; ++i)
    {//pre initialize 1/3 of the leds
      if(random(0,3) == 0)
      {
        colors[i].v = 255;
      }
      else
      {
        colors[i].v = 0;
      }
    }
  }
  ++otsData.reInitCounter;
  
  otsData.h += 126; //will overflow and run in circles
  
  for(int i = 0; i <colorSize; ++i)
  {
    if(colors[i].v == 0 && random(0,2) == 0)
    {//if led is off: 1/2 chance to turn it on
      colors[i].v = 255;
      colors[i].s = 255;
      colors[i].h = otsData.h;
    }
    else
    {//if led is on or was not chosen: turn off
      colors[i].v = 0;
    }
  }

  sprintf(otsData.text, "Speed: %d", map(potiValue, 0, 1023, 100, 0));
  return otsData.text;  
}

