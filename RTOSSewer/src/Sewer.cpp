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

#define MAINTHREAD_DELAY_LOOP 10000
#define MAINTHREAD_STACK_SIZE 256
#define MAINTHREAD_PRIORITY   tskIDLE_PRIORITY + 1

#define DATA_BIT_LSM303AGR    1
#define DATA_BIT_BMP280       2
#define DATA_BIT_VL53L0X      4
#define DATA_BIT_FDC1004      8


/*******************************************************************************
 * State
 ******************************************************************************/

struct {
  struct {
    char     sign[2]     = {0x53, 0x57}; // 'S', 'W'
    uint32_t version     = 18081800;
  } common;

  struct {
    uint8_t  active      = 0; // bit 0: LSM303AGR, 1: BMP280, 2: VL53L0X, 3: FDC1004
    uint16_t voltage     = 0;
  } board;

  struct {
    int8_t   temperature = 0;
  } lsm303agr;

  struct {
    float    temperature = 0;
    float    pressure    = 0;
    float    altitude    = 0;
  } bmp280;

  struct {
    int8_t distance      = 0;
  } vl53l0x;

  struct {
    float    capacity    = 0;
  } fdc1004;
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

  if (LSM303AGR_setup()) { data.board.active  = DATA_BIT_LSM303AGR; }
  if (BMP280_setup()   ) { data.board.active |= DATA_BIT_BMP280;    }
  if (VL53L0X_setup()  ) { data.board.active |= DATA_BIT_VL53L0X;   }
  if (FDC1004_setup()  ) { data.board.active |= DATA_BIT_FDC1004;   }

  Board_setLed(0b000);
}

static void Sewer_logData()
{
  LOG(VS("BOARD battery: "), VUI16(data.board.voltage), VS(" mV"));

  if (data.board.active & DATA_BIT_LSM303AGR) {
    LOG(VS("LSM303AGR temperature: "), VUI8(data.lsm303agr.temperature), VS(" *C"));
  }

  if (data.board.active & DATA_BIT_BMP280) {
    LOG(VS("BMP280 temperature: "), VF(data.bmp280.temperature), VS(" *C"));
    LOG(VS("BMP280 pressure: "),    VF(data.bmp280.pressure),    VS(" Pa"));
    LOG(VS("BMP280 altitude: "),    VF(data.bmp280.altitude),    VS(" m"));
  }

  if (data.board.active & DATA_BIT_VL53L0X) {
    if (data.vl53l0x.distance <= 2000) {
      LOG(VS("VL53L0X distance: "), VUI16(data.vl53l0x.distance), VS(" mm"));
    } else {
      LOGS("VL53L0X distance: out of range");
    }
  }

  if (data.board.active & DATA_BIT_FDC1004) {
    LOG(VS("FDC1004 capacitance: "), VUI32(data.fdc1004.capacity), VS(" fF"));
  }
}

static void Sewer_measureData()
{
  Board_setLed(0b010);

  LOGS("reading a measurement...");

  data.board.voltage = Board_measureVoltage();

  if (data.board.active & DATA_BIT_LSM303AGR) {
    data.lsm303agr.temperature = LSM303AGR_measureTemperature();
  }

  if (data.board.active & DATA_BIT_BMP280) {
    data.bmp280.temperature = BMP280_measureTemperature();
    data.bmp280.pressure    = BMP280_measurePressure();
    data.bmp280.altitude    = BMP280_measureAltitude();
  }

  if (data.board.active & DATA_BIT_VL53L0X) {
    data.vl53l0x.distance = VL53L0X_measureDistance();
  }

  if (data.board.active & DATA_BIT_FDC1004) {
    data.fdc1004.capacity = FDC1004_measureCapacity();
  }

  #if USE_LOGGER
  Sewer_logData();
  #endif

  Board_setLed(0b000);
}

static void Sewer_sendData()
{
  // Board_setLed(0b001);

  // LoRa_wakeUp();
  // LoRa_initOTAA();
  // LoRa_sleep();

  // Board_setLed(0b000);
}

static void Sewer_threadMain(void* pvParameters)
{
  Sewer_initModules();

  TickType_t ts = xTaskGetTickCount();

  for (;;) {
    Sewer_measureData();
    Sewer_sendData();

    LOGS("need delay " STR(MAINTHREAD_DELAY_LOOP) " ms");
    vTaskDelayUntil(&ts, pdMS_TO_TICKS(MAINTHREAD_DELAY_LOOP));
  }
}
