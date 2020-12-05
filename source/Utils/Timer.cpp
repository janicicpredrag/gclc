#include "Timer.h"

// ----------------------------------------------------------------------------

void CTimer::StartMeasuringTime() {
#if defined(_PLATFORM_WIN_) || defined(_PLATFORM_LINUX_)
  struct rusage usage;
  getrusage(RUSAGE_SELF, &usage);
  m_tStartTime = 1000000 * usage.ru_utime.tv_sec + usage.ru_utime.tv_usec +
                 1000000 * usage.ru_stime.tv_sec + usage.ru_stime.tv_usec;
#else
  time(&m_t0);
#endif
}

// ----------------------------------------------------------------------------

double CTimer::ElapsedTime() {
#if defined(_PLATFORM_WIN_) || defined(_PLATFORM_LINUX_)
  long int TimeElapsed, TimeCurrent;
  struct rusage usage;
  getrusage(RUSAGE_SELF, &usage);
  TimeCurrent = 1000000 * usage.ru_utime.tv_sec + usage.ru_utime.tv_usec +
                1000000 * usage.ru_stime.tv_sec + usage.ru_stime.tv_usec;
  TimeElapsed = TimeCurrent - m_tStartTime;
  return ((double)TimeElapsed / (double)1000000);
#else
  time_t t1;
  time(&t1);
  return (double)(t1 - m_t0);
#endif
}

// ----------------------------------------------------------------------------
