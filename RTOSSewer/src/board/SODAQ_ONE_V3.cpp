#include "SODAQ_ONE_V3.h"

#if USE_BOARD_SODAQ_ONE_V3

#if USE_WDT_SLEEP
#include <Sodaq_wdt.h>
#endif


/*******************************************************************************
 * State
 ******************************************************************************/

#if USE_BOARD_LED
static SemaphoreHandle_t ledMutex;
#endif


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void Board_setup()
{
  // turn off GPS
  pinMode(GPS_ENABLE, OUTPUT);
  digitalWrite(GPS_ENABLE, LOW);

  // turn off the power to the WDT
  PM->APBAMASK.reg &= ~PM_APBAMASK_WDT;

  // turn off LEDs
  pinMode(PIN_LED_BLUE,  OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_RED,   OUTPUT);
  digitalWrite(PIN_LED_BLUE,  HIGH);
  digitalWrite(PIN_LED_GREEN, HIGH);
  digitalWrite(PIN_LED_RED,   HIGH);

  // init board LEDs handler if necessary
  #if USE_BOARD_LED
  static StaticSemaphore_t ledMutexBuffer;
  ledMutex = xSemaphoreCreateMutexStatic(&ledMutexBuffer);
  Board_setLed(0b001);
  #endif

  // enable deepsleep
  #if USE_WDT_SLEEP
  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
  #endif

  // done
  #if USE_LOGGER_BOARD
  LOGS("SODAQ_ONE_V3 started");
  #endif
}

#if USE_WDT_SLEEP
uint32_t Board_sleep(uint32_t ms)
{
  #if USE_LOGGER
  USBDevice.standby();
  #endif

  #if USE_BOARD_LED
  if (ms > 200) { Board_setLed(0b010); }
  #endif

  uint32_t ms1 = Board_wdt_enable(ms);
  __DSB();
  __WFI();
  sodaq_wdt_disable();

  #if USE_BOARD_LED
  if (ms > 200) Board_setLed(0b001);
  #endif

  #if USE_LOGGER_WDT_SLEEP
  LOG(VS("need sleep: "), VUI32(ms), VS(", actual sleep: "), VUI32(ms1), VS(" ms"));
  #endif

  return ms1;
}
#else
uint32_t Board_sleep(uint32_t ms) { return 0; }
#endif


/*******************************************************************************
 * Public
 ******************************************************************************/

#if USE_BOARD_LED
void Board_setLed(uint8_t rgb)
{
  if (!xSemaphoreTake(ledMutex, 100)) {
    #if USE_LOGGER_BOARD
    LOGS("resource is busy");
    #endif
    return;
  }

  digitalWrite(PIN_LED_RED  , !(rgb & 0b100));
  digitalWrite(PIN_LED_GREEN, !(rgb & 0b010));
  digitalWrite(PIN_LED_BLUE , !(rgb & 0b001));

  xSemaphoreGive(ledMutex);
}
#endif // USE_BOARD_LED


/*******************************************************************************
 * Private
 ******************************************************************************/

#if USE_WDT_SLEEP
static uint32_t Board_wdt_enable(uint32_t ms)
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
#endif

#endif // USE_BOARD_SODAQ_ONE_V3
