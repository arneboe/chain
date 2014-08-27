#pragma once

//a simple macro that can be used to ensure that a certain time has passed. If the time has not passed the macro returns
#define WAIT(time, ret) \
  static unsigned long lastTime = 0; \
  static unsigned long currentTime = 0; \
  currentTime = millis(); \
  if(currentTime - lastTime >= time) \
  { \
    lastTime = currentTime; \
  } \
  else \
  { \
    return ret; \
  }

