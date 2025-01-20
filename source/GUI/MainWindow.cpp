#include "MainWindow.h"
#include "Import/JavaView.h"
#include "Utils/Version.h"
#include "ui_MainWindow.h"
#include <QFileDialog>
#include <QFontDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QPrintDialog>
#include <QPrinter>
#include <QVariant>
#include <QtGui>

// --------------------------------------------------------------------------------------------

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  setCentralWidget(ui->mdiArea);

  m_sWorkingGCLDirectory = ".";
  m_sWorkingExportDirectory = ".";

  m_statusFileStatus = new QLabel("No file loaded", this);
  m_statusFileStatus->setFrameStyle(QFrame::Panel | QFrame::Sunken);
  m_statusEditPos = new QLabel("", this);
  m_statusEditPos->setFrameStyle(QFrame::Panel | QFrame::Sunken);
  m_statusFindReplace = new QLabel("", this);
  m_statusFindReplace->setFrameStyle(QFrame::Panel | QFrame::Sunken);
  m_statusFileCompiled = new QLabel("", this);
  m_statusFileCompiled->setFrameStyle(QFrame::Panel | QFrame::Sunken);
  m_statusZoomFactor = new QLabel("", this);
  m_statusZoomFactor->setFrameStyle(QFrame::Panel | QFrame::Sunken);
  m_statusViewPos = new QLabel("", this);
  m_statusViewPos->setFrameStyle(QFrame::Panel | QFrame::Sunken);

  statusBar()->addPermanentWidget(m_statusFileStatus, 1);
  statusBar()->addPermanentWidget(m_statusEditPos, 1);
  statusBar()->addPermanentWidget(m_statusFindReplace, 1);
  statusBar()->addPermanentWidget(m_statusFileCompiled, 1);
  statusBar()->addPermanentWidget(m_statusZoomFactor, 1);
  statusBar()->addPermanentWidget(m_statusViewPos, 1);

  mPlay = new QToolButton();
  mPlay->setIcon(QIcon(":/icons/media-playback-pause.png"));

  mPlay->setEnabled(true);

  mSlider = new QSlider();
  mSlider->setRange(0, 1000);
  mSlider->setMinimumWidth(200);
  mSlider->setMaximumWidth(300);
  mSlider->setOrientation(Qt::Horizontal);

  mSliderText = new QLineEdit();
  mSliderText->setMinimumWidth(30);
  mSliderText->setMaximumWidth(40);
  mSliderText->setEnabled(false);

  ui->mainToolBar->addWidget(mPlay);
  ui->mainToolBar->addWidget(mSlider);
  ui->mainToolBar->addWidget(mSliderText);
  ui->mainToolBar->addSeparator();

  mHelp = new QToolButton();
  mHelp->setIcon(QIcon(":/icons/help.png"));
  mHelp->setEnabled(true);

  ui->mainToolBar->addWidget(mHelp);
  ui->mainToolBar->addSeparator();

  m_bPlaying = false;
  QObject::connect(mPlay, SIGNAL(clicked()), this, SLOT(playButtonClicked()));
  QObject::connect(mSlider, SIGNAL(valueChanged(int)), this,
                   SLOT(showAnimationFrame(int)));
  QObject::connect(mHelp, SIGNAL(clicked()), this, SLOT(openManual()));

  showMaximized();
  updateMainMenu();
}

// --------------------------------------------------------------------------------------------

MainWindow::~MainWindow() {
  ui->mdiArea->closeAllSubWindows();
  delete ui;
}

// --------------------------------------------------------------------------------------------

ChildWindow *MainWindow::activeChild() {
  if (ui->mdiArea->activeSubWindow())
    return qobject_cast<ChildWindow *>(ui->mdiArea->activeSubWindow());
  else
    return NULL;
}

// --------------------------------------------------------------------------------------------

void MainWindow::newDocument() {
  ChildWindow *ActiveGCLCDoc = activeChild();
  if (ActiveGCLCDoc != NULL)
    ActiveGCLCDoc->hideWatchWindow();
  createChild("untitled.gcl");
  activeChild()->setFileSaved(false);
  updateStatusBar();
  updateMainMenu();
}

// --------------------------------------------------------------------------------------------

