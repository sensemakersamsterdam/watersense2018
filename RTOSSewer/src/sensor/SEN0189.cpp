#include "../util/Collection.h"
#include "SEN0189.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define MEASUREMENTS_COUNT 5
#define PIN_SEN0189_ANALOG PIN_A1


/*******************************************************************************
 * Public
 ******************************************************************************/

uint16_t SEN0189_measure()
{
  uint16_t values[MEASUREMENTS_COUNT];

  for (uint8_t i = 0; i < MEASUREMENTS_COUNT; i++) { values[i] = analogRead(PIN_SEN0189_ANALOG); }

  return median(values, MEASUREMENTS_COUNT);
}
