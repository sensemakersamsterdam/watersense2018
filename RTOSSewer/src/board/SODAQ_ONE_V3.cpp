#include "SODAQ_ONE_V3.h"

#if USE_BOARD_SODAQ_ONE_V3


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
  pinMode(PIN_LED_BLUE,  OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_RED,   OUTPUT);

  digitalWrite(PIN_LED_BLUE,  HIGH);
  digitalWrite(PIN_LED_GREEN, HIGH);
  digitalWrite(PIN_LED_RED,   HIGH);

  #if USE_BOARD_LED
  static StaticSemaphore_t ledMutexBuffer;
  ledState = HIGH;
  ledMutex = xSemaphoreCreateMutexStatic(&ledMutexBuffer);
  #endif

  #if USE_LOGGER_BOARD
  LOGS("SODAQ_ONE_V3 started");
  #endif
}


/*******************************************************************************
 * Public
 ******************************************************************************/

#if USE_BOARD_LED
void Board_toggleLedBlue()
{
  Board_setLed(PIN_LED_BLUE, ledState ^ 1);
}

void Board_toggleLedGreen()
{
  Board_setLed(PIN_LED_GREEN, ledState ^ 1);
}

void Board_toggleLedRed()
{
  Board_setLed(PIN_LED_RED, ledState ^ 1);
}

void Board_turnOffLedBlue()
{
  Board_setLed(PIN_LED_BLUE, HIGH);
}

void Board_turnOffLedGreen()
{
  Board_setLed(PIN_LED_GREEN, HIGH);
}

void Board_turnOffLedRed()
{
  Board_setLed(PIN_LED_RED, HIGH);
}

void Board_turnOnLedBlue()
{
  Board_setLed(PIN_LED_BLUE, LOW);
}

void Board_turnOnLedGreen()
{
  Board_setLed(PIN_LED_GREEN, LOW);
}

void Board_turnOnLedRed()
{
  Board_setLed(PIN_LED_RED, LOW);
}
#endif // USE_BOARD_LED


/*******************************************************************************
 * Private
 ******************************************************************************/

#if USE_BOARD_LED
void Board_setLed(uint8_t pin, uint8_t state)
{
  if (!xSemaphoreTake(ledMutex, 100)) {
    #if USE_LOGGER_BOARD
    LOGS("resource is busy");
    #endif
    return;
  }

  ledState = state;
  digitalWrite(pin, state);

  xSemaphoreGive(ledMutex);
}
#endif // USE_BOARD_LED

#endif // USE_BOARD_SODAQ_ONE_V3
