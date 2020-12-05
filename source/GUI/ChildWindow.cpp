#include "ChildWindow.h"
#include "Export/EPSOutput.h"
#include "Export/LaTeXOutput.h"
#include "Export/PSTricksOutput.h"
#include "Export/QGraphicsViewOutput.h"
#include "Export/SVGOutput.h"
#include "Export/TikZOutput.h"
#include "ExportToRasterFormat.h"
#include "FindDialog.h"
#include "FreePointItem.h"
#include "GCLCEngine/GCLC.h"
#include "Input/EditorInput.h"
#include "Input/StringInput.h"
#include "Logging/DummyLog.h"
#include "Logging/FileLog.h"
#include "Logging/QOutputLog.h"
#include "ReplaceDialog.h"
#include <QApplication>
#include <QColorDialog>
#include <QMessageBox>

// --------------------------------------------------------------------------------------------

ChildWindow::ChildWindow(QWidget *parent) : QMdiSubWindow(parent) {
  m_ChildForm = new ChildForm(this);
  setWidget(m_ChildForm);

  m_ChildForm->ui->splitter->setStretchFactor(1, 5);
  m_ChildForm->ui->splitter_2->setStretchFactor(0, 15);

  m_zoomFactor = 1;
  getGraphicsOutput()->scale(m_zoomFactor, -m_zoomFactor);

  getGraphicsOutput()->setMouseTracking(true);

  m_Highlighter = new Highlighter(getEditor()->document());
  m_OutputHighlighter = new OutputHighlighter(getTextOutput()->document());
  m_FindText = "";

  m_Watch = new WatchWindow(this);
  QObject::connect(m_Watch, SIGNAL(changedcell(int, int)), this,
                   SLOT(updateWatchCell(int, int)));

  QObject::connect(m_ChildForm, SIGNAL(CursorMoved()), this,
                   SLOT(updateEditPos()));
  QObject::connect(m_ChildForm, SIGNAL(FileChanged()), this,
                   SLOT(updateFileChanged()));

  QPalette p = getGraphicsOutput()->palette();
  p.setColor(QPalette::Base, QColor(220, 220, 220));
  getGraphicsOutput()->setPalette(p);

  m_Scene = new GCLCGraphicsScene(parent);

  m_pQGraphicsViewOutput =
      new QGraphicsViewOutput(getGraphicsOutput(), m_Scene);

  m_nTotalTracedPoints = 0;
  m_bWatchWindowShown = false;
  m_pCompiler = NULL;

  m_bFileSaved = true;
  m_bCompiled = false;
}

// --------------------------------------------------------------------------------------------

ChildWindow::~ChildWindow() {
  m_Scene->clear();
  delete m_Highlighter;
  delete m_OutputHighlighter;
  delete m_pQGraphicsViewOutput;
  delete m_Scene;
  delete m_Watch;
  delete m_ChildForm;
  emit(removeMenuAction(getAction()));
  if (m_pCompiler)
    delete m_pCompiler;
}

// --------------------------------------------------------------------------------------------

bool ChildWindow::Save(QString fileName) {
  QFile outfile;
  outfile.setFileName(fileName);
  if (!outfile.open(QIODevice::WriteOnly | QIODevice::Text))
    return false;
  QTextStream out(&outfile);
  out << getEditor()->toPlainText() << endl;
  outfile.close();
  return true;
}

// --------------------------------------------------------------------------------------------

