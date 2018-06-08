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

  // TODO - move to logger
  Serial.begin(9600);
  delay(200);
  while (!Serial && millis() < 10000);

  xTaskCreateStatic(threadT0, NULL, configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, xT0Stack, &xT0TaskBuffer);
  vTaskStartScheduler();
}

void Sewer::idle()
{
  Serial.print("Idle: ");
  Serial.println(millis());
  delay(1000);
}


/*******************************************************************************
 * Private
 ******************************************************************************/

void Sewer::initSystem()
{
  static StaticTask_t xT1TaskBuffer;
  static StaticTask_t xT2TaskBuffer;
  static StackType_t  xT1Stack[configMINIMAL_STACK_SIZE];
  static StackType_t  xT2Stack[configMINIMAL_STACK_SIZE];

  SODAQONE::setup();

  I2C::setup();
  I2C::logDevices();

  BMP280::setup();
  if (BMP280::isReady) {
    xTaskCreateStatic(threadT1, NULL, configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, xT1Stack, &xT1TaskBuffer);
  }

  TOF::setup();
  if (TOF::isReady) {
    xTaskCreateStatic(threadT2, NULL, configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, xT2Stack, &xT2TaskBuffer);
  }
}

void Sewer::threadT0(void* pvParameters)
{
  initSystem();

  // TODO - use it as a logger
  while (1) {
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void Sewer::threadT1(void* pvParameters)
{
  vTaskDelay(pdMS_TO_TICKS(1000));

  while (1) {
    BMP280::measure();
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}

void Sewer::threadT2(void* pvParameters)
{
  while (1) {
    TOF::measure();
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}
