#ifndef SYSTEM_H
#define SYSTEM_H

class System {
public:
  static void setup();
  static void idle();
  static void scanI2C();
};

#endif
