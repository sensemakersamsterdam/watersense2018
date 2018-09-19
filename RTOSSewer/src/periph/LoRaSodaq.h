#include "../Common.h"

#ifndef LORA_SODAQ_H
#define LORA_SODAQ_H


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

bool LoRa_setup();


/*******************************************************************************
 * Public
 ******************************************************************************/

bool    LoRa_initOTAA();
uint8_t LoRa_send(const uint8_t *buffer, uint8_t size); // return 0: OK, 1: not sent, 2: not connected
void    LoRa_sleep();
void    LoRa_setSpreadingFactor(uint8_t spreadingFactor);
void    LoRa_wakeUp();

#endif // LORA_SODAQ_H
