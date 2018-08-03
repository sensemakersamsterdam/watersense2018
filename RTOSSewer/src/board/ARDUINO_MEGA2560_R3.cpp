#include "ARDUINO_MEGA2560_R3.h"

#if USE_BOARD_ARDUINO_MEGA2560_R3


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
  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN, LOW);

  #if USE_BOARD_LED
  static StaticSemaphore_t ledMutexBuffer;
  ledState = LOW;
  ledMutex = xSemaphoreCreateMutexStatic(&ledMutexBuffer);
  #endif

  #if USE_LOGGER_BOARD
  LOGS("ARDUINO_MEGA2560_R3 started");
  #endif
}

// TODO: implement Board_sleep
uint32_t Board_sleep(uint32_t ms) { return 0; }


/*******************************************************************************
 * Public
 ******************************************************************************/

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
  digitalWrite(LED_BUILTIN, state);

  xSemaphoreGive(ledMutex);
}
#endif // USE_BOARD_LED

#endif // USE_BOARD_ARDUINO_MEGA2560_R3
