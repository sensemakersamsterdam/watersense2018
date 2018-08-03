#include "FreeRTOSv10.0.1/FreeRTOS.h"
#include "FreeRTOSv10.0.1/task.h"

void loop();
void xPortSysTickHandler();
uint32_t Board_sleep(uint32_t ms);

/* SysTick handler used in real-time kernel. */

int sysTickHook()
{
  if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) { xPortSysTickHandler(); }
  return 0;
}

/* configSUPPORT_STATIC_ALLOCATION is set to 1, so the application must provide an implementation of
   vApplicationGetIdleTaskMemory() to provide the memory that is used by the Idle task.
   See https://www.freertos.org/a00110.html */

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize)
{
  /* If the buffers to be provided to the Idle task are declared inside this function then they must be declared
     static - otherwise they will be allocated on the stack and so not exists after this function exits. */
  static StaticTask_t xIdleTaskTCB;
  static StackType_t  uxIdleTaskStack[configMINIMAL_STACK_SIZE];

  /* Pass out a pointer to the StaticTask_t structure in which the Idle task's state will be stored. */
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

  /* Pass out the array that will be used as the Idle task's stack. */
  *ppxIdleTaskStackBuffer = uxIdleTaskStack;

  /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer. Note that, as the array is necessarily
     of type StackType_t, configMINIMAL_STACK_SIZE is specified in words, not bytes. */
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set to 1 in FreeRTOSConfig.h. It will be
   called on each iteration of the idle task. It is essential that code added to this hook function never attempts
   to block in any way (for example, call xQueueReceive() with a block time specified, or call vTaskDelay()). If the
   application makes use of the vTaskDelete() API function (as this demo application does) then it is also important
   that vApplicationIdleHook() is permitted to return to its calling function, because it is the responsibility of the
   idle task to clean up memory allocated by the kernel to any task that has since been deleted.
   See https://www.freertos.org/a00016.html */

#if configUSE_IDLE_HOOK > 0
void vApplicationIdleHook()
{
  loop();
}
#endif

/* Run time stack overflow checking is performed if configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook
   function is called if a stack overflow is detected.
   See https://www.freertos.org/Stacks-and-stack-overflow-checking.html */

#if configCHECK_FOR_STACK_OVERFLOW > 0
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
  vAssertCalled(__PRETTY_FUNCTION__);
}
#endif

/* See https://www.freertos.org/a00110.html#configASSERT */

#if configASSERT_DEFINED == 1
void vAssertCalled(const char *func)
{
  taskDISABLE_INTERRUPTS();
  for(;;);
}
#endif

/* Define the function that is called by portSUPPRESS_TICKS_AND_SLEEP(). */

#if configUSE_TICKLESS_IDLE == 2
void vApplicationSleep(TickType_t xExpectedIdleTime)
{
  /* Enter the low power state. */
  uint32_t x = Board_sleep(xExpectedIdleTime * portTICK_PERIOD_MS);

  /* Correct the kernels tick count to account for the time the microcontroller spent in its low power state. */
  vTaskStepTick(pdMS_TO_TICKS(x));
}
#endif
