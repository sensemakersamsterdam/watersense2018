#ifndef SEWER_H
#define SEWER_H

#include "Common.h"

class Sewer {
public:
  static void setup();
  static void idle();
private:
  static void initSystem();
  static void threadT0(void* pvParameters);
  static void threadT1(void* pvParameters);
  static void threadT2(void* pvParameters);
  static void threadT3(void* pvParameters);
};

#endif
