#include "../Common.h"

#if USE_BOARD_ARDUINO_MEGA2560_R3 && !defined(ARDUINO_MEGA2560_R3_H)
#define ARDUINO_MEGA2560_R3_H


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void Board_setup();


/*******************************************************************************
 * Public
 ******************************************************************************/

extern "C" uint32_t Board_sleep(uint32_t ms);

void Board_setLed(uint8_t state);

#endif // USE_BOARD_ARDUINO_MEGA2560_R3 && !defined(ARDUINO_MEGA2560_R3_H)
