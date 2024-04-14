// EditorInput.cpp: implementation of the QEditorInput class.
//////////////////////////////////////////////////////////////////////

#include "EditorInput.h"
#include <QTextEdit>
#include <QRegularExpression>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

QEditorInput::QEditorInput(QTextEdit *pTextBuffer) {
  text = pTextBuffer->toPlainText();
  string_pos = 0;
  m_iInputLine = 1;
  m_iPosition = 0;
}

QEditorInput::QEditorInput(QString &s) {
  text = s;
  string_pos = 0;
  m_iInputLine = 1;
  m_iPosition = 0;
}

void QEditorInput::Reset() {
  CGCLCInput::Reset();
  /*
      m_nTotalChars = m_nLineCount = m_pTextBuffer->GetLineCount();
          m_pLine = m_pTextBuffer->GetLineChars(0);
          m_nLineLen = m_pTextBuffer->GetLineLength(0);
  */
}

GReturnValue QEditorInput::ReadChar(unsigned char &c) {
  if (EndOfInputData())
    return rvG_EndOfData;
  c = text.at(string_pos++).toLatin1();
  if (c == '\n') {
    m_iInputLine++;
    m_iPosition = 0;
  } else
    m_iPosition++;
  return rvG_OK;
}

bool QEditorInput::EndOfInputData() { return (string_pos >= text.size()); }

void QEditorInput::Replace(QRegularExpression before, QString after) {
  text.replace(before, after);
}
