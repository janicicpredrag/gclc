#include "GCLC.h"

// ----------------------------------------------------------------------------

GCLCError CGCLC::cmark(int dir) {
  GCLC_object o;
  string pos;
  GCLCError iRv;
  double x, y;

  if ((iRv = ReadObject(GCLC_POINT, o)) != rvGCLCOK)
    return iRv;
  if (PrintComment("Marking point " + o.name + " by circle") != rvG_OK)
    return rvCannotExport;

  DrawCircle(o.p[0], o.p[1], o.p[0] + MARK_RADIUS, o.p[1], false, false,
             m_Basic_Area, MARK_CIRCLE_PRECISION);
  EmptyCircle(o.p[0], o.p[1], o.p[0] + MARK_RADIUS, o.p[1], m_Basic_Area);

  double DMD = 0.7 * DM; // diagonal distance

  switch (dir) {
  case GCLC_DIR_LT:
    x = o.p[0] - DMD;
    y = o.p[1] + DMD;
    pos = "[rb]";
    break;
  case GCLC_DIR_L:
    x = o.p[0] - DM;
    y = o.p[1];
    pos = "[r]";
    break;
  case GCLC_DIR_LB:
    x = o.p[0] - DMD;
    y = o.p[1] - DMD;
    pos = "[rt]";
    break;
  case GCLC_DIR_T:
    x = o.p[0];
    y = o.p[1] + DM;
    pos = "[b]";
    break;
  case GCLC_DIR_B:
    x = o.p[0];
    y = o.p[1] - DM;
    pos = "[t]";
    break;
  case GCLC_DIR_RT:
    x = o.p[0] + DMD;
    y = o.p[1] + DMD;
    pos = "[lb]";
    break;
  case GCLC_DIR_R:
    x = o.p[0] + DM;
    y = o.p[1];
    pos = "[l]";
    break;
  case GCLC_DIR_RB:
    x = o.p[0] + DMD;
    y = o.p[1] - DMD;
    pos = "[lt]";
    break;
  default:
    x = o.p[0];
    y = o.p[1];
    pos = "";
    break;
  }

  if (!pos.empty())
    if (PrintText(x, y, pos, o.name, m_Basic_Area) != rvG_OK)
      return rvCannotExport;

  //-------- Support for input exported to XML form ---------
  if (m_bXMLOutput && m_hXMLOutput.good()) {
    string sdir;
    switch (dir) {
    case 0:
      sdir = "0\" distance=\"0";
      break;
    case GCLC_DIR_LT:
      sdir = "135";
      break;
    case GCLC_DIR_L:
      sdir = "180";
      break;
    case GCLC_DIR_LB:
      sdir = "225";
      break;
    case GCLC_DIR_T:
      sdir = "90";
      break;
    case GCLC_DIR_B:
      sdir = "270";
      break;
    case GCLC_DIR_RT:
      sdir = "45";
      break;
    case GCLC_DIR_R:
      sdir = "0";
      break;
    case GCLC_DIR_RB:
      sdir = "315";
      break;
    default:
      sdir = "";
      break;
    }
    ChangeCurrentXMLGroup(eXMLlabel);
    Print(m_hXMLOutput, "\t\t<point_label direction=\"" + sdir + "\"><point>" +
                            o.name + "</point></point_label>\n");
    m_CurrentXMLgroup = eXMLlabel;
  }
  //-------- End of support for XML form ---------

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::mark(int dir) {
  GCLC_object o;
  string pos;
  GCLCError iRv;
  double x, y;

  if ((iRv = ReadObject(GCLC_POINT, o)) != rvGCLCOK)
    return iRv;
  if (PrintComment("Marking point " + o.name) != rvG_OK)
    return rvCannotExport;

  switch (dir) {
  case 0:
    x = o.p[0];
    y = o.p[1];
    pos = "[cc]";
    break;
  case GCLC_DIR_LT:
    x = o.p[0] - DM;
    y = o.p[1] + DM;
    pos = "[rb]";
    break;
  case GCLC_DIR_L:
    x = o.p[0] - DM;
    y = o.p[1];
    pos = "[r]";
    break;
  case GCLC_DIR_LB:
    x = o.p[0] - DM;
    y = o.p[1] - DM;
    pos = "[rt]";
    break;
  case GCLC_DIR_T:
    x = o.p[0];
    y = o.p[1] + DM;
    pos = "[b]";
    break;
  case GCLC_DIR_B:
    x = o.p[0];
    y = o.p[1] - DM;
    pos = "[t]";
    break;
  case GCLC_DIR_RT:
    x = o.p[0] + DM;
    y = o.p[1] + DM;
    pos = "[lb]";
    break;
  case GCLC_DIR_R:
    x = o.p[0] + DM;
    y = o.p[1];
    pos = "[l]";
    break;
  case GCLC_DIR_RB:
    x = o.p[0] + DM;
    y = o.p[1] - DM;
    pos = "[lt]";
    break;
  default:
    x = o.p[0];
    y = o.p[1];
    pos = "";
    break;
  }

  //-------- Support for input exported to XML form ---------
  if (m_bXMLOutput && m_hXMLOutput.good()) {
    string sdir;
    switch (dir) {
    case 0:
      sdir = "0\" distance=\"0";
      break;
    case GCLC_DIR_LT:
      sdir = "135";
      break;
    case GCLC_DIR_L:
      sdir = "180";
      break;
    case GCLC_DIR_LB:
      sdir = "225";
      break;
    case GCLC_DIR_T:
      sdir = "90";
      break;
    case GCLC_DIR_B:
      sdir = "270";
      break;
    case GCLC_DIR_RT:
      sdir = "45";
      break;
    case GCLC_DIR_R:
      sdir = "0";
      break;
    case GCLC_DIR_RB:
      sdir = "315";
      break;
    default:
      sdir = "";
      break;
    }
    ChangeCurrentXMLGroup(eXMLlabel);
    Print(m_hXMLOutput, "\t\t<point_label type=\"none\" direction=\"" + sdir +
                            "\"><point>" + o.name + "</point></point_label>\n");
    m_CurrentXMLgroup = eXMLlabel;
  }
  //-------- End of support for XML form ---------

  if (PrintText(x, y, pos, o.name, m_Basic_Area) != rvG_OK)
    return rvCannotExport;
  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::printat(int dir) {
  GCLC_object o;
  string pos;
  string text;
  GCLCError iRv;
  double x, y;

  if ((iRv = ReadObject(GCLC_POINT, o)) != rvGCLCOK)
    return iRv;
  if ((iRv = take_text(text)) != rvGCLCOK)
    return iRv;
  if (PrintComment("Printing text at point " + o.name) != rvG_OK)
    return rvCannotExport;

  double DMD = 0.7 * DM; // diagonal distance

  switch (dir) {
  case 0:
    x = o.p[0];
    y = o.p[1];
    pos = "[cc]";
    break;
  case GCLC_DIR_LT:
    x = o.p[0] - DMD;
    y = o.p[1] + DMD;
    pos = "[rb]";
    break;
  case GCLC_DIR_L:
    x = o.p[0] - DM;
    y = o.p[1];
    pos = "[r]";
    break;
  case GCLC_DIR_LB:
    x = o.p[0] - DMD;
    y = o.p[1] - DMD;
    pos = "[rt]";
    break;
  case GCLC_DIR_T:
    x = o.p[0];
    y = o.p[1] + DM;
    pos = "[b]";
    break;
  case GCLC_DIR_B:
    x = o.p[0];
    y = o.p[1] - DM;
    pos = "[t]";
    break;
  case GCLC_DIR_RT:
    x = o.p[0] + DMD;
    y = o.p[1] + DMD;
    pos = "[lb]";
    break;
  case GCLC_DIR_R:
    x = o.p[0] + DM;
    y = o.p[1];
    pos = "[l]";
    break;
  case GCLC_DIR_RB:
    x = o.p[0] + DMD;
    y = o.p[1] - DMD;
    pos = "[lt]";
    break;
  default:
    x = o.p[0];
    y = o.p[1];
    pos = "";
    break;
  }

  //-------- Support for input exported to XML form ---------
  if (m_bXMLOutput && m_hXMLOutput.good()) {
    string sdir;
    switch (dir) {
    case 0:
      sdir = "0 distance=\"0\"";
      break;
    case GCLC_DIR_LT:
      sdir = "135";
      break;
    case GCLC_DIR_L:
      sdir = "180";
      break;
    case GCLC_DIR_LB:
      sdir = "225";
      break;
    case GCLC_DIR_T:
      sdir = "90";
      break;
    case GCLC_DIR_B:
      sdir = "270";
      break;
    case GCLC_DIR_RT:
      sdir = "45";
      break;
    case GCLC_DIR_R:
      sdir = "0";
      break;
    case GCLC_DIR_RB:
      sdir = "315";
      break;
    default:
      sdir = "";
      break;
    }
    ChangeCurrentXMLGroup(eXMLlabel);
    Print(m_hXMLOutput, "\t\t<text_label direction=\"" + sdir + "\"><point>" +
                            o.name + "</point><text>" + text +
                            "</text></text_label>\n");
    m_CurrentXMLgroup = eXMLlabel;
  }
  //-------- End of support for XML form ---------

  if (PrintText(x, y, pos, text, m_Basic_Area) != rvG_OK)
    return rvCannotExport;

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::printvalueat(int dir) {
  GCLC_object o, o1;
  string pos;
  string text;
  string sObjectName;
  GCLCError iRv;
  double x, y;

  if ((iRv = ReadObject(GCLC_POINT, o)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadToken(sObjectName)) != rvGCLCOK)
    return rvGCLCIdExpected;
  if ((iRv = Value(sObjectName, o1)) != rvGCLCOK)
    return iRv;
  if (PrintComment("Printing value of " + sObjectName + " at point " +
                   o.name) != rvG_OK)
    return rvCannotExport;

  switch (o1.type) {
  case GCLC_NUMBER:
    text = d2s(o1.p[0], 2);
    break;
  case GCLC_POINT:
    text = "(" + d2s(o1.p[0], 2) + "," + d2s(o1.p[1], 2) + ")";
    break;
  case GCLC_LINE:
    text = "(" + d2s(o1.p[0], 2) + "," + d2s(o1.p[1], 2) + "," +
           d2s(o1.p[2], 2) + ")";
    break;
  case GCLC_CIRCLE:
    text = "(" + d2s(o1.p[0], 2) + "," + d2s(o1.p[1], 2) + "," +
           d2s(o1.p[2], 2) + "," + d2s(o1.p[3], 2) + ")";
    break;

  case GCLC_CONIC:
    text = "(" + d2s(o1.p[0], 2) + "," + d2s(o1.p[1], 2) + "," +
           d2s(o1.p[2], 2) + "," + d2s(o1.p[3], 2) + "," + d2s(o1.p[4], 2) +
           "," + d2s(o1.p[5], 2) + ")";
    break;

  default:
    break;
  }

  double DMD = 0.7 * DM; // diagonal distance

  switch (dir) {
  case 0:
    x = o.p[0];
    y = o.p[1];
    pos = "[cc]";
    break;
  case GCLC_DIR_LT:
    x = o.p[0] - DMD;
    y = o.p[1] + DMD;
    pos = "[rb]";
    break;
  case GCLC_DIR_L:
    x = o.p[0] - DM;
    y = o.p[1];
    pos = "[r]";
    break;
  case GCLC_DIR_LB:
    x = o.p[0] - DMD;
    y = o.p[1] - DMD;
    pos = "[rt]";
    break;
  case GCLC_DIR_T:
    x = o.p[0];
    y = o.p[1] + DM;
    pos = "[b]";
    break;
  case GCLC_DIR_B:
    x = o.p[0];
    y = o.p[1] - DM;
    pos = "[t]";
    break;
  case GCLC_DIR_RT:
    x = o.p[0] + DMD;
    y = o.p[1] + DMD;
    pos = "[lb]";
    break;
  case GCLC_DIR_R:
    x = o.p[0] + DM;
    y = o.p[1];
    pos = "[l]";
    break;
  case GCLC_DIR_RB:
    x = o.p[0] + DMD;
    y = o.p[1] - DMD;
    pos = "[lt]";
    break;
  default:
    x = o.p[0];
    y = o.p[1];
    pos = "";
    break;
  }

  //-------- Support for input exported to XML form ---------
  if (m_bXMLOutput && m_hXMLOutput.good()) {
    string sdir;
    switch (dir) {
    case 0:
      sdir = "0 distance=\"0\"";
      break;
    case GCLC_DIR_LT:
      sdir = "135";
      break;
    case GCLC_DIR_L:
      sdir = "180";
      break;
    case GCLC_DIR_LB:
      sdir = "225";
      break;
    case GCLC_DIR_T:
      sdir = "90";
      break;
    case GCLC_DIR_B:
      sdir = "270";
      break;
    case GCLC_DIR_RT:
      sdir = "45";
      break;
    case GCLC_DIR_R:
      sdir = "0";
      break;
    case GCLC_DIR_RB:
      sdir = "315";
      break;
    default:
      sdir = "";
      break;
    }
    ChangeCurrentXMLGroup(eXMLlabel);
    Print(m_hXMLOutput, "\t\t<text_label direction=\"" + sdir + "\"><point>" +
                            o.name.c_str() + "</point><text>" + text +
                            "</text></text_label>\n");
    m_CurrentXMLgroup = eXMLlabel;
  }
  //-------- End of support for XML form ---------

  if (PrintText(x, y, pos, text, m_Basic_Area) != rvG_OK)
    return rvCannotExport;

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------
