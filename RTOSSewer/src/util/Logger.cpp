#include <stdarg.h>
#include "Logger.h"


/*******************************************************************************
 * Functions
 ******************************************************************************/

static BaseType_t lock();
static void logPrefix(const char *func);
static void unlock();


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

  while (!Serial && millis() < 10000);

  logMutex = xSemaphoreCreateMutexStatic(&logMutexBuffer);

  LOGS("started");
}


/*******************************************************************************
 * Public
 ******************************************************************************/

void Logger_logs(const char *func, const char *s)
{
  if (!lock()) { return; }

  logPrefix(func);
  Serial.println(s);

  unlock();
}

void Logger_logf(const char *func, float f)
{
  if (!lock()) { return; }

  logPrefix(func);
  Serial.println(f);

  unlock();
}

void Logger_logi(const char *func, int i)
{
  if (!lock()) { return; }

  logPrefix(func);
  Serial.println(i);

  unlock();
}

void Logger_logt(const char *func, const char *fmt, ...)
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

static BaseType_t lock()
{
  BaseType_t b = xSemaphoreTake(logMutex, 100);

  if (!b) { LOGS("resource is busy"); }

  return b;
}

static void logPrefix(const char *func)
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
    BaseType_t a = 0;
    BaseType_t b = 0;
    BaseType_t i = 0;

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

static void unlock()
{
  xSemaphoreGive(logMutex);
}
