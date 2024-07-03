#include "../Input/GCLCInput.h"
#include "GCLC.h"

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_comment() {
  GReturnValue iRv;
  unsigned char c;
  std::string sXMLcomment;
  for (;;) {
    iRv = m_Input->ReadChar(c);
    if (iRv == rvG_EndOfData || c == '\n' || c == '\r')  {
        //-------- Support for input exported to XML form ---------
        if (m_bXMLOutput && m_hXMLOutput.good()) {
            ChangeCurrentXMLGroup(eXMLnone);
            Print(m_hXMLOutput, "<!-- " + sXMLcomment + " -->\n");
            m_CurrentXMLgroup = eXMLnone;
        }
        //-------- End of support for XML form ---------
        return rvGCLCOK;
    }
    if (iRv == rvG_InvalidInput)
      return rvGCLCInvalidInput;
    sXMLcomment += c;
  }
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::dim() {
  GCLCError iRv;
  double x, y;

  if ((iRv = ReadNumber(x)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(y)) != rvGCLCOK)
    return iRv;
  if (SetDim(x, y) != rvG_OK)
    return rvCannotExport;

  //-------- Support for input exported to XML form ---------
  if (m_bXMLOutput && m_hXMLOutput.good()) {
    ChangeCurrentXMLGroup(eXMLdraw);
    Print(m_hXMLOutput, "\t\t<dimensions width=\"" + d2s(x, -1) +
                            "\" height=\"" + d2s(y, -1) + "\"></dimensions>\n");
    m_CurrentXMLgroup = eXMLdraw;
  }
  //-------- End of support for XML form ---------

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_area() {
  GCLCError iRv;

  if ((iRv = ReadNumber(m_Basic_Area.x_lb)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(m_Basic_Area.y_lb)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(m_Basic_Area.x_rt)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(m_Basic_Area.y_rt)) != rvGCLCOK)
    return iRv;

  //-------- Support for input exported to XML form ---------
  if (m_bXMLOutput && m_hXMLOutput.good()) {
    ChangeCurrentXMLGroup(eXMLdraw);
    Print(m_hXMLOutput, "\t\t<area x1=\"" + d2s(m_Basic_Area.x_lb, -1) + "\" " +
                            "y1=\"" + d2s(m_Basic_Area.y_lb, -1) + "\" " +
                            "x2=\"" + d2s(m_Basic_Area.x_rt, -1) + "\" " +
                            "y2=\"" + d2s(m_Basic_Area.y_rt, -1) + "\"" +
                            "></area>\n");
    m_CurrentXMLgroup = eXMLdraw;
  }
  //-------- End of support for XML form ---------

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_color() {
  GCLCError iRv;
  double r, g, b;

  if ((iRv = ReadNumber(r)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(g)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(b)) != rvGCLCOK)
    return iRv;

  //-------- Support for input exported to XML form ---------
  if (m_bXMLOutput && m_hXMLOutput.good())
    m_sXMLNewColor =
        std::string(1, (char)(((int)r) / 16 > 9 ? ((int)r) / 16 - 10 + 'A'
                                           : ((int)r) / 16 + '0')) +
        std::string(1, (char)((int)r) % 16 > 9 ? ((int)r) % 16 - 10 + 'A'
                                          : ((int)r) % 16 + '0') +
        std::string(1, (char)((int)g) / 16 > 9 ? ((int)g) / 16 - 10 + 'A'
                                          : ((int)g) / 16 + '0') +
        std::string(1, (char)((int)g) % 16 > 9 ? ((int)g) % 16 - 10 + 'A'
                                          : ((int)g) % 16 + '0') +
        std::string(1, (char)((int)b) / 16 > 9 ? ((int)b) / 16 - 10 + 'A'
                                          : ((int)b) / 16 + '0') +
        std::string(1, (char)((int)b) % 16 > 9 ? ((int)b) % 16 - 10 + 'A'
                                          : ((int)b) % 16 + '0');
  //-------- End of support for XML form ---------

  if (PrintComment("Changing color " + i2s((unsigned int)r % 256) + " " +
                   i2s((unsigned int)g % 256) + " " +
                   i2s((unsigned int)b % 256)) != rvG_OK)
    return rvCannotExport;
  if (SetColor((unsigned char)r, (unsigned char)g, (unsigned char)b) != rvG_OK)
    return rvCannotExport;

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_background() {
  GCLCError iRv;
  double r, g, b;

  if ((iRv = ReadNumber(r)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(g)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(b)) != rvGCLCOK)
    return iRv;

  if (SetBackgroundColor((unsigned char)r, (unsigned char)g,
                         (unsigned char)b) != rvG_OK)
    return rvCannotExport;

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_fontsize() {
  GCLCError iRv;
  double num;

  if ((iRv = ReadNumber(num)) != rvGCLCOK)
    return iRv;

  if (num < 0)
    num = 0;
  if (num > 255)
    num = 255;

  if (SetFontSize((unsigned char)num))
    return rvGCLCOutOfMemory;
  m_FontSize = (int)num;

  //-------- Support for input exported to XML form ---------
  if (m_bXMLOutput && m_hXMLOutput.good())
    m_iXMLNewFontSize = (int)num;
  //-------- End of support for XML form ---------

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_arrow_style() {
  GCLCError iRv;

  if ((iRv = ReadNumber(ARROW_ANGLE)) != rvGCLCOK)
    return iRv;
  if (ARROW_ANGLE <= 0 || ARROW_ANGLE >= 180)
    ARROW_ANGLE = 15.0;

  if ((iRv = ReadNumber(ARROW_LENGTH)) != rvGCLCOK)
    return iRv;
  if (ARROW_LENGTH <= 0)
    ARROW_LENGTH = 3.0;

  if ((iRv = ReadNumber(ARROW_INNER_TOWARDS)) != rvGCLCOK)
    return iRv;
  if (ARROW_INNER_TOWARDS < 0 || ARROW_INNER_TOWARDS > 1.0)
    ARROW_INNER_TOWARDS = 2.0 / 3.0;

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_circleprecision() {
  GCLCError iRv;
  double num;

  if ((iRv = ReadNumber(num)) != rvGCLCOK)
    return iRv;
  CIRCLE_PRECISION = (int)num;

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_bezierprecision() {
  GCLCError iRv;
  double num;

  if ((iRv = ReadNumber(num)) != rvGCLCOK)
    return iRv;
  BEZIER_PRECISION = (int)num;

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_linethickness() {
  double num;
  GCLCError iRv;

  if ((iRv = ReadNumber(num)) != rvGCLCOK)
    return iRv;
  if (SetThickness(num) != rvG_OK)
    return rvCannotExport;

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::set_double() {
  if (SetThickness(-2) != rvG_OK)
    return rvCannotExport;
  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::set_normal() {
  if (SetThickness(-1) != rvG_OK)
    return rvCannotExport;
  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_dash() {
  double d;
  GCLCError iRv;

  if ((iRv = ReadNumber(d)) != rvGCLCOK)
    return iRv;
  if (SetDashLen(d, d / 2, d, d / 2) != rvG_OK)
    return rvCannotExport;
  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_dashstyle() {
  double d1, d2, d3, d4;
  GCLCError iRv;

  if ((iRv = ReadNumber(d1)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(d2)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(d3)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(d4)) != rvGCLCOK)
    return iRv;
  SetDashLen(d1, d2, d3, d4);

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_dmc() { return ReadNumber(DM); }

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_mcr() { return ReadNumber(MARK_RADIUS); }

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_mcp() {
  double d;
  GCLCError iRv;

  if ((iRv = ReadNumber(d)) != rvGCLCOK)
    return iRv;
  MARK_CIRCLE_PRECISION = (int)d;
  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_export(export_type format) {
  GCLCError iRv;
  std::string text;

  if ((iRv = take_text(text)) != rvGCLCOK)
    return iRv;
  DirectExport(text, format);

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------