void MainWindow::createChild(QString fileName) {
  ChildWindow *childWindow = new ChildWindow(ui->mdiArea);
  childWindow->setAttribute(Qt::WA_DeleteOnClose);
  childWindow->showMaximized();

  childWindow->setFilename(fileName);

  QAction *newAction = new QAction(this);
  newAction->setText(fileName);
  ui->menuFile->insertAction(ui->actionClose, newAction);
  // QVariant v = qVariantFromValue((void *)childWindow);
  QObject *object = childWindow;
  QVariant v = QVariant::fromValue(object);

  newAction->setData(v);

  ui->menuFile->insertSeparator(ui->actionClose);
  childWindow->setAction(newAction);

  childWindow->setFileCompiled(false);

  QObject::connect(newAction, SIGNAL(triggered()), this, SLOT(activateChild()));

  QObject::connect(childWindow, SIGNAL(updateMainStatusBar()), this,
                   SLOT(updateStatusBar()));
  QObject::connect(childWindow, SIGNAL(updateMainMenu()), this,
                   SLOT(updateMainMenu()));

  QObject::connect(childWindow, SIGNAL(setMainSliderRange(int, int)), this,
                   SLOT(setSliderRange(int, int)));
  QObject::connect(childWindow, SIGNAL(setMainSliderPos(int)), this,
                   SLOT(setSliderPos(int)));

  QObject::connect(childWindow, SIGNAL(removeMenuAction(QAction *)), this,
                   SLOT(removeAction(QAction *)));

  updateSlider();
}

// --------------------------------------------------------------------------------------------

void MainWindow::openDocument() {
  ChildWindow *ActiveGCLCDoc = activeChild();
  if (ActiveGCLCDoc != NULL)
    ActiveGCLCDoc->hideWatchWindow();

  QString fileName = QFileDialog::getOpenFileName(this, tr("Open GCLC File"),
                                                  m_sWorkingGCLDirectory,
                                                  tr("GCLC Files (*.gcl)"));

  if (fileName == "")
    return;

  QFileInfo fileInfo(fileName);
  m_sWorkingGCLDirectory = fileInfo.path();

  QFile file(fileName);
  if (!file.open(QFile::ReadOnly | QFile::Text))
    return;

  createChild(fileName);

  QTextStream GCLCFile(&file);
  activeChild()->getEditor()->setText(GCLCFile.readAll());
  file.close();

  activeChild()->setFileSaved(true);
  updateStatusBar();
  updateMainMenu();
}

// --------------------------------------------------------------------------------------------

void MainWindow::activateChild() {
  if (activeChild())
    activeChild()->hideWatchWindow();

  QAction *action = qobject_cast<QAction *>(sender());
  if (action) {
    QVariant v = action->data();
    ChildWindow *childWindow = (ChildWindow *)v.value<void *>();
    // childWindow->activateWindow();
    childWindow->showMaximized();
    childWindow->refreshWatchWindow();
  }
  updateStatusBar();
  updateMainMenu();
  updateSlider();
}

// --------------------------------------------------------------------------------------------

void MainWindow::saveDocument() {
  ChildWindow *ActiveGCLCDoc = activeChild();
  if (ActiveGCLCDoc->Save(ActiveGCLCDoc->getFileName())) {
    ActiveGCLCDoc->setFileSaved(true);
    updateStatusBar();
    updateMainMenu();
  }
}

// --------------------------------------------------------------------------------------------

void MainWindow::saveDocumentAs() {
  ChildWindow *ActiveGCLCDoc = activeChild();
  QString toSaveInFileName = QFileDialog::getSaveFileName(
      this, tr("Save as GCLC File"), m_sWorkingGCLDirectory,
      tr("GCLC Files (*.gcl)"));
  if (toSaveInFileName == "")
    return;

  if (!toSaveInFileName.endsWith(".gcl"))
    toSaveInFileName += ".gcl";

  QFileInfo fileInfo(toSaveInFileName);
  m_sWorkingGCLDirectory = fileInfo.path();

  if (ActiveGCLCDoc->Save(toSaveInFileName)) {
    ActiveGCLCDoc->setFileSaved(true);
    ActiveGCLCDoc->setFilename(toSaveInFileName);
    updateStatusBar();
    updateMainMenu();
    updateSlider();
  }
}

// --------------------------------------------------------------------------------------------

