// PSTricksOutput.h: interface for the CPSTricksOutput class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(PSTricksOUTPUT_H)
#define PSTricksOUTPUT_H

#include "GCLCOutput.h"

class CPSTricksOutput : public CGCLCOutput {
public:
  CPSTricksOutput(std::ostream &h);
  virtual ~CPSTricksOutput() = default;

  void Reset();
  GReturnValue Init(int x, int y);
  GReturnValue Close();
  GReturnValue SetPixel(double x, double y);
  GReturnValue DrawSegment(double x1, double y1, double x2, double y2);
  GReturnValue DrawArc(double x1, double y1, double x2, double y2, double phi,
                       int precision);
  GReturnValue FillTriangle(double x1, double y1, double x2, double y2,
                            double x3, double y3);
  GReturnValue FillRectangle(double x1, double y1, double x2, double y2);
  GReturnValue FillEllipseArc(double x1, double y1, double a, double b,
                              double phi1, double phi2, bool bFillTriangle);

  GReturnValue SetColor(unsigned char r, unsigned char g, unsigned char b);
  GReturnValue SetBackgroundColor(unsigned char r, unsigned char g,
                                  unsigned char b);
  GReturnValue SetThickness(double uThickness);
  GReturnValue PrintText(double x1, double y1, const std::string &sPosition,
                         const std::string &sText);
  GReturnValue PrintComment(const std::string &sText);
  GReturnValue Export(const std::string &sText, export_type format);
  GReturnValue SetFontSize(unsigned char size);

private:
  void PrintToOutput(const std::string &s);
  int OUTPUT_LINE;
  double LINE_WIDTH;
  std::ostream &OUTPUT;
  double m_dCurrentLineWidth;
  bool m_bColorsUsed;
  double m_nX; // width
  double m_nY; // height
};

#endif // !defined(PSTricksOUTPUT_H)
