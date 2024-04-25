#include "QGraphicsViewOutput.h"
#include "Utils/Utils.h"
#include "math.h"
#include <QGraphicsEllipseItem>
#include <QPolygon>

#define LINE_WIDTH 0.16

// --------------------------------------------------------------------------------------------

class QClippedTextItem : public QGraphicsSimpleTextItem {
public:
  QClippedTextItem(qreal clipw, qreal cliph, QGraphicsItem *parent = 0)
      : QGraphicsSimpleTextItem(parent) {
    m_ClipW = clipw;
    m_ClipH = cliph;
  }

protected:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget) {
    painter->scale(1, -1);
    painter->setClipping(true);
    // painter->setClipRect( -pos().x(), pos().y(), m_ClipW, -m_ClipH);
    painter->setClipRect(-pos().x(), pos().y() - m_ClipH, m_ClipW, m_ClipH);
    QGraphicsSimpleTextItem::paint(painter, option, widget);
  }
  QRectF boundingRect() const { return QRectF(0, -m_ClipH, m_ClipW, m_ClipH); }

private:
  qreal m_ClipW, m_ClipH;
};

// --------------------------------------------------------------------------------------------

class QGraphicsArcItem : public QGraphicsEllipseItem {
public:
  QGraphicsArcItem(qreal x, qreal y, qreal width, qreal height, qreal clipw,
                   qreal cliph, QGraphicsItem *parent = 0)
      : QGraphicsEllipseItem(x, y, width, height, parent) {
    m_ClipW = clipw;
    m_ClipH = cliph;
  }

protected:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem * /* option */,
             QWidget * /* widget */) {
    painter->setClipping(true);
    painter->setClipRect(0, 0, m_ClipW, m_ClipH);
    painter->setPen(pen());
    painter->setBrush(brush());
    painter->drawArc(rect(), startAngle(), spanAngle());
  }

private:
  qreal m_ClipW, m_ClipH;
};

// --------------------------------------------------------------------------------------------

class QGraphicsFilledPieItem : public QGraphicsEllipseItem {

public:
  QGraphicsFilledPieItem(qreal x, qreal y, qreal width, qreal height,
                         qreal clipw, qreal cliph, QPen pen,
                         QGraphicsItem *parent = 0)
      : QGraphicsEllipseItem(x, y, width, height, parent) {
    m_pen = pen;
    m_ClipW = clipw;
    m_ClipH = cliph;
  }

protected:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget) {
    painter->setClipping(true);
    painter->setClipRect(0, 0, m_ClipW, m_ClipH);
    painter->setPen(m_pen);
    painter->setBrush(m_pen.brush());
    QGraphicsEllipseItem::paint(painter, option, widget);
  }

private:
  qreal m_ClipW, m_ClipH;
  QPen m_pen;
};

// --------------------------------------------------------------------------------------------

class QGraphicsFilledChordItem : public QGraphicsEllipseItem {
public:
  QGraphicsFilledChordItem(qreal x, qreal y, qreal width, qreal height,
                           qreal phi1, qreal phi2, qreal clipw, qreal cliph,
                           QPen pen) {
    m_x = x;
    m_y = y;
    m_w = width;
    m_h = height;
    m_phi1 = phi1;
    m_phi2 = phi2;
    m_ClipW = clipw;
    m_ClipH = cliph;
    m_pen = pen;
  }

protected:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem * /* option */,
             QWidget * /* widget */) {
    painter->setClipping(true);
    painter->setClipRect(0, 0, m_ClipW, m_ClipH);
    painter->setPen(m_pen);
    painter->setBrush(m_pen.brush());
    painter->drawChord(m_x - m_w, (m_y - m_h), 2 * m_w, 2 * m_h, 16 * m_phi1,
                       16 * (m_phi2 - m_phi1));
  }
  QRectF boundingRect() const {
    // return QRectF(0,0,0,0);
    return QRectF(0, 0, m_ClipW, m_ClipH);
  }

private:
  qreal m_x, m_y, m_w, m_h, m_phi1, m_phi2, m_ClipW, m_ClipH;
  QPen m_pen;
};

// --------------------------------------------------------------------------------------------

QGraphicsViewOutput::QGraphicsViewOutput(QGraphicsView *pView,
                                         QGraphicsScene *pScene) {
  m_pView = pView;
  m_pScene = pScene;
  m_pView->setScene(m_pScene);
  m_pView->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
  m_pView->setDragMode(QGraphicsView::ScrollHandDrag);
  m_pView->setResizeAnchor(QGraphicsView::AnchorUnderMouse);
  m_pView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
  m_pView->ensureVisible(QRectF(0, 0, 0, 0));
}

// --------------------------------------------------------------------------------------------

void QGraphicsViewOutput::Reset() { m_pScene->clear(); }

// --------------------------------------------------------------------------------------------

void QGraphicsViewOutput::Update() { m_pView->repaint(); }

// --------------------------------------------------------------------------------------------

GReturnValue QGraphicsViewOutput::Init() { return Init(140, 100); }

// --------------------------------------------------------------------------------------------

