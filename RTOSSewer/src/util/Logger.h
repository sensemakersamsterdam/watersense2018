#ifndef LOGGER_H
#define LOGGER_H

#include "../Common.h"

#if DEBUG

#define LOGS(s) Logger_logs(__FUNCTION__, s)
#define LOGT(fmt, ...) Logger_logt(__FUNCTION__, fmt, __VA_ARGS__)

#define FRAC02(f) (abs((int)(f * 100)) % 100)

void Logger_setup();
void Logger_logSysinfo();
void Logger_logs(const char *func, const char *s);
void Logger_logt(const char *func, const char *fmt, ...);

#else

#define LOGS(s)
#define LOGT(fmt, ...)

#endif

#endif
