#include "RTOS.h"


/*******************************************************************************
 * Public
 ******************************************************************************/

void RTOS_delay(TickType_t xTimeIncrement)
{
  LOG(VS("need delay "), VUI32(xTimeIncrement), VS(" ms"));

  vTaskDelay(xTimeIncrement);
}

void RTOS_delay(TickType_t *pxPreviousWakeTime, TickType_t xTimeIncrement)
{
  LOG(VS("need delay "), VUI32(xTimeIncrement), VS(" ms"));

  vTaskDelayUntil(pxPreviousWakeTime, xTimeIncrement);
}
