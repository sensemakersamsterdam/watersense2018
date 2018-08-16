#include "SODAQ_ONE_V3.h"

#if USE_BOARD_SODAQ_ONE_V3

#include "../periph/I2C.h"

#if USE_DEEPSLEEP && USE_RTC
#include "../periph/RTC.h"
#endif

#if USE_DEEPSLEEP && USE_WDT
#include "../periph/WDT.h"
#endif


/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define ADC_AREF                  3.3f
#define BATVOLT_R1                4.7f
#define BATVOLT_R2                10.0f


/*******************************************************************************
 * State
 ******************************************************************************/

#if USE_BOARD_VOLTAGE
uint16_t Board_voltage = 0;
#endif


/*******************************************************************************
 * Private declarations
 ******************************************************************************/

static void Board_logCpuResetCause();
static uint32_t Board_sleep_RTC(uint32_t ms);
static uint32_t Board_sleep_WDT(uint32_t ms);


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

  // enable deepsleep
  #if USE_DEEPSLEEP
  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
  #endif

  #if USE_LOGGER
  Board_logCpuResetCause();
  #endif

  #if USE_I2C
  I2C_setup();
  #endif

  LOGS("SODAQ_ONE_V3 started");
}


/*******************************************************************************
 * Public
 ******************************************************************************/

void Board_fatalShutdown()
{
  LOGS("FATAL ERROR! SHUTDOWN");

  taskDISABLE_INTERRUPTS();

  #if USE_BOARD_LED
  for (;;) {
    Board_setLed(0b100);
    vTaskDelay(pdMS_TO_TICKS(1000));
    Board_setLed(0b000);
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
  #else
  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
  __DSB();
  __WFI();
  #endif
}

void Board_measure()
{
  #if USE_BOARD_VOLTAGE
  Board_voltage = (uint16_t)((ADC_AREF / 1.023) * (BATVOLT_R1 + BATVOLT_R2) / BATVOLT_R2 * (float)analogRead(BAT_VOLT));
  #endif
}

void Board_setLed(uint8_t rgb)
{
  digitalWrite(PIN_LED_RED  , !(rgb & 0b100));
  digitalWrite(PIN_LED_GREEN, !(rgb & 0b010));
  digitalWrite(PIN_LED_BLUE , !(rgb & 0b001));
}

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

static void Board_logCpuResetCause()
{
  LOG(VS("CPU reset by: "),
    PM->RCAUSE.bit.SYST            ? VS("Software ")     : (void)NULL,
    PM->RCAUSE.reg & PM_RCAUSE_WDT ? VS("Watchdog ")     : (void)NULL,
    PM->RCAUSE.bit.EXT             ? VS("External ")     : (void)NULL,
    PM->RCAUSE.bit.BOD33           ? VS("BOD33 ")        : (void)NULL,
    PM->RCAUSE.bit.BOD12           ? VS("BOD12 ")        : (void)NULL,
    PM->RCAUSE.bit.POR             ? VS("PowerOnReset ") : (void)NULL,
    VS("(0x"), VUI8H02(PM->RCAUSE.reg), VC(')')
  );
}

#if USE_DEEPSLEEP && USE_RTC
static uint32_t Board_sleep_RTC(uint32_t ms)
{
  uint32_t ms1 = RTC_setAlarm(ms);

  LOG(VS("need sleep: "), VUI32(ms), VS(" ms, will sleep: "), VUI32(ms1), VS(" ms"));

  #if USE_LOGGER
  USBDevice.standby();
  #endif

  __DSB();
  __WFI();

  return ms1;
}
#endif

#if USE_DEEPSLEEP && USE_WDT
static uint32_t Board_sleep_WDT(uint32_t ms)
{
  uint32_t ms1 = WDT_enable(ms);

  LOG(VS("need sleep: "), VUI32(ms), VS(" ms, will sleep: "), VUI32(ms1), VS(" ms"));

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
