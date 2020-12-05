#if !defined(TIME_H)
#define TIME_H

#include "../Utils/Utils.h"

class CTimer {
public:
  CTimer() {}
  virtual ~CTimer() {}

  void StartMeasuringTime();
  double ElapsedTime();

#if defined(_PLATFORM_WIN_) || defined(_PLATFORM_LINUX_)
  long int m_tStartTime;
#else
  time_t m_t0;
#endif
};

#endif // !defined(TIME_H)
