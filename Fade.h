#pragma once
#include "Mode.h"
#include "Time.h"
template <int NUM_LEDS>
class Fade : public Mode 
{
public:

  Fade(CHSV* colors) : Mode(colors, NUM_LEDS) 
  { }

  virtual void activate()
  {
    for(int i = 0; i < 4; ++i)
    {
      dir[i] = 1;
      current[i] = 0;
    }
    target[0] = 0;
    target[1] = 64;
    target[2] = 128;
    target[3] = 196;
  }
  
  virtual void update(const int potiValue)
  {
    WAIT(map(potiValue, 0, 1023, 0, 1000));

    for(int i = 0; i <4; ++i)
    {
      if(abs(target[i] - current[i]) < 3)
      {//select new target
        target[i] = random(0, 256);
        dir[i] = random(0,2) >= 1? 1 : -1;
      }      
      current[i] += dir[i];
    }
    
    int t = 0;
    for(int led = 0; led < NUM_LEDS; ++led, t = (t + 1) % 4)
    {
      colors[led].h = current[t];
      colors[led].s = 255;
      colors[led].v = 255;
    }
  }
   
  virtual bool msgChanged()
  {
    return false;
  }
  
  virtual const char* getName() 
  {
    return "Fade";
  }

private:
  unsigned char target[4];
  unsigned char current[4];
  char dir[4];
};
