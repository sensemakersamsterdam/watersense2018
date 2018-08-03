#include "Sewer.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define T0_STACK_SIZE 256
#define T1_STACK_SIZE 256
#define T2_STACK_SIZE 256
#define T3_STACK_SIZE 256

#define T0_DELAYL 10000
#define T1_DELAY0 1200
#define T1_DELAYL 5000
#define T2_DELAYL 5000
#define T3_DELAYL 5000


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

  #if USE_LORA_SODAQ
  LoRa_setup();
  #endif

  TickType_t x = xTaskGetTickCount();

  for (;;) {
    #if USE_LOGGER_MAIN
    LOG(VS("need sleep "), VUI32(T0_DELAYL), VS(" ms"));
    #endif
    vTaskDelayUntil(&x, T0_DELAYL);
  }
}

#if USE_BMP280
static void Sewer_T1(void* pvParameters)
{
  #if USE_LOGGER_MAIN
  LOG(VS("need sleep "), VUI32(T1_DELAY0), VS(" ms"));
  #endif
  vTaskDelay(T1_DELAY0);

  TickType_t x = xTaskGetTickCount();

  for (;;) {
    BMP280_measure();
    #if USE_LOGGER_MAIN
    LOG(VS("need sleep "), VUI32(T1_DELAYL), VS(" ms"));
    #endif
    vTaskDelayUntil(&x, pdMS_TO_TICKS(T1_DELAYL));
  }
}
#endif // USE_BMP280

#if USE_FDC1004
static void Sewer_T2(void* pvParameters)
{
  TickType_t x = xTaskGetTickCount();

  for (;;) {
    FDC1004_measure();
    #if USE_LOGGER_MAIN
    LOG(VS("need sleep "), VUI32(T2_DELAYL), VS(" ms"));
    #endif
    vTaskDelayUntil(&x, pdMS_TO_TICKS(T2_DELAYL));
  }
}
#endif // USE_VL53L0X

#if USE_VL53L0X
static void Sewer_T3(void* pvParameters)
{
  TickType_t x = xTaskGetTickCount();

  for (;;) {
    VL53L0X_measure();
    #if USE_LOGGER_MAIN
    LOG(VS("need sleep "), VUI32(T3_DELAYL), VS(" ms"));
    #endif
    vTaskDelayUntil(&x, pdMS_TO_TICKS(T3_DELAYL));
  }
}
#endif // USE_VL53L0X
