#include "SODAQ_ONE_V3.h"

#if USE_BOARD && defined(ARDUINO_SODAQ_ONE)


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
  pinMode(LED_BLUE,  OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED,   OUTPUT);

  digitalWrite(LED_BLUE,  HIGH);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_RED,   HIGH);

  #if USE_BOARD_LED
  static StaticSemaphore_t ledMutexBuffer;
  ledState = HIGH;
  ledMutex = xSemaphoreCreateMutexStatic(&ledMutexBuffer);
  #endif

  #if USE_LOGGER_BOARD
  LOGS("started");
  #endif
}


/*******************************************************************************
 * Public
 ******************************************************************************/

#if USE_BOARD_LED
void Board_toggleLedBlue()
{
  Board_setLed(LED_BLUE, ledState ^ 1);
}

void Board_toggleLedGreen()
{
  Board_setLed(LED_GREEN, ledState ^ 1);
}

void Board_toggleLedRed()
{
  Board_setLed(LED_RED, ledState ^ 1);
}

void Board_turnOffLedBlue()
{
  Board_setLed(LED_BLUE, HIGH);
}

void Board_turnOffLedGreen()
{
  Board_setLed(LED_GREEN, HIGH);
}

void Board_turnOffLedRed()
{
  Board_setLed(LED_RED, HIGH);
}

void Board_turnOnLedBlue()
{
  Board_setLed(LED_BLUE, LOW);
}

void Board_turnOnLedGreen()
{
  Board_setLed(LED_GREEN, LOW);
}

void Board_turnOnLedRed()
{
  Board_setLed(LED_RED, LOW);
}
#endif // USE_BOARD_LED


/*******************************************************************************
 * Private
 ******************************************************************************/

#if USE_BOARD_LED
void Board_setLed(uint8_t pin, uint8_t state)
{
  if (!xSemaphoreTake(ledMutex, 100)) { LOGS("resource is busy"); return; }

  ledState = state;
  digitalWrite(pin, state);

  xSemaphoreGive(ledMutex);
}
#endif // USE_BOARD_LED

#endif // USE_BOARD && defined(ARDUINO_SODAQ_ONE)
