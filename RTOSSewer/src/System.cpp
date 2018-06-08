#include <Arduino.h>
#include <Wire.h>
#include "System.h"

uint8_t System::ledGreenState = HIGH;

void System::setup()
{
  pinMode(LED_GREEN, OUTPUT);
  digitalWrite(LED_GREEN, ledGreenState);

  Serial.begin(9600);
  Wire.begin();
  delay(200);
  while (!Serial && millis() < 10000);
}

void System::idle()
{
  Serial.print("Idle: ");
  Serial.println(millis());
  delay(1000);
}

void System::scanI2C()
{
  Serial.println("I2C: scanning...");

  int nDevices = 0;

  for(byte address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);

    byte error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C: device found at address 0x");
      if (address < 16) {
        Serial.print("0");
      }

      Serial.println(address, HEX);

      nDevices++;
    } else if (error == 4) {
      Serial.print("I2C: unknow error at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }
  }

  if (nDevices == 0) {
    Serial.println("I2C: no devices found");
  } else {
    Serial.println("I2C: done");
  }
}

void System::toggleGreenLed()
{
  ledGreenState ^= 1;
  digitalWrite(LED_GREEN, ledGreenState);
}
