#ifndef BLINKTHREAD_H
#define BLINKTHREAD_H

class BlinkThread {
public:
  static void setup();
  static void loop();
private:
  static int blinkFlag;
};

#endif
