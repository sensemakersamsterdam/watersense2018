#include <Arduino.h>
#include "BlinkThread.h"

int BlinkThread::blinkFlag = LOW;

void BlinkThread::setup() {
  pinMode(LED_GREEN, OUTPUT);
  digitalWrite(LED_GREEN, HIGH);
}

void BlinkThread::loop() {
  digitalWrite(LED_GREEN, BlinkThread::blinkFlag);
  BlinkThread::blinkFlag ^= 1;
}
