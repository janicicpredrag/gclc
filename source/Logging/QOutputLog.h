#if !defined(QOutputLog_H)
#define QOutputLog_H

#include "Logging/GCLCLog.h"
#include <QPlainTextEdit>

class CDebugView;

class QOutputLog : public CGCLCLog {
public:
  QOutputLog(QPlainTextEdit *pOutput);
  virtual ~QOutputLog();

  virtual void Reset();
  virtual void AddText(const std::string &sText);

  bool IsOverLimit() { return m_bOverLimit; }

private:
  QPlainTextEdit *m_pOutput;
  int m_nTextLen;
  bool m_bOverLimit;
};

#endif // !defined(QOutputLog_H)
