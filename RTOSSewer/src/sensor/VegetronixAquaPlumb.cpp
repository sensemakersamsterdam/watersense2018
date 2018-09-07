#include "../util/Collection.h"
#include "VegetronixAquaPlumb.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define MEASUREMENTS_COUNT             5
#define PIN_VEGETRONIXAQUAPLUMB_ANALOG PIN_A0


/*******************************************************************************
 * Public
 ******************************************************************************/

uint16_t VEGETRONIXAQUAPLUMB_measure()
{
  uint16_t values[MEASUREMENTS_COUNT];

  for (uint8_t i = 0; i < MEASUREMENTS_COUNT; i++) { values[i] = analogRead(PIN_VEGETRONIXAQUAPLUMB_ANALOG); }

  return median(values, MEASUREMENTS_COUNT);
}
