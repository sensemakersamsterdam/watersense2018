#include "MB7092.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define PIN_MB7092_ANALOG PIN_A9
#define PIN_MB7092_TRIG   10


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void MB7092_setup()
{
  pinMode(PIN_MB7092_TRIG, OUTPUT);

  digitalWrite(PIN_MB7092_TRIG, LOW);
}


/*******************************************************************************
 * Public
 ******************************************************************************/

uint16_t MB7092_measureDistance()
{
  digitalWrite(PIN_MB7092_TRIG, LOW);
  vTaskDelay(pdMS_TO_TICKS(1));
  digitalWrite(PIN_MB7092_TRIG, HIGH);
  vTaskDelay(pdMS_TO_TICKS(500));

  uint32_t val = 0;
  for (uint8_t i = 0; i < 5; i++) { val += analogRead(PIN_MB7092_ANALOG); }

  digitalWrite(PIN_MB7092_TRIG, LOW);

  return val / 5;
}