bool MainWindow::closeDocument() {
  ChildWindow *ActiveGCLCDoc = activeChild();
  if (ActiveGCLCDoc) {
    if (ActiveGCLCDoc->close()) {
      removeAction(ActiveGCLCDoc->getAction());
      delete ActiveGCLCDoc;
      ActiveGCLCDoc = activeChild();
      if (ActiveGCLCDoc) {
        ActiveGCLCDoc->hideWatchWindow();
        ActiveGCLCDoc->showMaximized();
        ActiveGCLCDoc->refreshWatchWindow();
      }
      updateStatusBar();
      updateMainMenu();
      updateSlider();
      return true;
    } else
      return false;
  }
  return true;
}

// --------------------------------------------------------------------------------------------

void MainWindow::removeAction(QAction *a) {
  ui->menuFile->removeAction(a);
  updateStatusBar(true);
  updateMainMenu(true);
  updateSlider(true);
}

// --------------------------------------------------------------------------------------------

void MainWindow::onPrintGCLCSpecification() {
  ChildWindow *ActiveGCLCDoc = activeChild();
  if (ActiveGCLCDoc != NULL) {
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog dialog(&printer, this);
    dialog.setWindowTitle("Print GCLC Document");
    if (dialog.exec() == QDialog::Accepted)
      ActiveGCLCDoc->getEditor()->document()->print(&printer);
  }
}

// --------------------------------------------------------------------------------------------

void MainWindow::onPrintGCLCImage() {
  ChildWindow *ActiveGCLCDoc = activeChild();
  if (ActiveGCLCDoc != NULL) {
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog dialog(&printer, this);
    dialog.setWindowTitle("Print GCLC Document");
    if (dialog.exec() == QDialog::Accepted) {
      QPainter painter(&printer);
      painter.setRenderHint(QPainter::Antialiasing);
      ActiveGCLCDoc->getGraphicsOutput()->render(&painter);
    }
  }
}

// --------------------------------------------------------------------------------------------

void MainWindow::onUndo() {
  ChildWindow *ActiveGCLCDoc = activeChild();
  if (ActiveGCLCDoc != NULL)
    ActiveGCLCDoc->Undo();
}

// --------------------------------------------------------------------------------------------

void MainWindow::onRedo() {
  ChildWindow *ActiveGCLCDoc = activeChild();
  if (ActiveGCLCDoc != NULL)
    ActiveGCLCDoc->Redo();
}

// --------------------------------------------------------------------------------------------

void MainWindow::onCut() {
  ChildWindow *ActiveGCLCDoc = activeChild();
  if (ActiveGCLCDoc != NULL)
    ActiveGCLCDoc->Cut();
}

// --------------------------------------------------------------------------------------------

void MainWindow::onCopy() {
  ChildWindow *ActiveGCLCDoc = activeChild();
  if (ActiveGCLCDoc != NULL)
    ActiveGCLCDoc->Copy();
}

// --------------------------------------------------------------------------------------------

void MainWindow::onPaste() {
  ChildWindow *ActiveGCLCDoc = activeChild();
  if (ActiveGCLCDoc != NULL)
    ActiveGCLCDoc->Paste();
}

// --------------------------------------------------------------------------------------------

void MainWindow::onSelectAll() {
  ChildWindow *ActiveGCLCDoc = activeChild();
  if (ActiveGCLCDoc != NULL)
    ActiveGCLCDoc->SelectAll();
}

// --------------------------------------------------------------------------------------------

void MainWindow::onFind() {
  ChildWindow *ActiveGCLCDoc = activeChild();
  if (ActiveGCLCDoc != NULL)
    ActiveGCLCDoc->Find();
  ActiveGCLCDoc->showMaximized();
  //    ActiveGCLCDoc->activateWindow();
  ui->mdiArea->setActiveSubWindow(ActiveGCLCDoc);
  updateStatusBar();
}

// --------------------------------------------------------------------------------------------

void MainWindow::onFindNext() {
  ChildWindow *ActiveGCLCDoc = activeChild();
  if (ActiveGCLCDoc != NULL)
    ActiveGCLCDoc->FindNext();
}

// --------------------------------------------------------------------------------------------

void MainWindow::onFindPrevious() {
  ChildWindow *ActiveGCLCDoc = activeChild();
  if (ActiveGCLCDoc != NULL)
    ActiveGCLCDoc->FindPrevious();
}

// --------------------------------------------------------------------------------------------

