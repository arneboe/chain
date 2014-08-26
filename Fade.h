#pragma once
#include "Time.h"

struct FadeData
{
  unsigned char target[4];
  unsigned char current[4];
  char dir[4];  
};
static FadeData data;


void fadeInit()
{
  for(int i = 0; i < 4; ++i)
  {
    data.dir[i] = 1;
    data.current[i] = 0;
  }
  data.target[0] = 0;
  data.target[1] = 64;
  data.target[2] = 128;
  data.target[3] = 196;
}

void fadeUpdate(int potiValue, CHSV* colors, int colorSize)
{
  WAIT(map(potiValue, 0, 1023, 0, 1000));

  for(int i = 0; i <4; ++i)
  {
    if(abs(data.target[i] - data.current[i]) < 3)
    {//select new target
      data.target[i] = random(0, 256);
      data.dir[i] = random(0,2) >= 1? 1 : -1;
    }      
    data.current[i] += data.dir[i];
  }
  
  int t = 0;
  for(int led = 0; led < colorSize; ++led, t = (t + 1) % 4)
  {
    colors[led].h = data.current[t];
    colors[led].s = 255;
    colors[led].v = 255;
  }  
}


