#include "ARDUINO_MEGA2560_R3.h"

#if USE_BOARD_ARDUINO_MEGA2560_R3


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void Board_setup()
{
  #if USE_LOGGER_BOARD
  LOGS("started");
  #endif
}

#endif // USE_BOARD_ARDUINO_MEGA2560_R3