void MainWindow::onReplace() {
  ChildWindow *ActiveGCLCDoc = activeChild();
  if (ActiveGCLCDoc != NULL)
    ActiveGCLCDoc->Replace();
  ActiveGCLCDoc->showMaximized();
  ui->mdiArea->setActiveSubWindow(ActiveGCLCDoc);
  updateStatusBar();
}

// --------------------------------------------------------------------------------------------

void MainWindow::onTheoremProving() {
  bool EnableProvers = ui->actionEnable_Theorem_Proving->isChecked();
  ui->actionArea_Method->setEnabled(EnableProvers);
  ui->actionWu_s_Method->setEnabled(EnableProvers);
  ui->actionGroebner_Bases_Method->setEnabled(EnableProvers);
  ui->actionProof_Export_to_LaTeX->setEnabled(EnableProvers);
  ui->actionProof_Export_to_XML->setEnabled(EnableProvers);
}

// --------------------------------------------------------------------------------------------

void MainWindow::onArea() {
  if (ui->actionArea_Method->isChecked()) {
    ui->actionWu_s_Method->setChecked(false);
    ui->actionGroebner_Bases_Method->setChecked(false);
  }
}

// --------------------------------------------------------------------------------------------

void MainWindow::onWu() {
  if (ui->actionWu_s_Method->isChecked()) {
    ui->actionArea_Method->setChecked(false);
    ui->actionGroebner_Bases_Method->setChecked(false);
  }
}

// --------------------------------------------------------------------------------------------

void MainWindow::onGroebner() {
  if (ui->actionGroebner_Bases_Method->isChecked()) {
    ui->actionArea_Method->setChecked(false);
    ui->actionWu_s_Method->setChecked(false);
  }
}

// --------------------------------------------------------------------------------------------

void MainWindow::onProofsToLaTeX() {}

// --------------------------------------------------------------------------------------------

void MainWindow::onProofsToXML() {}

// --------------------------------------------------------------------------------------------

void MainWindow::onDeductionControl() {}

// --------------------------------------------------------------------------------------------

void MainWindow::buildCurrentGCLCDocument() {
  ChildWindow *ActiveGCLCDoc = activeChild();
  if (ActiveGCLCDoc == NULL)
    return;

  prover_config ProverStatus;
  if (ui->actionEnable_Theorem_Proving->isChecked()) {
    if (ui->actionWu_s_Method->isChecked())
      ProverStatus.TheoremProvingMethod = tpWuMethod;
    else if (ui->actionGroebner_Bases_Method->isChecked())
      ProverStatus.TheoremProvingMethod = tpGroebnerMethod;
    else
      ProverStatus.TheoremProvingMethod = tpAreaMethod;

    ProverStatus.bLaTeX = ui->actionProof_Export_to_LaTeX->isChecked();
    ProverStatus.bXML = ui->actionProof_Export_to_XML->isChecked();
    ProverStatus.bDeductionControl = ui->actionDeduction_Control->isChecked();
  } else
    ProverStatus.TheoremProvingMethod = tpNone;

  m_bPlaying = true;
  mPlay->setIcon(QIcon(":/icons/media-playback-pause.png"));

  QObject::disconnect(mSlider, 0, 0, 0);
  ActiveGCLCDoc->Build(ProverStatus);
  QObject::connect(mSlider, SIGNAL(valueChanged(int)), this,
                   SLOT(showAnimationFrame(int)));

  m_bPlaying = false;
  mPlay->setIcon(QIcon(":/icons/media-playback-start.png"));

  updateStatusBar();
  updateMainMenu();
}

// --------------------------------------------------------------------------------------------

void MainWindow::ImportJavaView() {
  QString inputFileName = QFileDialog::getOpenFileName(
      this, tr("Open JavaView File"), ".", tr("GCLC Files (*.jvx)"));

  setCursor(Qt::WaitCursor);

  std::ifstream hi(inputFileName.toStdString());
  if (!hi.is_open())
    return; // printf("File error. Cannot open input file.\n");

  QString gclcFileName = inputFileName.section(".", 0, 0) + ".gcl";

  CGCLCInput *pI = new CFileInput(hi);
  CJavaView jv2gcl;

  createChild(gclcFileName);
  ChildWindow *childWindow = activeChild();
  QTextEdit *editor = childWindow->getEditor();

  GReturnValue gRv = jv2gcl.Import(pI, editor);
  delete pI;
  hi.close();

  setCursor(Qt::ArrowCursor);
  if (gRv != rvG_OK)
    return;

  childWindow->show();
  childWindow->setFileSaved(false);
  updateStatusBar();
  updateMainMenu();
  updateSlider();
}

