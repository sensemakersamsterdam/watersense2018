#ifdef ARDUINO_SODAQ_ONE
#include "SODAQ_ONE_V3.h"


/*******************************************************************************
 * Functions
 ******************************************************************************/

void SODAQ_ONE_V3_setLed(uint8_t pin, uint8_t state);


/*******************************************************************************
 * State
 ******************************************************************************/

static uint8_t ledState;

static SemaphoreHandle_t ledMutex;


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void SODAQ_ONE_V3_setup()
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

void SODAQ_ONE_V3_toggleLedBlue()
{
  SODAQ_ONE_V3_setLed(LED_BLUE, ledState ^ 1);
}

void SODAQ_ONE_V3_toggleLedGreen()
{
  SODAQ_ONE_V3_setLed(LED_GREEN, ledState ^ 1);
}

void SODAQ_ONE_V3_toggleLedRed()
{
  SODAQ_ONE_V3_setLed(LED_RED, ledState ^ 1);
}

void SODAQ_ONE_V3_turnOffLedBlue()
{
  SODAQ_ONE_V3_setLed(LED_BLUE, HIGH);
}

void SODAQ_ONE_V3_turnOffLedGreen()
{
  SODAQ_ONE_V3_setLed(LED_GREEN, HIGH);
}

void SODAQ_ONE_V3_turnOffLedRed()
{
  SODAQ_ONE_V3_setLed(LED_RED, HIGH);
}

void SODAQ_ONE_V3_turnOnLedBlue()
{
  SODAQ_ONE_V3_setLed(LED_BLUE, LOW);
}

void SODAQ_ONE_V3_turnOnLedGreen()
{
  SODAQ_ONE_V3_setLed(LED_GREEN, LOW);
}

void SODAQ_ONE_V3_turnOnLedRed()
{
  SODAQ_ONE_V3_setLed(LED_RED, LOW);
}


/*******************************************************************************
 * Private
 ******************************************************************************/

void SODAQ_ONE_V3_setLed(uint8_t pin, uint8_t state)
{
  if (!xSemaphoreTake(ledMutex, 100)) { LOGS("resource is busy"); return; }

  ledState = state;
  digitalWrite(pin, state);

  xSemaphoreGive(ledMutex);
}

#endif
