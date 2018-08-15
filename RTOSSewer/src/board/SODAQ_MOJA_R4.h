#include "../Common.h"

#if USE_BOARD_SODAQ_MOJA_R4 && !defined(SODAQ_MOJA_R4_H)
#define SODAQ_MOJA_R4_H


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void Board_setup();


/*******************************************************************************
 * Public
 ******************************************************************************/

extern "C" uint32_t Board_sleep(uint32_t ms);

void Board_setLed(uint8_t state);

#endif // USE_BOARD_SODAQ_MOJA_R4 && !defined(SODAQ_MOJA_R4_H)
