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

  #ifndef ARDUINO_SODAQ_ONE
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

void Logger_printHEX02(BaseType_t i)
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


/*******************************************************************************
 * Private
 ******************************************************************************/

#if USE_LOGGER_SYSINFO
static void Logger_printSysinfo()
{
  #ifdef ARDUINO
  LOG(VS("ARDUINO: "), VUI16(ARDUINO / 10000), VC('.'), VUI16(ARDUINO / 100 % 100), VC('.'), VUI16(ARDUINO % 100));
  #endif

  #if defined(ARDUINO_ARCH_AVR) && defined(ARDUINO_AVR_MEGA2560)
  LOGS("ARDUINO ARCH: ARDUINO_ARCH_AVR, ARDUINO_AVR_MEGA2560");
  #endif

  #if defined(ARDUINO_ARCH_SAMD) && defined(__SAMD21G18A__)
  LOGS("ARDUINO ARCH: ARDUINO_ARCH_SAMD, __SAMD21G18A__");
  #endif

  #ifdef __VERSION__
  #ifdef __GNUG__
  LOGS("COMPILER: G++ " __VERSION__);
  #else
  LOGS("COMPILER: GCC " __VERSION__);
  #endif
  #endif

  #ifdef __ARM_ARCH
  #ifdef __ARM_ARCH_PROFILE
  LOG(VS("ARM ARCH: "), VUI16(__ARM_ARCH), VC(__ARM_ARCH_PROFILE));
  #else
  LOG(VS("ARM ARCH: "), VUI16(__ARM_ARCH));
  #endif
  #endif

  #ifdef __AVR_ARCH__
  LOG(VS("AVR ARCH: "), VUI16(__AVR_ARCH__));
  #endif

  #ifdef __AVR_ATmega2560__
  LOGS("MCU: ATmega2560");
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