GReturnValue QGraphicsViewOutput::Init(int x, int y) {
  m_OutlinePen.setColor(Qt::black);

  //    scalex = (m_pView->physicalDpiX()/25.4);
  m_ScaleX = (m_pView->physicalDpiX() / 21.7);
  m_ScaleY = m_ScaleX;
  m_OutlinePen.setWidth(m_ScaleX * LINE_WIDTH);
  // outlinePen.setWidth((300*LINE_WIDTH)/25.4);

  m_Font.setFamily("Times");
  m_Font.setBold(false);
  m_Font.setItalic(true);
  m_Fontsize = 8 * m_ScaleX * 0.3527;
  m_Font.setPixelSize(m_Fontsize);

  m_Xmin = 0;
  m_Xmax = x;
  m_Ymin = 0;
  m_Ymax = y;

  QBrush whiteBrush(QColor(255, 255, 255));
  QPen whitePen(QColor(255, 255, 255));

  m_pScene->setSceneRect(m_ScaleX * (m_Xmin), m_ScaleY * (m_Ymin),
                         m_ScaleX * (m_Xmax), m_ScaleY * (m_Ymax));
  m_pScene->setBackgroundBrush(QColor(220, 220, 220));
  m_pScene->addRect(m_ScaleX * (m_Xmin), m_ScaleY * (m_Ymin),
                    m_ScaleX * (m_Xmax), m_ScaleY * (m_Ymax), whitePen,
                    whiteBrush);
  return rvG_OK;
}

// --------------------------------------------------------------------------------------------

GReturnValue QGraphicsViewOutput::Close() { return rvG_OK; }

// --------------------------------------------------------------------------------------------

GReturnValue QGraphicsViewOutput::CloseAll() { return rvG_OK; }

// --------------------------------------------------------------------------------------------

GReturnValue QGraphicsViewOutput::SetPixel(double /* x */, double /* y */) {
  return rvG_OK;
}

// --------------------------------------------------------------------------------------------

GReturnValue QGraphicsViewOutput::DrawSegment(double x1, double y1, double x2,
                                              double y2) {
  m_pScene->addLine(m_ScaleX * x1, m_ScaleY * y1, m_ScaleX * x2, m_ScaleY * y2,
                    m_OutlinePen);
  return rvG_OK;
}

// --------------------------------------------------------------------------------------------

GReturnValue QGraphicsViewOutput::DrawArc(double x1, double y1, double x2,
                                          double y2, double phi,
                                          int /* precision */) {
  double angle1;
  double r = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
  angle1 = atan2(y2 - y1, x2 - x1);
  angle1 = 180.0 * angle1 / PI;
  while (angle1 > 360)
    angle1 -= 360;
  while (angle1 < 0)
    angle1 += 360;

  x1 *= m_ScaleX;
  y1 *= m_ScaleY;
  x2 *= m_ScaleX;
  y2 *= m_ScaleY;
  r = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));

  QGraphicsArcItem *item = new QGraphicsArcItem(
      x1 - r, (y1 - r), 2 * r, 2 * r, m_ScaleX * m_Xmax, m_ScaleY * m_Ymax);
  item->setPen(m_OutlinePen);
  //    item->setStartAngle(16*angle1);
  item->setStartAngle(-(16 * angle1 + 16 * phi));
  item->setSpanAngle(16 * phi);
  m_pScene->addItem(item);
  return rvG_OK;
}

// --------------------------------------------------------------------------------------------

GReturnValue QGraphicsViewOutput::SetColor(unsigned char r, unsigned char g,
                                           unsigned char b) {
  m_OutlinePen.setColor(QColor(r, g, b));
  return rvG_OK;
}

// --------------------------------------------------------------------------------------------

GReturnValue QGraphicsViewOutput::SetBackgroundColor(unsigned char r,
                                                     unsigned char g,
                                                     unsigned char b) {
  m_OutlinePen.setColor(QColor(r, g, b));
  FillRectangle(m_Xmin, m_Ymin, m_Xmax, m_Ymax);
  m_OutlinePen.setColor(QColor(0, 0, 0));
  return rvG_OK;
}

// --------------------------------------------------------------------------------------------

GReturnValue QGraphicsViewOutput::SetThickness(double uThickness) {
  if (uThickness < 0)
    uThickness = -uThickness * LINE_WIDTH;
  uThickness = m_ScaleX * uThickness;
  m_OutlinePen.setWidth(uThickness);
  return rvG_OK;
}

// --------------------------------------------------------------------------------------------

