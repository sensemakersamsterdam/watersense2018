#include "../periph/WDT.h"
#include "HCSR04.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define PIN_HCSR04_ECHO 7
#define PIN_HCSR04_TRIG 8
#define TIMEOUT_ECHO    500000 /* 500 ms */


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
  uint32_t val = 0;

  for (uint8_t i = 0; i < 5; i++) {
    WDT_reset();

    digitalWrite(PIN_HCSR04_TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(PIN_HCSR04_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_HCSR04_TRIG, LOW);
    val += pulseIn(PIN_HCSR04_ECHO, HIGH, TIMEOUT_ECHO);
    if (val == 0) { return 0; }
  }

  return val / 5;
}
