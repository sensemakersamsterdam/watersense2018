#include "Conductivity.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define PIN_CONDUCTIVITY_ANALOG PIN_A2
#define PIN_CONDUCTIVITY_TRIG   3


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void Conductivity_setup()
{
  pinMode(PIN_CONDUCTIVITY_TRIG, OUTPUT);

  digitalWrite(PIN_CONDUCTIVITY_TRIG, LOW);
}


/*******************************************************************************
 * Public
 ******************************************************************************/

uint16_t Conductivity_measure()
{
  digitalWrite(PIN_CONDUCTIVITY_TRIG, HIGH);
  vTaskDelay(pdMS_TO_TICKS(1));

  // TODO: use median

  uint32_t val = 0;
  for (uint8_t i = 0; i < 5; i++) { val += analogRead(PIN_CONDUCTIVITY_ANALOG); }

  digitalWrite(PIN_CONDUCTIVITY_TRIG, LOW);

  return val / 5;
}
