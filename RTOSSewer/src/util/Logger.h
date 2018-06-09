#ifndef LOGGER_H
#define LOGGER_H

#include "../Common.h"

#if DEBUG
#define LOGC(c) Logger_logc(__FUNCTION__, c)
#define LOGF(f) Logger_logf(__FUNCTION__, f)
#define LOGI(i) Logger_logi(__FUNCTION__, i)
#define LOGL(l) Logger_logl(__FUNCTION__, l)
#define LOGS(s) Logger_logs(__FUNCTION__, s)
#define LOGT(fmt, ...) Logger_logt(__FUNCTION__, fmt, __VA_ARGS__)
#define FRAC02(f) (abs((int)(f * 100)) % 100)
#else
#define LOGC(c)
#define LOGF(f)
#define LOGI(i)
#define LOGL(l)
#define LOGS(s)
#define LOGT(fmt, ...)
#endif

void Logger_setup();
void Logger_logc(const char *func, char c);
void Logger_logf(const char *func, float f);
void Logger_logi(const char *func, int i);
void Logger_logs(const char *func, const char *s);
void Logger_logt(const char *func, const char *fmt, ...);

#endif
