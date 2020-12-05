// FileLog.h: interface for the CFileLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(FILELOG_H)
#define FILELOG_H

#include "GCLCLog.h"

class CFileLog : public CGCLCLog {
public:
  CFileLog(ofstream &f);
  virtual ~CFileLog();

  virtual void Reset();
  virtual void AddText(const string &sText);

private:
  ofstream &m_f;
};

#endif // !defined(FILELOG_H)
