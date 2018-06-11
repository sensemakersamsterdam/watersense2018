#include "Sewer.h"
#include "periph/I2C.h"
#include "sensor/BMP280.h"
#include "sensor/VL53L0X.h"

#if defined(ARDUINO_AVR_MEGA2560)
#include "board/ARDUINO_MEGA2560_R3.h"
#elif defined(ARDUINO_SODAQ_ONE)
#include "board/SODAQ_ONE_V3.h"
#endif


/*******************************************************************************
 * Private functions declarations
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

  xTaskCreateStatic(Sewer_T0, "M", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, xT0Stack, &xT0TaskBuffer);
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

  #ifdef ARDUINO_AVR_MEGA2560
  ARDUINO_MEGA2560_R3_setup();
  #endif

  #ifdef ARDUINO_SODAQ_ONE
  SODAQ_ONE_V3_setup();
  #endif

  I2C_setup();

  if (BMP280_setup()) {
    xTaskCreateStatic(Sewer_T1, "1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, xT1Stack, &xT1TaskBuffer);
  }

  if (VL53L0X_setup()) {
    xTaskCreateStatic(Sewer_T2, "2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, xT2Stack, &xT2TaskBuffer);
  }
}

static void Sewer_T0(void* pvParameters)
{
  Sewer_initModules();

  while (true) {
    LOGS("zzz...");
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
