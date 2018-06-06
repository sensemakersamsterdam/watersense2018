/*
 * FreeRTOS Kernel V10.0.1
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html.
 *----------------------------------------------------------*/

#define configCPU_CLOCK_HZ                      F_CPU
#define configENABLE_BACKWARD_COMPATIBILITY     0
#define configIDLE_SHOULD_YIELD                 1
#define configMAX_PRIORITIES                    9
#define configMAX_TASK_NAME_LEN                 1
#define configMINIMAL_STACK_SIZE                ((unsigned short)128)
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS 0
#define configQUEUE_REGISTRY_SIZE               0
#define configTICK_RATE_HZ                      ((TickType_t)1000)
#define configUSE_16_BIT_TICKS                  0
#define configUSE_APPLICATION_TASK_TAG          0
#define configUSE_COUNTING_SEMAPHORES           0
#define configUSE_MUTEXES                       0
#define configUSE_NEWLIB_REENTRANT              0
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 0
#define configUSE_PREEMPTION                    1
#define configUSE_QUEUE_SETS                    0
#define configUSE_RECURSIVE_MUTEXES             0
#define configUSE_TASK_NOTIFICATIONS            1
#define configUSE_TICKLESS_IDLE                 0

/* Memory allocation related definitions. */

#define configAPPLICATION_ALLOCATED_HEAP        0
#define configSUPPORT_DYNAMIC_ALLOCATION        0
#define configSUPPORT_STATIC_ALLOCATION         1
#define configTOTAL_HEAP_SIZE                   0

/* Hook function related definitions. */

#define configCHECK_FOR_STACK_OVERFLOW          1
#define configUSE_DAEMON_TASK_STARTUP_HOOK      0
#define configUSE_IDLE_HOOK                     1
#define configUSE_MALLOC_FAILED_HOOK            0
#define configUSE_TICK_HOOK                     0

/* Run time and task stats gathering related definitions. */

#define configGENERATE_RUN_TIME_STATS           0
#define configUSE_TRACE_FACILITY                0
#define configUSE_STATS_FORMATTING_FUNCTIONS    0

/* Co-routine related definitions. */

#define configUSE_CO_ROUTINES                   0
#define configMAX_CO_ROUTINE_PRIORITIES         1

/* Software timer related definitions. */

#define configUSE_TIMERS                        0
#define configTIMER_QUEUE_LENGTH                10
#define configTIMER_TASK_PRIORITY               3
#define configTIMER_TASK_STACK_DEPTH            configMINIMAL_STACK_SIZE

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */

#define INCLUDE_eTaskGetState                   0
#define INCLUDE_uxTaskGetStackHighWaterMark     0
#define INCLUDE_uxTaskPriorityGet               0
#define INCLUDE_vTaskDelay                      1
#define INCLUDE_vTaskDelayUntil                 0
#define INCLUDE_vTaskDelete                     0
#define INCLUDE_vTaskPrioritySet                0
#define INCLUDE_vTaskSuspend                    0
#define INCLUDE_xQueueGetMutexHolder            0
#define INCLUDE_xSemaphoreGetMutexHolder        0
#define INCLUDE_xTaskAbortDelay                 0
#define INCLUDE_xTaskGetCurrentTaskHandle       0
#define INCLUDE_xTaskGetHandle                  0
#define INCLUDE_xTaskGetIdleTaskHandle          0
#define INCLUDE_xTaskGetSchedulerState          1
#define INCLUDE_xTaskResumeFromISR              0
#define INCLUDE_xTimerPendFunctionCall          0

/* Normal assert() semantics without relying on the provision of an assert.h
header file. */

#define configASSERT(x) if((x) == 0) { vAssertCalled(); }

/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
standard names. */

#define xPortPendSVHandler PendSV_Handler

#endif /* FREERTOS_CONFIG_H */