bool ChildWindow::Export(enum exportFormat format, QString fileName) {
  if (format == eraster) {
    if (!fileName.isNull()) {
      ExportToRasterFormat dialog;
      if (dialog.exec() == QDialog::Accepted) {
        int resScale = 1.5 * (dialog.getResolution() / 75);

        if (dialog.currentFrameOnly()) {
          m_Scene->clearSelection();
          QSize size(resScale * m_Scene->sceneRect().size().width(),
                     resScale * m_Scene->sceneRect().size().height());
          QImage image(size, QImage::Format_ARGB32);
          QPainter painter;
          painter.begin(&image);
          painter.translate(0, resScale * m_Scene->sceneRect().size().height());
          painter.scale(1, -1);
          m_Scene->render(&painter);
          painter.end();
          image.save(fileName);
          return true;
        } else {
          int startFrame = dialog.rangeStartFrame();
          int endFrame = dialog.rangeEndFrame();
          int initialCurrentFrame = m_nCurrentFrame;
          for (int i = startFrame; i <= endFrame && i <= m_nFrames; i++) {
            showAnimationFrame(i);
            QSize size(resScale * m_Scene->sceneRect().size().width(),
                       resScale * m_Scene->sceneRect().size().height());
            QImage image(size, QImage::Format_ARGB32);
            QPainter painter;
            painter.begin(&image);
            painter.translate(0,
                              resScale * m_Scene->sceneRect().size().height());
            painter.scale(1, -1);
            m_Scene->render(&painter);
            painter.end();

            QString fileName1 = fileName;
            int numDigits = QString::number(m_nFrames).length();
            fileName1.replace(
                '.', QString("%1").arg(i, numDigits, 10, QChar('0')) + ".");
            image.save(fileName1);
          }
          showAnimationFrame(initialCurrentFrame);
          m_pQGraphicsViewOutput->Update();
        }
      }
    }
    return false;
  }

  ofstream file(fileName.toStdString());
  if (!file.is_open())
    return false;

  CGCLCOutput *pOutput;
  switch (format) {
  case epic:
    pOutput = new CLaTeXOutput(file);
    break;

  case etikz:
    pOutput = new CTikZOutput(file);
    break;

  case epst:
    pOutput = new CPSTricksOutput(file);
    break;

  case eeps:
    pOutput = new CEPSOutput(file);
    break;

  case esvg:
    pOutput = new CSVGOutput(file);
    break;

  default:
    pOutput = new CLaTeXOutput(file);
    break;
  }

  m_pCompiler->Export(*pOutput);
  delete pOutput;

  file.close();
  return true;
}

// --------------------------------------------------------------------------------------------

bool ChildWindow::close() {
  if (!isFileSaved()) {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(
        this, "File not saved",
        "File " + getFileName() +
            " has not been saved. Close it without saving changes?",
        QMessageBox::Yes | QMessageBox::No);
    return (reply == QMessageBox::Yes);
  }
  return true;
}

// --------------------------------------------------------------------------------------------

void ChildWindow::closeEvent(QCloseEvent * /* event */) { close(); }

// --------------------------------------------------------------------------------------------

void ChildWindow::Undo() { getEditor()->undo(); }

// --------------------------------------------------------------------------------------------

void ChildWindow::Redo() { getEditor()->redo(); }

// --------------------------------------------------------------------------------------------

void ChildWindow::Cut() { getEditor()->cut(); }

// --------------------------------------------------------------------------------------------

void ChildWindow::Copy() { getEditor()->copy(); }

// --------------------------------------------------------------------------------------------

void ChildWindow::Paste() { getEditor()->paste(); }

// --------------------------------------------------------------------------------------------

void ChildWindow::SelectAll() { getEditor()->selectAll(); }

// --------------------------------------------------------------------------------------------

void ChildWindow::Find() {
  FindDialog dialog(this);
  dialog.setText(m_FindText);
  dialog.setFlags(m_FindFlags);
  if (dialog.exec() == QDialog::Accepted) {
    m_FindFlags = 0;
    m_FindText = dialog.getText();
    if (dialog.caseSensitive())
      m_FindFlags = QTextDocument::FindCaseSensitively;
    if (dialog.matchWholeWord())
      m_FindFlags |= QTextDocument::FindWholeWords;
    QList<QTextEdit::ExtraSelection> extraSelections;
    getEditor()->moveCursor(QTextCursor::Start);
    int findOccurrences = 0;
    while (getEditor()->find(m_FindText, m_FindFlags)) {
      QTextEdit::ExtraSelection extra;
      extra.cursor = getEditor()->textCursor();
      extra.format.setBackground(QColor(255, 255, 123));
      extraSelections.append(extra);
      findOccurrences++;
    }
    getEditor()->setExtraSelections(extraSelections);
    m_sFindReplaceStatus =
        QString::number(findOccurrences) + " occurrences found";
  }
}

