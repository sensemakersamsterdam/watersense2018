#include <stdarg.h>
#include "Logger.h"


/*******************************************************************************
 * Functions
 ******************************************************************************/

static BaseType_t Logger_lock();
static void Logger_logSysinfo();
static void Logger_logPrefix(const char *func);
static void Logger_unlock();


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

  Serial.begin(9600);
  while (!Serial && millis() < 10000);

  logMutex = xSemaphoreCreateMutexStatic(&logMutexBuffer);

  LOGS("started");

  Logger_logSysinfo();
}


/*******************************************************************************
 * Public
 ******************************************************************************/

void Logger_logc(const char *func, char c)
{
  if (!Logger_lock()) { return; }

  Logger_logPrefix(func);
  Serial.println(c);

  Logger_unlock();
}

void Logger_logf(const char *func, float f)
{
  if (!Logger_lock()) { return; }

  Logger_logPrefix(func);
  Serial.println(f);

  Logger_unlock();
}

void Logger_logi(const char *func, int i)
{
  if (!Logger_lock()) { return; }

  Logger_logPrefix(func);
  Serial.println(i);

  Logger_unlock();
}

void Logger_logl(const char *func, long l)
{
  if (!Logger_lock()) { return; }

  Logger_logPrefix(func);
  Serial.println(l);

  Logger_unlock();
}

void Logger_logs(const char *func, const char *s)
{
  if (!Logger_lock()) { return; }

  Logger_logPrefix(func);
  Serial.println(s);

  Logger_unlock();
}

void Logger_logt(const char *func, const char *fmt, ...)
{
  if (!Logger_lock()) { return; }

  char s[40];

  va_list args;
  va_start(args, fmt);
  vsnprintf(s, sizeof(s), fmt, args);
  va_end(args);

  Logger_logPrefix(func);
  Serial.println(s);

  Logger_unlock();
}


/*******************************************************************************
 * Private
 ******************************************************************************/

static BaseType_t Logger_lock()
{
  BaseType_t b = xSemaphoreTake(logMutex, 100);

  if (!b) { LOGS("resource is busy"); }

  return b;
}

static void Logger_logPrefix(const char *func)
{
  Serial.write('[');
  Serial.print(pcTaskGetName(NULL));
  Serial.write("][");
  Serial.print(millis());
  Serial.write("][");
  Serial.write(func);
  Serial.write("] ");
}

static void Logger_logSysinfo()
{
  #ifdef ARDUINO
  LOGT("ARDUINO: %d.%d.%d", ARDUINO / 10000, ARDUINO / 100 % 100, ARDUINO % 100);
  #endif

  #if defined(__ARM_ARCH) && defined(__ARM_ARCH_PROFILE)
  LOGT("ARM_ARCH: %d%c", __ARM_ARCH, __ARM_ARCH_PROFILE);
  #endif

  #ifdef __VERSION__
  #ifdef __GNUG__
  LOGS("COMPILER: G++ " __VERSION__);
  #else
  LOGS("COMPILER: GCC " __VERSION__);
  #endif
  #endif

  #ifdef F_CPU
  LOGT("F_CPU: %ld", F_CPU);
  #endif

  #if defined(ARDUINO_SODAQ_ONE) && defined(ARDUINO_ARCH_SAMD) && defined(__SAMD21G18A__)
  LOGS("MACROSES: ARDUINO_SODAQ_ONE, ARDUINO_ARCH_SAMD, __SAMD21G18A__");
  #endif

  #ifdef USB_MANUFACTURER
  LOGT("USB_MANUFACTURER: %s", USB_MANUFACTURER);
  #endif

  #ifdef USB_PRODUCT
  LOGT("USB_PRODUCT: %s", USB_PRODUCT);
  #endif
}

static void Logger_unlock()
{
  xSemaphoreGive(logMutex);
}
