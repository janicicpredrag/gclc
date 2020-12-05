// EditorInput.h: interface for the QEditorInput class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(EDITORINPUT_H)
#define EDITORINPUT_H

#include "Input/GCLCInput.h"
#include <QString>
#include <QTextEdit>

class QTextEdit;

class QEditorInput : public CGCLCInput {
public:
  QEditorInput(QTextEdit *pe);
  QEditorInput(QString &s);

  virtual ~QEditorInput();

  virtual void Reset();
  virtual GReturnValue ReadChar(unsigned char &c);
  virtual bool EndOfInputData();
  void Replace(QRegExp before, QString after);

  QString getText() { return text; }

private:
  QString text;
  int string_pos;
};

#endif // !defined(EDITORINPUT_H)
