#ifndef SYSTEM_H
#define SYSTEM_H

#include "Common.h"

class System {
public:
  static void setup();
  static void idle();
  static void scanI2C();
  static void toggleGreenLed();
private:
  static uint8_t ledGreenState;
};

#endif