// --------------------------------------------------------------------------------------------

void ChildWindow::FindNext() {
  QList<QTextEdit::ExtraSelection> extraSelections;
  if (getEditor()->find(m_FindText, m_FindFlags)) {
    QTextEdit::ExtraSelection extra;
    extra.cursor = getEditor()->textCursor();
    extra.format.setBackground(QColor(255, 255, 123));
    extraSelections.append(extra);
    getEditor()->setExtraSelections(extraSelections);
  }
}

// --------------------------------------------------------------------------------------------

void ChildWindow::FindPrevious() {
  QList<QTextEdit::ExtraSelection> extraSelections;
  if (getEditor()->find(m_FindText,
                        m_FindFlags | QTextDocument::FindBackward)) {
    QTextEdit::ExtraSelection extra;
    extra.cursor = getEditor()->textCursor();
    extra.format.setBackground(QColor(255, 255, 123));
    extraSelections.append(extra);
    getEditor()->setExtraSelections(extraSelections);
  }
}

// --------------------------------------------------------------------------------------------

void ChildWindow::Replace() {
  ReplaceDialog dialog(this);
  dialog.setFindText(m_FindText);
  dialog.setReplaceText(m_ReplaceText);
  dialog.setFlags(m_FindFlags);
  if (dialog.exec() == QDialog::Accepted) {
    m_FindFlags = 0;
    m_FindText = dialog.getFindText();
    m_ReplaceText = dialog.getReplaceText();
    if (dialog.caseSensitive())
      m_FindFlags = QTextDocument::FindCaseSensitively;
    if (dialog.matchWholeWord())
      m_FindFlags |= QTextDocument::FindWholeWords;
    QList<QTextEdit::ExtraSelection> extraSelections;
    getEditor()->moveCursor(QTextCursor::Start);
    int findOccurrences = 0;
    while (getEditor()->find(m_FindText, m_FindFlags)) {
      getEditor()->insertPlainText(m_ReplaceText);
      getEditor()->find(m_ReplaceText,
                        m_FindFlags | QTextDocument::FindBackward);
      QTextEdit::ExtraSelection extra;
      extra.cursor = getEditor()->textCursor();
      extra.format.setBackground(QColor(255, 255, 123));
      extraSelections.append(extra);
      findOccurrences++;
    }
    getEditor()->setExtraSelections(extraSelections);
    m_sFindReplaceStatus =
        QString::number(findOccurrences) + " occurrences replaced";
  }
}

// --------------------------------------------------------------------------------------------

void ChildWindow::zoomIn() {
  if (m_zoomFactor > 30)
    return;
  m_zoomFactor *= 1.25;
  getGraphicsOutput()->scale(1.25, 1.25);
  //    m_Scene->update(m_Scene->sceneRect());
  emit(updateMainStatusBar());
}

// --------------------------------------------------------------------------------------------

void ChildWindow::zoomOut() {
  if (m_zoomFactor < 0.03)
    return;
  m_zoomFactor /= 1.25;
  getGraphicsOutput()->scale(0.8, 0.8);
  //    m_Scene->update(m_Scene->sceneRect());
  emit(updateMainStatusBar());
}

// --------------------------------------------------------------------------------------------

void ChildWindow::zoomNormal() {
  getGraphicsOutput()->scale(1 / m_zoomFactor, 1 / m_zoomFactor);
  m_zoomFactor = 1;
  //    m_Scene->update(m_Scene->sceneRect());
  emit(updateMainStatusBar());
}

// --------------------------------------------------------------------------------------------

