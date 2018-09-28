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
