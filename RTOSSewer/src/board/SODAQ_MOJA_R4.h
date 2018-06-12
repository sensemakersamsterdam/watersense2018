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

#if USE_BOARD_LED
void Board_toggleLed();
void Board_turnOffLed();
void Board_turnOnLed();
#endif


/*******************************************************************************
 * Private
 ******************************************************************************/

#if USE_BOARD_LED
static void Board_setLed(uint8_t state);
#endif

#endif // USE_BOARD_SODAQ_MOJA_R4 && !defined(SODAQ_MOJA_R4_H)
