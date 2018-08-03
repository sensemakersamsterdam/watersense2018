#include "SODAQ_MOJA_R4.h"

#if USE_BOARD_SODAQ_MOJA_R4

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define PIN_LED 6


/*******************************************************************************
 * State
 ******************************************************************************/

#if USE_BOARD_LED
static uint8_t ledState;
static SemaphoreHandle_t ledMutex;
#endif


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void Board_setup()
{
  pinMode(PIN_LED, OUTPUT);

  digitalWrite(PIN_LED, LOW);

  #if USE_BOARD_LED
  static StaticSemaphore_t ledMutexBuffer;
  ledState = LOW;
  ledMutex = xSemaphoreCreateMutexStatic(&ledMutexBuffer);
  #endif

  #if USE_LOGGER_BOARD
  LOGS("SODAQ_MOJA_R4 started");
  #endif
}


/*******************************************************************************
 * Public
 ******************************************************************************/

// TODO: implement Board_sleep
uint32_t Board_sleep(uint32_t ms) { return 0; }

#if USE_BOARD_LED
void Board_toggleLed()
{
  Board_setLed(ledState ^ 1);
}

void Board_turnOffLed()
{
  Board_setLed(LOW);
}

void Board_turnOnLed()
{
  Board_setLed(HIGH);
}
#endif // USE_BOARD_LED


/*******************************************************************************
 * Private
 ******************************************************************************/

#if USE_BOARD_LED
void Board_setLed(uint8_t state)
{
  if (!xSemaphoreTake(ledMutex, 100)) {
    #if USE_LOGGER_BOARD
    LOGS("resource is busy");
    #endif
    return;
  }

  ledState = state;
  digitalWrite(PIN_LED, state);

  xSemaphoreGive(ledMutex);
}
#endif // USE_BOARD_LED

#endif // USE_BOARD_SODAQ_MOJA_R4
