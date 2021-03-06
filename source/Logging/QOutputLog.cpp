#include "QOutputLog.h"

// --------------------------------------------------------------------------------------------

QOutputLog::QOutputLog(QPlainTextEdit *pOutput) { m_pOutput = pOutput; }

// --------------------------------------------------------------------------------------------

QOutputLog::~QOutputLog() {}

// --------------------------------------------------------------------------------------------

void QOutputLog::Reset() { m_pOutput->clear(); }

// --------------------------------------------------------------------------------------------

void QOutputLog::AddText(const std::string &sText) {
  m_pOutput->insertPlainText(QString::fromStdString(sText));
}

// --------------------------------------------------------------------------------------------
