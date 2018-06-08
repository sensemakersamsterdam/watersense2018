#ifndef SODAQONE_H
#define SODAQONE_H

#include "../Common.h"

class SODAQONE {
public:
  static void setup();
  static void toggleLedBlue();
  static void toggleLedGreen();
  static void toggleLedRed();
  static void turnOffLedBlue();
  static void turnOffLedGreen();
  static void turnOffLedRed();
  static void turnOnLedBlue();
  static void turnOnLedGreen();
  static void turnOnLedRed();
private:
  static SemaphoreHandle_t ledMutex;
  static uint8_t ledState;
  static void setLed(uint8_t pin, uint8_t state);
};

#endif
