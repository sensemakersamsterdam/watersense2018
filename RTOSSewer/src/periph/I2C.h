#ifndef I2C_H
#define I2C_H

#include "../Common.h"

class I2C {
public:
  static void setup();
  static bool lock();
  static void logDevices();
  static void unlock();
private:
  static SemaphoreHandle_t busMutex;
};

#endif
