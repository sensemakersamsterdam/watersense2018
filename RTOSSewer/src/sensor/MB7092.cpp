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

#include "../util/Collection.h"
#include "MB7092.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define MEASUREMENTS_COUNT 5
#define PIN_MB7092_ANALOG  PIN_A9
#define PIN_MB7092_TRIG    10


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

  uint16_t values[MEASUREMENTS_COUNT];

  for (uint8_t i = 0; i < MEASUREMENTS_COUNT; i++) { values[i] = analogRead(PIN_MB7092_ANALOG); }

  uint16_t val = median(values, MEASUREMENTS_COUNT);

  digitalWrite(PIN_MB7092_TRIG, LOW);

  return val;
}
