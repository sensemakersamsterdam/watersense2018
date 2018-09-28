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
