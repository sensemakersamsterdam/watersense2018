#ifdef ARDUINO_AVR_MEGA2560
#include "ARDUINO_MEGA2560_R3.h"


/*******************************************************************************
 * Private functions declarations
 ******************************************************************************/

static void ARDUINO_MEGA2560_R3_logSysinfo();


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void ARDUINO_MEGA2560_R3_setup()
{
  LOGS("started");

  #if DEBUG
  ARDUINO_MEGA2560_R3_logSysinfo();
  #endif
}


/*******************************************************************************
 * Private
 ******************************************************************************/

static void ARDUINO_MEGA2560_R3_logSysinfo()
{
  #ifdef ARDUINO
  LOG(VS("ARDUINO: "), VUI16(ARDUINO / 10000), VC('.'), VUI16(ARDUINO / 100 % 100), VC('.'), VUI16(ARDUINO % 100));
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
  LOG(VS("AVR ARCH: "), VUI16(__AVR_ARCH__));
  #endif

  #ifdef __AVR_ATmega2560__
  LOGS("MCU: ATmega2560");
  #endif

  #ifdef F_CPU
  LOG(VS("F_CPU: "), VUI32(F_CPU));
  #endif
}

#endif
