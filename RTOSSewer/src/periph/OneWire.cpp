/*
RTOSSewer project: sensing water quality in a variety of ways.

Copyright (C) 2018 Sensemakers Amsterdam. All rights reserved.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <OneWire.h>

#include "OneWire.h"


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
