#include "Sewer.h"
#include "board/SODAQONE.h"
#include "periph/I2C.h"
#include "sensor/BMP280.h"
#include "sensor/VL53L0X.h"


/*******************************************************************************
 * Functions
 ******************************************************************************/

static void initModules();
static void threadT0(void* pvParameters);
static void threadT1(void* pvParameters);
static void threadT2(void* pvParameters);


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void Sewer_setup()
{
  static StaticTask_t xT0TaskBuffer;
  static StackType_t  xT0Stack[configMINIMAL_STACK_SIZE];

  xTaskCreateStatic(threadT0, "MAIN", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, xT0Stack, &xT0TaskBuffer);
  vTaskStartScheduler();
}

void Sewer_idle()
{
  #if DEBUG
  static unsigned long last = 0;
  unsigned long now = millis();
  if (now - last >= 1000) { LOGS("zzz..."); last = now; }
  #endif
}


/*******************************************************************************
 * Private
 ******************************************************************************/

static void initModules()
{
  static StaticTask_t xT1TaskBuffer;
  static StaticTask_t xT2TaskBuffer;
  static StackType_t  xT1Stack[configMINIMAL_STACK_SIZE];
  static StackType_t  xT2Stack[configMINIMAL_STACK_SIZE];

  #if DEBUG
  Logger_setup();
  #endif

  SODAQONE_setup();
  I2C_setup();

  #if DEBUG
  I2C_scan();
  #endif

  BMP280_setup();
  if (BMP280_isReady) {
    xTaskCreateStatic(threadT1, "TH01", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, xT1Stack, &xT1TaskBuffer);
  }

  VL53L0X_setup();
  if (VL53L0X_isReady) {
    xTaskCreateStatic(threadT2, "TH02", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, xT2Stack, &xT2TaskBuffer);
  }
}

static void threadT0(void* pvParameters)
{
  initModules();

  while (true) {
    // TODO - add code for main thread if necesary, for example turn on RED led if fatal error occurs
    vTaskDelay(pdMS_TO_TICKS(10000));
  }
}

static void threadT1(void* pvParameters)
{
  vTaskDelay(pdMS_TO_TICKS(1000));

  while (true) {
    BMP280_measure();
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}

static void threadT2(void* pvParameters)
{
  while (true) {
    VL53L0X_measure();
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}
