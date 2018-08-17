#include "../Common.h"

#ifndef ARDUINO_MEGA2560_R3_H
#define ARDUINO_MEGA2560_R3_H


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

#endif // ARDUINO_MEGA2560_R3_H
