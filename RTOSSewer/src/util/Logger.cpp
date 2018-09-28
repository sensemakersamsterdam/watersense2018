/*
RTOSSewer project: sensing water quality in a variety of ways.

Copyright (C) 2018 Sensemakers Amsterdam. All rights reserved.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "Logger.h"

#if USE_LOGGER


/*******************************************************************************
 * State
 ******************************************************************************/

static SemaphoreHandle_t logMutex;


/*******************************************************************************
 * Private declarations
 ******************************************************************************/

static void Logger_printSysinfo();


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void Logger_setup()
{
  static StaticSemaphore_t logMutexBuffer;

  #ifndef ARDUINO_ARCH_SAMD
  Serial.begin(9600);
  #endif

  while (!Serial && millis() < 10000);

  logMutex = xSemaphoreCreateMutexStatic(&logMutexBuffer);

  LOG_SETUP_RESULT_TEXT(true);

  Logger_printSysinfo();
}


/*******************************************************************************
 * Public
 ******************************************************************************/

BaseType_t Logger_begin(const char *func)
{
  if (!xSemaphoreTake(logMutex, 200)) { return false; }

  Logger_printCH('[');
  Logger_printSTR(pcTaskGetName(NULL));
  Logger_printCH(']');
  Logger_printCH('[');
  Logger_printUI32(xTaskGetTickCount());
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

void Logger_logSetupResultText(bool b)
{
  if (b) { VS("ok"); } else { VS("fail"); }
}

void Logger_printCH(BaseType_t c)
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

void Logger_printI32(int32_t i)
{
  if (i < 0) {
    Logger_printCH('-');
    Logger_printUI32(-i);
  } else {
    Logger_printUI32(i);
  }
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
  uint64_t j = 10;
  while (i / j > 0) { j *= 10; }

  while (true) {
    j /= 10;
    Logger_printCH(i / j + '0');
    if (j == 1) { break; }
    i %= j;
  }
}

void Logger_printUI8AH02(const uint8_t *ia, uint8_t size)
{
  for (BaseType_t i = 0; i < size; i++) {
    Logger_printUI8H02(ia[i]);
  }
}

void Logger_printUI8H02(uint8_t i)
{
  uint8_t a = i / 16;
  uint8_t b = i % 16;
  Logger_printCH(a < 10 ? a + '0' : a + 'A' - 10);
  Logger_printCH(b < 10 ? b + '0' : b + 'A' - 10);
}



/*******************************************************************************
 * Private
 ******************************************************************************/

static void Logger_printSysinfo()
{
  #if defined(PROJECT_NAME) && defined(PROJECT_VERSION)
  LOGS("PROJECT: " PROJECT_NAME " " STR(PROJECT_VERSION));
  #endif

  #ifdef ARDUINO
  LOG(VS("ARDUINO: "), VUI16(ARDUINO / 10000), VC('.'), VUI16(ARDUINO / 100 % 100), VC('.'), VUI16(ARDUINO % 100));
  #endif

  #ifdef ARDUINO_ARCH_SAMD
  #ifdef __SAMD21G18A__
  LOGS("ARDUINO ARCH: ARDUINO_ARCH_SAMD, __SAMD21G18A__");
  #else
  LOGS("ARDUINO ARCH: ARDUINO_ARCH_SAMD");
  #endif
  #endif

  #ifdef __ARM_ARCH
  #ifdef __ARM_ARCH_PROFILE
  LOG(VS("ARM ARCH: " STR(__ARM_ARCH)), VC(__ARM_ARCH_PROFILE));
  #else
  LOGS("ARM ARCH: " STR(__ARM_ARCH));
  #endif
  #endif

  #ifdef __VERSION__
  #ifdef __GNUG__
  LOGS("COMPILER: G++ " __VERSION__);
  #else
  LOGS("COMPILER: GCC " __VERSION__);
  #endif
  #endif

  #ifdef _NEWLIB_VERSION
  LOGS("NEWLIB: " _NEWLIB_VERSION)
  #endif

  #ifdef USB_MANUFACTURER
  LOGS("USB_MANUFACTURER: " USB_MANUFACTURER);
  #endif

  #ifdef USB_PRODUCT
  LOGS("USB_PRODUCT: " USB_PRODUCT);
  #endif

  #ifdef F_CPU
  LOG(VS("F_CPU: "), VUI32(F_CPU));
  #endif
}

#endif // USE_LOGGER
