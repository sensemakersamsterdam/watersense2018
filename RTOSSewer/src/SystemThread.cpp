#include <Arduino.h>
#include "SystemThread.h"

void SystemThread::setup() {
  SerialUSB.begin(9600);
  delay(200);
  while (!SerialUSB && millis() < 10000);
}

void SystemThread::loop() {
  SerialUSB.print(millis());
  SerialUSB.println(" idle");
  delay(1000);
}
