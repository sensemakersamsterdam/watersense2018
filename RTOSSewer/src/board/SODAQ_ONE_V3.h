#include "../Common.h"

#if USE_BOARD_SODAQ_ONE_V3 && !defined(SODAQ_ONE_V3_H)
#define SODAQ_ONE_V3_H


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void Board_setup();

extern "C" uint32_t Board_sleep(uint32_t ms);


/*******************************************************************************
 * Public
 ******************************************************************************/

#if USE_BOARD_LED
void Board_setLed(uint8_t rgb);
#endif


/*******************************************************************************
 * Private
 ******************************************************************************/

static uint32_t Board_wdt_enable(uint32_t ms);

#endif // USE_BOARD_SODAQ_ONE_V3 && !defined(SODAQ_ONE_V3_H)