// --------------------------------------------------------------------------------------------

void MainWindow::Export(enum exportFormat format) {
  ChildWindow *ActiveGCLCDoc = activeChild();
  if (!ActiveGCLCDoc->isFileCompiled())
    return;

  QString title, ext;
  QStringList filters;

  switch (format) {
  case epic:
    title = "Export to LaTeX pic File";
    filters << "LaTeX pic Files (*.pic)";
    ext = ".pic";
    break;

  case etikz:
    title = "Export to LaTeX TikZ File";
    filters << "LaTeX TikZ Files (*.tkz)";
    ext = ".tkz";
    break;

  case epst:
    title = "Export to LaTeX PS Trics File";
    filters << "LaTeX Trics Files (*.pst)";
    ext = ".pst";
    break;

  case eeps:
    title = "Export to EPS File";
    filters << "EPS Files (*.eps)";
    ext = ".eps";
    break;

  case esvg:
    title = "Export to SVG File";
    filters << "SVG Files (*.svg)";
    ext = ".svg";
    break;

  case eraster:
    title = "Export to Raster File Format (PNG, BMP, JPG)";
    filters << "PNG Files (*.png)"
            << "BMP Files (*.bmp)"
            << "JPG Files (*.jpg)";
    ext = ".png";
    break;

  default:
    break;
  }

  QFileDialog dialog(this, title, m_sWorkingExportDirectory);
  dialog.setAcceptMode(QFileDialog::AcceptSave);
  dialog.setNameFilters(filters);
  //    QString croped_fileName=ActiveGCLCDoc->getFileName().section(".",0,0);
  dialog.setDirectory(m_sWorkingExportDirectory);
  QFileInfo fileInfo(ActiveGCLCDoc->getFileName());
  dialog.selectFile(fileInfo.baseName());

  if (dialog.exec() == QDialog::Accepted) {
    QString toSaveInFileName = dialog.selectedFiles()[0];
    QString selectedFilter = dialog.selectedNameFilter();

    if (selectedFilter == "PNG Files (*.png)")
      ext = ".png";
    if (selectedFilter == "BMP Files (*.bmp)")
      ext = ".bmp";
    if (selectedFilter == "JPG Files (*.jpg)")
      ext = ".jpg";

    if (toSaveInFileName == "")
      return;
    if (!toSaveInFileName.endsWith(ext))
      toSaveInFileName += ext;

    ActiveGCLCDoc->Export(format, toSaveInFileName);

    QFileInfo fileInfo(toSaveInFileName);
    m_sWorkingExportDirectory = fileInfo.path();
  }
}

// --------------------------------------------------------------------------------------------

void MainWindow::ExportPic() { Export(epic); }

// --------------------------------------------------------------------------------------------

void MainWindow::ExportTikZ() { Export(etikz); }

// --------------------------------------------------------------------------------------------

void MainWindow::ExportPSTrics() { Export(epst); }

// --------------------------------------------------------------------------------------------

void MainWindow::ExportEPS() { Export(eeps); }

// --------------------------------------------------------------------------------------------

void MainWindow::ExportSVG() { Export(esvg); }

// --------------------------------------------------------------------------------------------

void MainWindow::ExportRaster() { Export(eraster); }

// --------------------------------------------------------------------------------------------

void MainWindow::ExportXML() {
  ChildWindow *ActiveGCLCDoc = activeChild();
  if (!ActiveGCLCDoc->isFileCompiled())
    return;

  QStringList filters;
  QString title = "Export to XML Format Specification";
  QString ext = ".xml";
  filters << "XML Files (*.xml)";

  QFileDialog dialog(this, title, m_sWorkingGCLDirectory);
  dialog.setAcceptMode(QFileDialog::AcceptSave);
  dialog.setNameFilters(filters);

  QString croped_fileName = ActiveGCLCDoc->getFileName().section(".", 0, 0);
  dialog.selectFile(croped_fileName);

  if (dialog.exec() == QDialog::Accepted) {
    QString toSaveInFileName = dialog.selectedFiles()[0];
    if (toSaveInFileName == "")
      return;
    if (!toSaveInFileName.endsWith(ext))
      toSaveInFileName += ext;
    ActiveGCLCDoc->ExportToXML(toSaveInFileName);
  }
}

