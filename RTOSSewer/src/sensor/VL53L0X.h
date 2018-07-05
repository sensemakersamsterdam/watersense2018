#include "../Common.h"

#if USE_VL53L0X && !defined(VL53L0X_H)
#define VL53L0X_H


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

bool VL53L0X_setup();


/*******************************************************************************
 * Public
 ******************************************************************************/

void VL53L0X_measure();

#endif // USE_VL53L0X && !defined(VL53L0X_H)
