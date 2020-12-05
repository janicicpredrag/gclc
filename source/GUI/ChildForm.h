#ifndef CHILDFORM_H
#define CHILDFORM_H

#include <QWidget>

namespace Ui {
class ChildForm;
}

class ChildForm : public QWidget {
  friend class ChildWindow;

  Q_OBJECT

public:
  explicit ChildForm(QWidget *parent = 0);
  ~ChildForm();

private slots:
  void on_editor_cursorPositionChanged();
  void on_editor_textChanged();

signals:
  void CursorMoved();
  void FileChanged();

private:
  Ui::ChildForm *ui;
};

#endif // CHILDFORM_H
