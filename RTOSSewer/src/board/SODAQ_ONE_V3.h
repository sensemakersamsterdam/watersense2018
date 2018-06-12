#include "../Common.h"

#if USE_BOARD && !defined(SODAQ_ONE_V3_H) && defined(ARDUINO_SODAQ_ONE)
#define SODAQ_ONE_V3_H

/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void Board_setup();


/*******************************************************************************
 * Public
 ******************************************************************************/

#if USE_BOARD_LED
void Board_toggleLedBlue();
void Board_toggleLedGreen();
void Board_toggleLedRed();
void Board_turnOffLedBlue();
void Board_turnOffLedGreen();
void Board_turnOffLedRed();
void Board_turnOnLedBlue();
void Board_turnOnLedGreen();
void Board_turnOnLedRed();
#endif // USE_BOARD_LED


/*******************************************************************************
 * Private
 ******************************************************************************/

#if USE_BOARD_LED
static void Board_setLed(uint8_t pin, uint8_t state);
#endif

#endif // USE_BOARD && !defined(SODAQ_ONE_V3_H) && defined(ARDUINO_SODAQ_ONE)
