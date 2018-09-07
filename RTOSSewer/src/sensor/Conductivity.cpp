#include "../util/Collection.h"
#include "Conductivity.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define MEASUREMENTS_COUNT      5
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

  uint16_t values[MEASUREMENTS_COUNT];

  for (uint8_t i = 0; i < MEASUREMENTS_COUNT; i++) { values[i] = analogRead(PIN_CONDUCTIVITY_ANALOG); }

  uint16_t val = median(values, MEASUREMENTS_COUNT);

  digitalWrite(PIN_CONDUCTIVITY_TRIG, LOW);

  return val;
}
