#pragma once

struct SpectrumData
{
  char text[1];
  float avg[3];
  float D;
  short min[3];
  short max[3];
  unsigned short frameCounter;
};
SpectrumData sData;


void spectrumInit()
{
  for(int i = 0; i < 3; ++i)
  {
    sData.avg[i] = 0;
    sData.min[i] = 0;
    sData.max[i] = 0;
  }
  sData.D = 8;
}

char* spectrumUpdate(int potiValue, CHSV* colors, int colorSize)
{
  sData.D = map(potiValue, 0, 1023, 1, 300);

  //WAIT(map(potiValue, 0, 1023, 0, 1000), htswData.text);
  eq.update();  
  
  //calculate moving averages, mins and maxes
  for(int i = 0; i < 6; i += 2)
  {
     const char index = i >> 1;
    const short val = map(eq.data[i] + eq.data[i+1], 0, 2048, 0, 1023);
    sData.avg[index] = val / sData.D + (sData.D - 1) / sData.D * sData.avg[index];
    if(val > sData.max[index]) sData.max[index] = val;
    if(val < sData.min[index]) sData.min[index] = val;
  }

  static unsigned char col = 0;
  ++col;
  for(int i = 0; i < 7; ++i)
  {
    colors[i].h = col;//map(sData.avg[0], 0, 1023, 0, 255);
    colors[i].s = 255;
    colors[i].v = 255;
  }
  for(int i = 7; i < 14; ++i)
  {
    colors[i].h = col + 100;//map(sData.avg[1], 0, 1023, 0, 255);
    colors[i].s = 255;
    colors[i].v = 255;
  }  

  for(int i = 14; i < colorSize; ++i)
  {
    colors[i].h = col+ 180;//map(sData.avg[2], 0, 1023, 0, 255);
    colors[i].s = 255;
    colors[i].v = 255;
  }

  for(int i = 0; i < 3; ++i)
  {
    ++sData.min[i];
    --sData.max[i];      
  }

  char buf[9];
  dtostrf(sData.D, 5, 2, buf);
  sprintf(sData.text, "D: %s", buf);
  return sData.text;
}

