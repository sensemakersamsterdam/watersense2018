#include "SODAQ_MOJA_R4.h"

#if USE_BOARD_SODAQ_MOJA_R4

#include "../periph/I2C.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define PIN_LED 6


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void Board_setup()
{
  pinMode(PIN_LED, OUTPUT);

  digitalWrite(PIN_LED, LOW);

  #if USE_I2C
  I2C_setup();
  #endif

  LOGS("SODAQ_MOJA_R4 started");
}


/*******************************************************************************
 * Public
 ******************************************************************************/

// TODO: implement Board_measure
void Board_measure() {}

void Board_setLed(uint8_t state)
{
  digitalWrite(PIN_LED, state);
}

// TODO: implement Board_sleep
uint32_t Board_sleep(uint32_t ms) { return 0; }


/*******************************************************************************
 * Private
 ******************************************************************************/

#endif // USE_BOARD_SODAQ_MOJA_R4
