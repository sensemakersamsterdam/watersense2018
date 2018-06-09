#include "Sewer.h"
#include "board/SODAQONE.h"
#include "periph/I2C.h"
#include "sensor/BMP280.h"
#include "sensor/TOF.h"


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void Sewer::setup()
{
  static StaticTask_t xT0TaskBuffer;
  static StackType_t  xT0Stack[configMINIMAL_STACK_SIZE];

  xTaskCreateStatic(threadT0, "MAIN", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, xT0Stack, &xT0TaskBuffer);
  vTaskStartScheduler();
}

void Sewer::idle()
{
  #if DEBUG
  static unsigned long last = 0;

  unsigned long now = millis();

  if (now - last >= 1000) {
    LOGS("zzz...");
    last = now;
  }
  #endif
}


/*******************************************************************************
 * Private
 ******************************************************************************/

void Sewer::initModules()
{
  static StaticTask_t xT1TaskBuffer;
  static StaticTask_t xT2TaskBuffer;
  static StackType_t  xT1Stack[configMINIMAL_STACK_SIZE];
  static StackType_t  xT2Stack[configMINIMAL_STACK_SIZE];

  #if DEBUG
  Logger::setup();
  #endif

  Board::setup();
  I2C::setup();

  #if DEBUG
  I2C::logDevices();
  #endif

  BMP280::setup();
  if (BMP280::isReady) {
    xTaskCreateStatic(threadT1, "TH01", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, xT1Stack, &xT1TaskBuffer);
  }

  TOF::setup();
  if (TOF::isReady) {
    xTaskCreateStatic(threadT2, "TH02", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, xT2Stack, &xT2TaskBuffer);
  }
}

void Sewer::threadT0(void* pvParameters)
{
  initModules();

  while (true) {
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void Sewer::threadT1(void* pvParameters)
{
  vTaskDelay(pdMS_TO_TICKS(1000));

  while (true) {
    BMP280::measure();
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}

void Sewer::threadT2(void* pvParameters)
{
  while (true) {
    TOF::measure();
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}
