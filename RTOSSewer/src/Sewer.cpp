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

  BMP280_setup();
  FDC1004_setup();
  VL53L0X_setup();
}

static void Sewer_logState()
{
  LOG(VS("BOARD battery: "), VUI16(Board_voltage), VS(" mV"));

  if (BMP280_active) {
    LOG(VS("BMP280 temperature: "), VF(BMP280_temperature), VS(" *C"));
    LOG(VS("BMP280 pressure: "),    VF(BMP280_pressure),    VS(" Pa"));
    LOG(VS("BMP280 altitude: "),    VF(BMP280_altitude),    VS(" m"));
  } else {
    LOGS("BMP280 temperature: N/A");
    LOGS("BMP280 pressure: N/A");
    LOGS("BMP280 altitude: N/A");
  }

  if (FDC1004_active) {
    LOG(VS("FDC1004 capacitance: "), VUI32(FDC1004_cap), VS(" fF"));
  } else {
    LOGS("FDC1004 capacitance: N/A");
  }

  if (LSM303AGR_active) {
    LOG(VS("LSM303AGR temperature: "), VUI8(LSM303AGR_temperature), VS(" *C"));
  } else {
    LOGS("LSM303AGR temperature: N/A");
  }

  if (VL53L0X_active) {
    if (VL53L0X_distance <= 2000) {
      LOG(VS("VL53L0X distance: "), VUI16(VL53L0X_distance), VS(" mm"));
    } else {
      LOGS("VL53L0X distance: out of range");
    }
  } else {
    LOGS("VL53L0X distance: N/A");
  }
}

static void Sewer_measure()
{
  LOGS("reading a measurement...");

  Board_measure();
  BMP280_measure();
  FDC1004_measure();
  VL53L0X_measure();
}

static void Sewer_prepareData()
{

}

static void Sewer_sendData()
{
  Board_setLed(0b001);

  // LoRa_wakeUp();
  // LoRa_initOTAA();
  // LoRa_sleep();

  Board_setLed(0b000);
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

    Sewer_prepareData();
    Sewer_sendData();

    LOGS("need delay " STR(MAINTHREAD_DELAY_LOOP) " ms");
    vTaskDelayUntil(&ts, pdMS_TO_TICKS(MAINTHREAD_DELAY_LOOP));
  }
}
