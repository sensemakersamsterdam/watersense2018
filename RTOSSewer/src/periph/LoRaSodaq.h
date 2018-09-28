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

#include "../Common.h"

#ifndef LORA_SODAQ_H
#define LORA_SODAQ_H


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

bool LoRa_setup();


/*******************************************************************************
 * Public
 ******************************************************************************/

bool    LoRa_initOTAA();
uint8_t LoRa_send(const uint8_t *buffer, uint8_t size); // return 0: OK, 1: not sent, 2: not connected
void    LoRa_sleep();
void    LoRa_setSpreadingFactor(uint8_t spreadingFactor);
void    LoRa_wakeUp();

#endif // LORA_SODAQ_H
