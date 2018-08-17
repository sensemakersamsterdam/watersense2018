#include "ARDUINO_UNO_R3.h"

#if USE_BOARD_ARDUINO_UNO_R3

#include "../periph/I2C.h"


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void Board_setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN, LOW);

  #if USE_I2C
  I2C_setup();
  #endif

  LOG_SETUP_RESULT_TEXT(true);
}


/*******************************************************************************
 * Public
 ******************************************************************************/

void Board_fatalShutdown()
{
  LOGS("FATAL ERROR! SHUTDOWN");

  taskDISABLE_INTERRUPTS();

  for (;;) { Board_sleep(60 * 60 * 1000); }
}

// TODO: implement Board_measure
void Board_measure() {}

void Board_setLed(uint8_t state)
{
  digitalWrite(LED_BUILTIN, state);
}

// TODO: implement Board_sleep
uint32_t Board_sleep(uint32_t ms) { return 0; }

#endif // USE_BOARD_ARDUINO_UNO_R3
