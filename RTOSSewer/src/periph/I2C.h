#ifndef I2C_H
#define I2C_H

#include "../Common.h"

void I2C_setup();
BaseType_t I2C_lock();
void I2C_scan();
void I2C_unlock();

#endif
