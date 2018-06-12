#include "ARDUINO_MEGA2560_R3.h"

#if USE_BOARD && ARDUINO_AVR_MEGA2560


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void Board_setup()
{
  #if USE_LOGGER_BOARD
  LOGS("started");
  #endif
}

#endif // USE_BOARD && ARDUINO_AVR_MEGA2560
