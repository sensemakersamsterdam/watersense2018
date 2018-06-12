#include "../Common.h"

#if USE_BOARD && !defined(ARDUINO_MEGA2560_R3_H) && defined(ARDUINO_AVR_MEGA2560)
#define ARDUINO_MEGA2560_R3_H

/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void Board_setup();

#endif // USE_BOARD && !defined(ARDUINO_MEGA2560_R3_H) && defined(ARDUINO_AVR_MEGA2560)
