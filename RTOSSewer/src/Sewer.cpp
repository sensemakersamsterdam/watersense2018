#include "Sewer.h"
#include "periph/I2C.h"
#include "sensor/BMP280.h"
#include "sensor/VL53L0X.h"

#ifdef ARDUINO_SODAQ_ONE
#include "board/SODAQ_ONE_V3.h"
#endif


/*******************************************************************************
 * Functions
 ******************************************************************************/

static void Sewer_initModules();
static void Sewer_T0(void* pvParameters);
static void Sewer_T1(void* pvParameters);
static void Sewer_T2(void* pvParameters);


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void Sewer_setup()
{
  static StaticTask_t xT0TaskBuffer;
  static StackType_t  xT0Stack[configMINIMAL_STACK_SIZE];

  xTaskCreateStatic(Sewer_T0, "MAIN", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, xT0Stack, &xT0TaskBuffer);
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

static void Sewer_initModules()
{
  static StaticTask_t xT1TaskBuffer;
  static StaticTask_t xT2TaskBuffer;
  static StackType_t  xT1Stack[configMINIMAL_STACK_SIZE];
  static StackType_t  xT2Stack[configMINIMAL_STACK_SIZE];

  #if DEBUG
  Logger_setup();
  #endif

  #ifdef ARDUINO_SODAQ_ONE
  SODAQ_ONE_V3_setup();
  #endif

  I2C_setup();

  #if DEBUG
  I2C_logDevices();
  #endif

  BMP280_setup();
  if (BMP280_isReady) {
    xTaskCreateStatic(Sewer_T1, "TH01", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, xT1Stack, &xT1TaskBuffer);
  }

  VL53L0X_setup();
  if (VL53L0X_isReady) {
    xTaskCreateStatic(Sewer_T2, "TH02", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, xT2Stack, &xT2TaskBuffer);
  }
}

static void Sewer_T0(void* pvParameters)
{
  Sewer_initModules();

  while (true) {
    // TODO - add code for main thread if necesary, for example turn on RED led if fatal error occurs
    vTaskDelay(pdMS_TO_TICKS(10000));
  }
}

static void Sewer_T1(void* pvParameters)
{
  vTaskDelay(pdMS_TO_TICKS(1000));

  while (true) {
    BMP280_measure();
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}

static void Sewer_T2(void* pvParameters)
{
  while (true) {
    VL53L0X_measure();
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}
