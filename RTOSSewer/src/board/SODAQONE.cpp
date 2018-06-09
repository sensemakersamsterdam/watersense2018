#include "SODAQONE.h"


/*******************************************************************************
 * Functions
 ******************************************************************************/

void setLed(uint8_t pin, uint8_t state);


/*******************************************************************************
 * State
 ******************************************************************************/

static uint8_t ledState;

static SemaphoreHandle_t ledMutex;


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void SODAQONE_setup()
{
  static StaticSemaphore_t ledMutexBuffer;

  pinMode(LED_BLUE,  OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED,   OUTPUT);

  digitalWrite(LED_BLUE,  HIGH);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_RED,   HIGH);

  ledState = HIGH;
  ledMutex = xSemaphoreCreateMutexStatic(&ledMutexBuffer);

  LOGS("started");
}


/*******************************************************************************
 * Public
 ******************************************************************************/

void SODAQONE_toggleLedBlue()
{
  setLed(LED_BLUE, ledState ^ 1);
}

void SODAQONE_toggleLedGreen()
{
  setLed(LED_GREEN, ledState ^ 1);
}

void SODAQONE_toggleLedRed()
{
  setLed(LED_RED, ledState ^ 1);
}

void SODAQONE_turnOffLedBlue()
{
  setLed(LED_BLUE, HIGH);
}

void SODAQONE_turnOffLedGreen()
{
  setLed(LED_GREEN, HIGH);
}

void SODAQONE_turnOffLedRed()
{
  setLed(LED_RED, HIGH);
}

void SODAQONE_turnOnLedBlue()
{
  setLed(LED_BLUE, LOW);
}

void SODAQONE_turnOnLedGreen()
{
  setLed(LED_GREEN, LOW);
}

void SODAQONE_turnOnLedRed()
{
  setLed(LED_RED, LOW);
}


/*******************************************************************************
 * Private
 ******************************************************************************/

void setLed(uint8_t pin, uint8_t state)
{
  if (xSemaphoreTake(ledMutex, 100) != pdTRUE) { return; }

  ledState = state;
  digitalWrite(pin, state);

  xSemaphoreGive(ledMutex);
}
