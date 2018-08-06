#include "SODAQ_ONE_V3.h"

#if USE_BOARD_SODAQ_ONE_V3

#if USE_DEEPSLEEP && USE_RTC
#include "../periph/RTC.h"
#endif

#if USE_DEEPSLEEP && USE_WDT
#include "../periph/WDT.h"
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
  #endif

  // enable deepsleep
  #if USE_DEEPSLEEP
  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
  #endif

  // done
  #if USE_LOGGER_BOARD
  LOGS("SODAQ_ONE_V3 started");
  #endif
}


/*******************************************************************************
 * Public
 ******************************************************************************/

void Board_fatalShutdown()
{
  #if USE_LOGGER
  LOGS("FATAL ERROR! SHUTDOWN");
  #endif

  taskDISABLE_INTERRUPTS();

  #if USE_BOARD_LED
  for (;;) {
    Board_setLed(0b100);
    vTaskDelay(1000);
    Board_setLed(0b000);
    vTaskDelay(1000);
  }
  #else
  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
  __DSB();
  __WFI();
  #endif
}

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

#if USE_DEEPSLEEP
uint32_t Board_sleep(uint32_t ms)
{
  #if USE_BOARD_LED
  if (ms > 200) { Board_setLed(0b010); }
  #endif

  uint32_t ms1 = 0;

  #if USE_RTC
  if (ms > 1000) { ms1 = Board_sleep_RTC(ms); } else {
  #endif
  #if USE_WDT
    if (ms >= 8) { ms1 = Board_sleep_WDT(ms); }
  #endif
  #if USE_RTC
  }
  #endif

  #if USE_BOARD_LED
  if (ms > 200) Board_setLed(0b000);
  #endif

  return ms1;
}
#else
uint32_t Board_sleep(uint32_t ms) { return 0; }
#endif // USE_DEEPSLEEP


/*******************************************************************************
 * Private
 ******************************************************************************/

#if USE_DEEPSLEEP && USE_RTC
uint32_t Board_sleep_RTC(uint32_t ms)
{
  uint32_t ms1 = RTC_setAlarm(ms);

  #if USE_LOGGER_DEEPSLEEP
  LOG(VS("need sleep: "), VUI32(ms), VS(" ms, will sleep: "), VUI32(ms1), VS(" ms"));
  #endif

  #if USE_LOGGER
  USBDevice.standby();
  #endif

  __DSB();
  __WFI();

  return ms1;
}
#endif

#if USE_DEEPSLEEP && USE_WDT
uint32_t Board_sleep_WDT(uint32_t ms)
{
  uint32_t ms1 = WDT_enable(ms);

  #if USE_LOGGER_DEEPSLEEP
  LOG(VS("need sleep: "), VUI32(ms), VS(" ms, will sleep: "), VUI32(ms1), VS(" ms"));
  #endif

  #if USE_LOGGER
  USBDevice.standby();
  #endif

  __DSB();
  __WFI();
  WDT_disable();

  return ms1;
}
#endif

#endif // USE_BOARD_SODAQ_ONE_V3
