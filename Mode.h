#pragma once
#include "Time.h"
#include "Color.h"

class Mode 
{
public:

  Mode(CHSV* colors, const int len) : colors(colors), colorSize(len) {}

  virtual void activate() = 0; //is called every time the mode is activated, should be used to initialize attributes etc.
  /*
  *@param potiValue A value from [0..1023] provided by the user interface. Can be used to select speed etc. or ignored
  */
  virtual void update(const int potiValue) = 0; //is called every frame, if this mode is selected
  virtual bool msgChanged() = 0; //returns true if the msg has changed since the last call to msgChanged()
  virtual const char* getName() = 0; //the name of this mode (needs to be short to fit in one display line)  

protected:
  CHSV* colors;
  int colorSize;
};
