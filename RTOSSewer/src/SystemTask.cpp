#include <Arduino.h>
#include "SystemTask.h"

void SystemTask::setup()
{
  Serial.begin(9600);
  delay(200);
  while (!Serial && millis() < 10000);
}

void SystemTask::loop()
{
  Serial.print(millis());
  Serial.println(" idle");
  delay(1000);
}
