#include <OneWire.h>

#include "OneWire.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define PIN_ONEWIREBUS 6


/*******************************************************************************
 * Public
 ******************************************************************************/

void OneWire_logDevices()
{
  LOGS("scanning...");

  OneWire ow(PIN_ONEWIREBUS);

  uint8_t address[8];

  if (!ow.search(address)) {
    LOGS("no devices found");
    return;
  }

  LOG(VS("rom: "), VUI8AH02(address, sizeof(address)));

  uint8_t count = 0;
  do {
    count++;

    if (OneWire::crc8(address, 7) != address[7]) {
      LOG(VS("device "), VUI8(count), VS(": error! CRC is not valid!"));
      break;
    }

    switch (address[0]) {
      case 0x10: LOG(VS("device "), VUI8(count), VS(": DS18S20")); break;
      case 0x22: LOG(VS("device "), VUI8(count), VS(": DS1822"));  break;
      case 0x28: LOG(VS("device "), VUI8(count), VS(": DS18B20")); break;
      default  : LOG(VS("device "), VUI8(count), VS(": unknown"));
    }
  } while (ow.search(address));

  LOGS("scanning done");
}