// --------------------------------------------------------------------------------------------

void MainWindow::changeFont() {
  ChildWindow *ActiveGCLCDoc = activeChild();
  ActiveGCLCDoc->getEditor()->setFont(
      QFontDialog::getFont(0, ActiveGCLCDoc->font()));
}

// --------------------------------------------------------------------------------------------

void MainWindow::changeOutputFont() {
  ChildWindow *ActiveGCLCDoc = activeChild();
  ActiveGCLCDoc->getTextOutput()->setFont(
      QFontDialog::getFont(0, ActiveGCLCDoc->font()));
}

// --------------------------------------------------------------------------------------------

void MainWindow::zoomIn() {
  ChildWindow *ActiveGCLCDoc = activeChild();
  if (ActiveGCLCDoc != NULL)
    ActiveGCLCDoc->zoomIn();
}

// --------------------------------------------------------------------------------------------

void MainWindow::zoomOut() {
  ChildWindow *ActiveGCLCDoc = activeChild();
  if (ActiveGCLCDoc != NULL)
    ActiveGCLCDoc->zoomOut();
}

// --------------------------------------------------------------------------------------------

void MainWindow::zoomNormal() {
  ChildWindow *ActiveGCLCDoc = activeChild();
  if (ActiveGCLCDoc != NULL)
    ActiveGCLCDoc->zoomNormal();
}

// --------------------------------------------------------------------------------------------

void MainWindow::onPickColor() {
  ChildWindow *ActiveGCLCDoc = activeChild();
  if (ActiveGCLCDoc != NULL)
    ActiveGCLCDoc->pickColor();
}

// --------------------------------------------------------------------------------------------

void MainWindow::onEnableHighlighting() {
  ChildWindow *ActiveGCLCDoc = activeChild();
  if (ActiveGCLCDoc != NULL)
    ActiveGCLCDoc->enableHighlighting(
        ui->actionEnable_Highlighting->isChecked());
}

// --------------------------------------------------------------------------------------------

void MainWindow::displayAbout() {
  QMessageBox::about(
      this, tr("About"),
      tr("This is GCLC " GCLC_VERSION "\nDeveloped and copyright "
         "(c) 1995-2025 by \nPredrag Janicic, University of "
         "Belgrade.\nLicensed under the Creative Commons licence CC BY-ND."));
}

// --------------------------------------------------------------------------------------------

void MainWindow::playButtonClicked() {
  ChildWindow *ActiveGCLCDoc = activeChild();
  if (ActiveGCLCDoc != NULL) {
    if (m_bPlaying == false) {
      m_bPlaying = true;
      mPlay->setIcon(QIcon(":/icons/media-playback-pause.png"));
      mSlider->setEnabled(false);
      QObject::disconnect(mSlider, 0, 0, 0);
      updateStatusBar();
      updateMainMenu();
      ActiveGCLCDoc->playAnimation(m_bPlaying);
      QObject::connect(mSlider, SIGNAL(valueChanged(int)), this,
                       SLOT(showAnimationFrame(int)));

      // at the end of animation, change playing status again
      m_bPlaying = false;
      mPlay->setIcon(QIcon(":/icons/media-playback-start.png"));
      mSlider->setEnabled(true);
      updateStatusBar();
      updateMainMenu();
      ActiveGCLCDoc->playAnimation(m_bPlaying);
    } else {
      m_bPlaying = false;
      mPlay->setIcon(QIcon(":/icons/media-playback-start.png"));
      mSlider->setEnabled(true);
      updateStatusBar();
      updateMainMenu();
      ActiveGCLCDoc->playAnimation(m_bPlaying);
    }
  }
}

// --------------------------------------------------------------------------------------------

void MainWindow::watch() {
  ChildWindow *ActiveGCLCDoc = activeChild();
  if (ActiveGCLCDoc != NULL)
    ActiveGCLCDoc->showWatchWindow();
}

// --------------------------------------------------------------------------------------------

void MainWindow::setSliderRange(int min, int max) {
  QObject::disconnect(mSlider, 0, 0, 0);
  mSlider->setRange(min, max);
  QObject::connect(mSlider, SIGNAL(valueChanged(int)), this,
                   SLOT(showAnimationFrame(int)));
}

// --------------------------------------------------------------------------------------------

