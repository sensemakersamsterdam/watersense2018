#include "WDT.h"

#if USE_WDT

#include <Sodaq_wdt.h>


/*******************************************************************************
 * Public
 ******************************************************************************/

uint32_t WDT_enable(uint32_t ms)
{
  uint8_t pr;

  if (ms >= 8000) {
    ms = 8000;
    pr = 0xA;
  } else if (ms >= 4000) {
    ms = 4000;
    pr = 0x09;
  } else if (ms >= 2000) {
    ms = 2000;
    pr = 0x08;
  } else if (ms >= 1000) {
    ms = 1000;
    pr = 0x07;
  } else if (ms >= 500) {
    ms = 500;
    pr = 0x06;
  } else if (ms >= 250) {
    ms = 250;
    pr = 0x05;
  } else if (ms >= 125) {
    ms = 125;
    pr = 0x04;
  } else if (ms >= 63) {
    ms = 63;
    pr = 0x03;
  } else if (ms >= 31) {
    ms = 31;
    pr = 0x02;
  } else if (ms >= 16) {
    ms = 16;
    pr = 0x01;
  } else {
    ms = 8;
    pr = 0x00;
  }

  sodaq_wdt_enable((wdt_period)pr);

  return ms;
}

void WDT_disable() { sodaq_wdt_disable(); }


#endif // USE_WDT
