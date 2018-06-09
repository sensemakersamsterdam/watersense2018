#include <stdarg.h>
#include "Logger.h"


/*******************************************************************************
 * State
 ******************************************************************************/

SemaphoreHandle_t Logger::logMutex;


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void Logger::setup()
{
  static StaticSemaphore_t logMutexBuffer;

  while (!Serial && millis() < 10000);

  logMutex = xSemaphoreCreateMutexStatic(&logMutexBuffer);

  LOGA("started");
}


/*******************************************************************************
 * Public
 ******************************************************************************/

void Logger::loga(const char* name, const char* s)
{
  if (!lock()) { return; }
  logPrefix(name);
  Serial.println(s);
  unlock();
}

void Logger::logf(const char* name, float f)
{
  if (!lock()) { return; }
  logPrefix(name);
  Serial.println(f);
  unlock();
}

void Logger::logi(const char* name, int i)
{
  if (!lock()) { return; }
  logPrefix(name);
  Serial.println(i);
  unlock();
}

void Logger::logt(const char* name, const char* fmt, ...)
{
  va_list args;

  char s[80];

  va_start(args, fmt);
  vsnprintf(s, sizeof(s), fmt, args);
  va_end(args);

  loga(name, s);
}


/*******************************************************************************
 * Private
 ******************************************************************************/

bool Logger::lock()
{
  return xSemaphoreTake(logMutex, 100) == pdTRUE;
}

void Logger::logPrefix(const char* name)
{
  portENTER_CRITICAL();
  char *s = pcTaskGetName(NULL);
  portEXIT_CRITICAL();

  Serial.write('[');
  Serial.print(s);
  Serial.write(']');
  Serial.write('[');
  Serial.print(millis());
  Serial.write(']');
  Serial.write('[');

  if (name != NULL) {
    int a = 0;
    int b = 0;
    int i = 0;

    while (name[i]) {
      if (name[i] == ' ') { a = i; }
      else if (name[i] == '(') { b = i; break; }
      i++;
    }

    for (i = a + 1; i < b; i++) { Serial.write(name[i]); }
  }

  Serial.write(']');
  Serial.write(' ');
}

void Logger::unlock()
{
  xSemaphoreGive(logMutex);
}
