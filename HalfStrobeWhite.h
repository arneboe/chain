#pragma once


struct HalfStrobeWhiteData
{
  bool off;
  char text[11];
};

HalfStrobeWhiteData htswData;

void halfStrobeWhiteInit()
{
  htswData.off = true;
}

char* halfStrobeWhiteUpdate(int potiValue, CHSV* colors, int colorSize)
{
  WAIT(map(potiValue, 0, 1023, 0, 1000), htswData.text);
  if(htswData.off)
  {
    htswData.off = false;
    //turn 1/2 of the leds on
    for(int i = 0; i < colorSize; ++i)
    {
      if(random(0,2) == 0)
      {
      colors[i].s = 0; 
      colors[i].v = 255;
      }
    }               
  }
  else
  {
    htswData.off = true;
    //turn all leds off
    for(int i = 0; i < colorSize; ++i)
    {
      colors[i].v = 0;
    }      
  }
  sprintf(htswData.text, "Speed: %d", map(potiValue, 0, 1023, 100, 0));
  return htswData.text;  
}

