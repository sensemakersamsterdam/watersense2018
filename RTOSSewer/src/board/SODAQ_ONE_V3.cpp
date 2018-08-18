#include "../periph/I2C.h"
#include "../periph/RTC.h"
#include "../periph/LoRaSodaq.h"
#include "../periph/WDT.h"
#include "../sensor/LSM303AGR.h"
#include "SODAQ_ONE_V3.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define ADC_AREF   3.3f
#define BATVOLT_R1 4.7f
#define BATVOLT_R2 10.0f


/*******************************************************************************
 * Private declarations
 ******************************************************************************/

static void     Board_logCpuResetCause();
static uint32_t Board_sleep_RTC(uint32_t ms);
static uint32_t Board_sleep_WDT(uint32_t ms);


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void Board_setup()
{
  // disable WDT and turn off the power to the WDT
  WDT_disable();

  // turn off GPS
  pinMode(GPS_ENABLE, OUTPUT);
  digitalWrite(GPS_ENABLE, LOW);

  // turn off LEDs
  pinMode(PIN_LED_BLUE,  OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_RED,   OUTPUT);
  digitalWrite(PIN_LED_BLUE,  HIGH);
  digitalWrite(PIN_LED_GREEN, HIGH);
  digitalWrite(PIN_LED_RED,   HIGH);

  // set sleep mode: deepsleep
  #if USE_DEEPSLEEP
  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
  #endif

  // show last CPU reset reason(s)
  #if USE_LOGGER
  Board_logCpuResetCause();
  #endif

  // init I2C
  I2C_setup();

  // init LSM303AGR, disable accelerometer and magnetometer
  // LSM303AGR_setup(); // note: it will be done separately

  // if we don't use magnetometer we need to make this low otherwise this pin on the LSM303AGR starts leaking current
  pinMode(MAG_INT, OUTPUT);
  digitalWrite(MAG_INT, LOW);

  // init LoRa, sleep LoRa
  // if (!LoRa_setup()) { Board_fatalShutdown(); } // note: it will be done separately

  LOG_SETUP_RESULT_TEXT(true);
}


/*******************************************************************************
 * Public
 ******************************************************************************/

void Board_fatalShutdown()
{
  LOGS("FATAL ERROR! SHUTDOWN");

  taskDISABLE_INTERRUPTS();

  for (;;) {
    Board_setLed(0b100);
    vTaskDelay(pdMS_TO_TICKS(1000));
    Board_setLed(0b000);
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

uint16_t Board_measureVoltage()
{
  return (uint16_t)((ADC_AREF / 1.023) * (BATVOLT_R1 + BATVOLT_R2) / BATVOLT_R2 * (float)analogRead(BAT_VOLT));
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
  return ms > 1000 ? Board_sleep_RTC(ms) : Board_sleep_WDT(ms);
}
#else
uint32_t Board_sleep(uint32_t ms) { return 0; }
#endif


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
