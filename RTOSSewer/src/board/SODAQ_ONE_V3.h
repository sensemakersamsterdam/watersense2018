#include "../Common.h"

#if USE_BOARD_SODAQ_ONE_V3 && !defined(SODAQ_ONE_V3_H)
#define SODAQ_ONE_V3_H


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void Board_setup();


/*******************************************************************************
 * Public
 ******************************************************************************/

extern "C" void Board_fatalShutdown();

void Board_setLed(uint8_t rgb);

extern "C" uint32_t Board_sleep(uint32_t ms);


/*******************************************************************************
 * Private
 ******************************************************************************/

uint32_t Board_sleep_RTC(uint32_t ms);
uint32_t Board_sleep_WDT(uint32_t ms);

#endif // USE_BOARD_SODAQ_ONE_V3 && !defined(SODAQ_ONE_V3_H)
