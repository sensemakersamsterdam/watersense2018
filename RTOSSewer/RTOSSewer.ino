#include "src/RTOSSewer.h"

static StaticTask_t xT0TaskBuffer;
static StackType_t  xT0Stack[configMINIMAL_STACK_SIZE];
static StaticTask_t xT1TaskBuffer;
static StackType_t  xT1Stack[configMINIMAL_STACK_SIZE];
static StaticTask_t xT2TaskBuffer;
static StackType_t  xT2Stack[configMINIMAL_STACK_SIZE];

void setup()
{
  System::setup();

  BMP280Sensor::setup();
  TOFSensor::setup();

  xTaskCreateStatic(threadT0, NULL, configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, xT0Stack, &xT0TaskBuffer);

  if (BMP280Sensor::isReady) {
    xTaskCreateStatic(threadT1, NULL, configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, xT1Stack, &xT1TaskBuffer);
  }

  if (TOFSensor::isReady) {
    xTaskCreateStatic(threadT2, NULL, configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, xT2Stack, &xT2TaskBuffer);
  }

  vTaskStartScheduler();
}

void loop()
{
  System::idle();
}

static void threadT0(void* pvParameters)
{
  while (1) {
    System::toggleGreenLed();

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

static void threadT1(void* pvParameters)
{
  vTaskDelay(pdMS_TO_TICKS(1000));

  while (1) {
    taskENTER_CRITICAL();
    BMP280Sensor::measure();
    taskEXIT_CRITICAL();

    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}

static void threadT2(void* pvParameters)
{
  while (1) {
    taskENTER_CRITICAL();
    TOFSensor::measure();
    taskEXIT_CRITICAL();

    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}

