#include "Sewer.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define MAINTHREAD_DELAY_LOOP 10000
#define MAINTHREAD_STACK_SIZE 256
#define MAINTHREAD_PRIORITY   tskIDLE_PRIORITY + 1


/*******************************************************************************
 * Private declarations
 ******************************************************************************/

static void Sewer_initModules();
static void Sewer_logState();
static void Sewer_measure();
static void Sewer_prepareData();
static void Sewer_sendData();
static void Sewer_threadMain(void* pvParameters);


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void Sewer_setup()
{
  static StaticTask_t xT0TaskBuffer;
  static StackType_t  xT0Stack[MAINTHREAD_STACK_SIZE];

  xTaskCreateStatic(Sewer_threadMain, "M", MAINTHREAD_STACK_SIZE, NULL, MAINTHREAD_PRIORITY, xT0Stack, &xT0TaskBuffer);
  vTaskStartScheduler();
}


/*******************************************************************************
 * Private
 ******************************************************************************/

static void Sewer_initModules()
{
  #if USE_LOGGER
  Logger_setup();
  #endif

  Board_setup();

  #if USE_BMP280
  BMP280_setup();
  #endif

  #if USE_FDC1004
  FDC1004_setup();
  #endif

  #if USE_VL53L0X
  VL53L0X_setup();
  #endif
}

static void Sewer_logState()
{
  #if USE_ONBOARD_VOLTAGE
  LOG(VS("BOARD battery: "), VUI16(Board_voltage), VS(" mV"));
  #endif

  #if USE_BMP280
  if (BMP280_active) {
    LOG(VS("BMP280 temperature: "), VF(BMP280_temperature), VS(" *C"));
    LOG(VS("BMP280 pressure: "),    VF(BMP280_pressure),    VS(" Pa"));
    LOG(VS("BMP280 altitude: "),    VF(BMP280_altitude),    VS(" m"));
  } else {
    LOGS("BMP280 temperature: N/A");
    LOGS("BMP280 pressure: N/A");
    LOGS("BMP280 altitude: N/A");
  }
  #endif

  #if USE_FDC1004
  if (FDC1004_active) {
    LOG(VS("FDC1004 capacitance: "), VUI32(FDC1004_cap), VS(" fF"));
  } else {
    LOGS("FDC1004 capacitance: N/A");
  }
  #endif

  #if USE_ONBOARD_TEMPERATURE && USE_LSM303AGR
  if (LSM303AGR_active) {
    LOG(VS("LSM303AGR temperature: "), VUI8(LSM303AGR_temperature), VS(" *C"));
  } else {
    LOGS("LSM303AGR temperature: N/A");
  }
  #endif

  #if USE_VL53L0X
  if (VL53L0X_active) {
    if (VL53L0X_distance <= 2000) {
      LOG(VS("VL53L0X distance: "), VUI16(VL53L0X_distance), VS(" mm"));
    } else {
      LOGS("VL53L0X distance: out of range");
    }
  } else {
    LOGS("VL53L0X distance: N/A");
  }
  #endif
}

static void Sewer_measure()
{
  LOGS("reading a measurement...");

  Board_measure();

  #if USE_BMP280
  BMP280_measure();
  #endif

  #if USE_FDC1004
  FDC1004_measure();
  #endif

  #if USE_VL53L0X
  VL53L0X_measure();
  #endif
}

static void Sewer_prepareData()
{

}

static void Sewer_sendData()
{
  #if USE_ONBOARD_LED
  Board_setLed(0b001);
  #endif

  // LoRa_wakeUp();
  // LoRa_initOTAA();
  // LoRa_sleep();

  #if USE_ONBOARD_LED
  Board_setLed(0b000);
  #endif
}

static void Sewer_threadMain(void* pvParameters)
{
  Sewer_initModules();

  TickType_t ts = xTaskGetTickCount();

  for (;;) {
    Sewer_measure();

    #if USE_LOGGER
    Sewer_logState();
    #endif

    #if USE_LORA_SODAQ
    Sewer_prepareData();
    Sewer_sendData();
    #endif

    LOGS("need delay " STR(MAINTHREAD_DELAY_LOOP) " ms");
    vTaskDelayUntil(&ts, pdMS_TO_TICKS(MAINTHREAD_DELAY_LOOP));
  }
}
