// EPSOutput.cpp: implementation of the CEPSOutput class.
//
//////////////////////////////////////////////////////////////////////

#include "EPSOutput.h"
#include "../Utils/Utils.h"
#include <cmath>

#define SCALE 2.8346
#define X_OFFSET 0
#define Y_OFFSET 0

// ----------------------------------------------------------------------------

CEPSOutput::CEPSOutput(std::ostream &h) : OUTPUT(h) {
  LINE_WIDTH = 0.16;
  m_dCurrentLineWidth = LINE_WIDTH;
  m_iCurrentFontSize = 8;

  m_r = 0;
  m_g = 0;
  m_b = 0;
}

// ----------------------------------------------------------------------------

GReturnValue CEPSOutput::SetThickness(double uThickness) {
  if (uThickness < 0)
    PrintToOutput(d2s(-SCALE * uThickness * LINE_WIDTH, 2) +
                  " setlinewidth\n");
  else
    PrintToOutput(d2s(SCALE * uThickness, 2) + " setlinewidth\n");
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CEPSOutput::SetPixel(double /*x*/, double /*y*/) { return rvG_OK; }

// ----------------------------------------------------------------------------

void CEPSOutput::Reset() {}

// ----------------------------------------------------------------------------

GReturnValue CEPSOutput::SetFontSize(unsigned char size) {
  m_iCurrentFontSize = (size <= 0) ? 10 : size;
  PrintToOutput("/Times-Italic findfont\n");
  PrintToOutput(i2s(m_iCurrentFontSize) + " scalefont\nsetfont\n");
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CEPSOutput::SetColor(unsigned char r, unsigned char g,
                                  unsigned char b) {
  PrintToOutput(d2s(r / 255.0, 2) + " " + d2s(g / 255.0, 2) + " " +
                d2s(b / 255.0, 2) + " setrgbcolor\n");
  m_r = r;
  m_g = g;
  m_b = b;
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CEPSOutput::SetBackgroundColor(unsigned char r, unsigned char g,
                                            unsigned char b) {
  PrintToOutput(d2s(r / 255.0, 2) + " " + d2s(g / 255.0, 2) + " " +
                d2s(b / 255.0, 2) + " setrgbcolor\n");
  PrintToOutput("newpath " + d2s((X_OFFSET)*SCALE, 2) + " " +
                d2s((Y_OFFSET)*SCALE, 2) + " " +
                d2s((X_OFFSET + m_nX) * SCALE, 2) + " " +
                d2s((Y_OFFSET + m_nY) * SCALE, 2) + " rectfill\n");
  PrintToOutput(d2s(m_r / 255.0, 2) + " " + d2s(m_g / 255.0, 2) +
                " " + d2s(m_b / 255.0, 2) + " setrgbcolor\n");
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CEPSOutput::PrintText(double x, double y, const std::string &sPosition,
                                   const std::string &sText) {
  PrintToOutput(d2s((X_OFFSET + x) * SCALE, 2) + " " +
                d2s((Y_OFFSET + y) * SCALE, 2) + " moveto\n");
  if (sPosition == "[rb]") {
    PrintToOutput("(" + sText + ") dup stringwidth pop neg 0 rmoveto show\n");
  } else if (sPosition == "[r]") {
    PrintToOutput("(" + sText + ") dup stringwidth pop neg -" +
                  d2s((2.0 / 3) * m_iCurrentFontSize / 2.0, 2) +
                  " rmoveto show\n");
  } else if (sPosition == "[rt]") {
    PrintToOutput("(" + sText + ") dup stringwidth pop neg -" +
                  d2s((2.0 / 3) * m_iCurrentFontSize, 2) +
                  " rmoveto show\n");
  } else if (sPosition == "[b]") {
    PrintToOutput("(" + sText +
                  ") dup stringwidth pop 2 div neg 0 rmoveto show\n");
  } else if (sPosition == "[t]") {
    PrintToOutput("(" + sText + ") dup stringwidth pop 2 div neg -" +
                  d2s((2.0 / 3) * m_iCurrentFontSize, 2) +
                  " rmoveto show\n");
  } else if (sPosition == "[lb]") {
    PrintToOutput("(" + sText + ") show\n");
  } else if (sPosition == "[l]") {
    PrintToOutput("0 -" + d2s((2.0 / 3) * m_iCurrentFontSize / 2.0, 2) +
                  " rmoveto (" + sText + ") show\n");
  } else if (sPosition == "[lt]") {
    PrintToOutput("0 -" + d2s((2.0 / 3) * m_iCurrentFontSize, 2) +
                  " rmoveto (" + sText + ") show\n");
  } else if (sPosition == "") {
    PrintToOutput("(" + sText + ") dup stringwidth pop 2 div neg -" +
                  d2s((2.0 / 3) * m_iCurrentFontSize / 2.0, 2) +
                  " rmoveto show\n");
  } else if (sPosition == "[cc]") {
    PrintToOutput("(" + sText + ") dup stringwidth pop 2 div neg -" +
                  d2s((2.0 / 3) * m_iCurrentFontSize / 2.0, 2) +
                  " rmoveto show\n");
  }
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CEPSOutput::PrintComment(const std::string &sText) {
  PrintToOutput("\n% " + sText + "\n");
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CEPSOutput::Export(const std::string &sText, export_type format) {
  if (format == eEPSoutput) {
    PrintToOutput("\n% Directly exported");
    PrintToOutput("\n" + sText + "\n");
  }
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CEPSOutput::DrawSegment(double x1, double y1, double x2,
                                     double y2) {
  if ((x1 - x2 < 0.01) && (x2 - x1 < 0.01) && (y1 - y2 < 0.01) &&
      (y2 - y1 < 0.01))
    return rvG_OK;
  PrintToOutput("newpath ");
  PrintToOutput(d2s((X_OFFSET + x1) * SCALE, 2) + " " +
                d2s((Y_OFFSET + y1) * SCALE, 2) + " moveto ");
  PrintToOutput(d2s((x2 - x1) * SCALE, 2) + " " +
                d2s((y2 - y1) * SCALE, 2) + " rlineto ");
  PrintToOutput("stroke\n");
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CEPSOutput::DrawArc(double x1, double y1, double x2, double y2,
                                 double phi, int /* precision */) {
  double r, phi1;
  phi1 = 180 * atan2(y2 - y1, x2 - x1) / PI;
  x1 = (X_OFFSET + x1) * SCALE;
  y1 = (Y_OFFSET + y1) * SCALE;
  x2 = (X_OFFSET + x2) * SCALE;
  y2 = (Y_OFFSET + y2) * SCALE;
  r = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
  PrintToOutput("newpath " + d2s(x1, 2) + " " + d2s(y1, 2) + " " +
                d2s(r, 2) + " " + d2s(phi1, 2) + " " +
                d2s(phi1 + phi, 2) + " arc \n");
  PrintToOutput("stroke\n");
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CEPSOutput::Close() {
  PrintToOutput("showpage\n%EOF\n");
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CEPSOutput::Init(int x, int y) {
  if (!OUTPUT.good())
    return rvG_CannotOpenOutputFile;
  PrintToOutput("%!PS-Adobe-3.0 EPSF-3.0\n");
  PrintToOutput("%%BoundingBox: " + i2s((int)((X_OFFSET)*SCALE + 0.5)) + " " +
                i2s((int)((Y_OFFSET)*SCALE + 0.5)) + " " +
                i2s((int)((X_OFFSET + x) * SCALE + 0.5)) + " " +
                i2s((int)((Y_OFFSET + y) * SCALE + 0.5)) + "\n");
  PrintToOutput("%%BeginProlog\n");
  PrintToOutput("%%EndProlog\n");
  PrintToOutput("/Times-Italic findfont\n");
  PrintToOutput(i2s(m_iCurrentFontSize) + " scalefont\nsetfont\n");
  PrintToOutput(d2s(SCALE * m_dCurrentLineWidth, 2) + " setlinewidth\n\n");

  PrintToOutput("/ellipse {7 dict begin\n");
  PrintToOutput("/endangle exch def\n");
  PrintToOutput("/startangle exch def\n");
  PrintToOutput("/yradius exch def\n");
  PrintToOutput("/xradius exch def\n");
  PrintToOutput("/yC exch def\n");
  PrintToOutput("/xC exch def\n");
  PrintToOutput("/savematrix matrix currentmatrix def\n");
  PrintToOutput("xC yC translate\n");
  PrintToOutput("xradius yradius scale\n");
  PrintToOutput("0 0 1 startangle endangle arc\n");
  PrintToOutput("savematrix setmatrix\n");
  PrintToOutput("end\n");
  PrintToOutput("} def\n\n");

  m_nX = x;
  m_nY = y;
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CEPSOutput::FillTriangle(double x1, double y1, double x2,
                                      double y2, double x3, double y3) {
  PrintToOutput("newpath " + d2s((X_OFFSET + x1) * SCALE, 2) + " " +
                d2s((Y_OFFSET + y1) * SCALE, 2) + " moveto " +
                d2s(x2 * SCALE, 2) + " " + d2s(y2 * SCALE, 2) +
                " lineto " + d2s(x3 * SCALE, 2) + " " +
                d2s(y3 * SCALE, 2) + " lineto\n");
  PrintToOutput("gsave " + d2s(m_r / 255.0, 2) + " " +
                d2s(m_g / 255.0, 2) + " " + d2s(m_b / 255.0, 2) +
                " setrgbcolor fill grestore\n");
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CEPSOutput::FillRectangle(double x1, double y1, double x2,
                                       double y2) {
  PrintToOutput("newpath " + d2s((X_OFFSET + x1) * SCALE, 2) + " " +
                d2s((Y_OFFSET + y1) * SCALE, 2) + " moveto " +
                d2s((x2 - x1) * SCALE, 2) + " 0.00 rlineto 0.00 " +
                d2s((y2 - y1) * SCALE, 2) + " rlineto " +
                d2s((x1 - x2) * SCALE, 2) + " 0.00 rlineto\n");
  PrintToOutput("gsave " + d2s(m_r / 255.0, 2) + " " +
                d2s(m_g / 255.0, 2) + " " + d2s(m_b / 255.0, 2) +
                " setrgbcolor fill grestore\n");
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CEPSOutput::FillEllipseArc(double x1, double y1, double a,
                                        double b, double phi1, double phi2,
                                        bool bFillTriangle) {
  while (phi1 < 0)
    phi1 += 360;
  while (phi1 > 360)
    phi1 -= 360;
  while (phi2 < 0)
    phi2 += 360;
  while (phi2 > 360)
    phi2 -= 360;

  x1 = (X_OFFSET + x1) * SCALE;
  y1 = (Y_OFFSET + y1) * SCALE;

  a = a * SCALE;
  b = b * SCALE;

  if (phi1 == 0 && phi2 == 360) {
    PrintToOutput("newpath " + d2s(x1, 2) + " " + d2s(y1, 2) + " " +
                  d2s(a, 2) + " " + d2s(b, 2) + " 0 360 ellipse\n");
    PrintToOutput("gsave " + d2s(m_r / 255.0, 2) + " " +
                  d2s(m_g / 255.0, 2) + " " + d2s(m_b / 255.0, 2) +
                  " setrgbcolor fill grestore\n");
    return rvG_OK;
  }

  // transform the angles, w.r.t. the def for ellipse
  double phi1t = transform_ellipse_angle(a, b, phi1);
  double phi2t = transform_ellipse_angle(a, b, phi2);

  if (bFillTriangle)
    PrintToOutput("newpath " + d2s(x1, 2) + " " + d2s(y1, 2) +
                  " moveto " + d2s(x1, 2) + " " + d2s(y1, 2) + " " +
                  d2s(a, 2) + " " + d2s(b, 2) + " " +
                  d2s(phi1t, 2) + " " + d2s(phi2t, 2) + " ellipse\n");
  else
    PrintToOutput("newpath " + d2s(x1, 2) + " " + d2s(y1, 2) + " " +
                  d2s(a, 2) + " " + d2s(b, 2) + " " +
                  d2s(phi1t, 2) + " " + d2s(phi2t, 2) + " ellipse\n");

  PrintToOutput("gsave " + d2s(m_r / 255.0, 2) + " " +
                d2s(m_g / 255.0, 2) + " " + d2s(m_b / 255.0, 2) +
                " setrgbcolor fill grestore\n");
  return rvG_OK;
}

// ----------------------------------------------------------------------------

void CEPSOutput::PrintToOutput(const std::string &s) { Print(OUTPUT, s); }

// ----------------------------------------------------------------------------
