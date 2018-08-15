#include "../Common.h"

#ifndef RTOS_H
#define RTOS_H

/*******************************************************************************
 * Public
 ******************************************************************************/

void RTOS_delay(TickType_t xTimeIncrement);

void RTOS_delay(TickType_t *pxPreviousWakeTime, TickType_t xTimeIncrement);

#endif // RTOS_H
