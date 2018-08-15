#include "ARDUINO_UNO_R3.h"

#if USE_BOARD_ARDUINO_UNO_R3


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void Board_setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN, LOW);

  LOGS("ARDUINO_UNO_R3 started");
}


/*******************************************************************************
 * Public
 ******************************************************************************/

void Board_setLed(uint8_t state)
{
  digitalWrite(LED_BUILTIN, state);
}

// TODO: implement Board_sleep
uint32_t Board_sleep(uint32_t ms) { return 0; }

#endif // USE_BOARD_ARDUINO_UNO_R3
