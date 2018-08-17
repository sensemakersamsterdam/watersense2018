#include "../Common.h"

#ifndef SODAQ_MOJA_R4_H
#define SODAQ_MOJA_R4_H


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void Board_setup();


/*******************************************************************************
 * Public
 ******************************************************************************/

extern "C" uint32_t Board_sleep(uint32_t ms);

void Board_measure();
void Board_setLed(uint8_t state);

extern "C" void Board_fatalShutdown();

#endif // SODAQ_MOJA_R4_H
