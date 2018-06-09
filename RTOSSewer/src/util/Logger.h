#ifndef LOGGER_H
#define LOGGER_H

#include "../Common.h"

#if DEBUG
  #define LOGA(s) Logger::loga(__PRETTY_FUNCTION__, s)
  #define LOGI(f) Logger::logf(__PRETTY_FUNCTION__, f)
  #define LOGI(i) Logger::logi(__PRETTY_FUNCTION__, i)
  #define LOGT(fmt, ...) Logger::logt(__PRETTY_FUNCTION__, fmt, __VA_ARGS__)
#else
  #define LOGA(s)
  #define LOGI(f)
  #define LOGI(i)
  #define LOGT(fmt, ...)
#endif

#define FRAC02(f) (abs((int)(f * 100)) % 100)

class Logger {
public:
  static void setup();
  static void loga(const char* name, const char* s);
  static void logf(const char* name, float f);
  static void logi(const char* name, int i);
  static void logt(const char* name, const char* fmt, ...);
private:
  static SemaphoreHandle_t logMutex;
  static bool lock();
  static void logPrefix(const char* name);
  static void unlock();
};

#endif
