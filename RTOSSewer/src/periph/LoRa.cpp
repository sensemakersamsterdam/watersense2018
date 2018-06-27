#include "LoRa.h"

#if USE_LORA

#include <TheThingsNetwork.h>


/*******************************************************************************
 * State
 ******************************************************************************/

static TheThingsNetwork ttn(Serial1, SerialUSB, TTN_FP_EU868);


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void LoRa_setup()
{
  Serial1.begin(57600);

  #if USE_LOGGER_LORA
  LOGS("started");
  #endif

  #if USE_LOGGER_LORA_DEVICEINFO
  LoRa_logDeviceInfo();
  #endif
}


/*******************************************************************************
 * Private
 ******************************************************************************/

#if USE_LOGGER_LORA_DEVICEINFO
static void LoRa_logDeviceInfo()
{
  LOGS("receiving...");
  ttn.showStatus();
  LOGS("done");
}
#endif

#endif // USE_LORA