void MainWindow::setSliderPos(int pos) {
  QObject::disconnect(mSlider, 0, 0, 0);
  mSlider->setValue(pos);
  mSliderText->setText(QString::number(pos));
  QObject::connect(mSlider, SIGNAL(valueChanged(int)), this,
                   SLOT(showAnimationFrame(int)));
}

// --------------------------------------------------------------------------------------------

void MainWindow::showAnimationFrame(int pos) {
  ChildWindow *ActiveGCLCDoc = activeChild();
  if (ActiveGCLCDoc != NULL) {
    mSliderText->setText(QString::number(pos));
    ActiveGCLCDoc->showAnimationFrame(pos /*,mSlider->maximum()*/);
  }
}

// --------------------------------------------------------------------------------------------

void MainWindow::onShowFreePoints() {
  ChildWindow *ActiveGCLCDoc = activeChild();
  if (ActiveGCLCDoc != NULL)
    ActiveGCLCDoc->showFreePoints();
}

// --------------------------------------------------------------------------------------------

void MainWindow::updateStatusBar(bool bNoFile) {
  ChildWindow *ActiveGCLCDoc = activeChild();
  bool bFileExists;
  if (bNoFile)
    bFileExists = false;
  else
    bFileExists = (ActiveGCLCDoc != NULL);

  if (!bFileExists) {
    m_statusFileStatus->setText("No file loaded");
    m_statusEditPos->setText("");
    m_statusFindReplace->setText("");
    m_statusFileCompiled->setText("");
    m_statusZoomFactor->setText("");
    m_statusViewPos->setText("");
  } else {
    if (ActiveGCLCDoc->isFileSaved())
      m_statusFileStatus->setText(tr("File saved"));
    else
      m_statusFileStatus->setText(tr("File not saved"));

    m_statusFindReplace->setText(ActiveGCLCDoc->getFindReplaceStatus());

    if (ActiveGCLCDoc->isFileCompiled()) {
      m_statusFileCompiled->setText(tr("File compiled"));
      m_statusZoomFactor->setText(
          "Zoom Factor: " + QString::number(ActiveGCLCDoc->getZoomFactor()));
      m_statusViewPos->setText(
          "Cursor Position: (" +
          QString::number(ActiveGCLCDoc->getViewCursorPos().x()) + "," +
          QString::number(ActiveGCLCDoc->getViewCursorPos().y()) + ")");
    } else
      m_statusFileCompiled->setText(tr("File not compiled"));

    QString s = ActiveGCLCDoc->getCursorPos();
    m_statusEditPos->setText(s);
  }
}

// --------------------------------------------------------------------------------------------

