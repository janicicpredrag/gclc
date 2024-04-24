// SVGOutput.cpp: implementation of the CSVGOutput class.
//
//////////////////////////////////////////////////////////////////////

#include "SVGOutput.h"
#include "../Utils/Utils.h"
#include <cmath>

#define SCALE 1

// ----------------------------------------------------------------------------

CSVGOutput::CSVGOutput(std::ostream &h) : OUTPUT(h) {
  LINE_WIDTH = 0.16;
  m_dCurrentLineWidth = LINE_WIDTH;
  m_iCurrentFontSize = 8;
  m_sCurrentColor = "#000000";
}

// ----------------------------------------------------------------------------

GReturnValue CSVGOutput::SetThickness(double uThickness) {
  m_dCurrentLineWidth =
      (uThickness < 0) ? -uThickness * LINE_WIDTH : uThickness;
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CSVGOutput::SetPixel(double /*x*/, double /*y*/) { return rvG_OK; }

// ----------------------------------------------------------------------------

void CSVGOutput::Reset() {}

// ----------------------------------------------------------------------------

GReturnValue CSVGOutput::SetFontSize(unsigned char size) {
  if (size <= 0)
    m_iCurrentFontSize = 10;
  else
    m_iCurrentFontSize = size;
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CSVGOutput::SetColor(unsigned char r, unsigned char g,
                                  unsigned char b) {
  m_sCurrentColor = "#" + toHexString((unsigned int)r) +
                    toHexString((unsigned int)g) + toHexString((unsigned int)b);
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CSVGOutput::SetBackgroundColor(unsigned char r, unsigned char g,
                                            unsigned char b) {
  std::string sCurrentColor = "#" + toHexString((unsigned int)(r)) +
                         toHexString((unsigned int)(g)) +
                         toHexString((unsigned int)(b));
  PrintToOutput("<rect fill=\"" + sCurrentColor +
                "\" stroke=\"#000000\" stroke-width=\"0\" x=\"0.00\" "
                "y=\"0.00\" width=\"" +
                d2s(SCALE * (double)m_nX, 2) + "\" height=\"" +
                d2s(SCALE * (double)m_nY, 2) + "\"/>\n");
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CSVGOutput::PrintText(double x, double y, const std::string &sPosition,
                                   const std::string &sText) {
  std::string pos;
  if (sPosition == "[rb]")
    pos = "text-anchor=\"end\"";
  else if (sPosition == "[r]")
    pos = "text-anchor=\"end\" dy=\"" +
          d2s(m_iCurrentFontSize / 2 / PT_PER_MM, 6) + "\"";
  else if (sPosition == "[rt]")
    pos = "text-anchor=\"end\" dy=\"" +
          d2s(m_iCurrentFontSize / PT_PER_MM, 6) + "\"";
  else if (sPosition == "[b]")
    pos = "text-anchor=\"middle\"";
  else if (sPosition == "[t]")
    pos = "text-anchor=\"middle\" dy=\"" +
          d2s(m_iCurrentFontSize / PT_PER_MM, 6) + "\"";
  else if (sPosition == "[lb]")
    ;
  else if (sPosition == "[l]")
    pos = "dy=\"" + d2s(m_iCurrentFontSize / 2 / PT_PER_MM, 6) + "\"";
  else if (sPosition == "[lt]")
    pos = "dy=\"" + d2s(m_iCurrentFontSize / PT_PER_MM, 6) + "\"";
  else if (sPosition == "")
    pos = "text-anchor=\"middle\" dy=\"" +
          d2s(m_iCurrentFontSize / 2 / PT_PER_MM, 6) + "\"";
  else if (sPosition == "[cc]")
    pos = "text-anchor=\"middle\" dy=\"" +
          d2s(m_iCurrentFontSize / 2 / PT_PER_MM, 6) + "\"";

  PrintToOutput("<text x=\"" + d2s(x * SCALE, 2) + "\" y=\"" +
                d2s((m_nY - y) * SCALE, 2) + "\" style=\"font-size:" +
                d2s(m_iCurrentFontSize / PT_PER_MM, 2) +
                "pt; font-style:italic; fill:" + m_sCurrentColor + "\" " + pos +
                ">" + sText + "</text>\n");
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CSVGOutput::PrintComment(const std::string &sText) {
  PrintToOutput("\n<!-- " + sText + " -->\n");
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CSVGOutput::Export(const std::string &sText, export_type format) {
  if (format == eSVGoutput) {
    PrintToOutput("\n<!-- Directly exported -->");
    PrintToOutput("\n" + sText + "\n");
  }
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CSVGOutput::DrawSegment(double x1, double y1, double x2,
                                     double y2) {
  if ((x1 - x2 < 0.01) && (x2 - x1 < 0.01) && (y1 - y2 < 0.01) &&
      (y2 - y1 < 0.01))
    return rvG_OK;
  PrintToOutput("<line x1=\"" + d2s(x1 * SCALE, 2) + "\" y1=\"" +
                d2s((m_nY - y1) * SCALE, 2) + "\" x2=\"" +
                d2s(x2 * SCALE, 2) + "\" y2=\"" +
                d2s((m_nY - y2) * SCALE, 2) + "\" style=\"stroke:" +
                m_sCurrentColor + "; stroke-width:" +
                d2s(m_dCurrentLineWidth * SCALE, 2) + "\"/>\n");

  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CSVGOutput::DrawArc(double x1, double y1, double x2, double y2,
                                 double phi, int /* precision */) {
  double r = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));

  if (phi == 360) {
    PrintToOutput("<circle cx=\"" + d2s(x1 * SCALE, 2) + "\" cy=\"" +
                  d2s((m_nY - y1) * SCALE, 2) + "\" r=\"" +
                  d2s(r * SCALE, 2) + "\" fill=\"none\" stroke=\"" +
                  m_sCurrentColor + "\" stroke-width=\"" +
                  d2s(m_dCurrentLineWidth * SCALE, 2) + "\"/>\n");
  } else {
    double x3, y3, sinn, cosn;

    sinn = sin(PI * phi / 180);
    cosn = cos(PI * phi / 180);

    x3 = (x2 - x1) * cosn - (y2 - y1) * sinn + x1;
    y3 = (y2 - y1) * cosn + (x2 - x1) * sinn + y1;

    x1 = x1 * SCALE;
    y1 = (m_nY - y1) * SCALE;

    x2 = x2 * SCALE;
    y2 = (m_nY - y2) * SCALE;

    x3 = x3 * SCALE;
    y3 = (m_nY - y3) * SCALE;

    r = r * SCALE;

    int bLarge, bSweep;

    if (phi >= 180)
      bLarge = 1;
    else
      bLarge = 0;

    bSweep = 0;

    PrintToOutput("<path d=\"M" + d2s(x2, 2) + "," + d2s(y2, 2) +
                  "  a" + d2s(r, 2) + "," + d2s(r, 2) + " 0 " +
                  i2s(bLarge) + "," + i2s(bSweep) + " " +
                  d2s(x3 - x2, 2) + "," + d2s(y3 - y2, 2) +
                  "\" fill=\"none\" stroke=\"" + m_sCurrentColor +
                  "\" stroke-width=\"" +
                  d2s(m_dCurrentLineWidth * SCALE, 2) + "\"/>\n");
  }

  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CSVGOutput::Close() {
  PrintToOutput("</svg>\n");
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CSVGOutput::Init(int x, int y) {
  if (!OUTPUT.good())
    return rvG_CannotOpenOutputFile;

  m_nX = x; // width
  m_nY = y; // height
  PrintToOutput("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
  PrintToOutput("<svg width=\"" + i2s(int(x)) + "mm\" height=\"" +
                i2s(int(y)) + "mm\" viewBox=\"0 0 " + i2s(int(x * SCALE)) +
                " " + i2s(int(y * SCALE)) +
                "\" version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\">\n");
  PrintToOutput("<title>GCLCOutputFigure</title>\n");
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CSVGOutput::FillTriangle(double x1, double y1, double x2,
                                      double y2, double x3, double y3) {
  PrintToOutput(
      "<path d=\"M" + d2s(x1 * SCALE, 2) + "," +
      d2s((m_nY - y1) * SCALE, 2) + " L " + d2s(x2 * SCALE, 2) + "," +
      d2s((m_nY - y2) * SCALE, 2) + " L " + d2s(x3 * SCALE, 2) + "," +
      d2s((m_nY - y3) * SCALE, 2) + " z\" fill=\"" + m_sCurrentColor +
      "\" stroke=\"none\"/>\n");
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CSVGOutput::FillRectangle(double x1, double y1, double x2,
                                       double y2) {
  PrintToOutput(
      "<path d=\"M" + d2s(x1 * SCALE, 2) + "," +
      d2s((m_nY - y1) * SCALE, 2) + " L " + d2s(x2 * SCALE, 2) + "," +
      d2s((m_nY - y1) * SCALE, 2) + " L " + d2s(x2 * SCALE, 2) + "," +
      d2s((m_nY - y2) * SCALE, 2) + " L " + d2s(x1 * SCALE, 2) + "," +
      d2s((m_nY - y2) * SCALE, 2) + " z\" fill=\"" + m_sCurrentColor +
      "\" stroke=\"none\"/>\n");
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CSVGOutput::FillEllipseArc(double x1, double y1, double a,
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

  if (phi1 == 0 && phi2 == 360) {
    if (a == b) {
      PrintToOutput("<circle cx=\"" + d2s(x1 * SCALE, 2) + "\" cy=\"" +
                    d2s((m_nY - y1) * SCALE, 2) + "\" r=\"" +
                    d2s(a * SCALE, 2) + "\" fill=\"" + m_sCurrentColor +
                    "\" stroke=\"none\"/>\n");
      return rvG_OK;
    } else {
      PrintToOutput("<ellipse cx=\"" + d2s(x1 * SCALE, 2) + "\" cy=\"" +
                    d2s((m_nY - y1) * SCALE, 2) + "\" rx=\"" +
                    d2s(a * SCALE, 2) + "\" ry=\"" +
                    d2s(b * SCALE, 2) + "\" fill=\"" + m_sCurrentColor +
                    "\" stroke=\"none\"/>\n");
      return rvG_OK;
    }
  }

  double sinn, cosn, d;

  sinn = sin(PI * phi1 / 180);
  cosn = cos(PI * phi1 / 180);
  d = sqrt(sinn * sinn / (b * b) + cosn * cosn / (a * a));

  double xs = cosn / d + x1;
  double ys = sinn / d + y1;

  sinn = sin(PI * phi2 / 180);
  cosn = cos(PI * phi2 / 180);
  d = sqrt(sinn * sinn / (b * b) + cosn * cosn / (a * a));

  double xe = cosn / d + x1;
  double ye = sinn / d + y1;

  x1 = x1 * SCALE;
  y1 = (m_nY - y1) * SCALE;

  xs = xs * SCALE;
  ys = (m_nY - ys) * SCALE;

  xe = xe * SCALE;
  ye = (m_nY - ye) * SCALE;

  a = a * SCALE;
  b = b * SCALE;

  int bLarge, bSweep;

  if (phi2 > phi1) {
    if (phi2 - phi1 >= 180)
      bLarge = 1;
    else
      bLarge = 0;
  } else {
    if (phi1 - phi2 > 180)
      bLarge = 0;
    else
      bLarge = 1;
  }

  bSweep = 0;

  if (bFillTriangle)
    PrintToOutput("<path d=\"M " + d2s(x1, 2) + "," + d2s(y1, 2) +
                  " L " + d2s(xs, 2) + "," + d2s(ys, 2) + " a" +
                  d2s(a, 2) + " " + d2s(b, 2) + " 0 " +
                  i2s(bLarge) + " " + i2s(bSweep) + " " +
                  d2s(xe - xs, 2) + " " + d2s(ye - ys, 2) +
                  " z\" fill=\"" + m_sCurrentColor + "\" stroke=\"none\"/>\n");
  else
    PrintToOutput("<path d=\"M " + d2s(xs, 2) + "," + d2s(ys, 2) +
                  " a" + d2s(a, 2) + "," + d2s(b, 2) + " 0 " +
                  i2s(bLarge) + " " + i2s(bSweep) + " " +
                  d2s(xe - xs, 2) + " " + d2s(ye - ys, 2) +
                  " z\" fill=\"" + m_sCurrentColor + "\" stroke=\"none\"/>\n");

  return rvG_OK;
}

// ----------------------------------------------------------------------------

void CSVGOutput::PrintToOutput(const std::string &s) { Print(OUTPUT, s); }

// ----------------------------------------------------------------------------