void ChildWindow::enableHighlighting(bool enable) {
  if (enable)
    m_Highlighter->setDocument(getEditor()->document());
  else
    m_Highlighter->setDocument(NULL);
}

// --------------------------------------------------------------------------------------------

void ChildWindow::pickColor() {
  QColor color = QColorDialog::getColor(Qt::white);
  if (color.isValid()) {
    QString colorStr;
    colorStr = " " + QString::number(color.red()) + " " +
               QString::number(color.green()) + " " +
               QString::number(color.blue());
    getEditor()->insertPlainText(colorStr);
  }
}

// --------------------------------------------------------------------------------------------

bool ChildWindow::ExportToXML(QString fileName) {
  QEditorInput Input(getEditor());
  QOutputLog Log(getTextOutput());
  ofstream ho(fileName.toStdString());
  if (!ho.is_open())
    return false;
  Log.AddText("\nExporting to XML...\n");

  // CGCLC C(Input, Log);
  // C.EnableXMLoutput(&ho);
  // GReturnValue r = C.Import("", NULL, NULL);

  prover_config ProverConfig;
  ProverConfig.TheoremProvingMethod = tpNone;
  CGCLC C(Input, Log, ProverConfig, true, ho);
  eGCLC_conjecture_status prover_output;
  double prover_time;
  GReturnValue r = C.Import(prover_output, prover_time);
  ho.close();

  if (r == rvG_OK)
    Log.AddText("completed.\n");
  else
    Log.AddText("failed.\n");
  Log.AddText("\n--------------------------\n");

  return (r == rvG_OK);
}

// --------------------------------------------------------------------------------------------

bool ChildWindow::Build(prover_config &Prover_params) {
  int nRet;
  QEditorInput Input(getEditor());
  QOutputLog Log(getTextOutput());
  if (m_pCompiler)
    delete m_pCompiler;
  QFileInfo fileInfo(getFileName());
  Prover_params.sTheoremFileName = fileInfo.baseName().toStdString();
  ofstream XMLoutput;
  CGCLC *pc = new CGCLC(Input, Log, Prover_params, false, XMLoutput);
  m_pCompiler = pc;

  //  if (Prover_params.TheoremProvingMethod == tpNone)
  // nRet = m_pCompiler->Import("", NULL, NULL);
  //      nRet = m_pCompiler->Import("", NULL, NULL);
  //  else {
  // QFileInfo fileInfo(getFileName());
  // QString proofFileName = fileInfo.baseName();
  //    QString proofFileName = getFileName().section(".",0,0);
  // nRet =
  //    m_pCompiler->Import(proofFileName.toStdString(), &Prover_params,
  //    NULL);
  eGCLC_conjecture_status prover_output;
  double prover_time;
  nRet = m_pCompiler->Import(prover_output, prover_time);
  //  }

  if (nRet != rvG_OK) {
    int nErr, nLine, nPos;
    string sErr;
    m_pCompiler->GetError(nErr, sErr, nLine, nPos);
    annotateLineInEditor(nLine, QColor(255, 0, 0)); // locate error in editor
    setFileCompiled(false);
    updateMainStatusBar();
    return false;
  }

  setFileCompiled(true);
  updateMainStatusBar();
  QObject::connect(m_Scene, SIGNAL(viewCursorMoved(qreal, qreal)), this,
                   SLOT(updateViewCursorPos(qreal, qreal)));

  if (!m_pCompiler->GetAnimationFrames(m_nFrames, m_nSpeed))
    m_nFrames = 1;

  m_nTotalTracedPoints = 0;
  if (m_nFrames > 1) {
    QString coordinate = "(\\s+)(-?)(\\d+)((\\.\\d+)?)";
    QRegExp rx_traced("\\b(trace)(\\s+)(\\w)+" + coordinate + coordinate +
                      coordinate);
    QString input = getEditor()->toPlainText(); // fixme to avoid copying string
    QString point_decl;

    int pos = 0;
    while ((pos = rx_traced.indexIn(input, pos)) != -1) {
      point_decl = rx_traced.cap(0);
      pos += point_decl.length();
      m_nTotalTracedPoints++;
    }
    if (m_nTotalTracedPoints > 0) {
      m_TracedPoints.clear();
      m_TracedPoints.resize(m_nFrames * m_nTotalTracedPoints);
      for (int i = 0; i < m_nFrames * m_nTotalTracedPoints; i++)
        m_TracedPoints[i].defined = false;
    }
  }

  m_nCurrentFrame = 1;
  emit(setMainSliderRange(1, m_nFrames));
  emit(setMainSliderPos(1));

  // showAnimationFrame(1);
  m_pQGraphicsViewOutput->Reset();
  if (nRet == rvG_OK)
    m_pCompiler->Export(*m_pQGraphicsViewOutput);
  m_pQGraphicsViewOutput->Update();
  updateWatchWindow(m_pCompiler);

  return true;
}

