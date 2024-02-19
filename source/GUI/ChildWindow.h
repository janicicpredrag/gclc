#ifndef CHILDWINDOW_H
#define CHILDWINDOW_H

#include "ChildForm.h"
#include "Export/QGraphicsViewOutput.h"
#include "GCLCEngine/GCLC.h"
#include "Highlighter.h"
#include "OutputHighlighter.h"
#include "WatchWindow.h"
#include "ui_ChildForm.h"
#include <QGraphicsSceneMouseEvent>
#include <QMdiSubWindow>
#include <QTextEdit>
#include <QWidget>
#include <QtGui>

namespace Ui {
class ChildWindow;
}

enum exportFormat { epic, etikz, epst, eeps, esvg, eraster };

// --------------------------------------------------------------------------------------------

class GCLCGraphicsScene : public QGraphicsScene {
  Q_OBJECT

public:
  GCLCGraphicsScene(QObject *parent) : QGraphicsScene(parent) {}

signals:
  void viewCursorMoved(qreal, qreal);

protected:
  void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    QPointF P = mouseEvent->scenePos();
    emit(viewCursorMoved(P.x(), P.y()));

    QGraphicsScene::mouseMoveEvent(mouseEvent);
    update();
    mouseEvent->setAccepted(false);
  }

  //   void wheelEvent(QCloseEvent *event)    {
  //     update(sceneRect());
  //  qDebug() << "wheel event";
  //}
};

// --------------------------------------------------------------------------------------------

typedef struct _traced_point {
  bool defined;
  double x, y;
} traced_point;

// --------------------------------------------------------------------------------------------

class ChildWindow : public QMdiSubWindow {
  Q_OBJECT

public:
  explicit ChildWindow(QWidget *parent = 0);
  ~ChildWindow();

  bool close();

  bool Build(prover_config &Prover_params);
  bool Save(QString fileName);
  bool Export(enum exportFormat format, QString fileName);
  bool ExportToXML(QString fileName);

  void Undo();
  void Redo();
  void Cut();
  void Copy();
  void Paste();
  void SelectAll();
  void Find();
  void FindNext();
  void FindPrevious();
  void Replace();

  void zoomIn();
  void zoomOut();
  void zoomNormal();

  void setFilename(QString Name) { m_sFileName = Name; }
  QString getFileName() { return m_sFileName; }

  void setFileSaved(bool saved) { m_bFileSaved = saved; }
  bool isFileSaved() { return m_bFileSaved; }

  void setFileCompiled(bool saved) { m_bCompiled = saved; }
  bool isFileCompiled() { return m_bCompiled; }

  QString getFindReplaceStatus() { return m_sFindReplaceStatus; }

  void setCursorPos(QString pos) { m_CursorPos = pos; }
  QString getCursorPos() { return m_CursorPos; }

  void setAction(QAction *a) { m_Action = a; }
  QAction *getAction() { return m_Action; }

  qreal getZoomFactor() { return m_zoomFactor; }
  QPointF getViewCursorPos() { return m_ViewCursorPosition; }

  void enableHighlighting(bool enable);
  void pickColor();

  void showFreePoints();
  void showAnimationFrame(int i);
  void showRemainingAnimationFrames();
  void playAnimation(bool bPlay);
  int getAnimationFrames() { return m_nFrames; }
  int getCurrentFrame() { return m_nCurrentFrame; }

  void showWatchWindow();
  void hideWatchWindow() { m_Watch->hide(); }
  void refreshWatchWindow() {
    if (m_bWatchWindowShown)
      m_Watch->show();
  }

  ChildForm *getGCLCspec() { return m_ChildForm; }
  QTextEdit *getEditor() { return m_ChildForm->ui->editor; }
  QGraphicsView *getGraphicsOutput() { return m_ChildForm->ui->graphicsOutput; }
  QPlainTextEdit *getTextOutput() { return m_ChildForm->ui->output; }

protected:
  void closeEvent(QCloseEvent *event);

public slots:
  void updateFileChanged();
  void updateEditPos();
  void updateFileStatus(bool saved);
  void updateWatchCell(int row, int column);
  void updateViewCursorPos(qreal x, qreal y);

signals:
  void setMainSliderRange(int min, int max);
  void setMainSliderPos(int pos);
  void updateMainStatusBar();
  void updateMainMenu();
  void removeMenuAction(QAction *);

private:
  void annotateLineInEditor(int nLine, QColor color);
  void updateWatchWindow(CGCompiler *pc);

  ChildForm *m_ChildForm;
  GCLCGraphicsScene *m_Scene;
  QGraphicsViewOutput *m_pQGraphicsViewOutput;
  Highlighter *m_Highlighter;
  OutputHighlighter *m_OutputHighlighter;
  qreal m_zoomFactor;

  QString m_sFileName;
  QAction *m_Action;
  QPointF m_ViewCursorPosition;
  QString m_CursorPos;

  QString m_FindText;
  QString m_ReplaceText;
  QTextDocument::FindFlags m_FindFlags;
  QString m_sFindReplaceStatus;

  bool m_bFileSaved;
  bool m_bCompiled;
  CGCompiler *m_pCompiler;

  int m_nFrames;
  int m_nSpeed;
  int m_nCurrentFrame;
  bool m_bPlayingAnimation;
  int m_nTotalTracedPoints;
  std::vector<traced_point> m_TracedPoints;

  bool m_bWatchWindowShown;
  WatchWindow *m_Watch;
};

#endif // CHILDWINDOW_H
