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

#include <Sodaq_RN2483.h>

#include "../periph/WDT.h"
#include "LoRaSodaq.h"


/*******************************************************************************
 * Configuration
 ******************************************************************************/

#include "../LoRaConfig.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define JOINNETWORK_MAX_RETRIES 3
#define SEND_MAX_RETRIES        3

#define SERIAL_DEBUG SerialUSB
#define SERIAL_LORA  Serial1


/*******************************************************************************
 * Private declarations
 ******************************************************************************/

static void LoRa_logDeviceInfo();
static void LoRa_logTransmissionResult(uint8_t result);


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

bool LoRa_setup()
{
  LOGS("starting...");

  SERIAL_LORA.begin(LoRaBee.getDefaultBaudRate());

  #if USE_LOGGER
  LoRaBee.setDiag(SERIAL_DEBUG);
  #endif

  bool b = LoRaBee.init(SERIAL_LORA, LORA_RESET);

  if (b) {
    #if USE_LOGGER
    LoRa_logDeviceInfo();
    #endif

    LoRa_sleep();
  }

  LOG_SETUP_RESULT_TEXT(b);

  return b;
}


/*******************************************************************************
 * Public
 ******************************************************************************/

bool LoRa_initOTAA()
{
  uint8_t eui[8];

  if (LoRaBee.getHWEUI(eui, sizeof(eui)) != 8) {
    LOGS("LoRa is down");
    return false;
  }

  bool b = false;

  for (uint8_t i = 0; i < JOINNETWORK_MAX_RETRIES; i++) {
    LOG(VS("init OTAA: attempt "), VUI8(i), VS("..."));

    LoRaBee.hardwareReset();

    if (LoRaBee.initOTA(eui, APP_EUI, APP_KEY, false)) { b = true; break; }
  }

  LOG(b ? VS("accepted") : VS("denied"));

  return b;
}

uint8_t LoRa_send(const uint8_t *buffer, uint8_t size)
{
  for (uint8_t i = 0; i < SEND_MAX_RETRIES; i++) {
    LOG(VS("send: attempt "), VUI8(i), VS("..."));

    uint8_t result = LoRaBee.send(1, buffer, size);

    #if USE_LOGGER
    LoRa_logTransmissionResult(result);
    #endif

    if (result == NoError) { return 0; }

    if (result == Busy || result == Timeout) {
      WDT_disable();
      vTaskDelay(pdMS_TO_TICKS(10000));
      WDT_enable();
      continue;
    }

    if (result == NoResponse || result == InternalError || result == NetworkFatalError || result == NotConnected) {
      if (!LoRa_initOTAA()) { return 2; }
      continue;
    }

    if (result == PayloadSizeError) { return 1; }
  }

  return 1;
}

void LoRa_sleep()
{
  LoRaBee.sleep();

  LOGS("sleep mode on");
}

void LoRa_setSpreadingFactor(uint8_t spreadingFactor)
{
  LoRaBee.setSpreadingFactor(spreadingFactor);

  LOG(VS("spreading factor: "), VUI8(spreadingFactor));
}

void LoRa_wakeUp()
{
  LoRaBee.wakeUp();

  LOGS("active mode on");
}


/*******************************************************************************
 * Private
 ******************************************************************************/

static void LoRa_logDeviceInfo()
{
  union {
    char version[32];
    uint8_t eui[8];
  } u;

  uint8_t i = LoRaBee.getVersion(u.version, sizeof(u.version));
  if (i > 1) { LOG(VS("version: "), VSZ(u.version)); } else { LOGS("version: N/A"); }

  i = LoRaBee.getHWEUI(u.eui, sizeof(u.eui));
  if (i > 0) { LOG(VS("device EUI: "), VUI8AH02(u.eui, i)); } else { LOGS("device EUI: N/A"); }
}

static void LoRa_logTransmissionResult(uint8_t result)
{
  switch (result)
  {
    case NoError:           LOGS("NoError");           break;
    case NoResponse:        LOGS("NoResponse");        break;
    case Timeout:           LOGS("Timeout");           break;
    case PayloadSizeError:  LOGS("PayloadSizeError");  break;
    case InternalError:     LOGS("InternalError");     break;
    case Busy:              LOGS("Busy");              break;
    case NetworkFatalError: LOGS("NetworkFatalError"); break;
    case NotConnected:      LOGS("NotConnected");      break;
    case NoAcknowledgment:  LOGS("NoAcknowledgment");  break;
    default:                LOGS("Unknown");           break; // should be impossible
  }
}
