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

#include <Arduino.h>
#include <Sodaq_wdt.h>

#include "WDT.h"


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void WDT_setup() {
  // generic clock generator 2, divisor = 32 (2^(DIV+1))
  GCLK->GENDIV.reg = GCLK_GENDIV_ID(2) | GCLK_GENDIV_DIV(4);

  // enable clock generator 2 using low-power 32KHz oscillator.
  // with /32 divisor above, this yields 1024Hz(ish) clock.
  GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(2) | GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_OSCULP32K | GCLK_GENCTRL_DIVSEL;
  while(GCLK->STATUS.bit.SYNCBUSY);

  // WDT clock = clock gen 2
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID_WDT | GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK2;

  // enable WDT early-warning interrupt
  NVIC_DisableIRQ(WDT_IRQn);
  NVIC_ClearPendingIRQ(WDT_IRQn);
  NVIC_SetPriority(WDT_IRQn, 0); // top priority
  NVIC_EnableIRQ(WDT_IRQn);
}


/*******************************************************************************
 * Public
 ******************************************************************************/

void WDT_disable()
{
  sodaq_wdt_disable();

  LOGS("disabled");
}

void WDT_enable()
{
  PM->APBAMASK.reg |= PM_APBAMASK_WDT;  // turn the power to the WDT module on

  WDT->CTRL.reg = 0;                    // disable watchdog for config
  while(WDT->STATUS.bit.SYNCBUSY);

  WDT->INTENCLR.bit.EW = 1;             // disable early warning interrupt
  WDT->CONFIG.bit.PER  = 10;            // set period 8 sec for chip reset
  WDT->CTRL.bit.WEN    = 0;             // disable window mode
  while(WDT->STATUS.bit.SYNCBUSY);      // sync CTRL write

  WDT_reset();                          // clear watchdog interval
  WDT->CTRL.bit.ENABLE = 1;             // start watchdog now
  while(WDT->STATUS.bit.SYNCBUSY);

  LOGS("enabled, period 8 sec");
}

void WDT_reset() { sodaq_wdt_reset(); }
