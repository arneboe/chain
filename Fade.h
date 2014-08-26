#pragma once
#include "Time.h"

struct FadeData
{
  unsigned char target[4];
  unsigned char current[4];
  char dir[4];  
  char text[11];//Speed: XXX\0
};
static FadeData fadeData;


void fadeInit()
{
  for(int i = 0; i < 4; ++i)
  {
    fadeData.dir[i] = 1;
    fadeData.current[i] = 0;
  }
  fadeData.target[0] = 0;
  fadeData.target[1] = 64;
  fadeData.target[2] = 128;
  fadeData.target[3] = 196;
}

char* fadeUpdate(int potiValue, CHSV* colors, int colorSize)
{
  WAIT(map(potiValue, 0, 1023, 0, 1000), fadeData.text);

  for(int i = 0; i <4; ++i)
  {
    if(abs(fadeData.target[i] - fadeData.current[i]) < 3)
    {//select new target
      fadeData.target[i] = random(0, 256);
      fadeData.dir[i] = random(0,2) >= 1? 1 : -1;
    }      
    fadeData.current[i] += fadeData.dir[i];
  }
  
  int t = 0;
  for(int led = 0; led < colorSize; ++led, t = (t + 1) % 4)
  {
    colors[led].h = fadeData.current[t];
    colors[led].s = 255;
    colors[led].v = 255;
  }
  
  sprintf(fadeData.text, "Speed: %d", map(potiValue, 0, 1023, 100, 0));
  return fadeData.text;
}


