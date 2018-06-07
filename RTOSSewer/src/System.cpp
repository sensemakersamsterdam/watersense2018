#include <Arduino.h>
#include "System.h"

void System::setup()
{
  Serial.begin(9600);
  delay(200);
  while (!Serial && millis() < 10000);
}

void System::idle()
{
  Serial.print(millis());
  Serial.println(" idle");
  delay(1000);
}
