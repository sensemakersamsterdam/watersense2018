#include "Logger.h"

#if USE_LOGGER


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

  #ifndef ARDUINO_ARCH_SAMD
  Serial.begin(9600);
  #endif

  while (!Serial && millis() < 10000);

  logMutex = xSemaphoreCreateMutexStatic(&logMutexBuffer);

  LOGS("started");

  #if USE_LOGGER_SYSINFO
  Logger_printSysinfo();
  #endif
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

#if USE_LOGGER_SYSINFO
static void Logger_printSysinfo()
{
  #ifdef ARDUINO
  LOG(VS("ARDUINO: "), VUI16(ARDUINO / 10000), VC('.'), VUI16(ARDUINO / 100 % 100), VC('.'), VUI16(ARDUINO % 100));
  #endif

  #ifdef ARDUINO_ARCH_AVR
  #if defined(ARDUINO_AVR_MEGA2560)
  LOGS("ARDUINO ARCH: ARDUINO_ARCH_AVR, ARDUINO_AVR_MEGA2560");
  #elif defined(ARDUINO_AVR_PRO)
  LOGS("ARDUINO ARCH: ARDUINO_ARCH_AVR, ARDUINO_AVR_PRO");
  #elif defined(ARDUINO_AVR_UNO)
  LOGS("ARDUINO ARCH: ARDUINO_ARCH_AVR, ARDUINO_AVR_UNO");
  #else
  LOGS("ARDUINO ARCH: ARDUINO_ARCH_AVR");
  #endif
  #endif

  #ifdef ARDUINO_ARCH_SAMD
  #if defined(__SAMD21G18A__)
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

  #ifdef __AVR_ARCH__
  LOGS("AVR ARCH: " STR(__AVR_ARCH__));
  #endif

  #ifdef __AVR_DEVICE_NAME__
  LOGS("AVR DEVICE: " STR(__AVR_DEVICE_NAME__));
  #endif

  #ifdef __AVR_LIBC_VERSION_STRING__
  LOGS("AVR LIBC: " __AVR_LIBC_VERSION_STRING__)
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
#endif // USE_LOGGER_SYSINFO

#endif // USE_LOGGER
