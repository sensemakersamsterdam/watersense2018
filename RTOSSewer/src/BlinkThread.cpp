#include <Arduino.h>
#include "BlinkThread.h"

static int blinkFlag = LOW;

void BlinkThread::setup() {
  pinMode(LED_GREEN, OUTPUT);
  digitalWrite(LED_GREEN, HIGH);
}

void BlinkThread::loop() {
  digitalWrite(LED_GREEN, blinkFlag);
  blinkFlag ^= 1;
}
