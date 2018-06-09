#ifndef VL53L0X_H
#define VL53L0X_H

#include "../Common.h"

extern BaseType_t VL53L0X_isReady;

void VL53L0X_setup();
void VL53L0X_measure();

#endif
