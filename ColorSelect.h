#pragma once

struct ColorSelectData
{
  unsigned char h;
  char text[11];//Speed: XXX\0
};
static ColorSelectData csData;


void colorSelectInit()
{
    sprintf(csData.text, " ");
}

char* colorSelectUpdate(int potiValue, CHSV* colors, int colorSize)
{
  csData.h = map(potiValue, 0, 1023, 0, 255);
  unsigned char s = 255;
  //special case for white:
  if(csData.h == 0)
  {
    s = 0;
  }


  for(int led = 0; led < colorSize; ++led)
  {
    colors[led].h = csData.h;
    colors[led].s = s;
    colors[led].v = 255;
  }
  return csData.text;
}


