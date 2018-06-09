#ifndef BMP280_H
#define BMP280_H

#include "../Common.h"

extern BaseType_t BMP280_isReady;

void BMP280_setup();
void BMP280_measure();

#endif
