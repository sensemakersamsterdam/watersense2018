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
#include "SEN0189.h"


/*******************************************************************************
 * Public
 ******************************************************************************/

uint16_t SEN0189_measure()
{
  uint16_t values[SEN0189_TB_ATTEMPTS];

  for (uint8_t i = 0; i < SEN0189_TB_ATTEMPTS; i++) { values[i] = analogRead(PIN_SEN0189_ANALOG); }

  return SEN0189_TB_CALIB_OFFSET + SEN0189_TB_CALIB_COEFF * median(values, SEN0189_TB_ATTEMPTS);
}
