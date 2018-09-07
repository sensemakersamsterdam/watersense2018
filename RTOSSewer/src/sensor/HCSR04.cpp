#include "../periph/WDT.h"
#include "../util/Collection.h"
#include "HCSR04.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define MEASUREMENTS_COUNT 5
#define PIN_HCSR04_ECHO    7
#define PIN_HCSR04_TRIG    8
#define TIMEOUT_ECHO       500000 /* 500 ms */


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void HCSR04_setup()
{
  pinMode(PIN_HCSR04_TRIG, OUTPUT);
  pinMode(PIN_HCSR04_ECHO, INPUT);

  digitalWrite(PIN_HCSR04_TRIG, LOW);
}


/*******************************************************************************
 * Public
 ******************************************************************************/

uint16_t HCSR04_measureDistance()
{
  uint16_t values[MEASUREMENTS_COUNT];

  for (uint8_t i = 0; i < MEASUREMENTS_COUNT; i++) {
    WDT_reset();

    digitalWrite(PIN_HCSR04_TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(PIN_HCSR04_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_HCSR04_TRIG, LOW);
    values[i] = pulseIn(PIN_HCSR04_ECHO, HIGH, TIMEOUT_ECHO);
    if (values[i] == 0) { return 0; }
  }

  return median(values, MEASUREMENTS_COUNT);
}
