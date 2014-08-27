#pragma once
#include "Equalizer.h"
#include "RingBuffer.h"

#define PIN_MSG_OUT A5
#define PIN_MSG_STROBE 11
#define PIN_MSG_RESET 10


struct MusicFadeData
{
  char text[30];
  float fadeDecay;
  float fadeSpeed;
  bool bassDetected;
  unsigned char h;
  short avgs[2];
  float D;
  bool rising;
  char dir;
  unsigned long lastTime;
};
MusicFadeData mfData;
Equalizer eq(PIN_MSG_OUT, PIN_MSG_STROBE, PIN_MSG_RESET);

void musicFadeInit()
{
  mfData.fadeDecay = 0.5;
  mfData.fadeSpeed = 0.0; //per ms
  mfData.bassDetected = false;
  mfData.h = 0; //current fade target
  mfData.avgs[0] = 0;
  mfData.avgs[1] = 0;
  mfData.D = 8; 
  mfData.rising = true;
  mfData.dir = 1;
  mfData.lastTime = millis();
}

char* musicFadeUpdate(int potiValue, CHSV* colors, int colorSize)
{
  mfData.D = map(potiValue, 0, 1023, 1, 300);

  //WAIT(map(potiValue, 0, 1023, 0, 1000), htswData.text);
  eq.update();  
  
  //we ignore high frequencies
  const short bass = eq.data[0];
  const short aboveBass = eq.data[1];
  
  //see http://donlehmanjr.com/Science/03%20Decay%20Ave/032.htm
  const short oldAvg = mfData.avgs[0];
  mfData.avgs[0] = bass / mfData.D + (mfData.D - 1) / mfData.D * mfData.avgs[0];
  
  unsigned long currentTime = millis();
  const short dt = currentTime - mfData.lastTime;
  
  const short increment = dt * mfData.fadeSpeed * mfData.dir;
  if(abs(increment) > 0)
  {
    mfData.lastTime = currentTime;
    //fade
    for(int i = 0; i < colorSize; ++i)
    {
      colors[i].h += increment;
    }
  }
  
  //if the fade target has been reached, choose a ne
  if(abs(short(colors[0].h) - short(mfData.h)) < 5)
  {
    mfData.h += 126; //wrap around
    mfData.dir = random(0,2) == 0 ? 1 : -1;
  }
  
  mfData.fadeSpeed *= mfData.fadeDecay;
  if(mfData.fadeSpeed < 0.1) mfData.fadeSpeed = 0.1;
  
  
  
  if(mfData.bassDetected)
  {//avoid to detect two peaks close together
    WAIT(70, mfData.text);//will return till time is up
    mfData.bassDetected = false;
  }
  
  short peak = 0;
  if(oldAvg > mfData.avgs[0] && mfData.rising)
  {//no longer rising, peak detected
    peak = 1000;
    mfData.rising = false;
    mfData.bassDetected = true;
    mfData.fadeSpeed = 0.45;
  }
  else if(mfData.avgs[0] > oldAvg && (!mfData.rising))
  {
    mfData.rising = true;
  }
  
  //mfData.avgs[1] = aboveBass / mfData.D + (mfData.D - 1) / mfData.D * mfData.avgs[0];
  

  
  
  int16_t comPkt[6];   //Buffer to hold the packet, note it is 16bit data type
  
  comPkt[0] = 0xCDAB;                     //Packet Header, indicating start of packet.
  comPkt[1] = 8;                          //Size of data payload in bytes.
  comPkt[2] = (int16_t) bass;    //Channel 1 data. 16bit signed integer
  comPkt[3] = (int16_t) mfData.avgs[0];    //Channel 2 data. 16bit signed integer
  comPkt[4] = (int16_t) peak;    //Channel 3 data. 16bit signed integer
  comPkt[5] = (int16_t) (mfData.fadeSpeed * 1000.0f);    //Channel 4 data. 16bit signed integer
  Serial.write((uint8_t *)comPkt, 12);

 //Serial_sendBuffer(USART1, (uint8_t *) comPkt, 12);
  
  
  if(bass >= mfData.avgs[0] * 1.3) //|| 
    // &&aboveBass >= mfData.avgs[1] * 1.5)
    {//bass detected
      mfData.bassDetected = true; //this will cause small wait on the next iteration
      //switch color
      mfData.h += 126;
      for(int i = 0; i < colorSize; ++i)
      {
        colors[i].h = mfData.h;
        colors[i].s = 255;
        colors[i].v = 255;
      }
    }
  char buf[8];
  dtostrf(mfData.D, 5, 2, buf);
  sprintf(mfData.text, "D: %s", buf);
  return mfData.text;
}

