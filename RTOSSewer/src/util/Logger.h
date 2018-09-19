#include "../Common.h"

#ifndef LOGGER_H
#define LOGGER_H


/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define STR(s) XSTR(s)
#define XSTR(s) #s

#if USE_LOGGER

#define VC(c)              Logger_printCH(c)
#define VSZ(s)             Logger_printSTR(s)
#define VF(f)              Logger_printFL(f)
#define VI8(i)             Logger_printI32(i)
#define VUI8(i)            Logger_printUI32(i)
#define VUI16(i)           Logger_printUI32(i)
#define VUI32(i)           Logger_printUI32(i)
#define VUI8AH02(ia, size) Logger_printUI8AH02(ia, size)
#define VUI8H02(i)         Logger_printUI8H02(i)

#ifdef ARDUINO_ARCH_SAMD
#define VS(s) Logger_printSTR(s)
#else
#define VS(s) Logger_printPSTR(PSTR(s))
#endif

#define LOG(...) { if (Logger_begin(__FUNCTION__)) { __VA_ARGS__; Logger_end(); } }
#define LOGS(s) LOG(VS(s))
#define LOG_SETUP_RESULT_TEXT(b) LOG(Logger_logSetupResultText(b))

#else

#define VC(c)
#define VSZ(s)
#define VF(f)
#define VI8(i)
#define VUI8(i)
#define VUI16(i)
#define VUI32(i)
#define VUI8AH02(ia, size)
#define VUI8H02(i)
#define VS(s)
#define LOG(...)
#define LOGS(s)
#define LOG_SETUP_RESULT_TEXT

#endif // USE_LOGGER


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void Logger_setup();


/*******************************************************************************
 * Public
 ******************************************************************************/

BaseType_t Logger_begin(const char *func);
void Logger_end();
void Logger_logMeasurementText();
void Logger_logSetupResultText(bool b);
void Logger_printCH(BaseType_t c);
void Logger_printFL(float f);
void Logger_printI32(int32_t i);
void Logger_printPSTR(const char *s);
void Logger_printSTR(const char *s);
void Logger_printUI8AH02(const uint8_t *ia, uint8_t size);
void Logger_printUI8H02(uint8_t i);
void Logger_printUI32(uint32_t i);

#endif // LOGGER_H