GReturnValue QGraphicsViewOutput::PrintText(double x, double y,
                                            const std::string &sPosition,
                                            const std::string &sText) {
  QGraphicsSimpleTextItem *gts = new QGraphicsSimpleTextItem;
  gts->setFont(m_Font);
  gts->setText(QString::fromStdString(sText));
  gts->setBrush(m_OutlinePen.brush());
  qreal w = gts->boundingRect().width();
  qreal h = gts->boundingRect().height();
  delete gts;

  QClippedTextItem *gt =
      new QClippedTextItem(m_ScaleX * m_Xmax, m_ScaleY * m_Ymax);
  gt->setFont(m_Font);
  gt->setText(QString::fromStdString(sText));
  gt->setBrush(m_OutlinePen.brush());
  x = m_ScaleX * x - w / 2;
  y = m_ScaleY * y + h / 2;

  /*    if (strchr (sPosition, 'r'))
         x -= w/2;
      if (strchr (sPosition, 'l'))
         x += w/2;
      if (strchr (sPosition, 'b'))
          y += 0.273*h;
      if (strchr (sPosition, 't'))
          y -= 0.288*h;*/

  if (sPosition.find('r') != std::string::npos)
    x -= w / 2;
  if (sPosition.find('l') != std::string::npos)
    x += w / 2;
  if (sPosition.find('b') != std::string::npos)
    y += 0.273 * h;
  if (sPosition.find('t') != std::string::npos)
    y -= 0.288 * h;

  gt->setPos(x, y);
  m_pScene->addItem(gt);

  // outlinePen.setColor(QColor(200, 0, 0));
  // m_pScene->addLine(x,y+h,x+w,y+h, outlinePen);
  // m_pScene->addLine(x+w,y+h,x+w,y, outlinePen);
  // m_pScene->addLine(x,y,x+w,y, outlinePen);
  // m_pScene->addLine(x,y,x,y+h, outlinePen);

  return rvG_OK;
}

// --------------------------------------------------------------------------------------------

GReturnValue QGraphicsViewOutput::SetFontSize(unsigned char size) {
  m_Fontsize = size * m_ScaleX * 0.3527;
  m_Font.setPixelSize(m_Fontsize);
  return rvG_OK;
}

// --------------------------------------------------------------------------------------------

GReturnValue QGraphicsViewOutput::FillTriangle(double x1, double y1, double x2,
                                               double y2, double x3,
                                               double y3) {
  QPolygonF polygon;
  polygon << QPointF(m_ScaleX * x1, m_ScaleY * y1)
          << QPointF(m_ScaleX * x2, m_ScaleY * y2)
          << QPointF(m_ScaleX * x3, m_ScaleY * y3);
  m_pScene->addPolygon(polygon, m_OutlinePen, m_OutlinePen.brush());
  return rvG_OK;
}

// --------------------------------------------------------------------------------------------

GReturnValue QGraphicsViewOutput::FillRectangle(double x1, double y1, double x2,
                                                double y2) {
  QPolygonF polygon;
  polygon << QPointF(m_ScaleX * x1, m_ScaleY * y1)
          << QPointF(m_ScaleX * x2, m_ScaleY * y1)
          << QPointF(m_ScaleX * x2, m_ScaleY * y2)
          << QPointF(m_ScaleX * x1, m_ScaleY * y2);
  m_pScene->addPolygon(polygon, m_OutlinePen, m_OutlinePen.brush());
  return rvG_OK;
}

// --------------------------------------------------------------------------------------------

GReturnValue QGraphicsViewOutput::FillEllipseArc(double x1, double y1, double a,
                                                 double b, double phi1,
                                                 double phi2,
                                                 bool bFillTriangle) {
  QPen pen = m_OutlinePen;
  pen.setWidth(0);

  if (bFillTriangle) {
    QGraphicsFilledPieItem *item = new QGraphicsFilledPieItem(
        m_ScaleX * (x1 - a), m_ScaleY * (y1 - b), m_ScaleX * 2 * a,
        m_ScaleY * 2 * b, m_ScaleX * m_Xmax, m_ScaleY * m_Ymax, m_OutlinePen);
    item->setPen(pen);
    item->setBrush(pen.brush());
    if (phi1 != 0 || phi2 - phi1 != 360) {
      qreal phi1t = transform_ellipse_angle(a, b, phi1);
      qreal phi2t = transform_ellipse_angle(a, b, phi2);
      if (phi2t < phi1t)
        phi2t += 360;
      //          item->setStartAngle(16*phi1t);
      item->setStartAngle(-16 * phi2t);
      item->setSpanAngle(16 * phi2t - 16 * phi1t);
    }
    m_pScene->addItem(item);
  } else {
    if (phi1 != 0 || phi2 - phi1 != 360) {
      qreal phi1t = transform_ellipse_angle(a, b, phi1);
      qreal phi2t = transform_ellipse_angle(a, b, phi2);
      if (phi2t < phi1t)
        phi2t += 360;
      // QGraphicsFilledChordItem* item = new
      // QGraphicsFilledChordItem(m_ScaleX*x1, m_ScaleY*y1, m_ScaleX*a,
      // m_ScaleY*b, phi1t, phi2t, m_ScaleX*m_Xmax, m_ScaleY*m_Ymax,
      // m_OutlinePen);
      QGraphicsFilledChordItem *item = new QGraphicsFilledChordItem(
          m_ScaleX * (x1), m_ScaleY * (y1), m_ScaleX * a, m_ScaleY * b, -phi2t,
          -phi1t, m_ScaleX * m_Xmax, m_ScaleY * m_Ymax, pen);
      m_pScene->addItem(item);
    }
  }
  return rvG_OK;
}

// --------------------------------------------------------------------------------------------

void QGraphicsViewOutput::PrintToOutput(const std::string & /*s*/) { return; }
