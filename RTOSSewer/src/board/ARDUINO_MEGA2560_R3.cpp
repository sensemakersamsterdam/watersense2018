#include "ARDUINO_MEGA2560_R3.h"

#if USE_BOARD_ARDUINO_MEGA2560_R3

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

  LOGS("ARDUINO_MEGA2560_R3 started");
}


/*******************************************************************************
 * Public
 ******************************************************************************/

// TODO: implement Board_measure
void Board_measure() {}

void Board_setLed(uint8_t state)
{
  digitalWrite(LED_BUILTIN, state);
}

// TODO: implement Board_sleep
uint32_t Board_sleep(uint32_t ms) { return 0; }

#endif // USE_BOARD_ARDUINO_MEGA2560_R3
