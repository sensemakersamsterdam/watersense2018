#ifdef ARDUINO_AVR_MEGA2560
#include "ARDUINO_MEGA2560_R3.h"


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void ARDUINO_MEGA2560_R3_setup()
{
  LOGS("started");
}


/*******************************************************************************
 * Public
 ******************************************************************************/

void ARDUINO_MEGA2560_R3_logSysinfo()
{
  #ifdef ARDUINO
  LOGT("ARDUINO: %d.%d.%d", ARDUINO / 10000, ARDUINO / 100 % 100, ARDUINO % 100);
  #endif

  #if defined(ARDUINO_ARCH_AVR) && defined(ARDUINO_AVR_MEGA2560)
  LOGS("ARDUINO ARCH: ARDUINO_ARCH_AVR, ARDUINO_AVR_MEGA2560");
  #endif

  #ifdef __VERSION__
  #ifdef __GNUG__
  LOGS("COMPILER: G++ " __VERSION__);
  #else
  LOGS("COMPILER: GCC " __VERSION__);
  #endif
  #endif

  #ifdef __AVR_ARCH__
  LOGT("AVR ARCH: %d", __AVR_ARCH__);
  #endif

  #ifdef __AVR_ATmega2560__
  LOGS("MCU: ATmega2560");
  #endif
  
  #ifdef F_CPU
  LOGT("F_CPU: %ld", F_CPU);
  #endif
}

#endif
