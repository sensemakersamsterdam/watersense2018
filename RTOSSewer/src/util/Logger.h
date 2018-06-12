#include "../Common.h"

#if USE_LOGGER && !defined(LOGGER_H)
#define LOGGER_H


/*******************************************************************************
 * Definitions
 ******************************************************************************/

#ifdef ARDUINO_ARCH_SAMD
#define VS(s) Logger_printSTR(s)
#else
#define VS(s) Logger_printPSTR(PSTR(s))
#endif

#define VC(c)    Logger_printCH(c)
#define VF(f)    Logger_printFL(f)
#define VH02(i)  Logger_printHEX02(i)
#define VUI16(i) Logger_printUI32(i)
#define VUI32(i) Logger_printUI32(i)

#define LOG(...) { if (Logger_begin(__FUNCTION__)) { __VA_ARGS__; Logger_end(); } }
#define LOGS(s) LOG(VS(s))


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void Logger_setup();


/*******************************************************************************
 * Public
 ******************************************************************************/

BaseType_t Logger_begin(const char *func);
void Logger_end();
void Logger_printCH(BaseType_t c);
void Logger_printFL(float f);
void Logger_printHEX02(BaseType_t i);
void Logger_printPSTR(const char *s);
void Logger_printSTR(const char *s);
void Logger_printUI32(uint32_t i);


/*******************************************************************************
 * Private
 ******************************************************************************/

#if USE_LOGGER_SYSINFO
static void Logger_printSysinfo();
#endif

#endif // USE_LOGGER && !defined(LOGGER_H)
