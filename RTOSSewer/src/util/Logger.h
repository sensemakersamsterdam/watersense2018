#ifndef LOGGER_H
#define LOGGER_H

#include "../Common.h"

#if DEBUG

void Logger_setup();

#if defined(ARDUINO_ARCH_SAMD)
#define VS(s) Logger_printSTR(s)
#else
#define VS(s) Logger_printPSTR(PSTR(s))
#endif

#define VC(c) Logger_printCH(c)
#define VF(f) Logger_printFL(f)
#define VH02(i) Logger_printHEX02(i)
#define VUI16(i) Logger_printUI32(i)
#define VUI32(i) Logger_printUI32(i)

#define LOG(...) { if (Logger_begin(__FUNCTION__)) { __VA_ARGS__; Logger_end(); } }
#define LOGS(s) LOG(VS(s))

BaseType_t Logger_begin(const char *func);
void Logger_end();
void Logger_printCH(uint8_t c);
void Logger_printFL(float f);
void Logger_printHEX02(uint8_t i);
void Logger_printPSTR(const char *s);
void Logger_printSTR(const char *s);
void Logger_printUI32(uint32_t i);

#else

#define LOG(...)
#define LOGS(s)

#endif

#endif
