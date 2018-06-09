#ifndef LOGGER_H
#define LOGGER_H

#include "../Common.h"

#if DEBUG
#define LOGF(f) Logger::logf(__PRETTY_FUNCTION__, f)
#define LOGI(i) Logger::logi(__PRETTY_FUNCTION__, i)
#define LOGS(s) Logger::logs(__PRETTY_FUNCTION__, s)
#define LOGT(fmt, ...) Logger::logt(__PRETTY_FUNCTION__, fmt, __VA_ARGS__)
#else
#define LOGF(f)
#define LOGI(i)
#define LOGS(s)
#define LOGT(fmt, ...)
#endif

#define FRAC02(f) (abs((int)(f * 100)) % 100)

class Logger {
public:
  static void setup();
  static void logf(const char *func, float f);
  static void logi(const char *func, int i);
  static void logs(const char *func, const char *s);
  static void logt(const char *func, const char *fmt, ...);
private:
  static SemaphoreHandle_t logMutex;
  static bool lock();
  static void logPrefix(const char *func);
  static void unlock();
};

#endif
