// FileLog.h: interface for the CFileLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(FILELOG_H)
#define FILELOG_H

#include "GCLCLog.h"

class CFileLog : public CGCLCLog {
public:
  CFileLog(std::ostream &f);
  virtual ~CFileLog();

  virtual void Reset();
  virtual void AddText(const std::string &sText);

private:
  std::ostream &m_f;
};

#endif // !defined(FILELOG_H)
