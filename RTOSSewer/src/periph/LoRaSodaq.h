#include "../Common.h"

#if USE_LORA_SODAQ && !defined(LORA_SODAQ_H)
#define LORA_SODAQ_H


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

bool LoRa_setup();


/*******************************************************************************
 * Public
 ******************************************************************************/

bool LoRa_initOTAA();
bool LoRa_send(const uint8_t *buffer, uint8_t size);
void LoRa_sleep();
void LoRa_wakeUp();


/*******************************************************************************
 * Private
 ******************************************************************************/

static void LoRa_logDeviceInfo();
static void LoRa_logTransmissionResult(uint8_t result);

#endif // USE_SODAQ_LORA && defined(LORA_SODAQ_H)
