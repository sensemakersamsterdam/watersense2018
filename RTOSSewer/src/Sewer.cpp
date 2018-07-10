#include "Sewer.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define T0_STACK_SIZE 256
#define T1_STACK_SIZE 256
#define T2_STACK_SIZE 256
#define T3_STACK_SIZE 256


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void Sewer_setup()
{
  static StaticTask_t xT0TaskBuffer;
  static StackType_t  xT0Stack[T0_STACK_SIZE];

  xTaskCreateStatic(Sewer_T0, "M", T0_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, xT0Stack, &xT0TaskBuffer);
  vTaskStartScheduler();
}

void Sewer_idle()
{
  #if USE_LOGGER_MAIN
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
  #if USE_LOGGER
  Logger_setup();
  #endif

  Board_setup();

  #if USE_I2C
  I2C_setup();
  #endif

  #if USE_BMP280
  static StaticTask_t xT1TaskBuffer;
  static StackType_t  xT1Stack[T1_STACK_SIZE];
  if (BMP280_setup()) {
    xTaskCreateStatic(Sewer_T1, "1", T1_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, xT1Stack, &xT1TaskBuffer);
  }
  #endif

  #if USE_FDC1004
  static StaticTask_t xT2TaskBuffer;
  static StackType_t  xT2Stack[T2_STACK_SIZE];
  if (FDC1004_setup()) {
    xTaskCreateStatic(Sewer_T2, "2", T2_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, xT2Stack, &xT2TaskBuffer);
  }
  #endif

  #if USE_VL53L0X
  static StaticTask_t xT3TaskBuffer;
  static StackType_t  xT3Stack[T3_STACK_SIZE];
  if (VL53L0X_setup()) {
    xTaskCreateStatic(Sewer_T3, "3", T3_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, xT3Stack, &xT3TaskBuffer);
  }
  #endif
}

static void Sewer_T0(void* pvParameters)
{
  Sewer_initModules();

  #if USE_LORA
  LoRa_setup();
  #endif

  while (true) {
    #if USE_LOGGER_MAIN
    LOGS("zzz...");
    #endif
    vTaskDelay(pdMS_TO_TICKS(10000));
  }
}

#if USE_BMP280
static void Sewer_T1(void* pvParameters)
{
  vTaskDelay(pdMS_TO_TICKS(1200));

  while (true) {
    BMP280_measure();
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}
#endif

#if USE_FDC1004
static void Sewer_T2(void* pvParameters)
{
  while (true) {
    FDC1004_measure();
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
#endif // USE_VL53L0X

#if USE_VL53L0X
static void Sewer_T3(void* pvParameters)
{
  while (true) {
    VL53L0X_measure();
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}
#endif // USE_VL53L0X
