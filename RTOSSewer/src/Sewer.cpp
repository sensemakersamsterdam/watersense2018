#include "board/SODAQ_ONE_V3.h"
#include "periph/I2C.h"
#include "periph/LoRaSodaq.h"
#include "periph/OneWire.h"
#include "periph/WDT.h"
#include "sensor/BMP280.h"
#include "sensor/DS18B20.h"
#include "sensor/FDC1004.h"
#include "sensor/HCSR04.h"
#include "sensor/LSM303AGR.h"
#include "sensor/MB7092.h"
#include "sensor/VL53L0X.h"
#include "Sewer.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define MAINTHREAD_DELAY_LOOP 10000
#define MAINTHREAD_STACK_SIZE 256
#define MAINTHREAD_PRIORITY   tskIDLE_PRIORITY + 1

#define PIN_SENSORS_POWER     11

#define DATA_BIT_LSM303AGR    1
#define DATA_BIT_BMP280       2
#define DATA_BIT_VL53L0X      4
#define DATA_BIT_FDC1004      8
#define DATA_BIT_MB7092       16
#define DATA_BIT_HCSR04       32
#define DATA_BIT_DS18B20      64


/*******************************************************************************
 * State
 ******************************************************************************/

struct __attribute__((__packed__)) {
  uint32_t version               = PROJECT_VERSION;
  uint8_t  board_modules         = 0;
  uint16_t board_voltage         = 0;
  int8_t   lsm303agr_temperature = 0;
  float    bmp280_temperature    = 0;
  float    bmp280_pressure       = 0;
  uint16_t vl53l0x_distance      = 0;
  float    fdc1004_capacity      = 0;
  uint16_t mb7092_distance       = 0;
  uint16_t hcsr04_distance       = 0;
  float    ds18b20_temperature   = 0;
} data;


/*******************************************************************************
 * Private declarations
 ******************************************************************************/

static void Sewer_initModules();
static void Sewer_logData();
static void Sewer_measureData();
static void Sewer_sendData();
static void Sewer_threadMain(void* pvParameters);


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void Sewer_setup()
{
  pinMode(PIN_SENSORS_POWER, OUTPUT);
  digitalWrite(PIN_SENSORS_POWER, LOW);

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

  WDT_setup();

  WDT_enable();
  Board_setup();

  #if USE_LOGGER
  digitalWrite(PIN_SENSORS_POWER, HIGH);
  I2C_logDevices();
  OneWire_logDevices();
  digitalWrite(PIN_SENSORS_POWER, LOW);
  #endif

  WDT_disable();

  vTaskDelay(pdMS_TO_TICKS(200));
}

static void Sewer_logData()
{
  LOG(VS("BOARD battery: "), VUI16(data.board_voltage), VS(" mV"));

  if (data.board_modules & DATA_BIT_LSM303AGR) {
    LOG(VS("LSM303AGR temperature: "), VUI8(data.lsm303agr_temperature), VS(" *C"));
  }

  if (data.board_modules & DATA_BIT_BMP280) {
    LOG(VS("BMP280 temperature: "), VF(data.bmp280_temperature), VS(" *C"));
    LOG(VS("BMP280 pressure: "),    VF(data.bmp280_pressure),    VS(" Pa"));
  }

  if (data.board_modules & DATA_BIT_VL53L0X) {
    if (data.vl53l0x_distance <= 2000) {
      LOG(VS("VL53L0X distance: "), VUI16(data.vl53l0x_distance), VS(" mm"));
    } else {
      LOGS("VL53L0X distance: out of range");
    }
  }

  if (data.board_modules & DATA_BIT_FDC1004) {
    LOG(VS("FDC1004 capacitance: "), VUI32(data.fdc1004_capacity), VS(" fF"));
  }

  if (data.board_modules & DATA_BIT_MB7092) {
    LOG(VS("MB7092 distance: "), VUI16(data.mb7092_distance));
  }

  if (data.board_modules & DATA_BIT_HCSR04) {
    LOG(VS("HCSR04 distance: "), VUI16(data.hcsr04_distance));
  }

  if (data.board_modules & DATA_BIT_DS18B20) {
    LOG(VS("DS18B20 temperature: "), VF(data.ds18b20_temperature));
  }
}

static void Sewer_measureData()
{
  LOGS("reading a measurement...");

  digitalWrite(PIN_SENSORS_POWER, HIGH);

  data.board_voltage = Board_measureVoltage();

  I2C_enable();

  if (LSM303AGR_isActive()) {
    data.board_modules        |= DATA_BIT_LSM303AGR;
    data.lsm303agr_temperature = LSM303AGR_measureTemperature();
  } else {
    data.board_modules        &= ~DATA_BIT_LSM303AGR;
    data.lsm303agr_temperature = 0;
  }

  if (BMP280_setup()) {
    data.board_modules     |= DATA_BIT_BMP280;
    data.bmp280_temperature = BMP280_measureTemperature();
    data.bmp280_pressure    = BMP280_measurePressure();
  } else {
    data.board_modules     &= ~DATA_BIT_BMP280;
    data.bmp280_temperature = 0;
    data.bmp280_pressure    = 0;
  }

  if (VL53L0X_setup()) {
    data.board_modules   |= DATA_BIT_VL53L0X;
    data.vl53l0x_distance = VL53L0X_measureDistance();
  } else {
    data.board_modules   &= ~DATA_BIT_VL53L0X;
    data.vl53l0x_distance = 0;
  }

  if (FDC1004_setup()) {
    data.board_modules   |= DATA_BIT_FDC1004;
    data.fdc1004_capacity = FDC1004_measureCapacity();
  } else {
    data.board_modules   &= ~DATA_BIT_FDC1004;
    data.fdc1004_capacity = 0;
  }

  I2C_disable();

  MB7092_setup();
  data.board_modules  |= DATA_BIT_MB7092;
  data.mb7092_distance = MB7092_measureDistance();

  HCSR04_setup();
  data.board_modules  |= DATA_BIT_HCSR04;
  data.hcsr04_distance = HCSR04_measureDistance();

  if (DS18B20_setup()) {
    data.board_modules      |= DATA_BIT_DS18B20;
    data.ds18b20_temperature = DS18B20_measureTemperature();
  } else {
    data.board_modules      &= ~DATA_BIT_DS18B20;
    data.ds18b20_temperature = 0;
  }

  digitalWrite(PIN_SENSORS_POWER, LOW);

  #if USE_LOGGER
  Sewer_logData();
  #endif
}

static void Sewer_sendData()
{
  static bool b = false;

  LOG(VS("data, "), VUI8(sizeof(data)), VS(" bytes: "), VUI8AH02((const uint8_t*)&data, sizeof(data)));

  LoRa_wakeUp();
  if (!b) { b = LoRa_initOTAA(); }
  if (b) {
    if (LoRa_send((const uint8_t*)&data, sizeof(data)) == 2) { b = false; }
  }
  LoRa_sleep();
}

static void Sewer_threadMain(void* pvParameters)
{
  Sewer_initModules();

  TickType_t ts = xTaskGetTickCount();

  for (;;) {
    WDT_enable();

    Sewer_measureData();

    #if USE_LORA
    Sewer_sendData();
    #endif

    WDT_disable();

    LOGS("need delay " STR(MAINTHREAD_DELAY_LOOP) " ms");
    vTaskDelayUntil(&ts, pdMS_TO_TICKS(MAINTHREAD_DELAY_LOOP));
  }
}
