#include <Arduino.h>
#include <Wire.h>
#include "Common.h"
#include "System.h"

/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void System::setup()
{
  Serial.begin(9600);
  Wire.begin();
  delay(200);
  while (!Serial && millis() < 10000);
}


/*******************************************************************************
 * Public
 ******************************************************************************/

void System::idle()
{
  Serial.print("Idle: ");
  Serial.println(millis());
  delay(1500);
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

  Serial.println(nDevices ? "I2C: scanning done" : "I2C: no devices found");
}
