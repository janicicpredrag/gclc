// TikZOutput.cpp: implementation of the CTikZOutput class.
//
//////////////////////////////////////////////////////////////////////

#include "TikZOutput.h"
#include "../Utils/Utils.h"
#include <cmath>

// ----------------------------------------------------------------------------

CTikZOutput::CTikZOutput(std::ostream &h) : OUTPUT(h) {
  OUTPUT_LINE = 0;
  LINE_WIDTH = 0.16;
  m_dCurrentLineWidth = LINE_WIDTH;
  m_bColorsUsed = false;
}

// ----------------------------------------------------------------------------

void CTikZOutput::Reset() {}

// ----------------------------------------------------------------------------

GReturnValue CTikZOutput::Init(int x, int y) {
  if (!OUTPUT.good())
    return rvG_CannotOpenOutputFile;

  OUTPUT_LINE++;
  PrintToOutput("\\begin{tikzpicture}\n");
  OUTPUT_LINE++;
  PrintToOutput("\\clip (0,0) rectangle (" + d2s(x / 10.0, 6) + "," +
                d2s(y / 10.0, 6) + ");\n");
  OUTPUT_LINE++;
  PrintToOutput("{\\footnotesize\n");
  OUTPUT_LINE++;

  m_nX = (double)x;
  m_nY = (double)y;
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CTikZOutput::Close() {
  if (m_bColorsUsed) {
    PrintToOutput("\\color{black}\n");
    OUTPUT_LINE++;
  }
  PrintToOutput("}\n");
  OUTPUT_LINE++;
  PrintToOutput("\\end{tikzpicture}\n");
  OUTPUT_LINE++;
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CTikZOutput::SetPixel(double /*x*/, double /*y*/) {
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CTikZOutput::DrawSegment(double x1, double y1, double x2,
                                      double y2) {
  if ((x1 - x2 < 0.01) && (x2 - x1 < 0.01) && (y1 - y2 < 0.01) &&
      (y2 - y1 < 0.01))
    return rvG_OK;
  PrintToOutput("\\draw [line width=" + d2s(m_dCurrentLineWidth / 10, 3) +
                "cm] (" + d2s(x1 / 10, 6) + "," + d2s(y1 / 10, 6) +
                ") -- (" + d2s(x2 / 10, 6) + "," + d2s(y2 / 10, 6) +
                ");%\n");
  OUTPUT_LINE++;
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CTikZOutput::DrawArc(double x1, double y1, double x2, double y2,
                                  double phi, int /*precision*/) {
  double r = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));

  if (phi == 360) {
    PrintToOutput("\\draw [line width=" +
                  d2s(m_dCurrentLineWidth / 10, 3) + "cm] (" +
                  d2s(x1 / 10, 6) + "," + d2s(y1 / 10, 6) +
                  ") circle (" + d2s(r / 10, 6) + ");%\n");
    OUTPUT_LINE++;
    return rvG_OK;
  }

  /*TikZ expect integers as angles for the arc command (?!), so one arc has
  to be drawn and two small line segments */
  double angle1;

  angle1 = atan2(y2 - y1, x2 - x1);
  angle1 = 180.0 * angle1 / PI;
  while (angle1 > 360.0)
    angle1 -= 360;
  while (angle1 < 0.0)
    angle1 += 360;

  std::string sPath;
  MakeArcPath(x1, y1, r, r, angle1, angle1 + phi, sPath);

  PrintToOutput("\\draw [line width=" + d2s(m_dCurrentLineWidth / 10, 3) +
                "cm] " + sPath + ";%\n");
  OUTPUT_LINE++;
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CTikZOutput::SetColor(unsigned char r, unsigned char g,
                                   unsigned char b) {
  std::string sColorName = "r" + i2s((unsigned int)r) + "g" +
                      i2s((unsigned int)g) + "b" + i2s((unsigned int)b);
  PrintToOutput("\\definecolor{" + sColorName + "}{rgb}{" +
                d2s(r / 255.0, 6) + "," + d2s(g / 255.0, 6) + "," +
                d2s(b / 255.0, 6) + "}%\n");
  PrintToOutput("\\color{" + sColorName + "}% \n");
  OUTPUT_LINE++;
  OUTPUT_LINE++;
  m_bColorsUsed = true;
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CTikZOutput::SetBackgroundColor(unsigned char r, unsigned char g,
                                             unsigned char b) {
  std::string sColorName = "r" + i2s((unsigned int)r) + "g" +
                      i2s((unsigned int)g) + "b" + i2s((unsigned int)b);
  PrintToOutput("\\definecolor{" + sColorName + "}{rgb}{" +
                d2s(r / 255.0, 6) + "," + d2s(g / 255.0, 6) + "," +
                d2s(b / 255.0, 6) + "}%\n");
  PrintToOutput("\\fill [" + sColorName + "] (0,0) -- (" +
                d2s(m_nX / 10, 6) + ",0) -- (" + d2s(m_nX / 10, 6) +
                "," + d2s(m_nY / 10, 6) + ") -- (" + d2s(0.0, 6) +
                "," + d2s(m_nY / 10, 6) + ") -- cycle;%\n");
  OUTPUT_LINE++;
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CTikZOutput::SetFontSize(unsigned char size) {
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

GReturnValue CTikZOutput::SetThickness(double uThickness) {
  if (uThickness < 0) {
    m_dCurrentLineWidth = -uThickness * LINE_WIDTH;
  } else {
    m_dCurrentLineWidth = uThickness;
  }
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CTikZOutput::PrintText(double x, double y, const std::string &sPosition,
                                    const std::string &sText) {
  std::string position;
  if (sPosition == "[rb]") {
    position = "[anchor=south east]";
    x += 1;
    y -= 1;
  } else if (sPosition == "[r]") {
    position = "[anchor=east]";
    x += 1;
  } else if (sPosition == "[rt]") {
    position = "[anchor=north east]";
    x += 1;
    y += 1;
  } else if (sPosition == "[b]") {
    position = "[anchor=south]";
    y -= 1;
  } else if (sPosition == "[t]") {
    position = "[anchor=north]";
    y += 1;
  } else if (sPosition == "[lb]") {
    position = "[anchor=south west]";
    x -= 1;
    y -= 1;
  } else if (sPosition == "[l]") {
    position = "[anchor=west]";
    x -= 1;
  } else if (sPosition == "[lt]") {
    position = "[anchor=north west]";
    x -= 1;
    y += 1;
  } else if (sPosition == "") {
    position = "";
  } else if (sPosition == "[cc]") {
    position = "";
  }

  PrintToOutput("\\draw (" + d2s(x / 10, 6) + "," + d2s(y / 10, 6) +
                ") node " + position + " { $" + sText + "$ };%\n");
  OUTPUT_LINE++;
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CTikZOutput::PrintComment(const std::string &sText) {
  PrintToOutput("\n% " + sText + "\n");
  OUTPUT_LINE++;
  OUTPUT_LINE++;
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CTikZOutput::Export(const std::string &sText, export_type format) {
  if (format == eTikZoutput || format == eGenericLaTeXoutput) {
    PrintToOutput("\n% Directly exported");
    PrintToOutput("\n" + sText + "\n");
  }
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CTikZOutput::FillTriangle(double x1, double y1, double x2,
                                       double y2, double x3, double y3) {
  PrintToOutput("\\fill (" + d2s(x1 / 10, 6) + "," + d2s(y1 / 10, 6) +
                ") -- (" + d2s(x2 / 10, 6) + "," + d2s(y2 / 10, 6) +
                ") -- (" + d2s(x3 / 10, 6) + "," + d2s(y3 / 10, 6) +
                ");%\n");
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CTikZOutput::FillRectangle(double x1, double y1, double x2,
                                        double y2) {
  PrintToOutput("\\fill (" + d2s(x1 / 10, 6) + "," + d2s(y1 / 10, 6) +
                ") -- (" + d2s(x2 / 10, 6) + "," + d2s(y1 / 10, 6) +
                ") -- (" + d2s(x2 / 10, 6) + "," + d2s(y2 / 10, 6) +
                ") -- (" + d2s(x1 / 10, 6) + "," + d2s(y2 / 10, 6) +
                ");%\n");
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CTikZOutput::FillEllipseArc(double x1, double y1, double a,
                                         double b, double phi1, double phi2,
                                         bool bFillTriangle) {
  while (phi1 < 0.0)
    phi1 += 360.0;
  while (phi1 > 360.0)
    phi1 -= 360.0;
  while (phi2 < 0.0)
    phi2 += 360.0;
  while (phi2 > 360.0)
    phi2 -= 360;

  if (phi1 == 0 && phi2 == 360) {
    if (a == b) {
      PrintToOutput("\\fill (" + d2s(x1 / 10, 6) + "," +
                    d2s(y1 / 10, 6) + ") circle (" +
                    d2s(a / 10.0, 6) + ");%\n");
      OUTPUT_LINE++;
      return rvG_OK;
    } else {
      PrintToOutput("\\fill (" + d2s(x1 / 10, 6) + "," +
                    d2s(y1 / 10, 6) + ") ellipse (" +
                    d2s(a / 10.0, 6) + " and " + d2s(b / 10.0, 6) +
                    ");%\n");
      OUTPUT_LINE++;
      return rvG_OK;
    }
  }

  std::string sPath;
  MakeArcPath(x1, y1, a, b, phi1, phi2, sPath);

  if (bFillTriangle)
    PrintToOutput("\\fill (" + d2s(x1 / 10, 6) + "," +
                  d2s(y1 / 10, 6) + ") -- " + sPath + " -- cycle;%\n");
  else
    PrintToOutput("\\fill " + sPath + " -- cycle;%\n");

  OUTPUT_LINE++;
  return rvG_OK;
}

// ----------------------------------------------------------------------------

void CTikZOutput::PrintToOutput(const std::string &s) { Print(OUTPUT, s); }

// ----------------------------------------------------------------------------
