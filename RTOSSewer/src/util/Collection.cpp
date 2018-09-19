#include "Collection.h"


/*******************************************************************************
 * Public
 ******************************************************************************/

uint16_t median(uint16_t *values, uint8_t size)
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

uint8_t round8(float f)
{
  uint8_t i = (uint8_t)f;
  if (f - i > 0.5F) { i++; }
  return i;
}

uint32_t round32(float f)
{
  uint32_t i = (uint32_t)f;
  if (f - i > 0.5F) { i++; }
  return i;
}
