#include "board/SODAQ_ONE_V3.h"
#include "periph/LoRaSodaq.h"
#include "sensor/BMP280.h"
#include "sensor/FDC1004.h"
#include "sensor/LSM303AGR.h"
#include "sensor/VL53L0X.h"
#include "Sewer.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define MAINTHREAD_DELAY_LOOP 300000
#define MAINTHREAD_STACK_SIZE 256
#define MAINTHREAD_PRIORITY   tskIDLE_PRIORITY + 1

#define DATA_BIT_LSM303AGR    1
#define DATA_BIT_BMP280       2
#define DATA_BIT_VL53L0X      4
#define DATA_BIT_FDC1004      8


/*******************************************************************************
 * State
 ******************************************************************************/

struct __attribute__((__packed__)) {
  uint32_t version               = PROJECT_VERSION;
  uint8_t  board_modules         = 0; // bit 0: LSM303AGR, 1: BMP280, 2: VL53L0X, 3: FDC1004
  uint16_t board_voltage         = 0;
  int8_t   lsm303agr_temperature = 0;
  float    bmp280_temperature    = 0;
  float    bmp280_pressure       = 0;
  float    bmp280_altitude       = 0;
  uint16_t vl53l0x_distance      = 0;
  float    fdc1004_capacity      = 0;
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

  Board_setLed(0b010);

  if (!LoRa_setup()) { Board_fatalShutdown(); }

  if (LSM303AGR_setup()) { data.board_modules  = DATA_BIT_LSM303AGR; }
  if (BMP280_setup()   ) { data.board_modules |= DATA_BIT_BMP280;    }
  if (VL53L0X_setup()  ) { data.board_modules |= DATA_BIT_VL53L0X;   }
  if (FDC1004_setup()  ) { data.board_modules |= DATA_BIT_FDC1004;   }

  Board_setLed(0b000);
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
    LOG(VS("BMP280 altitude: "),    VF(data.bmp280_altitude),    VS(" m"));
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

  LOG(VS("data, "), VUI8(sizeof(data)), VS(" bytes: "), VUI8AH02((const uint8_t*)&data, sizeof(data)));
}

static void Sewer_measureData()
{
  Board_setLed(0b010);

  LOGS("reading a measurement...");

  data.board_voltage = Board_measureVoltage();

  if (data.board_modules & DATA_BIT_LSM303AGR) {
    data.lsm303agr_temperature = LSM303AGR_measureTemperature();
  }

  if (data.board_modules & DATA_BIT_BMP280) {
    data.bmp280_temperature = BMP280_measureTemperature();
    data.bmp280_pressure    = BMP280_measurePressure();
    data.bmp280_altitude    = BMP280_measureAltitude();
  }

  if (data.board_modules & DATA_BIT_VL53L0X) {
    data.vl53l0x_distance = VL53L0X_measureDistance();
  }

  if (data.board_modules & DATA_BIT_FDC1004) {
    data.fdc1004_capacity = FDC1004_measureCapacity();
  }

  #if USE_LOGGER
  Sewer_logData();
  #endif

  Board_setLed(0b000);
}

static void Sewer_sendData()
{
  static bool b = false;

  Board_setLed(0b001);

  LoRa_wakeUp();
  if (!b) { b = LoRa_initOTAA(); }
  if (b) { LoRa_send((const uint8_t*)&data, sizeof(data)); }
  LoRa_sleep();

  Board_setLed(0b000);
}

static void Sewer_threadMain(void* pvParameters)
{
  Sewer_initModules();

  TickType_t ts = xTaskGetTickCount();

  for (;;) {
    Sewer_measureData();

    #if USE_LORA
    Sewer_sendData();
    #endif

    LOGS("need delay " STR(MAINTHREAD_DELAY_LOOP) " ms");
    vTaskDelayUntil(&ts, pdMS_TO_TICKS(MAINTHREAD_DELAY_LOOP));
  }
}