void MainWindow::updateMainMenu(bool bNoFile) {
  ChildWindow *ActiveGCLCDoc = activeChild();
  bool bFileExists, bFileCompiled;

  if (bNoFile)
    bFileExists = false;
  else
    bFileExists = (ActiveGCLCDoc != NULL);

  if (ActiveGCLCDoc != NULL)
    bFileCompiled = ActiveGCLCDoc->isFileCompiled();
  else
    bFileCompiled = false;

  ui->actionNew->setEnabled(!m_bPlaying);
  ui->actionOpen->setEnabled(!m_bPlaying);
  ui->actionSave->setEnabled(bFileExists && !m_bPlaying &&
                             !ActiveGCLCDoc->isFileSaved());
  ui->actionSave_As->setEnabled(bFileExists && !m_bPlaying);
  ui->actionClose->setEnabled(bFileExists && !m_bPlaying);
  ui->actionGCLC_Specification->setEnabled(bFileExists && !m_bPlaying);
  ui->actionGCLC_Image->setEnabled(bFileExists && !m_bPlaying && bFileCompiled);
  ui->menuImport->setEnabled(!m_bPlaying);
  ui->actionExit->setEnabled(!m_bPlaying);

  ui->actionUndo->setEnabled(bFileExists && !m_bPlaying);
  ui->actionRedo->setEnabled(bFileExists && !m_bPlaying);
  ui->actionCut->setEnabled(bFileExists && !m_bPlaying);
  ui->actionCopy->setEnabled(bFileExists && !m_bPlaying);
  ui->actionPaste->setEnabled(bFileExists && !m_bPlaying);
  ui->actionSelect_All->setEnabled(bFileExists && !m_bPlaying);
  ui->actionFind->setEnabled(bFileExists && !m_bPlaying);
  ui->actionFind_Next->setEnabled(bFileExists && !m_bPlaying);
  ui->actionFind_Previous->setEnabled(bFileExists && !m_bPlaying);
  ui->actionReplace->setEnabled(bFileExists && !m_bPlaying);
  ui->actionChange_font->setEnabled(bFileExists && !m_bPlaying);
  ui->actionChange_output_font->setEnabled(bFileExists && !m_bPlaying);
  ui->actionEnable_Highlighting->setChecked(bFileExists && !m_bPlaying);
  ui->actionEnable_Highlighting->setEnabled(bFileExists && !m_bPlaying);
  ui->actionPick_Color->setEnabled(bFileExists && !m_bPlaying);

  ui->actionEnable_Theorem_Proving->setEnabled(bFileExists && !m_bPlaying);
  ui->actionEnable_Theorem_Proving->setChecked(bFileExists && !m_bPlaying);
  if (bFileExists && !m_bPlaying && !ui->actionArea_Method->isChecked() &&
      !ui->actionWu_s_Method->isChecked() &&
      !ui->actionGroebner_Bases_Method->isChecked()) {
    ui->actionArea_Method->setChecked(true);
  }
  ui->actionArea_Method->setEnabled(bFileExists && !m_bPlaying);
  ui->actionWu_s_Method->setEnabled(bFileExists && !m_bPlaying);
  ui->actionGroebner_Bases_Method->setEnabled(bFileExists && !m_bPlaying);
  ui->actionProof_Export_to_LaTeX->setEnabled(bFileExists && !m_bPlaying);
  ui->actionProof_Export_to_XML->setEnabled(bFileExists && !m_bPlaying);
  ui->actionDeduction_Control->setEnabled(bFileExists && !m_bPlaying);

  ui->actionBuild->setEnabled(bFileExists && !m_bPlaying);

  ui->menuExport->setEnabled(bFileExists && !m_bPlaying && bFileCompiled);
  ui->actionZoom_In->setEnabled(bFileExists && bFileCompiled);
  ui->actionZoom_Out->setEnabled(bFileExists && bFileCompiled);
  ui->actionZoom_Normal->setEnabled(bFileExists && bFileCompiled);
  ui->actionShowFreePoints->setEnabled(bFileExists && !m_bPlaying &&
                                       bFileCompiled);
  ui->actionWatch_Window->setEnabled(bFileExists && !m_bPlaying &&
                                     bFileCompiled);

  ui->actionManual->setEnabled(!m_bPlaying);
  ui->actionAbout->setEnabled(!m_bPlaying);

  if (bFileExists)
    setWindowTitle("GCLC [" + ActiveGCLCDoc->getFileName() + "]");
}

// --------------------------------------------------------------------------------------------

void MainWindow::updateSlider(bool bNoFile) {
  ChildWindow *ActiveGCLCDoc = activeChild();
  bool bFileExists;
  if (bNoFile)
    bFileExists = false;
  else
    bFileExists = (ActiveGCLCDoc != NULL);

  mSlider->setEnabled(bFileExists && ActiveGCLCDoc->isFileCompiled());
  mPlay->setEnabled(bFileExists);
  if (bFileExists) {
    if (ActiveGCLCDoc->isFileCompiled()) {
      setSliderRange(1, ActiveGCLCDoc->getAnimationFrames());
      setSliderPos(ActiveGCLCDoc->getCurrentFrame());
    } else {
      setSliderRange(1, 1);
      setSliderPos(1);
    }
  }
}

// --------------------------------------------------------------------------------------------

void MainWindow::openManual() {
  QTemporaryDir dir;
  if (!dir.isValid())
    return;
  QFile HelpFile(":/manual/gclc_man.pdf");
  QString s = dir.path();
  // QString s = qApp->applicationDirPath();
  s.append("gclc_man.pdf");
  if (HelpFile.copy(s))
    QDesktopServices::openUrl(QUrl::fromLocalFile(s));
}

// --------------------------------------------------------------------------------------------

bool MainWindow::close() {
  while (activeChild())
    if (!closeDocument())
      return false;
  QMainWindow::close();
  return true;
}

// --------------------------------------------------------------------------------------------

void MainWindow::closeEvent(QCloseEvent *event) {
  if (!close())
    event->ignore();
}

// --------------------------------------------------------------------------------------------
