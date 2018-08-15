#include "../Common.h"

#if USE_BOARD_ARDUINO_UNO_R3 && !defined(ARDUINO_UNO_R3_H)
#define ARDUINO_UNO_R3_H


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void Board_setup();


/*******************************************************************************
 * Public
 ******************************************************************************/

extern "C" uint32_t Board_sleep(uint32_t ms);

void Board_setLed(uint8_t state);

#endif // USE_BOARD_ARDUINO_UNO_R3 && !defined(ARDUINO_UNO_R3_H)
