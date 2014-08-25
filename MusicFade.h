#pragma once
#include "Mode.h"
#include "Equalizer.h"
#include "RingBuffer.h"

#define PIN_MSG_OUT A5
#define PIN_MSG_STROBE 11
#define PIN_MSG_RESET 10


//modulo that handles negative correctly
int mod(int x, int m) 
{
    return (x%m + m)%m;
}



template <int NUM_LEDS>
class MusicFade : public Mode 
{
public:

  MusicFade(CHSV* colors) : Mode(colors, NUM_LEDS), eq(PIN_MSG_OUT, PIN_MSG_STROBE, PIN_MSG_RESET), currentMillis(0),
                            lastMillis(0)  {}

  virtual void activate()
  {
  }
  
  virtual void update(const int potiValue)
  {
    static RingBuffer<short, 40> buffers[7];
    const int noise = 120;
    currentMillis = millis();
    static float fadeSpeedMs = 1.0; //how many degrees should the fade move per ms
    static int direction = 1;
    const float fadeDecay = 0.7;
    const int targetOffset = 5;
    const int timeDiff = currentMillis - lastMillis;
    eq.update();
      
    if(abs(fadeTarget - currentColor.h) <= targetOffset)
    {
      fadeTarget = (currentColor.h + 128 + random(-targetOffset, targetOffset)) % 256;
      if(random(2) >= 1) direction = 1; else direction = -1;    
    }
    
    for(int i = 0; i < 7; ++i)
    {
      if(eq.data[i] < noise) break;
      buffers[i].push(eq.data[i]);
      if(eq.data[i] >= buffers[i].getAvg() * 1.5)
      {
        fadeSpeedMs = 0.45;
        break;
      }
    }
     
    const float offset = fadeSpeedMs * timeDiff;
    if(offset >= 1)//else wait a little bit more
    {
      //do the fading
      currentColor.h = mod(currentColor.h + (direction * offset), 256);
      currentColor.s = 255;
      currentColor.v = 255;
      fadeSpeedMs *= fadeDecay;
      lastMillis = currentMillis;
      
      for(int i = 0; i < NUM_LEDS; ++i)
      {
        colors[i] = currentColor;
      }
      
    }
  }
   
  virtual bool msgChanged()
  {
    return false;
  }
  
  virtual const char* getName() 
  {
    return "Music Fade";
  }

private:
  Equalizer eq;
  unsigned currentMillis;
  unsigned lastMillis;
  unsigned char fadeTarget;
  CHSV currentColor;  
};
