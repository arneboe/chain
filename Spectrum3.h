#pragma once

struct Spectrum3Data
{
  char text[5];
  float avg;
  float D;
  short min;
  short max;
  unsigned char h;
};
Spectrum3Data s3Data;


void spectrum3Init()
{
  s3Data.avg = 0;
  s3Data.D = 3;
  s3Data.min = 0;
  s3Data.max = 0;
  s3Data.h = 0;
}

char* spectrum3Update(int potiValue, CHSV* colors, int colorSize)
{
  //WAIT(map(potiValue, 0, 1023, 0, 1000), htswData.text);
  eq.update();  
  
  short val = 0;
  for(int i = 0; i < 7; ++i)
  {
    if(eq.data[i] > val) val = eq.data[i];
  }
  if(s3Data.max < val) s3Data.max = val;
  if(s3Data.min > val) s3Data.min = val;
  s3Data.avg = val / s3Data.D + (s3Data.D - 1) / s3Data.D * s3Data.avg;
  
  short v = map(s3Data.avg, s3Data.min, s3Data.max * 0.8, 50, 255);
  if(v > 255) v = 255; //can happen because of the * 0.8 in map()
  for(short i = 0; i < colorSize; ++i)
  {
    colors[i].h = s3Data.h;
    colors[i].s = 255;
    colors[i].v = v;
  }
  

    

  ++s3Data.min;
  --s3Data.max;      
  
  ++s3Data.h;

  sprintf(s3Data.text, "bla");
  return s3Data.text;
}

