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

#include "Collection.h"


/*******************************************************************************
 * Public
 ******************************************************************************/

float median(float values[], uint8_t size)
{
  if (size == 1) { return values[0]; }

  for (uint8_t i = 1; i < size; i++) {
    for (uint8_t j = i; j > 0 && values[j - 1] > values[j]; j--) {
      uint16_t k = values[j - 1];
      values[j - 1] = values[j];
      values[j] = k;
    }
  }

  return size % 2 > 0 ? values[size / 2] : (values[size / 2 - 1] + values[size / 2]) / 2;
}

int8_t median(int8_t values[], uint8_t size)
{
  if (size == 1) { return values[0]; }

  for (uint8_t i = 1; i < size; i++) {
    for (uint8_t j = i; j > 0 && values[j - 1] > values[j]; j--) {
      uint16_t k = values[j - 1];
      values[j - 1] = values[j];
      values[j] = k;
    }
  }

  return size % 2 > 0 ? values[size / 2] : (values[size / 2 - 1] + values[size / 2]) / 2;
}

uint16_t median(uint16_t values[], uint8_t size)
{
  if (size == 1) { return values[0]; }

  for (uint8_t i = 1; i < size; i++) {
    for (uint8_t j = i; j > 0 && values[j - 1] > values[j]; j--) {
      uint16_t k = values[j - 1];
      values[j - 1] = values[j];
      values[j] = k;
    }
  }

  return size % 2 > 0 ? values[size / 2] : (values[size / 2 - 1] + values[size / 2]) / 2;
}
