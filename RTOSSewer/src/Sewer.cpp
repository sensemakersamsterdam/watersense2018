#include "Sewer.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define T0_STACK_SIZE 256
#define T0_DELAY_LOOP 10000


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void Sewer_setup()
{
  static StaticTask_t xT0TaskBuffer;
  static StackType_t  xT0Stack[T0_STACK_SIZE];

  xTaskCreateStatic(Sewer_threadMain, "M", T0_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, xT0Stack, &xT0TaskBuffer);
  vTaskStartScheduler();
}


/*******************************************************************************
 * Private
 ******************************************************************************/

static void Sewer_threadMain(void* pvParameters)
{
  #if USE_LOGGER
  Logger_setup();
  #endif

  Board_setup();

  #if USE_LORA_SODAQ
  if (!LoRa_setup()) { Board_fatalShutdown(); }
  LoRa_wakeUp();
  LoRa_initOTAA();
  LoRa_sleep();
  #endif

  I2C_setup();

  #if USE_BMP280
  bool b0 = BMP280_setup();
  #endif

  #if USE_FDC1004
  bool b1 = FDC1004_setup();
  #endif

  #if USE_VL53L0X
  bool b2 = VL53L0X_setup();
  #endif

  TickType_t x = xTaskGetTickCount();

  for (;;) {
    #if USE_BMP280
    if (b0) { BMP280_measure();  }
    #endif

    #if USE_FDC1004
    if (b1) { FDC1004_measure(); }
    #endif

    #if USE_VL53L0X
    if (b2) { VL53L0X_measure(); }
    #endif

    RTOS_delay(&x, pdMS_TO_TICKS(T0_DELAY_LOOP));
  }
}
