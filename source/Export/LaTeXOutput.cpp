// LaTeXOutput.cpp: implementation of the CLaTeXOutput class.
//
//////////////////////////////////////////////////////////////////////

#include "LaTeXOutput.h"
#include "../Utils/Utils.h"
#include <cmath>

using namespace std;

// ----------------------------------------------------------------------------

CLaTeXOutput::CLaTeXOutput(ostream &h) : OUTPUT(h) {
  OUTPUT_LINE = 0;
  LINE_WIDTH = 0.16;
  COUNTER = 1;
  PRECISION = 72;
  m_dCurrentLineWidth = LINE_WIDTH;
  m_bColorsUsed = false;
}

// ----------------------------------------------------------------------------

void CLaTeXOutput::Reset() {}

// ----------------------------------------------------------------------------

GReturnValue CLaTeXOutput::Init(int x, int y) {
  if (!OUTPUT.good())
    return rvG_CannotOpenOutputFile;

  OUTPUT_LINE = 0;
  PrintToOutput("\\unitlength 1mm\n");
  OUTPUT_LINE++;
  PrintToOutput("\\begin{picture}(" + d2s((double)x, 2) + "," +
                d2s((double)y, 2) + ")\n");
  OUTPUT_LINE++;
  PrintToOutput("{\\footnotesize\n");
  OUTPUT_LINE++;
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CLaTeXOutput::Close() {
  if (m_bColorsUsed) {
    PrintToOutput("\\color{black}\n");
    OUTPUT_LINE++;
  }
  PrintToOutput("}\n");
  OUTPUT_LINE++;
  PrintToOutput("\\end{picture}\n");
  OUTPUT_LINE++;
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CLaTeXOutput::SetPixel(double /*x*/, double /*y*/) {
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CLaTeXOutput::DrawSegment(double x1, double y1, double x2,
                                       double y2) {
  if ((x1 - x2 < 0.01) && (x2 - x1 < 0.01) && (y1 - y2 < 0.01) &&
      (y2 - y1 < 0.01))
    return rvG_OK;
  PrintToOutput("\\gclcline{" + d2s(x1, 2) + "}{" + d2s(y1, 2) +
                "}{" + i2s(COUNTER) + "}{" + d2s(x2, 2) + "}{" +
                d2s(y2, 2) + "}{" + i2s(COUNTER + 1) + "}{" +
                d2s(m_dCurrentLineWidth, 2) + "}%\n");
  COUNTER++;
  COUNTER++;
  OUTPUT_LINE++;
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CLaTeXOutput::DrawArc(double x1, double y1, double x2, double y2,
                                   double phi, int precision) {
  double x3, y3, x1c, y1c, x2c, y2c, r, x2start, y2start;
  int i;
  GReturnValue iRv;

  r = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));

  int num = (int)((precision * r) / 10);
  if (num < precision)
    num = precision;

  double sinn = sin(2 * PI / num);
  double cosn = cos(2 * PI / num);

  x2start = x2;
  y2start = y2;

  if (phi >= 0) {
    for (i = 1; ((360.00 * (double)i) / num) <= phi; i++) {
      x3 = x1 + (x2 - x1) * cosn - (y2 - y1) * sinn;
      y3 = y1 + (y2 - y1) * cosn + (x2 - x1) * sinn;

      x1c = x2;
      y1c = y2;
      x2c = x3;
      y2c = y3;
      iRv = DrawSegment(x1c, y1c, x2c, y2c);
      if (iRv != rvG_OK)
        return iRv;

      x2 = x3;
      y2 = y3;
    }
  } else {
    // negative angles
    for (i = -1; ((360.00 * (double)i) / num) >= phi; i--) {
      x3 = x1 + (x2 - x1) * cosn + (y2 - y1) * sinn;
      y3 = y1 + (y2 - y1) * cosn - (x2 - x1) * sinn;

      x1c = x2;
      y1c = y2;
      x2c = x3;
      y2c = y3;
      iRv = DrawSegment(x1c, y1c, x2c, y2c);
      if (iRv != rvG_OK)
        return iRv;

      x2 = x3;
      y2 = y3;
    }
  }

  // connect the last small arc with the last point on the big arc
  x3 = x1 + (x2start - x1) * cos(2 * PI * phi / 360) -
       (y2start - y1) * sin(2 * PI * phi / 360);
  y3 = y1 + (y2start - y1) * cos(2 * PI * phi / 360) +
       (x2start - x1) * sin(2 * PI * phi / 360);

  x1c = x2;
  y1c = y2;
  x2c = x3;
  y2c = y3;
  iRv = DrawSegment(x1c, y1c, x2c, y2c);
  if (iRv != rvG_OK)
    return iRv;

  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CLaTeXOutput::SetColor(unsigned char r, unsigned char g,
                                    unsigned char b) {
  string sColorName = "r" + i2s((unsigned int)r) + "g" +
                      i2s((unsigned int)g) + "b" + i2s((unsigned int)b);
  PrintToOutput("\\definecolor{" + sColorName + "}{rgb}{" +
                d2s(r / 255.0, 2) + "," + d2s(g / 255.0, 2) + "," +
                d2s(b / 255.0, 2) + "}%\n");
  OUTPUT_LINE++;
  PrintToOutput("\\color{" + sColorName + "}% \n");
  OUTPUT_LINE++;
  m_bColorsUsed = true;
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CLaTeXOutput::SetBackgroundColor(unsigned char /* r */,
                                              unsigned char /* g */,
                                              unsigned char /* b */) {
  // ignore
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CLaTeXOutput::SetFontSize(unsigned char size) {
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

GReturnValue CLaTeXOutput::SetThickness(double uThickness) {
  m_dCurrentLineWidth =
      (uThickness < 0 ? -uThickness * LINE_WIDTH : uThickness);
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CLaTeXOutput::PrintText(double x, double y,
                                     const string &sPosition,
                                     const string &sText) {
  PrintToOutput("\\put(" + d2s(x, 2) + "," + d2s(y, 2) +
                "){\\makebox(0,0)" + sPosition + "{$" + sText + "$}}%\n");
  OUTPUT_LINE++;
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CLaTeXOutput::PrintComment(const string &sText) {
  PrintToOutput("\n% " + sText + "\n");
  OUTPUT_LINE++;
  OUTPUT_LINE++;
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CLaTeXOutput::Export(const string &sText, export_type format) {
  if (format == eLaTeXoutput || format == eGenericLaTeXoutput) {
    PrintToOutput("\n% Directly exported");
    PrintToOutput("\n" + sText + "\n");
  }
  OUTPUT_LINE++;
  OUTPUT_LINE++;
  OUTPUT_LINE++;
  return rvG_OK;
}

// ----------------------------------------------------------------------------

void CLaTeXOutput::PrintToOutput(const string &s) { Print(OUTPUT, s); }

// ----------------------------------------------------------------------------
