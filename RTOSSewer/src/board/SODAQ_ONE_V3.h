#include "../Common.h"

#ifndef SODAQ_ONE_V3_H
#define SODAQ_ONE_V3_H


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void Board_setup();


/*******************************************************************************
 * Public
 ******************************************************************************/

extern "C" void     Board_fatalShutdown();
extern "C" uint32_t Board_sleep(uint32_t ms);

uint16_t Board_measureVoltage();
void     Board_setLed(uint8_t rgb);

#endif // SODAQ_ONE_V3_H
