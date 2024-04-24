// GCLCOutput.h: interface for the CGCLCOutput class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(GCLCOUTPUT_H)
#define GCLCOUTPUT_H

#include "../Utils/Common.h"
#include <fstream>
#include <ostream>
#include <string>

/* ********************************************************
The class CGCLCOutput provides an abstract interface
common for all export formats. A support for a new
format requires implementing a new class that inherits
CGCLCOutput.
******************************************************** */

class CGCLCOutput {
public:
  CGCLCOutput();
  virtual ~CGCLCOutput() = 0;

  virtual void Reset() = 0;
  virtual GReturnValue Init(int x, int y) = 0;
  virtual GReturnValue Close() = 0;
  virtual GReturnValue SetPixel(double x, double y) = 0;
  virtual GReturnValue DrawSegment(double x1, double y1, double x2,
                                   double y2) = 0;
  virtual GReturnValue DrawArc(double x1, double y1, double x2, double y2,
                               double phi, int precision) = 0;
  virtual GReturnValue FillTriangle(double x1, double y1, double x2, double y2,
                                    double x3, double y3) = 0;
  virtual GReturnValue FillRectangle(double x1, double y1, double x2,
                                     double y2) = 0;
  virtual GReturnValue FillEllipseArc(double x1, double y1, double a, double b,
                                      double phi1, double phi2,
                                      bool bFillTriangle) = 0;

  virtual GReturnValue SetColor(unsigned char r, unsigned char g,
                                unsigned char b) = 0;
  virtual GReturnValue SetBackgroundColor(unsigned char r, unsigned char g,
                                          unsigned char b) = 0;
  virtual GReturnValue SetThickness(double uThickness) = 0;
  virtual GReturnValue PrintText(double x1, double y1, const std::string &sPosition,
                                 const std::string &sText) = 0;
  virtual GReturnValue PrintComment(const std::string &sText) = 0;
  virtual GReturnValue Export(const std::string &sText, export_type format) = 0;
  virtual GReturnValue SetFontSize(unsigned char size) = 0;
  virtual void SetPointCounter(int /*counter*/) {}
  virtual int GetPointCounter() { return 0; }

private:
  virtual void PrintToOutput(const std::string &s) = 0;
};

#endif // !defined(GCLCOUTPUT_H)
