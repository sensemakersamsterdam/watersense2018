#include "../Common.h"

#if USE_LORA && !defined(LORA_H)
#define LORA_H


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void LoRa_setup();


/*******************************************************************************
 * Public
 ******************************************************************************/

bool LoRa_send(const uint8_t *buffer, uint8_t size);
void LoRa_sendTest();


/*******************************************************************************
 * Private
 ******************************************************************************/

static bool LoRa_initOTA();

#if USE_LOGGER_LORA_DEVICEINFO
static void LoRa_logDeviceInfo();
#endif

#if USE_LOGGER_LORA
static void LoRa_logTransmissionResult(uint8_t result);
#endif

#endif // USE_LORA && defined(LORA_H)
