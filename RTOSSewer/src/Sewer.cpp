#include "board/SODAQ_ONE_V3.h"
#include "periph/I2C.h"
#include "periph/LoRaSodaq.h"
#include "periph/OneWire.h"
#include "periph/WDT.h"
#include "sensor/BMP280.h"
#include "sensor/Conductivity.h"
#include "sensor/DS18B20.h"
#include "sensor/HCSR04.h"
#include "sensor/LSM303AGR.h"
#include "sensor/MB7092.h"
#include "sensor/SEN0189.h"
#include "sensor/VegetronixAquaPlumb.h"
#include "sensor/VL53L0X.h"
#include "Sewer.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define MAINTHREAD_DELAY_LOOP        10000
#define MAINTHREAD_STACK_SIZE        384
#define MAINTHREAD_PRIORITY          tskIDLE_PRIORITY + 1

#define PIN_SENSORS_POWER            11

#define DATA_BIT_LSM303AGR           1
#define DATA_BIT_BMP280              2
#define DATA_BIT_VL53L0X             4
#define DATA_BIT_MB7092              8
#define DATA_BIT_HCSR04              16
#define DATA_BIT_DS18B20             32
#define DATA_BIT_CONDUCTIVITY        64
#define DATA_BIT_SEN0189             128
#define DATA_BIT_VEGETRONIXAQUAPLUMB 256


/*******************************************************************************
 * State
 ******************************************************************************/

struct __attribute__((__packed__)) {
  uint32_t version                   = PROJECT_VERSION;
  uint16_t board_modules             = 0;
  uint16_t board_voltage             = 0;
  int8_t   lsm303agr_temperature     = 0;
  float    bmp280_temperature        = 0;
  float    bmp280_pressure           = 0;
  uint16_t vl53l0x_distance          = 0;
  uint16_t mb7092_distance           = 0;
  uint16_t hcsr04_distance           = 0;
  float    ds18b20_temperature       = 0;
  uint16_t conductivity_value        = 0;
  uint16_t sen0189_value             = 0;
  uint16_t vegetronixaquaplumb_value = 0;
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
  digitalWrite(PIN_SENSORS_POWER, HIGH);

  Board_setup();

  #if USE_LOGGER
  I2C_logDevices();
  OneWire_logDevices();
  #endif

  digitalWrite(PIN_SENSORS_POWER, LOW);
  WDT_disable();

  vTaskDelay(pdMS_TO_TICKS(200));
}

static void Sewer_logData()
{
  LOG(VS("BOARD battery: "), VUI16(data.board_voltage));

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

  if (data.board_modules & DATA_BIT_MB7092) {
    LOG(VS("MB7092 distance: "), VUI16(data.mb7092_distance));
  }

  if (data.board_modules & DATA_BIT_HCSR04) {
    LOG(VS("HCSR04 distance: "), VUI16(data.hcsr04_distance));
  }

  if (data.board_modules & DATA_BIT_DS18B20) {
    LOG(VS("DS18B20 temperature: "), VF(data.ds18b20_temperature));
  }

  if (data.board_modules & DATA_BIT_CONDUCTIVITY) {
    LOG(VS("Conductivity value: "), VUI16(data.conductivity_value));
  }

  if (data.board_modules & DATA_BIT_SEN0189) {
    LOG(VS("SEN0189 value: "), VUI16(data.sen0189_value));
  }

  if (data.board_modules & DATA_BIT_VEGETRONIXAQUAPLUMB) {
    LOG(VS("VegetronixAquaPlumb value: "), VUI16(data.vegetronixaquaplumb_value));
  }
}

static void Sewer_measureData()
{
  LOGS("reading a measurement...");

  SYSCTRL->BOD33.bit.ENABLE = 0; // disabled brown-out detect
  digitalWrite(PIN_SENSORS_POWER, HIGH);
  vTaskDelay(pdMS_TO_TICKS(3));
  SYSCTRL->BOD33.bit.ENABLE = 1; // enabled brown-out detect

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

  I2C_disable();

  MB7092_setup();
  data.mb7092_distance = MB7092_measureDistance();
  if (data.mb7092_distance > 10) {
    data.board_modules |= DATA_BIT_MB7092;
  } else {
    data.board_modules &= ~DATA_BIT_MB7092;
  }

  HCSR04_setup();
  data.hcsr04_distance = HCSR04_measureDistance();
  if (data.hcsr04_distance > 0) {
    data.board_modules |= DATA_BIT_HCSR04;
  } else {
    data.board_modules &= ~DATA_BIT_HCSR04;
  }

  if (DS18B20_setup()) {
    data.board_modules      |= DATA_BIT_DS18B20;
    data.ds18b20_temperature = DS18B20_measureTemperature();
  } else {
    data.board_modules      &= ~DATA_BIT_DS18B20;
    data.ds18b20_temperature = 0;
  }

  Conductivity_setup();
  data.conductivity_value = Conductivity_measure();
  if (data.conductivity_value <= 1000) {
    data.board_modules |= DATA_BIT_CONDUCTIVITY;
  } else {
    data.board_modules &= ~DATA_BIT_CONDUCTIVITY;
  }

  data.sen0189_value = SEN0189_measure();
  if (data.sen0189_value <= 1000) {
    data.board_modules |= DATA_BIT_SEN0189;
  } else {
    data.board_modules &= ~DATA_BIT_SEN0189;
  }

  data.vegetronixaquaplumb_value = VEGETRONIXAQUAPLUMB_measure();
  if (data.vegetronixaquaplumb_value <= 1000) {
    data.board_modules |= DATA_BIT_VEGETRONIXAQUAPLUMB;
  } else {
    data.board_modules &= ~DATA_BIT_VEGETRONIXAQUAPLUMB;
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