// --------------------------------------------------------------------------------------------

void ChildWindow::showRemainingAnimationFrames() {
  if (m_nCurrentFrame == m_nFrames)
    return;
  QTime t;
  t.start();
  int startFrame = m_nCurrentFrame;
  for (; m_nCurrentFrame <= m_nFrames && m_bPlayingAnimation;
       m_nCurrentFrame++) {
    do {
      qApp->processEvents();
    } while (t.elapsed() <= 1000 * (m_nCurrentFrame - startFrame) / m_nSpeed);
    emit(setMainSliderPos(m_nCurrentFrame));
    showAnimationFrame(m_nCurrentFrame);
  }
  m_nCurrentFrame--;
}

// --------------------------------------------------------------------------------------------

void ChildWindow::showAnimationFrame(int i) {
  int pos;
  double x, y, x0, y0, x1, y1;

  m_nCurrentFrame = i;

  QString coordinate = "(\\s+)(-?)(\\d+)((\\.\\d+)?)";
  QRegExp rx("\\b(point)(\\s+)(\\w)+" + coordinate + coordinate + coordinate +
             coordinate);

  QString input = getEditor()->toPlainText(); // fixme to avoid copying string
  QString point_decl;

  pos = 0;
  while ((pos = rx.indexIn(input, pos)) != -1) {
    point_decl = rx.cap(0);

    QTextStream myteststream(&point_decl);
    QString point, name;
    myteststream >> point >> name >> x0 >> y0 >> x1 >> y1;

    if (m_nFrames == 1) {
      x = x0;
      y = y0;
    } else {
      x = x0 + (m_nCurrentFrame - 1) * (x1 - x0) / (m_nFrames - 1);
      y = y0 + (m_nCurrentFrame - 1) * (y1 - y0) / (m_nFrames - 1);
    }

    QString after = QString("point %1 %2 %3").arg(name).arg(x).arg(y);
    pos += after.length();
    input.replace(QRegExp(point_decl), after);
  }

  CDummyLog Log;
  QEditorInput Input(input);
  // CGCLC C(Input, Log);
  // GReturnValue nRet = C.Import("", NULL, NULL);

  prover_config ProverConfig;
  ProverConfig.TheoremProvingMethod = tpNone;
  ofstream ho;
  eGCLC_conjecture_status prover_output = e_idle;
  double prover_time = 0;
  CGCLC C(Input, Log, ProverConfig, false, ho);
  GReturnValue nRet = C.Import(prover_output, prover_time);

  m_pQGraphicsViewOutput->Reset();
  if (nRet == rvG_OK)
    C.Export(*m_pQGraphicsViewOutput);

  // Handling traced points
  if (m_nTotalTracedPoints > 0) {
    QRegExp rx_traced("\\b(trace)(\\s+)(\\w)+" + coordinate + coordinate +
                      coordinate);
    double r, g, b;
    pos = 0;
    int traced_point_index = 0;
    while ((pos = rx_traced.indexIn(input, pos)) != -1) {
      point_decl = rx_traced.cap(0);
      QTextStream myteststream(&point_decl);
      string v;
      QString trace, name;
      myteststream >> trace >> name >> r >> g >> b;
      int pointIndex =
          m_nTotalTracedPoints * (m_nCurrentFrame - 1) + traced_point_index;
      if (!m_TracedPoints[pointIndex].defined) {
        if (C.GetValue(name.toUtf8().constData(), v) == rvG_OK) {

          size_t pos1 = v.find('(', 0) + 1;
          size_t pos2 = v.find(',', 0) + 2;
          bool b1 = (pos1 == string::npos
                         ? false
                         : convert(v.substr(pos1, pos2 - pos1 - 2), x1));
          bool b2 = (pos1 == string::npos
                         ? false
                         : convert(v.substr(pos2, v.size() - pos2 - 1), y1));

          if (b1 && b2) {
            m_TracedPoints[pointIndex].x = x1;
            m_TracedPoints[pointIndex].y = y1;
            m_TracedPoints[pointIndex].defined = true;
          }
        }
      }
      for (int i = 2; i <= m_nCurrentFrame; i++) {
        int pointIndex = m_nTotalTracedPoints * (i - 1) + traced_point_index;
        int pointPrevIndex =
            m_nTotalTracedPoints * (i - 2) + traced_point_index;

        if (m_TracedPoints[pointIndex].defined &&
            m_TracedPoints[pointPrevIndex].defined) {
          x0 = m_TracedPoints[pointIndex].x;
          y0 = m_TracedPoints[pointIndex].y;
          x1 = m_TracedPoints[pointPrevIndex].x;
          y1 = m_TracedPoints[pointPrevIndex].y;

          QPen oldPen, newPen;
          oldPen = m_pQGraphicsViewOutput->GetPen();
          newPen = oldPen;
          newPen.setColor(QColor(r, g, b));

          m_pQGraphicsViewOutput->SetPen(newPen);
          m_pQGraphicsViewOutput->DrawSegment(x0, y0, x1, y1);
          m_pQGraphicsViewOutput->SetPen(oldPen);
        }
      }
      pos += point_decl.length();
      traced_point_index++;
    }
  }

  m_pQGraphicsViewOutput->Update();
  updateWatchWindow(&C);
}

