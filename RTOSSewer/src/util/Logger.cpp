#include <stdarg.h>
#include "Logger.h"


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
}


/*******************************************************************************
 * Public
 ******************************************************************************/

void Logger_logSysinfo()
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

void Logger_logs(const char *func, const char *s)
{
  Logger_logt(func, "%s", s);
}

void Logger_logt(const char *func, const char *fmt, ...)
{
  char s[40];

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
