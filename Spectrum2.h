#pragma once

struct Spectrum2Data
{
  char text[20];
  float avg;
  float D;
  short min;
  short max;
  unsigned char h1;
  unsigned char h2;
};
Spectrum2Data s2Data;


void spectrum2Init()
{
  s2Data.avg = 0;
  s2Data.D = 3;
  s2Data.min = 0;
  s2Data.max = 0;
  s2Data.h1 = 0;
  s2Data.h2 = 128;
}

char* spectrum2Update(int potiValue, CHSV* colors, int colorSize)
{
  //WAIT(map(potiValue, 0, 1023, 0, 1000), htswData.text);
  eq.update();  
  
  short val = 0;
  for(int i = 0; i < 7; ++i)
  {
    if(eq.data[i] > val) val = eq.data[i];
  }
  if(s2Data.max < val) s2Data.max = val;
  if(s2Data.min > val) s2Data.min = val;
  s2Data.avg = val / s2Data.D + (s2Data.D - 1) / s2Data.D * s2Data.avg;
  
  const short upper = map(s2Data.avg, s2Data.min, s2Data.max,1, 20);
  for(short i = 0; i < upper; ++i)
  {
    colors[i].h = s2Data.h1;
    colors[i].s = 255;
    colors[i].v = 255;
  }
  
  int16_t comPkt[6];   //Buffer to hold the packet, note it is 16bit data type
  
  comPkt[0] = 0xCDAB;                     //Packet Header, indicating start of packet.
  comPkt[1] = 2;                          //Size of data payload in bytes.
  comPkt[2] = (int16_t) upper * 100;    //Channel 1 data. 16bit signed integer
  //comPkt[3] = (int16_t) mfData.avgs[0];    //Channel 2 data. 16bit signed integer
 // comPkt[4] = (int16_t) peak;    //Channel 3 data. 16bit signed integer
//  comPkt[5] = (int16_t) (mfData.fadeSpeed * 1000.0f);    //Channel 4 data. 16bit signed integer
  Serial.write((uint8_t *)comPkt, 6);
  
  for(int i = upper; i < colorSize; ++i)
  {
    colors[i].h = s2Data.h2;
    colors[i].s = 255;
    colors[i].v = 0;
  }
    

  ++s2Data.min;
  --s2Data.max;      
  
  ++s2Data.h1;
  --s2Data.h2;

  sprintf(s2Data.text, "bla");
  return s2Data.text;
}

