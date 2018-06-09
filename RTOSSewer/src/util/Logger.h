#ifndef LOGGER_H
#define LOGGER_H

#include "../Common.h"

#if DEBUG
#define LOGF(f) Logger_logf(__PRETTY_FUNCTION__, f)
#define LOGI(i) Logger_logi(__PRETTY_FUNCTION__, i)
#define LOGS(s) Logger_logs(__PRETTY_FUNCTION__, s)
#define LOGT(fmt, ...) Logger_logt(__PRETTY_FUNCTION__, fmt, __VA_ARGS__)
#else
#define LOGF(f)
#define LOGI(i)
#define LOGS(s)
#define LOGT(fmt, ...)
#endif

#define FRAC02(f) (abs((int)(f * 100)) % 100)

void Logger_setup();
void Logger_logf(const char *func, float f);
void Logger_logi(const char *func, int i);
void Logger_logs(const char *func, const char *s);
void Logger_logt(const char *func, const char *fmt, ...);

#endif
