#ifndef SYSTEMTHREAD_H
#define SYSTEMTHREAD_H

class BlinkThread {
public:
  static void setup();
  static void loop();
private:
  static int blinkFlag;
};

#endif
