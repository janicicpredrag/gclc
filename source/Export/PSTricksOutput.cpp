// PSTricksOutput.cpp: implementation of the CPSTricksOutput class.
//
//////////////////////////////////////////////////////////////////////

#include "PSTricksOutput.h"
#include "../Utils/Utils.h"
#include <cmath>

// ----------------------------------------------------------------------------

CPSTricksOutput::CPSTricksOutput(std::ostream &h) : OUTPUT(h) {
  OUTPUT_LINE = 0;
  LINE_WIDTH = 0.16;
  m_dCurrentLineWidth = LINE_WIDTH;
  m_bColorsUsed = false;
}

// ----------------------------------------------------------------------------

void CPSTricksOutput::Reset() {}

// ----------------------------------------------------------------------------

GReturnValue CPSTricksOutput::Init(int x, int y) {
  if (!OUTPUT.good())
    return rvG_CannotOpenOutputFile;

  PrintToOutput("\\pspicture*(0,0)(" + d2s(x / 10.0, 2) + "," +
                d2s(y / 10.0, 2) + ")\n");
  PrintToOutput("\\psset{unit=1mm}\n");
  Print(OUTPUT, "\\psset{linewidth=" + d2s(m_dCurrentLineWidth, 2) + "mm}%\n");
  OUTPUT_LINE++;
  OUTPUT_LINE++;
  OUTPUT_LINE++;
  PrintToOutput("{\\footnotesize\n");
  OUTPUT_LINE++;

  m_nX = (double)x;
  m_nY = (double)y;
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CPSTricksOutput::Close() {
  if (m_bColorsUsed) {
    PrintToOutput("\\color{black}\n");
    OUTPUT_LINE++;
  }
  PrintToOutput("}\n");
  OUTPUT_LINE++;
  PrintToOutput("\\endpspicture\n");
  OUTPUT_LINE++;
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CPSTricksOutput::SetPixel(double /*x*/, double /*y*/) {
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CPSTricksOutput::DrawSegment(double x1, double y1, double x2,
                                          double y2) {
  if ((x1 - x2 < 0.01) && (x2 - x1 < 0.01) && (y1 - y2 < 0.01) &&
      (y2 - y1 < 0.01))
    return rvG_OK;
  PrintToOutput("\\qline (" + d2s(x1, 6) + "," + d2s(y1, 6) + ")(" +
                d2s(x2, 6) + "," + d2s(y2, 6) + ")\n");
  OUTPUT_LINE++;

  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CPSTricksOutput::DrawArc(double x1, double y1, double x2,
                                      double y2, double phi,
                                      int /*precision*/) {
  double r = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));

  if (phi == 360) {
    PrintToOutput("\\pscircle (" + d2s(x1, 2) + "," + d2s(y1, 2) + "){" +
                  d2s(r, 2) + "}\n");
    OUTPUT_LINE++;
    return rvG_OK;
  }

  double angle1, angle2;

  angle1 = atan2(y2 - y1, x2 - x1);
  angle1 = 180.0 * angle1 / PI;
  while (angle1 > 360)
    angle1 -= 360;
  while (angle1 < 0)
    angle1 += 360;

  angle2 = angle1 + phi;
  while (angle2 > 360)
    angle2 -= 360;
  while (angle2 < 0)
    angle2 += 360;

  PrintToOutput("\\psarc(" + d2s(x1, 2) + "," + d2s(y1, 2) + "){" + d2s(r, 2) +
                "}{" + d2s(angle1, 2) + "}{" + d2s(angle2, 2) + "}\n");

  OUTPUT_LINE++;
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CPSTricksOutput::SetColor(unsigned char r, unsigned char g,
                                       unsigned char b) {
  std::string sColorName = "r" + i2s((unsigned int)r) + "g" + i2s((unsigned int)g) +
                      "b" + i2s((unsigned int)b);
  m_bColorsUsed = true;
  PrintToOutput("\\definecolor{" + sColorName + "}{rgb}{" + d2s(r / 255.0, 6) +
                "," + d2s(g / 255.0, 6) + "," + d2s(b / 255.0, 6) + "}%\n");
  PrintToOutput("\\psset{linecolor=" + sColorName + "}\n");
  PrintToOutput("\\color{" + sColorName + "}% \n");
  OUTPUT_LINE++;
  OUTPUT_LINE++;
  OUTPUT_LINE++;
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CPSTricksOutput::SetBackgroundColor(unsigned char r,
                                                 unsigned char g,
                                                 unsigned char b) {
  std::string sColorName = "r" + i2s((unsigned int)r) + "g" + i2s((unsigned int)g) +
                      "b" + i2s((unsigned int)b);
  PrintToOutput("\\definecolor{" + sColorName + "}{rgb}{" + d2s(r / 255.0, 6) +
                "," + d2s(g / 255.0, 6) + "," + d2s(b / 255.0, 6) + "}%\n");
  PrintToOutput("\\pspolygon*[linecolor=" + sColorName + "](" + d2s(0.0, 2) +
                "," + d2s(0.0, 2) + ")(" + d2s(m_nX, 2) + "," + d2s(0.0, 2) +
                ")(" + d2s(m_nX, 2) + "," + d2s(m_nY, 2) + ")(" + d2s(0.0, 2) +
                "," + d2s(m_nY, 2) + ")\n");
  OUTPUT_LINE++;
  OUTPUT_LINE++;
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CPSTricksOutput::SetFontSize(unsigned char size) {
  if ((1 <= size) && (size <= 5))
    PrintToOutput("\\tiny\n");
  else if ((6 <= size) && (size <= 7))
    PrintToOutput("\\scriptsize\n");
  else if (size == 8)
    PrintToOutput("\\footnotesize\n");
  else if (size == 9)
    PrintToOutput("\\small\n");
  else if (size == 10)
    PrintToOutput("\\normalsize\n");
  else if ((11 <= size) && (size <= 12))
    PrintToOutput("\\large\n");
  else if ((13 <= size) && (size <= 14))
    PrintToOutput("\\Large\n");
  else if ((15 <= size) && (size <= 17))
    PrintToOutput("\\LARGE\n");
  else if ((18 <= size) && (size <= 20))
    PrintToOutput("\\huge\n");
  else // if (21<=size)
    PrintToOutput("\\Huge\n");

  OUTPUT_LINE++;
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CPSTricksOutput::SetThickness(double uThickness) {
  m_dCurrentLineWidth =
      (uThickness < 0) ? -uThickness * LINE_WIDTH : uThickness;
  Print(OUTPUT, "\\psset{linewidth=" + d2s(m_dCurrentLineWidth, 2) + "mm}%\n");
  OUTPUT_LINE++;
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CPSTricksOutput::PrintText(double x, double y,
                                        const std::string &sPosition,
                                        const std::string &sText) {
  PrintToOutput("\\rput" + sPosition + "(" + d2s(x, 2) + "," + d2s(y, 2) +
                "){$" + sText + "$}\n");
  OUTPUT_LINE++;
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CPSTricksOutput::PrintComment(const std::string &sText) {
  PrintToOutput("\n% " + sText + "\n");
  OUTPUT_LINE++;
  OUTPUT_LINE++;
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CPSTricksOutput::Export(const std::string &sText, export_type format) {
  if (format == ePSTricksoutput || format == eGenericLaTeXoutput) {
    PrintToOutput("\n% Directly exported");
    PrintToOutput("\n" + sText + "\n");
  }
  OUTPUT_LINE++;
  OUTPUT_LINE++;
  OUTPUT_LINE++;
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CPSTricksOutput::FillTriangle(double x1, double y1, double x2,
                                           double y2, double x3, double y3) {
  PrintToOutput("\\pspolygon*(" + d2s(x1, 2) + "," + d2s(y1, 2) + ")(" +
                d2s(x2, 2) + "," + d2s(y2, 2) + ")(" + d2s(x3, 2) + "," +
                d2s(y3, 2) + ")\n");
  OUTPUT_LINE++;
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CPSTricksOutput::FillRectangle(double x1, double y1, double x2,
                                            double y2) {
  PrintToOutput("\\pspolygon*(" + d2s(x1, 2) + "," + d2s(y1, 2) + ")(" +
                d2s(x2, 2) + "," + d2s(y1, 2) + ")(" + d2s(x2, 2) + "," +
                d2s(y2, 2) + ")(" + d2s(x1, 2) + "," + d2s(y2, 2) + ")\n");
  OUTPUT_LINE++;
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CPSTricksOutput::FillEllipseArc(double x1, double y1, double a,
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
      PrintToOutput("\\pscircle*(" + d2s(x1, 2) + "," + d2s(y1, 2) + "){" +
                    d2s(a, 2) + "}\n");
      OUTPUT_LINE++;
      return rvG_OK;
    } else {
      PrintToOutput("\\psellipse*(" + d2s(x1, 2) + "," + d2s(y1, 2) + ")(" +
                    d2s(a, 2) + "," + d2s(b, 2) + ")\n");
      OUTPUT_LINE++;
      return rvG_OK;
    }
  }

  double xt1, yt1, xt2, yt2;
  phi1 = transform_ellipse_angle(a, b, phi1);
  xt1 = x1 + a * cos(PI * phi1 / 180);
  yt1 = y1 + b * sin(PI * phi1 / 180);

  phi2 = transform_ellipse_angle(a, b, phi2);
  xt2 = x1 + a * cos(PI * phi2 / 180);
  yt2 = y1 + b * sin(PI * phi2 / 180);

  if (bFillTriangle)
    PrintToOutput("\\pspolygon*(" + d2s(x1, 2) + "," + d2s(y1, 2) + ")(" +
                  d2s(xt1, 2) + "," + d2s(yt1, 2) + ")(" + d2s(xt2, 2) + "," +
                  d2s(yt2, 2) + ")\n");

  PrintToOutput("\\psellipticarc*(" + d2s(x1, 2) + "," + d2s(y1, 2) + ")(" +
                d2s(a, 2) + "," + d2s(b, 2) + "){" + d2s(phi1, 2) + "}{" +
                d2s(phi2, 2) + "}\n");
  OUTPUT_LINE++;
  OUTPUT_LINE++;
  return rvG_OK;
}

// ----------------------------------------------------------------------------

void CPSTricksOutput::PrintToOutput(const std::string &s) { Print(OUTPUT, s); }

// ----------------------------------------------------------------------------
