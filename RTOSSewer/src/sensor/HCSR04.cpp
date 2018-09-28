/*
RTOSSewer project: sensing water quality in a variety of ways.

Copyright (C) 2018 Sensemakers Amsterdam. All rights reserved.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "../periph/WDT.h"
#include "../util/Collection.h"
#include "HCSR04.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define TIMEOUT_ECHO 500000 /* 500 ms */


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
  uint16_t values[HCSR04_DI_ATTEMPTS];

  for (uint8_t i = 0; i < HCSR04_DI_ATTEMPTS; i++) {
    WDT_reset();
    digitalWrite(PIN_HCSR04_TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(PIN_HCSR04_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_HCSR04_TRIG, LOW);
    values[i] = pulseIn(PIN_HCSR04_ECHO, HIGH, TIMEOUT_ECHO);
    if (values[i] == 0) { return 0; }
  }

  return HCSR04_DI_CALIB_OFFSET + HCSR04_DI_CALIB_COEFF * median(values, HCSR04_DI_ATTEMPTS);
}
