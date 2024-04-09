#include "QOutputLog.h"

// --------------------------------------------------------------------------------------------

QOutputLog::QOutputLog(QPlainTextEdit *pOutput) { m_pOutput = pOutput; }

// --------------------------------------------------------------------------------------------

void QOutputLog::Reset() { m_pOutput->clear(); }

// --------------------------------------------------------------------------------------------

void QOutputLog::AddText(const std::string &sText) {
  m_pOutput->insertPlainText(QString::fromStdString(sText));
}

// --------------------------------------------------------------------------------------------
