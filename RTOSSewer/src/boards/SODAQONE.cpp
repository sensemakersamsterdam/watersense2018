#include "../Common.h"
#include "SODAQONE.h"

/*******************************************************************************
 * State
 ******************************************************************************/

uint8_t SODAQONE::ledState;


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void SODAQONE::setup()
{
  pinMode(LED_BLUE,  OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED,   OUTPUT);

  digitalWrite(LED_BLUE,  HIGH);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_RED,   HIGH);

  ledState = HIGH;
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
  // TODO - use MUTEX

  ledState = state;
  digitalWrite(pin, state);
}
