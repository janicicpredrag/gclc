#if !defined(QGraphicsViewOutput_H)
#define QGraphicsViewOutput_H

#include "GCLCOutput.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPen>

class QGraphicsViewOutput : public CGCLCOutput {
public:
  QGraphicsViewOutput(QGraphicsView *pView, QGraphicsScene *pScene);
  virtual ~QGraphicsViewOutput() = default;

  virtual void Reset();
  virtual GReturnValue Init();
  virtual GReturnValue Init(int x, int y);
  virtual GReturnValue Close();
  virtual GReturnValue CloseAll();
  virtual GReturnValue SetPixel(double x, double y);
  virtual GReturnValue DrawSegment(double x1, double y1, double x2, double y2);
  virtual GReturnValue DrawArc(double x1, double y1, double x2, double y2,
                               double phi, int precision);
  virtual GReturnValue FillTriangle(double x1, double y1, double x2, double y2,
                                    double x3, double y3);
  virtual GReturnValue FillRectangle(double x1, double y1, double x2,
                                     double y2);
  virtual GReturnValue FillEllipseArc(double x1, double y1, double a, double b,
                                      double phi1, double phi2,
                                      bool bFillTriangle);

  virtual GReturnValue SetColor(unsigned char r, unsigned char g,
                                unsigned char b);
  virtual GReturnValue SetBackgroundColor(unsigned char r, unsigned char g,
                                          unsigned char b);
  virtual GReturnValue SetThickness(double uThickness);
  virtual GReturnValue PrintText(double x, double y,
                                 const std::string &sPosition,
                                 const std::string &sText);
  virtual GReturnValue PrintComment(const std::string & /* sText */) {
    return rvG_OK;
  }
  virtual GReturnValue Export(const std::string & /* sText */,
                              export_type /* format */) {
    return rvG_OK;
  }
  virtual GReturnValue SetFontSize(unsigned char size);

  virtual void SetPen(QPen pen) { m_OutlinePen = pen; }
  virtual QPen GetPen() { return m_OutlinePen; }

  void Update();
  qreal getScale() { return m_ScaleX; }

private:
  void PrintToOutput(const std::string &s);
  QGraphicsScene *m_pScene;
  QGraphicsView *m_pView;
  QPen m_OutlinePen;
  QFont m_Font;

  float m_ScaleX, m_ScaleY;
  int m_Xmin, m_Xmax, m_Ymin, m_Ymax;
  int m_Fontsize;
};

#endif // !defined(QGraphicsViewOutput_H)