// --------------------------------------------------------------------------------------------

void ChildWindow::playAnimation(bool bPlay) {
  if (!m_bPlayingAnimation && bPlay) {
    m_bPlayingAnimation = true;
    showRemainingAnimationFrames();
  } else
    m_bPlayingAnimation = false;
}

// --------------------------------------------------------------------------------------------

void ChildWindow::showFreePoints() {
  int pos;
  double x0, y0, x1, y1;

  QFont font;
  font.setFamily("Times");
  font.setBold(true);
  font.setItalic(true);

  QString coordinate = "(\\s+)(-?)(\\d+)((\\.\\d+)?)";
  QRegExp rx("\\b(point)(\\s+)(\\w)+" + coordinate + coordinate);

  QString point_decl;
  qreal s = m_pQGraphicsViewOutput->getScale();

  pos = 0;
  while ((pos = rx.indexIn(getEditor()->toPlainText(), pos)) != -1) {
    point_decl = rx.cap(0);
    QTextStream myteststream(&point_decl);
    QString point, name;
    myteststream >> point >> name >> x0 >> y0;
    pos += 1;

    // QGraphicsEllipseItem* item = new QGraphicsEllipseItem(s*(x0-3),
    // -s*(y0-3), s*2*3, -s*2*3);
    QGraphicsEllipseItem *item = new QGraphicsEllipseItem(
        s * (x0 - 3), s * (y0 - 3), s * 2 * 3, s * 2 * 3);
    item->setBrush(QBrush(QColor(150, 250, 150)));
    item->setPen(QPen(QColor(10, 10, 10)));
    m_Scene->addItem(item);

    QGraphicsSimpleTextItem *gt = new QGraphicsSimpleTextItem;
    gt->setFont(font);
    gt->setText(name);
    qreal w = gt->boundingRect().width();
    qreal h = gt->boundingRect().height();
    // gt->setPos(s*x0-w/2,-s*y0-h/2);
    gt->setPos(s * x0 - w / 2, s * y0 + h / 2);
    // gt->scale(1,-1);
    gt->setTransform(QTransform::fromScale(1, -1));
    m_Scene->addItem(gt);

    QGraphicsSimpleTextItem *gtm = new QGraphicsSimpleTextItem;

    FreePointItem *fitem = new FreePointItem;
    // fitem->setRect(s*(x0-3), -s*(y0-3), s*2*3, -s*2*3);
    fitem->setRect(s * (x0 - 3), s * (y0 - 3), s * 2 * 3, s * 2 * 3);
    fitem->setBrush(QBrush(QColor(0, 150, 0)));
    fitem->setFlag(QGraphicsItem::ItemIsMovable, true);
    fitem->setFlag(QGraphicsItem::ItemIsSelectable, true);
    // fitem->setParams(getEditor(),s,name,gtm,-(s*x0-w/2),-(-s*y0-h/2),false);
    fitem->setParams(getEditor(), s, name, gtm, -(s * x0 - w / 2),
                     (-s * y0 - h / 2), false);
    m_Scene->addItem(fitem);

    gtm->setFont(font);
    gtm->setText(name);
    // gtm->setPos(s*x0-w/2,-s*y0-h/2);
    gtm->setPos(s * x0 - w / 2, s * y0 + h / 2);
    // gtm->scale(1,-1);
    gtm->setTransform(QTransform::fromScale(1, -1));
    m_Scene->addItem(gtm);
  }

  QRegExp rxa("\\b(point)(\\s+)(\\w)+" + coordinate + coordinate + coordinate +
              coordinate);
  pos = 0;
  while ((pos = rxa.indexIn(getEditor()->toPlainText(), pos)) != -1) {
    point_decl = rxa.cap(0);
    QTextStream myteststream(&point_decl);
    QString point, name;
    myteststream >> point >> name >> x0 >> y0 >> x1 >> y1;
    pos += 1;

    // QGraphicsEllipseItem* item = new QGraphicsEllipseItem(s*(x1-3),
    // -s*(y1-3), s*2*3, -s*2*3);
    QGraphicsEllipseItem *item = new QGraphicsEllipseItem(
        s * (x1 - 3), s * (y1 - 3), s * 2 * 3, s * 2 * 3);
    item->setBrush(QBrush(QColor(250, 250, 150)));
    item->setPen(QPen(QColor(10, 10, 10)));
    m_Scene->addItem(item);

    QGraphicsSimpleTextItem *gt = new QGraphicsSimpleTextItem;
    gt->setFont(font);
    gt->setText(name);
    qreal w = gt->boundingRect().width();
    qreal h = gt->boundingRect().height();
    // gt->setPos(s*x1-w/2,-s*y1-h/2);
    gt->setPos(s * x1 - w / 2, s * y1 + h / 2);
    // gt->scale(1,-1);
    gt->setTransform(QTransform::fromScale(1, -1));
    m_Scene->addItem(gt);

    QGraphicsSimpleTextItem *gtm = new QGraphicsSimpleTextItem;

    FreePointItem *fitem = new FreePointItem;
    // fitem->setRect(s*(x1-3), -s*(y1-3), s*2*3, -s*2*3);
    fitem->setRect(s * (x1 - 3), s * (y1 - 3), s * 2 * 3, s * 2 * 3);
    fitem->setBrush(QBrush(QColor(150, 150, 0)));
    fitem->setFlag(QGraphicsItem::ItemIsMovable, true);
    fitem->setFlag(QGraphicsItem::ItemIsSelectable, true);
    // fitem->setParams(getEditor(),s,name,gtm,-(s*x1-w/2),-(-s*y1-h/2),true);
    fitem->setParams(getEditor(), s, name, gtm, -(s * x1 - w / 2),
                     (-s * y1 - h / 2), true);
    m_Scene->addItem(fitem);

    gtm->setFont(font);
    gtm->setText(name);
    //       gtm->setPos(s*x1-w/2,-s*y1-h/2);
    gtm->setPos(s * x1 - w / 2, s * y1 + h / 2);
    // gtm->scale(1,-1);
    gtm->setTransform(QTransform::fromScale(1, -1));
    m_Scene->addItem(gtm);
  }

  m_Scene->update();
}

