#ifndef TOFTASK_H
#define TOFTASK_H

#include "../Task.h"

class TOFTask {
public:
  static bool isReady;
  static void setup();
  static void loop();
private:
  static int poweronFlag;
};

#endif
