#include "../Common.h"

#ifndef WDT_H
#define WDT_H


/*******************************************************************************
 * Public
 ******************************************************************************/

uint32_t WDT_enable(uint32_t ms);

void WDT_disable();

#endif // WDT_H