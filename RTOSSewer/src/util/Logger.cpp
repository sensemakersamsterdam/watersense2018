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

BaseType_t Logger_begin(const char *func)
{
  if (!xSemaphoreTake(logMutex, 100)) { return false; }

  Serial.write('[');
  Serial.write(pcTaskGetName(NULL));
  Serial.write(']');
  Serial.write('[');
  Serial.print(millis());
  Serial.write(']');
  Serial.write('[');
  Serial.write(func);
  Serial.write(']');
  Serial.write(' ');

  return true;
}

void Logger_end()
{
  Serial.println();
  xSemaphoreGive(logMutex);
}

void Logger_printHex2(BaseType_t b)
{
  if (b < 10) { Serial.write(' '); }
  Serial.print(b, HEX);
}

void Logger_printPSTR(const char *s)
{
  while (1) {
    unsigned char c = pgm_read_byte(s++);
    if (c == 0) break;
    Serial.write(c);
  }
}

void Logger_printSTR(const char *s)
{
  while (1) {
    unsigned char c = *s++;
    if (c == 0) break;
    Serial.write(c);
  }
}

#endif
