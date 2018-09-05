#include "MB7092.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define PIN_TRIG   11
#define PIN_ANALOG PIN_A10


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void MB7092_setup()
{
  pinMode(PIN_TRIG, OUTPUT);
}


/*******************************************************************************
 * Public
 ******************************************************************************/

uint16_t MB7092_measureDistance()
{
  digitalWrite(PIN_TRIG, LOW);
  vTaskDelay(pdMS_TO_TICKS(1));
  digitalWrite(PIN_TRIG, HIGH);
  vTaskDelay(pdMS_TO_TICKS(500));

  uint32_t val = 0;
  for (uint8_t i = 0; i < 5; i++) { val += analogRead(PIN_ANALOG); }

  digitalWrite(PIN_TRIG, LOW);

  return val / 5;
}
