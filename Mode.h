#pragma once
#include "Time.h"
#include "Color.h"
template <int NUM_COLORS>
class Mode 
{
public:

  virtual void activate() = 0; //is called every time the mode is activated, should be used to initialize attributes etc.
  virtual void update() = 0; //is called every frame, if this mode is selected
  virtual bool msgChanged() = 0; //returns true if the msg has changed since the last call to msgChanged()
  void getColors(Color*& cols, int& len)
  {
    cols = &colors[0];
    len = NUM_COLORS;
  }

protected:
  String name; //the name of this mode (needs to be short to fit in one display line)  
  String msg; //some message, will be placed in the second line of the screen
  Color colors[NUM_COLORS];
};
