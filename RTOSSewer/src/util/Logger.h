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

#define VC(c) Serial.write(c)
#define VF(f) Serial.print(f)
#define VH2(i) Logger_printHex2(i)
#define VI(i) Serial.print(i)
#define VL(l) Serial.print(l)

#define LOG(...) { if (Logger_begin(__FUNCTION__)) { __VA_ARGS__; Logger_end(); } }
#define LOGS(s) LOG(VS(s))

BaseType_t Logger_begin(const char *func);
void Logger_end();
void Logger_printHex2(BaseType_t b);
void Logger_printPSTR(const char *s);
void Logger_printSTR(const char *s);

#else

#define LOG(...)
#define LOGS(s)

#endif

#endif
