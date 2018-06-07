#include "src/RTOSSewer.h"

static StaticTask_t xT1TaskBuffer;
static StackType_t  xT1Stack[configMINIMAL_STACK_SIZE];

void setup()
{
  System::setup();

  BMP280Sensor::setup();
  TOFSensor::setup();

  if (TOFSensor::isReady) {
    xTaskCreateStatic(threadT1, NULL, configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, xT1Stack, &xT1TaskBuffer);
  }

  vTaskStartScheduler();
}

void loop()
{
  System::idle();
}

static void threadT1(void* pvParameters)
{
  while (1) {
    TOFSensor::measure();
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

