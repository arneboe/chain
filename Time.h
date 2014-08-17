//a simple macro that can be used to ensure that a certain time has passed. If the time has not passed the macro returns
#define wait(time) \
  static int lastTime = 0; \
  static int currentTime = 0; \
  currentTime = millis(); \
  if(currentTime - lastTime >= time) \
  { \
    lastTime = currentTime; \
  } \
  else \
  { \
    return; \
  }
