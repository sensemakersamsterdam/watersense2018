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

  Logger_printCH('[');
  Logger_printSTR(pcTaskGetName(NULL));
  Logger_printCH(']');
  Logger_printCH('[');
  Logger_printUI32(millis());
  Logger_printCH(']');
  Logger_printCH('[');
  Logger_printSTR(func);
  Logger_printCH(']');
  Logger_printCH(' ');

  return true;
}

void Logger_end()
{
  Logger_printCH('\r');
  Logger_printCH('\n');
  xSemaphoreGive(logMutex);
}

void Logger_printCH(uint8_t c)
{
  Serial.write(c);
}

void Logger_printFL(float f)
{
  if (f < 0) {
    Logger_printCH('-');
    f = -f;
  }

  uint32_t i = (int32_t)f;
  Logger_printUI32(i);

  i = (uint32_t)((f - i) * 10000);
  Logger_printCH('.');

  for (uint16_t j = 1000; j; j /= 10) {
    Logger_printCH(i / j + '0');
    i %= j;
  }
}

void Logger_printHEX02(uint8_t i)
{
  uint8_t a = i / 16;
  uint8_t b = i % 16;
  Logger_printCH(a < 10 ? a + '0' : a + 'A' - 10);
  Logger_printCH(b < 10 ? b + '0' : b + 'A' - 10);
}

void Logger_printPSTR(const char *s)
{
  while (true) {
    uint8_t c = pgm_read_byte(s++);
    if (c == 0) { break; }
    Logger_printCH(c);
  }
}

void Logger_printSTR(const char *s)
{
  while (true) {
    uint8_t c = *s++;
    if (c == 0) { break; }
    Logger_printCH(c);
  }
}

void Logger_printUI32(uint32_t i)
{
  uint32_t j = 10;
  while (i / j > 0) { j *= 10; }

  while (true) {
    j /= 10;
    Logger_printCH(i / j + '0');
    if (j == 1) { break; }
    i %= j;
  };
}

#endif
