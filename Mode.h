#pragma once
#include "Time.h"
#include "Color.h"

class Mode 
{
public:

  Mode(CHSV* colors, const int len) : colors(colors), colorSize(len) {}

  virtual void activate() = 0; //is called every time the mode is activated, should be used to initialize attributes etc.
  virtual void update() = 0; //is called every frame, if this mode is selected
  virtual bool msgChanged() = 0; //returns true if the msg has changed since the last call to msgChanged()
  virtual const char* getName() = 0; //the name of this mode (needs to be short to fit in one display line)  

protected:
  CHSV* colors;
  int colorSize;
};
