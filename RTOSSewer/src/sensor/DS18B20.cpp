#include <OneWire.h>
#include <DallasTemperature.h>

#include "DS18B20.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define PIN_ONEWIREBUS 6
#define DEVICE_INDEX   0 // device 1 => index 0


/*******************************************************************************
 * State
 ******************************************************************************/

static OneWire oneWire(PIN_ONEWIREBUS);
static DallasTemperature sensors(&oneWire);


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

bool DS18B20_setup()
{
  sensors.begin();

  DeviceAddress deviceAddress;
  bool b = sensors.getAddress(deviceAddress, DEVICE_INDEX);

  LOG_SETUP_RESULT_TEXT(b);

  return b;
}


/*******************************************************************************
 * Public
 ******************************************************************************/

float DS18B20_measureTemperature()
{
  sensors.requestTemperatures();
  return sensors.getTempCByIndex(DEVICE_INDEX);
}