// --------------------------------------------------------------------------------------------

void ChildWindow::annotateLineInEditor(int nLine, QColor color) {
  const QTextBlock &block =
      getEditor()->document()->findBlockByNumber(nLine - 1);
  QTextCursor cursor(block);
  cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, 0);
  getEditor()->setTextCursor(cursor);
  getEditor()->setFocus();

  QTextEdit::ExtraSelection highlight;
  highlight.cursor = getEditor()->textCursor();
  highlight.format.setProperty(QTextFormat::FullWidthSelection, true);
  highlight.format.setBackground(color);

  QList<QTextEdit::ExtraSelection> extraSelections;
  extraSelections << highlight;
  getEditor()->setExtraSelections(extraSelections);
}

// --------------------------------------------------------------------------------------------

void ChildWindow::showWatchWindow() {
  m_bWatchWindowShown = true;
  m_Watch->showValues();
}

// --------------------------------------------------------------------------------------------

void ChildWindow::updateWatchWindow(CGCompiler *pc) {
  if (!m_Watch->isVisible())
    return;

  QString name;
  string v;

  for (int row = 0; row < 10; row++) {
    name = m_Watch->getObjectName(row, 0);
    if (name != "") {
      if (pc->GetValue(name.toUtf8().constData(), v) == rvG_OK)
        m_Watch->setObjectValue(row, 1, QString::fromStdString(v));
      else
        m_Watch->setObjectValue(row, 1, "undefined");
    }
  }
  m_Watch->show();
}

