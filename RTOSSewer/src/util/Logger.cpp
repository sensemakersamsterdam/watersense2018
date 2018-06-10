#include "Logger.h"
#if DEBUG
#include <stdarg.h>


/*******************************************************************************
 * State
 ******************************************************************************/

static SemaphoreHandle_t logMutex;


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void Logger_setup()
{
  static StaticSemaphore_t logMutexBuffer;

  #ifndef ARDUINO_SODAQ_ONE
  Serial.begin(9600);
  #endif

  while (!Serial && millis() < 10000);

  logMutex = xSemaphoreCreateMutexStatic(&logMutexBuffer);

  LOGS("started");
}


/*******************************************************************************
 * Public
 ******************************************************************************/

void Logger_logs(const char *func, const char *s)
{
  Logger_logt(func, "%s", s);
}

void Logger_logt(const char *func, const char *fmt, ...)
{
  char s[96];

  va_list args;
  va_start(args, fmt);
  vsnprintf(s, sizeof(s), fmt, args);
  va_end(args);

  if (!xSemaphoreTake(logMutex, 100)) { LOGS("resource is busy"); return; }

  Serial.write('[');
  Serial.write(pcTaskGetName(NULL));
  Serial.write("][");
  Serial.print(millis());
  Serial.write("][");
  Serial.write(func);
  Serial.write("] ");
  Serial.println(s);

  xSemaphoreGive(logMutex);
}

#endif
