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

  LOGS("started");
}


/*******************************************************************************
 * Public
 ******************************************************************************/

void Logger::logs(const char *func, const char *s)
{
  if (!lock()) { return; }
  logPrefix(func);
  Serial.println(s);
  unlock();
}

void Logger::logf(const char *func, float f)
{
  if (!lock()) { return; }
  logPrefix(func);
  Serial.println(f);
  unlock();
}

void Logger::logi(const char *func, int i)
{
  if (!lock()) { return; }
  logPrefix(func);
  Serial.println(i);
  unlock();
}

void Logger::logt(const char *func, const char *fmt, ...)
{
  if (!lock()) { return; }

  char s[40];

  va_list args;
  va_start(args, fmt);
  vsnprintf(s, sizeof(s), fmt, args);
  va_end(args);

  logPrefix(func);
  Serial.println(s);

  unlock();
}


/*******************************************************************************
 * Private
 ******************************************************************************/

bool Logger::lock()
{
  return xSemaphoreTake(logMutex, 100) == pdTRUE;
}

void Logger::logPrefix(const char *func)
{
  const char *tn = pcTaskGetName(NULL);

  Serial.write('[');
  Serial.print(tn);
  Serial.write(']');
  Serial.write('[');
  Serial.print(millis());
  Serial.write(']');
  Serial.write('[');

  if (func != NULL) {
    int a = 0;
    int b = 0;
    int i = 0;

    while (func[i]) {
      if (func[i] == ' ') { a = i; }
      else if (func[i] == '(') { b = i; break; }
      i++;
    }

    for (i = a + 1; i < b; i++) { Serial.write(func[i]); }
  }

  Serial.write(']');
  Serial.write(' ');
}

void Logger::unlock()
{
  xSemaphoreGive(logMutex);
}
