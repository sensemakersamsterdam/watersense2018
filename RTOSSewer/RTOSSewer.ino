#include "src/RTOSSewer.h"

static StaticTask_t xT1TaskBuffer;
static StackType_t  xT1Stack[configMINIMAL_STACK_SIZE];

void setup() {
  SystemThread::setup();

  xTaskCreateStatic(threadT1, NULL, configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, xT1Stack, &xT1TaskBuffer);
  vTaskStartScheduler();
}

void loop() {
  SystemThread::loop();
}

static void threadT1(void* pvParameters)
{
  BlinkThread::setup();

  while (1) {
    BlinkThread::loop();
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
