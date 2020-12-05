// GCLCLog.h: interface for the CGCLCLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(GCLCLOG_H)
#define GCLCLOG_H

#include <string>

using namespace std;

class CGCLCLog {
public:
  CGCLCLog();
  virtual ~CGCLCLog();

  virtual void Reset() = 0;
  virtual void AddText(const string &sText) = 0;
};

#endif // !defined(GCLCLOG_H)
