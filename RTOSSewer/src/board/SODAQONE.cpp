#include "SODAQONE.h"


/*******************************************************************************
 * State
 ******************************************************************************/

uint8_t SODAQONE::ledState;

SemaphoreHandle_t SODAQONE::ledMutex;


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void SODAQONE::setup()
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
}


/*******************************************************************************
 * Public
 ******************************************************************************/

void SODAQONE::toggleLedBlue()
{
  setLed(LED_BLUE, ledState ^ 1);
}

void SODAQONE::toggleLedGreen()
{
  setLed(LED_GREEN, ledState ^ 1);
}

void SODAQONE::toggleLedRed()
{
  setLed(LED_RED, ledState ^ 1);
}

void SODAQONE::turnOffLedBlue()
{
  setLed(LED_BLUE, HIGH);
}

void SODAQONE::turnOffLedGreen()
{
  setLed(LED_GREEN, HIGH);
}

void SODAQONE::turnOffLedRed()
{
  setLed(LED_RED, HIGH);
}

void SODAQONE::turnOnLedBlue()
{
  setLed(LED_BLUE, LOW);
}

void SODAQONE::turnOnLedGreen()
{
  setLed(LED_GREEN, LOW);
}

void SODAQONE::turnOnLedRed()
{
  setLed(LED_RED, LOW);
}


/*******************************************************************************
 * Private
 ******************************************************************************/

void SODAQONE::setLed(uint8_t pin, uint8_t state)
{
  if (xSemaphoreTake(ledMutex, 100) != pdTRUE) { return; }

  ledState = state;
  digitalWrite(pin, state);

  xSemaphoreGive(ledMutex);
}
