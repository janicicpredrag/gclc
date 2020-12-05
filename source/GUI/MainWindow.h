#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ChildWindow.h"
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QSlider>
#include <QToolButton>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

public slots:
  void newDocument();
  void openDocument();
  void saveDocument();
  void saveDocumentAs();
  bool closeDocument();
  void onPrintGCLCSpecification();
  void onPrintGCLCImage();

  void ImportJavaView();
  void ExportPic();
  void ExportTikZ();
  void ExportPSTrics();
  void ExportEPS();
  void ExportSVG();
  void ExportRaster();
  void ExportXML();
  void Export(enum exportFormat format);

  void onUndo();
  void onRedo();
  void onCut();
  void onCopy();
  void onPaste();
  void onSelectAll();
  void onFind();
  void onFindNext();
  void onFindPrevious();
  void onReplace();
  void onEnableHighlighting();
  void onPickColor();
  void changeFont();
  void changeOutputFont();

  void onTheoremProving();
  void onArea();
  void onWu();
  void onGroebner();
  void onProofsToLaTeX();
  void onProofsToXML();
  void onDeductionControl();

  void buildCurrentGCLCDocument();
  void zoomIn();
  void zoomOut();
  void zoomNormal();
  void watch();

  void displayAbout();
  void openManual();

  void setSliderRange(int min, int max);
  void setSliderPos(int pos);
  void playButtonClicked();
  void showAnimationFrame(int);

  void onShowFreePoints();

  void updateStatusBar(bool bNoFile = false);
  void updateMainMenu(bool bNoFile = false);
  void updateSlider(bool bNoFile = false);

  void removeAction(QAction *a);
  void activateChild();
  ChildWindow *activeChild();

  bool close();

protected:
  void closeEvent(QCloseEvent *event);

private:
  void createChild(QString fileName);

  Ui::MainWindow *ui;

  QString m_sWorkingGCLDirectory;
  QString m_sWorkingExportDirectory;

  QSlider *mSlider;
  QLineEdit *mSliderText;
  QToolButton *mPlay;
  QToolButton *mHelp;
  bool m_bPlaying;

  QLabel *m_statusEditPos;
  QLabel *m_statusFileStatus;
  QLabel *m_statusFindReplace;
  QLabel *m_statusFileCompiled;
  QLabel *m_statusZoomFactor;
  QLabel *m_statusViewPos;
};

#endif // MAINWINDOW_H