// --------------------------------------------------------------------------------------------

void ChildWindow::updateWatchCell(int row, int column) {
  GReturnValue rv;
  QString name;
  string v;
  if (column != 0)
    return;
  name = m_Watch->getObjectName(row, 0);
  if (name != "") {
    rv = m_pCompiler->GetValue(name.toUtf8().constData(), v);
    if (rv == rvG_OK) {
      m_Watch->setObjectValue(row, 1, QString::fromStdString(v));
    } else
      m_Watch->setObjectValue(row, 1, "undefined");
  }
}

// --------------------------------------------------------------------------------------------

void ChildWindow::updateEditPos() {
  setCursorPos(
      "Ln: " + QString::number(getEditor()->textCursor().blockNumber() + 1) +
      " (out of " + QString::number(getEditor()->document()->lineCount()) +
      ") Col: " +
      QString::number(getEditor()->textCursor().columnNumber() + 1));
  emit(updateMainStatusBar());
}

// --------------------------------------------------------------------------------------------

void ChildWindow::updateFileChanged() {
  QList<QTextEdit::ExtraSelection> extraSelections;
  getEditor()->setExtraSelections(extraSelections);

  m_sFindReplaceStatus = "";
  emit(updateMainStatusBar());

  updateFileStatus(false);
}

// --------------------------------------------------------------------------------------------

void ChildWindow::updateFileStatus(bool saved) {
  setFileSaved(saved);
  emit(updateMainStatusBar());
  emit(updateMainMenu());
}

// --------------------------------------------------------------------------------------------

void ChildWindow::updateViewCursorPos(qreal x, qreal y) {
  qreal s = m_pQGraphicsViewOutput->getScale();
  m_ViewCursorPosition.setX(x / s);
  m_ViewCursorPosition.setY(y / s);
  emit(updateMainStatusBar());
  emit(updateMainMenu());
}

// --------------------------------------------------------------------------------------------
