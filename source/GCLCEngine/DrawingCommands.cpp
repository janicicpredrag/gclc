#include "../Input/StringInput.h"
#include "GCLC.h"
#include <cmath>

#include "../GraphDrawing/arclayereddrawing.h"
#include "../GraphDrawing/barycenterdrawing.h"

// ----------------------------------------------------------------------------

GCLCError CGCLC::draw_point() {
  GCLC_object o;
  GCLCError iRv;

  if ((iRv = ReadObject(GCLC_POINT, o)) != rvGCLCOK)
    return iRv;
  if (SetPixel(o.p[0], o.p[1], m_Basic_Area) != rvG_OK)
    return rvCannotExport;
  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::draw_segment(bool dashed) {
  GCLC_object o1, o2;
  GCLCError iRv;

  if ((iRv = ReadObject(GCLC_POINT, o1)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadObject(GCLC_POINT, o2)) != rvGCLCOK)
    return iRv;

  //-------- Support for input exported to XML form ---------
  if (m_bXMLOutput && m_hXMLOutput.good()) {
    if (dashed)
      ChangeCurrentXMLGroup(eXMLdrawdashed);
    else
      ChangeCurrentXMLGroup(eXMLdraw);
    Print(m_hXMLOutput, "\t\t<segment><point>" + o1.name + "</point><point>" +
                            o2.name + "</point></segment>\n");
    m_CurrentXMLgroup = dashed ? eXMLdrawdashed : eXMLdraw;
  }
  //-------- End of support for XML form ---------

  if (PrintComment("Drawing segment " + o1.name + " " + o2.name) != rvG_OK)
    return rvCannotExport;
  if (DrawSegment(o1.p[0], o1.p[1], o2.p[0], o2.p[1], true, dashed,
                  m_Basic_Area) != rvG_OK)
    return rvCannotExport;

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::draw_line(bool dashed) {
  GCLC_object o1, o2;
  GCLCError iRv;

  if ((iRv = ReadNextObject(o1)) != rvGCLCOK)
    return iRv;
  if (o1.type == GCLC_POINT) {
    if ((iRv = ReadObject(GCLC_POINT, o2)) != rvGCLCOK)
      return iRv;

    //-------- Support for input exported to XML form ---------
    if (m_bXMLOutput && m_hXMLOutput.good()) {
      if (dashed)
        ChangeCurrentXMLGroup(eXMLdrawdashed);
      else
        ChangeCurrentXMLGroup(eXMLdraw);
      Print(m_hXMLOutput, "\t\t<line_pp><point>" + o1.name + "</point><point>" +
                              o2.name + "</point></line_pp>\n");
      m_CurrentXMLgroup = dashed ? eXMLdrawdashed : eXMLdraw;
    }
    //-------- End of support for XML form ---------
  } else {
    if (o1.type == GCLC_LINE) {
      double a, b, c;
      a = o1.p[0];
      b = o1.p[1];
      c = o1.p[2];

      if (a == 0) {
        o1.p[0] = 0;
        o1.p[1] = (-c / b);
        o2.p[0] = 1;
        o2.p[1] = (-c / b);
      } else {
        if (b == 0) {
          o1.p[0] = (-c / a);
          o1.p[1] = 0;
          o2.p[0] = (-c / a);
          o2.p[1] = 1;
        } else {
          o1.p[0] = 0;
          o1.p[1] = (-c / b);
          o2.p[0] = 1;
          o2.p[1] = (-c - a) / b;
        }
      }

      //-------- Support for input exported to XML form ---------
      if (m_bXMLOutput && m_hXMLOutput.good()) {
        if (dashed)
          ChangeCurrentXMLGroup(eXMLdrawdashed);
        else
          ChangeCurrentXMLGroup(eXMLdraw);
        Print(m_hXMLOutput, "\t\t<line>" + o1.name + "</line>\n");
        m_CurrentXMLgroup = dashed ? eXMLdrawdashed : eXMLdraw;
      }
      //-------- End of support for XML form ---------
    } else
      return rvGCLCWrongType;
  }

  if ((fabs(o1.p[0] - o2.p[0]) <= EPS) && (fabs(o1.p[1] - o2.p[1]) <= EPS))
    return rvGCLCBadLine;

  std::string sComment;
  if (o1.type == GCLC_POINT)
    sComment = "Drawing line " + o1.name + " " + o2.name;
  else
    sComment = "Drawing line " + o1.name;
  if (PrintComment(sComment) != rvG_OK)
    return rvCannotExport;
  if (DrawLineSensitive(o1.p[0], o1.p[1], o2.p[0], o2.p[1], dashed,
                        m_Basic_Area) != rvG_OK)
    return rvCannotExport;

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::draw_vector() {
  GCLC_object o1, o2;
  double x0, y0, x1, y1, d, c, s;
  GCLCError iRv;

  if ((iRv = ReadObject(GCLC_POINT, o1)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadObject(GCLC_POINT, o2)) != rvGCLCOK)
    return iRv;

  if (PrintComment("Drawing vector " + o1.name + " " + o2.name) != rvG_OK)
    return rvCannotExport;
  if (DrawSegment(o1.p[0], o1.p[1], o2.p[0], o2.p[1], true, false,
                  m_Basic_Area) != rvG_OK)
    return rvCannotExport;

  d = distance2d(o1, o2);
  x0 = (o1.p[0] - o2.p[0]) / d + o2.p[0];
  y0 = (o1.p[1] - o2.p[1]) / d + o2.p[1];

  c = cos(PI / 24);
  s = sin(PI / 24);
  x1 = (3 / d) * ((o1.p[0] - o2.p[0]) * c + (o1.p[1] - o2.p[1]) * s) + o2.p[0];
  y1 = (3 / d) * (-(o1.p[0] - o2.p[0]) * s + (o1.p[1] - o2.p[1]) * c) + o2.p[1];

  if (DrawSegment(x1, y1, o2.p[0], o2.p[1], true, false, m_Basic_Area) !=
      rvG_OK)
    return rvCannotExport;
  if (DrawSegment(x1, y1, x0, y0, true, false, m_Basic_Area) != rvG_OK)
    return rvCannotExport;

  s = -s;
  x1 = (3 / d) * ((o1.p[0] - o2.p[0]) * c + (o1.p[1] - o2.p[1]) * s) + o2.p[0];
  y1 = (3 / d) * (-(o1.p[0] - o2.p[0]) * s + (o1.p[1] - o2.p[1]) * c) + o2.p[1];

  if (DrawSegment(x1, y1, o2.p[0], o2.p[1], true, false, m_Basic_Area) !=
      rvG_OK)
    return rvCannotExport;
  if (DrawSegment(x1, y1, x0, y0, true, false, m_Basic_Area) != rvG_OK)
    return rvCannotExport;

  //-------- Support for input exported to XML form ---------
  if (m_bXMLOutput && m_hXMLOutput.good()) {
    ChangeCurrentXMLGroup(eXMLdraw);
    Print(m_hXMLOutput, "\t\t<vector><point>" + o1.name + "</point><point>" +
                            o2.name + "</point></vector>\n");
    m_CurrentXMLgroup = eXMLdraw;
  }
  //-------- End of support for XML form ---------

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::draw_arrow() {
  GCLC_object o1, o2;
  double x0, y0, x1, y1, x2, y2, d, c, s, q;
  GCLCError iRv;

  if ((iRv = ReadObject(GCLC_POINT, o1)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadObject(GCLC_POINT, o2)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(q)) != rvGCLCOK)
    return iRv;

  o2.p[0] = (o2.p[0] - o1.p[0]) * q + o1.p[0];
  o2.p[1] = (o2.p[1] - o1.p[1]) * q + o1.p[1];
  d = distance2d(o1, o2);

  if (d < EPS)
    return rvGCLCOK;

  c = cos(ARROW_ANGLE * PI / 360);
  s = sin(ARROW_ANGLE * PI / 360);

  c = cos(ARROW_ANGLE * PI / 360);
  s = sin(ARROW_ANGLE * PI / 360);
  x1 =
      (ARROW_LENGTH / d) * ((o1.p[0] - o2.p[0]) * c + (o1.p[1] - o2.p[1]) * s) +
      o2.p[0];
  y1 = (ARROW_LENGTH / d) *
           (-(o1.p[0] - o2.p[0]) * s + (o1.p[1] - o2.p[1]) * c) +
       o2.p[1];

  s = -s;
  x2 =
      (ARROW_LENGTH / d) * ((o1.p[0] - o2.p[0]) * c + (o1.p[1] - o2.p[1]) * s) +
      o2.p[0];
  y2 = (ARROW_LENGTH / d) *
           (-(o1.p[0] - o2.p[0]) * s + (o1.p[1] - o2.p[1]) * c) +
       o2.p[1];

  x0 = (x1 + x2) / 2.0 + (o2.p[0] - (x1 + x2) / 2.0) * ARROW_INNER_TOWARDS;
  y0 = (y1 + y2) / 2.0 + (o2.p[1] - (y1 + y2) / 2.0) * ARROW_INNER_TOWARDS;

  if (PrintComment("Drawing arrow " + o1.name + " " + o2.name + " " +
                   d2s(q, 2)) != rvG_OK)
    return rvCannotExport;

  if (DrawSegment(x1, y1, o2.p[0], o2.p[1], true, false, m_Basic_Area) !=
      rvG_OK)
    return rvCannotExport;
  if (DrawSegment(x1, y1, x0, y0, true, false, m_Basic_Area) != rvG_OK)
    return rvCannotExport;
  if (DrawSegment(x2, y2, o2.p[0], o2.p[1], true, false, m_Basic_Area) !=
      rvG_OK)
    return rvCannotExport;
  if (DrawSegment(x2, y2, x0, y0, true, false, m_Basic_Area) != rvG_OK)
    return rvCannotExport;

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::draw_circle(bool dashed) {
  GCLC_object o1, o2;
  GCLCError iRv;

  if ((iRv = ReadNextObject(o1)) != rvGCLCOK)
    return iRv;

  if (o1.type == GCLC_POINT) {
    if ((iRv = ReadObject(GCLC_POINT, o2)) != rvGCLCOK)
      return iRv;

    //-------- Support for input exported to XML form ---------
    if (m_bXMLOutput && m_hXMLOutput.good()) {
      if (!dashed) {
        ChangeCurrentXMLGroup(eXMLdraw);
        Print(m_hXMLOutput, "\t\t<circle_cp><center>" + o1.name +
                                "</center><point>" + o2.name +
                                "</point></circle_cp>\n");
        m_CurrentXMLgroup = eXMLdraw;
      } else {
        ChangeCurrentXMLGroup(eXMLdrawdashed);
        Print(m_hXMLOutput, "\t\t<circle_cp><center>" + o1.name +
                                "</center><point>" + o2.name +
                                "</point></circle_cp>\n");
        m_CurrentXMLgroup = eXMLdrawdashed;
      }
    }
    //-------- End of support for XML form ---------
  } else {
    if (o1.type == GCLC_CIRCLE) {
      o2.p[0] = o1.p[0] + o1.p[2];
      o2.p[1] = o1.p[1];
    } else
      return rvGCLCWrongType;

    //-------- Support for input exported to XML form ---------
    if (m_bXMLOutput && m_hXMLOutput.good()) {
      if (!dashed) {
        ChangeCurrentXMLGroup(eXMLdraw);
        Print(m_hXMLOutput, "\t\t<circle>" + o1.name + "</circle>\n");
        m_CurrentXMLgroup = eXMLdraw;
      } else {
        ChangeCurrentXMLGroup(eXMLdrawdashed);
        Print(m_hXMLOutput, "\t\t<circle>" + o1.name + "</circle>\n");
        m_CurrentXMLgroup = eXMLdrawdashed;
      }
    }
    //-------- End of support for XML form ---------
  }

  std::string sComment;
  if (o1.type == GCLC_POINT)
    sComment = "Drawing circle " + o1.name + " " + o2.name;
  else
    sComment = "Drawing circle " + o1.name;
  if (PrintComment(sComment) != rvG_OK)
    return rvCannotExport;

  // 17.07.2006.
  // Radical changes made concerning drawing of circle and arcs
  // All these is now moved to CListOfPrimitives and is handled there.
  if (DrawCircle(o1.p[0], o1.p[1], o2.p[0], o2.p[1], true, dashed, m_Basic_Area,
                 CIRCLE_PRECISION) != rvG_OK) // 15.07.2006
    return rvCannotExport;

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::draw_arc(bool dashed, bool positive) {
  GCLC_object o1, o2;
  GCLCError iRv;
  double angle;

  iRv = ReadObject(GCLC_POINT, o1);
  if (iRv != rvGCLCOK)
    return iRv;
  iRv = ReadObject(GCLC_POINT, o2);
  if (iRv != rvGCLCOK)
    return iRv;
  iRv = ReadNumber(angle);
  if (iRv != rvGCLCOK)
    return iRv;

  //-------- Support for input exported to XML form ---------
  if (m_bXMLOutput && m_hXMLOutput.good()) {
    ChangeCurrentXMLGroup(eXMLdraw);
    Print(m_hXMLOutput, "\t\t<arc><center>" + o1.name + "</center><point>" +
                            o2.name + "</point><angle>" + d2s(angle, -1) +
                            "</angle></arc>\n");
    m_CurrentXMLgroup = eXMLdraw;
  }
  //-------- End of support for XML form ---------

  if (positive) {
    while (angle < 0)
      angle = 360 + angle;
    while (angle > 360)
      angle = angle - 360;
  }

  if (PrintComment("Drawing arc " + o1.name + " " + o2.name + " " +
                   d2s(angle, 2)) != rvG_OK)
    return rvCannotExport;

  if (angle >= 0) {
    if (DrawArcSensitive(o1.p[0], o1.p[1], o2.p[0], o2.p[1], angle, dashed,
                         m_Basic_Area,
                         CIRCLE_PRECISION) != rvG_OK) // 15.07.2006
      return rvCannotExport;
  } else {
    double sinn = sin(PI * angle / 180);
    double cosn = cos(PI * angle / 180);

    double x =
        (o2.p[0] - o1.p[0]) * cosn - (o2.p[1] - o1.p[1]) * sinn + o1.p[0];
    double y =
        (o2.p[1] - o1.p[1]) * cosn + (o2.p[0] - o1.p[0]) * sinn + o1.p[1];

    if (DrawArcSensitive(o1.p[0], o1.p[1], x, y, -angle, dashed, m_Basic_Area,
                         CIRCLE_PRECISION) != rvG_OK) // 15.07.2006
      return rvCannotExport;
  }

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::draw_ellipse(bool dashed) {
  GCLC_object o1, o2, o3;
  int num, i;
  GCLCError iRv;
  double x1, x2, x3, y1, y2, y3, sinn, cosn, a, b, sina, cosa, pom;

  iRv = ReadObject(GCLC_POINT, o1);
  if (iRv != rvGCLCOK)
    return iRv;
  iRv = ReadObject(GCLC_POINT, o2);
  if (iRv != rvGCLCOK)
    return iRv;
  iRv = ReadObject(GCLC_POINT, o3);
  if (iRv != rvGCLCOK)
    return iRv;

  x1 = o1.p[0];
  y1 = o1.p[1];
  x2 = o2.p[0];
  y2 = o2.p[1];
  x3 = o3.p[0];
  y3 = o3.p[1];
  a = distance2d(o1, o2);

  num = (int)((CIRCLE_PRECISION * a) / 10);

  sinn = sin(2 * PI / num);
  cosn = cos(2 * PI / num);

  if (a < EPS)
    return rvGCLCBadEllipse;

  sina = (y2 - y1) / a;
  cosa = (x2 - x1) / a;

  pom = x1 + (x3 - x1) * cosa + (y3 - y1) * sina;
  y3 = y1 + (y3 - y1) * cosa - (x3 - x1) * sina;
  x3 = pom;

  if (fabs(a * a - (x3 - x1) * (x3 - x1)) < EPS)
    return rvGCLCBadEllipse;
  if ((y3 - y1) * (y3 - y1) * a * a / (a * a - (x3 - x1) * (x3 - x1)) < 0)
    return rvGCLCBadEllipse;
  b = sqrt((y3 - y1) * (y3 - y1) * a * a / (a * a - (x3 - x1) * (x3 - x1)));

  if (PrintComment("Drawing ellipse " + o1.name + " " + o2.name + " " +
                   o3.name) != rvG_OK)
    return rvCannotExport;

  for (i = 1; i <= num; i++) {
    pom = x1 + (x2 - x1) * cosa + (y2 - y1) * sina;
    y2 = y1 + (y2 - y1) * cosa - (x2 - x1) * sina;
    x2 = pom;

    pom = x1 + (x2 - x1) * cosn - (y2 - y1) * sinn * a / b;
    y3 = y1 + (y2 - y1) * cosn + (x2 - x1) * sinn * b / a;
    x3 = pom;

    pom = x1 + (x2 - x1) * cosa - (y2 - y1) * sina;
    y2 = y1 + (y2 - y1) * cosa + (x2 - x1) * sina;
    x2 = pom;

    pom = x1 + (x3 - x1) * cosa - (y3 - y1) * sina;
    y3 = y1 + (y3 - y1) * cosa + (x3 - x1) * sina;
    x3 = pom;

    if (!dashed || i % 3)
      DrawSegment(x2, y2, x3, y3, true, false, m_Basic_Area);
    x2 = x3;
    y2 = y3;
  }
  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::draw_ellipsearc(bool dashed, bool bFourGivenPoints,
                                 bool bByAngles) {
  GCLC_object o1, o2, o3, o4;
  int i, num;
  GCLCError iRv;
  double x1, x2, x3, xa, xb, y1, y2, y3, ya, yb, sinn, cosn, angle0, angle, a,
      b, pom, sina, cosa;
  double x, y, x_start, y_start, x_end, y_end;

  if ((iRv = ReadObject(GCLC_POINT, o1)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadObject(GCLC_POINT, o2)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadObject(GCLC_POINT, o3)) != rvGCLCOK)
    return iRv;

  if (bFourGivenPoints) {
    if ((iRv = ReadObject(GCLC_POINT, o4)) != rvGCLCOK)
      return iRv;
  } else if (bByAngles) {
    if ((iRv = ReadNumber(angle0)) != rvGCLCOK)
      return iRv;
  }

  if ((iRv = ReadNumber(angle)) != rvGCLCOK)
    return iRv;

  x1 = o1.p[0];
  y1 = o1.p[1];
  x2 = o2.p[0];
  y2 = o2.p[1];
  x3 = o3.p[0];
  y3 = o3.p[1];
  a = distance2d(o1, o2);

  if (a < EPS)
    return rvGCLCBadEllipse;

  sina = (y2 - y1) / a;
  cosa = (x2 - x1) / a;

  pom = x1 + (x3 - x1) * cosa + (y3 - y1) * sina;
  y3 = y1 + (y3 - y1) * cosa - (x3 - x1) * sina;
  x3 = pom;

  if (fabs(a * a - (x3 - x1) * (x3 - x1)) < EPS)
    return rvGCLCBadEllipse;
  if ((y3 - y1) * (y3 - y1) * a * a / (a * a - (x3 - x1) * (x3 - x1)) < 0)
    return rvGCLCBadEllipse;
  b = sqrt((y3 - y1) * (y3 - y1) * a * a / (a * a - (x3 - x1) * (x3 - x1)));

  std::string sComment;
  if (bFourGivenPoints)
    sComment = "Drawing ellipse arc " + o1.name + " " + o2.name + " " +
               o3.name + " " + o4.name + " " + d2s(angle, 2);
  else {
    if (bByAngles)
      sComment = "Drawing ellipse arc " + o1.name + " " + o2.name + " " +
                 o3.name + " " + d2s(angle0, 2) + " " + d2s(angle, 2);
    else
      sComment = "Drawing ellipse arc " + o1.name + " " + o2.name + " " +
                 o3.name + " " + d2s(angle, 2);
  }
  if (PrintComment(sComment) != rvG_OK)
    return rvCannotExport;

  double angle1;
  if (bFourGivenPoints) // this argument added 30.07.2007
  {
    x_start = o4.p[0];
    y_start = o4.p[1];
    angle0 = 180 * atan2(y2 - y1, x2 - x1) / PI;
    angle0 = 180 * atan2(y_start - y1, x_start - x1) / PI - angle0;
    angle1 = transform_ellipse_angle(a, b, angle0 + angle);
    angle0 = transform_ellipse_angle(a, b, angle0);
  } else if (bByAngles) // this argument added 12.08.2007
  {
    angle1 = transform_ellipse_angle(a, b, angle0 + angle);
    angle0 = transform_ellipse_angle(a, b, angle0);

    xa = x1 + a * cos(PI * angle0 / 180);
    ya = y1 + b * sin(PI * angle0 / 180);
    x_start = x1 + (xa - x1) * cosa - (ya - y1) * sina;
    y_start = y1 + (ya - y1) * cosa + (xa - x1) * sina;
  } else {
    x_start = o2.p[0];
    y_start = o2.p[1];
    angle0 = 0;
    angle1 = transform_ellipse_angle(a, b, angle0 + angle);
  }

  xa = x1 + a * cos(PI * angle1 / 180);
  ya = y1 + b * sin(PI * angle1 / 180);
  x_end = x1 + (xa - x1) * cosa - (ya - y1) * sina;
  y_end = y1 + (ya - y1) * cosa + (xa - x1) * sina;

  if (angle1 < angle0)
    angle0 -= 360;
  if (angle1 - angle0 > 360)
    angle0 += 360;

  num = (int)((CIRCLE_PRECISION * a) / 10);

  sinn = sin(2 * PI / num);
  cosn = cos(2 * PI / num);

  xb = x_start;
  yb = y_start;

  // now using angle as a parameter that is compatible with filling functions
  // 11.08.2007
  for (i = 1; ((360.00 * i) / num) < (angle1 - angle0) + EPS; i++) {
    xa = x1 + (xb - x1) * cosa + (yb - y1) * sina;
    ya = y1 + (yb - y1) * cosa - (xb - x1) * sina;

    x = x1 + (xa - x1) * cosn - (ya - y1) * sinn * a / b;
    y = y1 + (ya - y1) * cosn + (xa - x1) * sinn * b / a;

    xa = x1 + (x - x1) * cosa - (y - y1) * sina;
    ya = y1 + (y - y1) * cosa + (x - x1) * sina;

    if (!dashed || i % 3)
      DrawSegment(xa, ya, xb, yb, true, false, m_Basic_Area);

    xb = xa;
    yb = ya;
  }

  // draw the last arc fragment 11.08.2007
  DrawSegment(xb, yb, x_end, y_end, true, false, m_Basic_Area);

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::draw_bezier3(bool dashed) {
  GCLC_object o1, o2, o3;
  double x1, x2, x3, y1, y2, y3, x, y, xx, yy, t;
  GCLCError iRv;
  int i;

  if ((iRv = ReadObject(GCLC_POINT, o1)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadObject(GCLC_POINT, o2)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadObject(GCLC_POINT, o3)) != rvGCLCOK)
    return iRv;

  x1 = o1.p[0];
  y1 = o1.p[1];
  x2 = o2.p[0];
  y2 = o2.p[1];
  x3 = o3.p[0];
  y3 = o3.p[1];

  if (PrintComment("Drawing Bezier curve " + o1.name + " " + o2.name + " " +
                   o3.name) != rvG_OK)
    return rvCannotExport;

  for (i = 0; i <= BEZIER_PRECISION; i++) {
    t = i / (double)BEZIER_PRECISION;

    x = (1 - t) * (1 - t) * x1 + 2 * t * (1 - t) * x2 + t * t * x3;
    y = (1 - t) * (1 - t) * y1 + 2 * t * (1 - t) * y2 + t * t * y3;

    if (i != 0)
      if (!dashed || (i % 3))
        if (DrawSegment(x, y, xx, yy, true, false, m_Basic_Area) != rvG_OK)
          return rvCannotExport;
    xx = x;
    yy = y;
  }
  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::draw_bezier4(bool dashed) {
  GCLC_object o1, o2, o3, o4;
  double x1, x2, x3, y1, y2, y3, x4, y4, x, y, xx, yy, t;
  GCLCError iRv;
  int i;

  if ((iRv = ReadObject(GCLC_POINT, o1)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadObject(GCLC_POINT, o2)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadObject(GCLC_POINT, o3)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadObject(GCLC_POINT, o4)) != rvGCLCOK)
    return iRv;

  x1 = o1.p[0];
  y1 = o1.p[1];
  x2 = o2.p[0];
  y2 = o2.p[1];
  x3 = o3.p[0];
  y3 = o3.p[1];
  x4 = o4.p[0];
  y4 = o4.p[1];

  if (PrintComment("Drawing Bezier curve " + o1.name + " " + o2.name + " " +
                   o3.name + " " + o4.name) != rvG_OK)
    return rvCannotExport;

  for (i = 0; i <= BEZIER_PRECISION; i++) {
    t = i / (double)BEZIER_PRECISION;

    x = (1 - t) * (1 - t) * (1 - t) * x1 + 3 * t * (1 - t) * (1 - t) * x2 +
        3 * t * t * (1 - t) * x3 + t * t * t * x4;
    y = (1 - t) * (1 - t) * (1 - t) * y1 + 3 * t * (1 - t) * (1 - t) * y2 +
        3 * t * t * (1 - t) * y3 + t * t * t * y4;

    if (i != 0)
      if (!dashed || (i % 3))
        if (DrawSegment(x, y, xx, yy, true, false, m_Basic_Area) != rvG_OK)
          return rvCannotExport;
    xx = x;
    yy = y;
  }
  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::draw_polygon() {
  GCLC_object o1, o2;
  double x1, x2, x3, y1, y2, y3, sinn, cosn, n;
  GCLCError iRv;
  int i, num;

  if ((iRv = ReadObject(GCLC_POINT, o1)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadObject(GCLC_POINT, o2)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(n)) != rvGCLCOK)
    return iRv;

  num = (int)n;
  sinn = sin(2 * PI / num);
  cosn = cos(2 * PI / num);

  x1 = o1.p[0];
  y1 = o1.p[1];
  x2 = o2.p[0];
  y2 = o2.p[1];

  if (PrintComment("Drawing polygon " + o1.name + " " + o2.name + " " +
                   d2s(n, 2)) != rvG_OK)
    return rvCannotExport;
  for (i = 1; i <= num; i++) {
    x3 = x1 + (x2 - x1) * cosn - (y2 - y1) * sinn;
    y3 = y1 + (y2 - y1) * cosn + (x2 - x1) * sinn;
    if (DrawSegment(x2, y2, x3, y3, true, false, m_Basic_Area) != rvG_OK)
      return rvCannotExport;
    x2 = x3;
    y2 = y3;
  }
  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::draw_tree() {
  GCLCError iRv;
  std::string tree;
  int i, iMaxHeight, iMaxNodesAtLevel[1024];
  double dFigureWidth, dFigureHeight, dRotationAngle, number;
  GCLC_object o;
  int iStyle;

  if ((iRv = ReadObject(GCLC_POINT, o)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(dFigureWidth)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(dFigureHeight)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(number)) != rvGCLCOK)
    return iRv;

  iStyle = (int)number;
  if (iStyle < 1)
    iStyle = 1;
  if (iStyle > 4)
    iStyle = 4;

  if ((iRv = ReadNumber(dRotationAngle)) != rvGCLCOK)
    return iRv;
  if ((iRv = take_text(tree)) != rvGCLCOK)
    return iRv;

  for (i = 0; i < 1024; i++)
    iMaxNodesAtLevel[i] = 0;

  iMaxHeight = 0;
  iMaxNodesAtLevel[0] = 1;
  if (check_tree(tree, 0, iMaxNodesAtLevel, iMaxHeight))
    return rvGCLCLIllFormedTree;

  if (PrintComment("Drawing tree " + o.name) != rvG_OK)
    return rvCannotExport;

  if (draw_all_nodes(dFigureWidth, dFigureHeight / iMaxHeight, tree,
                     iMaxNodesAtLevel, 0, 1, 0, 0, 0, false, o.p[0], o.p[1],
                     iStyle, dRotationAngle, o.name))
    return rvGCLCLIllFormedTree;

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

int CGCLC::check_tree(const std::string &sTree, int iLevel, int iMaxNodesAtLevel[],
                      int &iMaxHeight) {
  std::string sSubTree;
  unsigned subTreeStart;
  int iBrackets = 0;
  unsigned i = 0;

  if (iLevel > iMaxHeight)
    iMaxHeight = iLevel;

  while (((sTree[i] == ' ') || (sTree[i] == '\n') || (sTree[i] == '\t') ||
          (sTree[i] == '\r')) &&
         (i < sTree.size()))
    i++;

  // read the name of the root
  while ((sTree[i] != '{') && (sTree[i] != '}') && (sTree[i] != ' ') &&
         (sTree[i] != '\n') && (sTree[i] != '\t') && (sTree[i] != '\r') &&
         (i < sTree.size()))
    i++;

  if (i >= sTree.size())
    return 0; // empty tree

  bool bSubTreeProcessed;

  int iDesc = 0;
  for (;;) {
    bSubTreeProcessed = false;

    // search for the first subtree
    while (((sTree[i] == ' ') || (sTree[i] == '\n') || (sTree[i] == '\t') ||
            (sTree[i] == '\r')) &&
           (i < sTree.size()))
      i++;

    if (i >= sTree.size())
      return 0; // no more subtrees

    // search for a subtree
    if (sTree[i] == '{') {
      iBrackets++;
      i++;
      subTreeStart = i;
    } else
      return -1;

    while ((i < sTree.size()) && !bSubTreeProcessed) {
      if (sTree[i] == '{')
        iBrackets++;

      if (sTree[i] == '}')
        iBrackets--;

      if (iBrackets == 0) {
        sSubTree = sTree.substr(subTreeStart, i - subTreeStart);
        // if (check_tree(sSubTree, (sTree - sSubTree) + i, iLevel +
        // 1,iMaxNodesAtLevel, iMaxHeight))
        if (check_tree(sSubTree, iLevel + 1, iMaxNodesAtLevel, iMaxHeight))
          return -1;
        else {
          bSubTreeProcessed = true;
          iDesc++;
          if (iDesc > iMaxNodesAtLevel[iLevel + 1])
            iMaxNodesAtLevel[iLevel + 1] = iDesc;
        }
      }
      i++;
    }
  }
  return 0;
}

// ----------------------------------------------------------------------------

int CGCLC::draw_all_nodes(double dFigureWidth, double dLevelHeight,
                          const std::string &sTree, int iMaxNodesAtLevel[],
                          int iLevel, int iNumberOfSiblings, int iCurrentNode,
                          double dParentX, double dParentY, bool bParentNoName,
                          double dRootX, double dRootY, int iStyle,
                          double dRotationAngle, const std::string &sRefPointName) {
  std::string sSubTree;
  unsigned subTreeStart;

  int iBrackets, j;
  unsigned i = 0;
  bool bNoName;

  while (((sTree[i] == ' ') || (sTree[i] == '\n') || (sTree[i] == '\t') ||
          (sTree[i] == '\r')) &&
         (i < sTree.size()))
    i++;

  unsigned sStart = i;

  // read the name of the root
  while ((sTree[i] != '{') && (sTree[i] != '}') && (sTree[i] != ' ') &&
         (sTree[i] != '\n') && (sTree[i] != '\t') && (sTree[i] != '\r') &&
         (i < sTree.size()))
    i++;

  std::string sName;
  sName = sTree.substr(sStart, i - sStart);
  bNoName = (sName == "_");

  int iTotalWidth = 1;
  for (j = 0; j < iLevel; j++)
    iTotalWidth *= iMaxNodesAtLevel[j];
  double dNodeWidth =
      (dFigureWidth / (double)iTotalWidth) / (double)iNumberOfSiblings;

  double x0, y0, x, y, cosr, sinr, x1, y1, x2, y2, k;

  if (iLevel == 0) {
    x0 = dRootX;
    y0 = dRootY;
  } else {
    x0 = dParentX + (iCurrentNode - iNumberOfSiblings / 2.0) * dNodeWidth +
         dNodeWidth / 2;
    y0 = dRootY - dLevelHeight * iLevel;
  }

  cosr = cos(PI * dRotationAngle / 180);
  sinr = sin(PI * dRotationAngle / 180);

  x = dRootX + (x0 - dRootX) * cosr - (y0 - dRootY) * sinr;
  y = dRootY + (x0 - dRootX) * sinr + (y0 - dRootY) * cosr;

  if (iLevel > 0 && iCurrentNode == 0 && (iStyle == 2 || iStyle == 4)) {
    double px1, py1, px2, py2;
    px1 = dRootX + (dParentX - dRootX) * cosr - (dParentY - dRootY) * sinr;
    py1 = dRootY + (dParentX - dRootX) * sinr + (dParentY - dRootY) * cosr;

    px2 = dRootX + (dParentX - dRootX) * cosr -
          (dParentY - dLevelHeight / 2.0 - dRootY) * sinr;
    py2 = dRootY + (dParentX - dRootX) * sinr +
          (dParentY - dLevelHeight / 2.0 - dRootY) * cosr;

    if (iStyle == 4 && !bParentNoName) {
      if (fabs(py2 - py1) > MM_PER_PT * m_FontSize) {
        k = MM_PER_PT * m_FontSize / (py2 - py1);
        k = fabs(0.6 * k);
      } else
        k = 0.6;

      px1 = px1 + (px2 - px1) * k;
      py1 = py1 + (py2 - py1) * k;
    }
    if (DrawSegment(px1, py1, px2, py2, true, false, m_Basic_Area) != rvG_OK)
      return -1;

    double d = ((iNumberOfSiblings - 1.0) * dFigureWidth) /
               ((double)iTotalWidth * iNumberOfSiblings * 2.0);
    px1 = dRootX + (dParentX - d - dRootX) * cosr -
          (dParentY - dLevelHeight / 2.0 - dRootY) * sinr;
    py1 = dRootY + (dParentX - d - dRootX) * sinr +
          (dParentY - dLevelHeight / 2.0 - dRootY) * cosr;

    px2 = dRootX + (dParentX + d - dRootX) * cosr -
          (dParentY - dLevelHeight / 2.0 - dRootY) * sinr;
    py2 = dRootY + (dParentX + d - dRootX) * sinr +
          (dParentY - dLevelHeight / 2.0 - dRootY) * cosr;
    if (DrawSegment(px1, py1, px2, py2, true, false, m_Basic_Area) != rvG_OK)
      return -1;
  }

  if (sName.size() == 0)
    return 0; // empty tree

  if (iStyle == 1 || iStyle == 2) {
    DrawCircle(x, y, x + MARK_RADIUS, y, false, false, m_Basic_Area,
               MARK_CIRCLE_PRECISION);
    EmptyCircle(x, y, x + MARK_RADIUS, y, m_Basic_Area);
  }

  double px = x, py = y;
  if (iLevel > 0) {
    if (iStyle == 1 || iStyle == 3) {
      px = dRootX + (dParentX - dRootX) * cosr - (dParentY - dRootY) * sinr;
      py = dRootY + (dParentX - dRootX) * sinr + (dParentY - dRootY) * cosr;
      if (iStyle == 3) {
        if (fabs(py - y) > MM_PER_PT * m_FontSize) {
          k = MM_PER_PT * m_FontSize / (py - y);
          k = fabs(0.6 * k);
        } else
          k = 0.6;

        if (bNoName) {
          x1 = x;
          y1 = y;
        } else {
          x1 = x + (px - x) * k;
          y1 = y + (py - y) * k;
        }

        if (bParentNoName) {
          x2 = px;
          y2 = py;
        } else {
          x2 = x + (px - x) * (1 - k);
          y2 = y + (py - y) * (1 - k);
        }
        if (DrawSegment(x1, y1, x2, y2, true, false, m_Basic_Area) != rvG_OK)
          return -1;
      } else {
        if (DrawSegment(x, y, px, py, true, false, m_Basic_Area) != rvG_OK)
          return -1;
      }
    }

    if (iStyle == 2 || iStyle == 4) {
      px = dRootX + (x0 - dRootX) * cosr -
           (y0 + dLevelHeight / 2.0 - dRootY) * sinr;
      py = dRootY + (x0 - dRootX) * sinr +
           (y0 + dLevelHeight / 2.0 - dRootY) * cosr;
      if (iStyle == 4) {
        if (fabs(py - y) > MM_PER_PT * m_FontSize) {
          k = MM_PER_PT * m_FontSize / (py - y);
          k = fabs(0.6 * k);
        } else
          k = 0.6;

        if (bNoName) {
          x1 = x;
          y1 = y;
        } else {
          x1 = px + (x - px) * (1 - k);
          y1 = py + (y - py) * (1 - k);
        }
        x2 = px;
        y2 = py;
        if (DrawSegment(x1, y1, x2, y2, true, false, m_Basic_Area) != rvG_OK)
          return -1;
      } else {
        if (DrawSegment(x, y, px, py, true, false, m_Basic_Area) != rvG_OK)
          return -1;
      }
    }
  }

  if (Let(sRefPointName + sName, GCLC_POINT, x, y, 0, 0.00, 0.00, 0.00) !=
      rvGCLCOK) // 03.07.2007.
    return -1;

  double DMD = 0.7 * DM; // diagonal distance
  std::string pos;

  if (iStyle == 1 || iStyle == 2) {
    if (x > px) {
      if (y > py) {
        x += DMD;
        y -= DMD;
        pos = "[lt]";
      } else {
        x += DMD;
        y += DMD;
        pos = "[lb]";
      }
    } else {
      if (y > py) {
        x -= DMD;
        y -= DMD;
        pos = "[rt]";
      } else {
        x -= DMD;
        y += DMD;
        pos = "[rb]";
      }
    }
  } else
    pos = "[cc]";

  // 03.07.2007.
  if (sName[0] != '_') {
    if (PrintText(x, y, pos, sName, m_Basic_Area) != rvG_OK)
      return -1;
  }

  if (i >= sTree.size())
    return 0; // no subtrees

  int i0 = i;
  iBrackets = 0;
  bool bSubTreeProcessed, bDone = false;
  // counting the number of subtrees
  for (j = 0; !bDone; j++) {
    bSubTreeProcessed = false;
    while (((sTree[i] == ' ') || (sTree[i] == '\n') || (sTree[i] == '\t') ||
            (sTree[i] == '\r')) &&
           (i < sTree.size()))
      i++;

    if (i < sTree.size()) {
      iBrackets++;
      i++;
      subTreeStart = i;

      while ((i < sTree.size()) && !bSubTreeProcessed) {
        if (sTree[i] == '{')
          iBrackets++;

        if (sTree[i] == '}')
          iBrackets--;

        if (iBrackets == 0)
          bSubTreeProcessed = true;
        i++;
      }
    } else
      bDone = true;
  }
  int iNumberOfSubtrees = j - 1;

  if (iNumberOfSubtrees > 0) {
    i = i0; // go back
    iBrackets = 0;
    for (j = 0;; j++) {
      bSubTreeProcessed = false;

      // search for the first subtree
      while (((sTree[i] == ' ') || (sTree[i] == '\n') || (sTree[i] == '\t') ||
              (sTree[i] == '\r')) &&
             (i < sTree.size()))
        i++;

      if (i >= sTree.size())
        return 0; // no more subtrees

      // search for a subtree
      if (sTree[i] == '{') {
        iBrackets++;
        i++;
        subTreeStart = i;
      } else
        return -1;

      while ((i < sTree.size()) && !bSubTreeProcessed) {
        if (sTree[i] == '{')
          iBrackets++;

        if (sTree[i] == '}')
          iBrackets--;

        if (iBrackets == 0) {
          sSubTree = sTree.substr(subTreeStart, i - subTreeStart);
          if (draw_all_nodes(dFigureWidth, dLevelHeight, sSubTree,
                             iMaxNodesAtLevel, iLevel + 1, iNumberOfSubtrees, j,
                             x0, y0, bNoName, dRootX, dRootY, iStyle,
                             dRotationAngle, sRefPointName))
            return -1;
          else
            bSubTreeProcessed = true;
        }
        i++;
      }
    }
  }

  return 0;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::draw_graph_a() {
  // 	Arc Layered Method
  std::string sToken1;
  std::string sToken2;
  std::string sNodes;
  std::string sEdges;
  GCLCError iRv, iRv2;
  int i, j;

  double dFigureWidth, dRotationAngle;
  GCLC_object o;

  if ((iRv = ReadObject(GCLC_POINT, o)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(dFigureWidth)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(dRotationAngle)) != rvGCLCOK)
    return iRv;
  if ((iRv = take_text(sNodes)) != rvGCLCOK)
    return iRv;
  if ((iRv = take_text(sEdges)) != rvGCLCOK)
    return iRv;

  Graph graph;

  // reading graph nodes
  std::map<std::string, int> Nodes;
  CStringInput input(sNodes);
  i = 1;
  while ((iRv = ReadToken(input, sToken1)) == rvGCLCOK) {
    graph.addNode(i);
    Nodes.insert(std::pair<std::string, int>(sToken1, i++));
  }

  if (iRv != rvGCLCEndOfInput)
    return rvGCLCLInvalidGraphRepresentation;

  // reading graph edges
  CStringInput inputE(sEdges);
  while ((iRv = ReadToken(inputE, sToken1)) == rvGCLCOK &&
         (iRv2 = ReadToken(inputE, sToken2)) == rvGCLCOK) {
    if ((i = Nodes[sToken1]) == 0 || (j = Nodes[sToken2]) == 0) {
      return rvGCLCLInvalidGraphRepresentation;
    } else {
      // adding verteces (since these are methods for drawing undirected
      // graphs), both direction edges have to be added (e.g.: 1->2 i 2->1)
      graph.addEdge(i, j);
      graph.addEdge(j, i);
    }
  }

  if (iRv != rvGCLCEndOfInput)
    return rvGCLCLInvalidGraphRepresentation;
  if (!graph.isConnected())
    return rvGCLCLInvalidGraphRepresentation;

  struct Point p;
  p.x = o.p[0];
  p.y = o.p[1];

  Settings settingsArclayered01(ARC_LAYERED, DOWN_UP, GCLC, 10, 10);
  ArcLayeredDrawing canvas(graph, settingsArclayered01, p, dFigureWidth);
  canvas.draw();

  std::map<int, struct Point> coordinates = canvas.getCoordinates();

  std::map<std::string, int>::iterator it;
  for (it = Nodes.begin(); it != Nodes.end(); it++) {
    double x = coordinates[it->second].x;
    double y = coordinates[it->second].y;

    rotate(o.p[0], o.p[1], x, y, dRotationAngle, x, y);

    DrawCircle(x, y, x + MARK_RADIUS, y, false, false, m_Basic_Area,
               MARK_CIRCLE_PRECISION);
    EmptyCircle(x, y, x + MARK_RADIUS, y, m_Basic_Area);

    if (it->first[0] != '_') {
      if (PrintText(x, y, "[rb]", it->first, m_Basic_Area) != rvG_OK)
        return rvGCLCOutOfMemory;
    }

    if (Let(o.name + it->first, GCLC_POINT, x, y, 0, 0.00, 0.00, 0.00) != rvGCLCOK)
      return rvGCLCOutOfMemory;

    if (PrintComment("Labelling node " + o.name + it->first) != rvG_OK)
      return rvCannotExport;
  }

  for (i = 1; i <= graph.getNodesNumber(); i++)
    for (j = i; j <= graph.getNodesNumber(); j++) {
      if (graph.containsEdge(i, j)) {
        if (canvas.isArc(i, j)) {
          double x = (coordinates[i].x + coordinates[j].x) / 2;
          double y = coordinates[i].y;
          double x1 = coordinates[i].x;
          double y1 = coordinates[i].y;
          double x2 = coordinates[j].x;
          double y2 = coordinates[j].y;

          if (coordinates[i].x > x) {
            rotate(o.p[0], o.p[1], x, y, dRotationAngle, x, y);
            rotate(o.p[0], o.p[1], x1, y1, dRotationAngle, x1, y1);
            rotate(o.p[0], o.p[1], x2, y2, dRotationAngle, x2, y2);

            double xx, yy;
            rotate(x, y, x2, y2, 90, xx, yy);

            if (DrawArcSensitive(xx, yy, x1, y1, 90, false, m_Basic_Area,
                                 CIRCLE_PRECISION) != rvG_OK)
              return rvCannotExport;
          } else {
            rotate(o.p[0], o.p[1], x, y, dRotationAngle, x, y);
            rotate(o.p[0], o.p[1], x1, y1, dRotationAngle, x1, y1);
            rotate(o.p[0], o.p[1], x2, y2, dRotationAngle, x2, y2);

            double xx, yy;
            rotate(x, y, x1, y1, 90, xx, yy);

            if (DrawArcSensitive(xx, yy, x2, y2, 90, false, m_Basic_Area,
                                 CIRCLE_PRECISION) != rvG_OK)
              return rvCannotExport;
          }
        } else {
          double x1 = coordinates[i].x;
          double y1 = coordinates[i].y;
          double x2 = coordinates[j].x;
          double y2 = coordinates[j].y;
          rotate(o.p[0], o.p[1], x1, y1, dRotationAngle, x1, y1);
          rotate(o.p[0], o.p[1], x2, y2, dRotationAngle, x2, y2);

          if (DrawSegment(x1, y1, x2, y2, true, false, m_Basic_Area) != rvG_OK)
            return rvCannotExport;
        }
      }
    }

  coordinates.clear();

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::draw_graph_b() {
  // 	Baricenter Method
  std::string sReferenceName;
  std::string sToken1;
  std::string sToken2;
  std::string sNodes;
  std::string sEdges;
  std::string sComment;
  GCLC_object o;
  GCLCError iRv, iRv2;
  int i, j;

  if ((iRv = ReadToken(sReferenceName)) != rvGCLCOK)
    return iRv;
  if ((iRv = take_text(sNodes)) != rvGCLCOK)
    return iRv;
  if ((iRv = take_text(sEdges)) != rvGCLCOK)
    return iRv;

  Graph graph;

  // reading graph nodes
  std::map<std::string, int> Nodes;
  std::map<int, struct Point> fixedVertices;
  CStringInput inputN(sNodes);
  i = 1;
  while ((iRv = ReadToken(inputN, sToken1)) == rvGCLCOK &&
         (iRv2 = ReadToken(inputN, sToken2)) == rvGCLCOK) {
    graph.addNode(i);
    Nodes.insert(std::pair<std::string, int>(sToken1, i));

    if (sToken2[0] != '_') {
      if (Value(sToken2, o) == rvGCLCOK && o.type == GCLC_POINT) {
        fixedVertices[i].x = o.p[0];
        fixedVertices[i].y = o.p[1];
      } else
        return rvGCLCLInvalidGraphRepresentation;
    }
    i++;
  }

  if (iRv != rvGCLCEndOfInput)
    return rvGCLCLInvalidGraphRepresentation;

  // reading graph edges
  CStringInput inputE(sEdges);
  while ((iRv = ReadToken(inputE, sToken1)) == rvGCLCOK &&
         (iRv2 = ReadToken(inputE, sToken2)) == rvGCLCOK) {
    if ((i = Nodes[sToken1]) == 0 || (j = Nodes[sToken2]) == 0) {
      return rvGCLCLInvalidGraphRepresentation;
    } else {
      // adding verteces (since these are methods for drawing undirected
      // graphs), both direction edges have to be added (e.g.: 1->2 i 2->1)
      graph.addEdge(i, j);
      graph.addEdge(j, i);
    }
  }
  if (iRv != rvGCLCEndOfInput)
    return rvGCLCLInvalidGraphRepresentation;

  if (!graph.isConnected())
    return rvGCLCLInvalidGraphRepresentation;

  Settings settingsBarycenter01(BARYCENTER, DEFAULT, GCLC, 1, 0);
  BarycenterDrawing canvas{graph, settingsBarycenter01, fixedVertices};
  canvas.draw();

  std::map<int, struct Point> coordinates = canvas.getCoordinates();

  std::map<std::string, int>::iterator it;
  for (it = Nodes.begin(); it != Nodes.end(); it++) {
    double x = coordinates[it->second].x;
    double y = coordinates[it->second].y;

    DrawCircle(x, y, x + MARK_RADIUS, y, false, false, m_Basic_Area,
               MARK_CIRCLE_PRECISION);
    EmptyCircle(x, y, x + MARK_RADIUS, y, m_Basic_Area);

    if (it->first[0] != '_') {
      if (PrintText(x, y, "[rb]", it->first, m_Basic_Area) != rvG_OK)
        return rvGCLCOutOfMemory;
    }

    std::string sPointName = sReferenceName + (it->first);
    if (Let(sPointName, GCLC_POINT, x, y, 0, 0.00, 0.00, 0.00) != rvGCLCOK)
      return rvGCLCOutOfMemory;
    if (PrintComment("Labelling node " + sPointName) != rvG_OK)
      return rvCannotExport;
  }

  for (i = 1; i <= graph.getNodesNumber(); i++)
    for (j = i + 1; j <= graph.getNodesNumber(); j++) {
      if (graph.containsEdge(i, j)) {
        if (canvas.isArc(i, j)) {
          double x = (coordinates[i].x + coordinates[j].x) / 2;
          double y = (coordinates[i].y + coordinates[j].y) / 2;
          double x1 = coordinates[i].x;
          double y1 = coordinates[i].y;
          double x2 = coordinates[j].x;
          double y2 = coordinates[j].y;

          double xx, yy;
          rotate(x, y, x2, y2, 90, xx, yy);

          if (DrawArcSensitive(xx, yy, x1, y1, 90, false, m_Basic_Area,
                               CIRCLE_PRECISION) != rvG_OK)
            return rvCannotExport;
        } else {
          double x1 = coordinates[i].x;
          double y1 = coordinates[i].y;
          double x2 = coordinates[j].x;
          double y2 = coordinates[j].y;

          if (DrawSegment(x1, y1, x2, y2, true, false, m_Basic_Area) != rvG_OK)
            return rvCannotExport;
        }
      }
    }

  coordinates.clear();

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::fill_triangle() {
  GCLC_object o1, o2, o3;
  GCLCError iRv;

  if ((iRv = ReadObject(GCLC_POINT, o1)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadObject(GCLC_POINT, o2)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadObject(GCLC_POINT, o3)) != rvGCLCOK)
    return iRv;

  if (PrintComment("Filling triangle " + o1.name + " " + o2.name + " " +
                   o3.name) != rvG_OK)
    return rvCannotExport;
  if (FillTriangle(o1.p[0], o1.p[1], o2.p[0], o2.p[1], o3.p[0], o3.p[1],
                   m_Basic_Area) != rvG_OK)
    return rvCannotExport;

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::fill_rectangle() {
  GCLC_object o1, o2;
  GCLCError iRv;

  if ((iRv = ReadObject(GCLC_POINT, o1)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadObject(GCLC_POINT, o2)) != rvGCLCOK)
    return iRv;

  if (PrintComment("Filling rectangle left-bottom: " + o1.name +
                   " right-top: " + o2.name) != rvG_OK)
    return rvCannotExport;
  if (FillRectangle(o1.p[0], o1.p[1], o2.p[0], o2.p[1], m_Basic_Area) != rvG_OK)
    return rvCannotExport;

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::fill_circle() {
  GCLC_object o1, o2;
  GCLCError iRv;

  if ((iRv = ReadNextObject(o1)) != rvGCLCOK)
    return iRv;
  if (o1.type == GCLC_POINT) {
    if ((iRv = ReadObject(GCLC_POINT, o2)) != rvGCLCOK)
      return iRv;
  } else {
    if (o1.type == GCLC_CIRCLE) {
      o2.p[0] = o1.p[0] + o1.p[2];
      o2.p[1] = o1.p[1];
    } else
      return rvGCLCWrongType;
  }

  std::string sComment;
  if (o1.type == GCLC_POINT)
    sComment = "Filling circle " + o1.name + " " + o2.name;
  else
    sComment = "Filling circle " + o1.name;
  if (PrintComment(sComment) != rvG_OK)
    return rvCannotExport;

  double r = distance2d(o1, o2);
  if (FillEllipseArc(o1.p[0], o1.p[1], r, r, 0, 360, m_Basic_Area, true) !=
      rvG_OK)
    return rvCannotExport;

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::fill_ellipse() {
  GCLC_object o1;
  double a, b;
  GCLCError iRv;

  if ((iRv = ReadObject(GCLC_POINT, o1)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(a)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(b)) != rvGCLCOK)
    return iRv;

  if (PrintComment("Filling ellipse " + o1.name + " " + d2s(a, 2) + " " +
                   d2s(b, 2)) != rvG_OK)
    return rvCannotExport;
  if (FillEllipseArc(o1.p[0], o1.p[1], a, b, 0, 360, m_Basic_Area, true) !=
      rvG_OK)
    return rvCannotExport;

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::fill_arc(bool bFillTriangle) {
  GCLC_object o1, o2;
  double n;
  GCLCError iRv;

  if ((iRv = ReadObject(GCLC_POINT, o1)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadObject(GCLC_POINT, o2)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(n)) != rvGCLCOK)
    return iRv;

  if (PrintComment("Filling circle arc " + o1.name + " " + o2.name + " " +
                   d2s(n, 2)) != rvG_OK)
    return rvCannotExport;

  double r = distance2d(o1, o2);

  double phi1 = atan2(o2.p[1] - o1.p[1], o2.p[0] - o1.p[0]);
  phi1 = phi1 * 180 / PI;

  if (FillEllipseArc(o1.p[0], o1.p[1], r, r, phi1, phi1 + n, m_Basic_Area,
                     bFillTriangle) != rvG_OK)
    return rvCannotExport;

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::fill_ellipsearc(bool bFillTriangle) {
  GCLC_object o1;
  double a, b, phi1, phi2;
  GCLCError iRv;

  if ((iRv = ReadObject(GCLC_POINT, o1)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(a)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(b)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(phi1)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(phi2)) != rvGCLCOK)
    return iRv;

  if (PrintComment("Filling elliptical arc " + o1.name + " " + d2s(a, 2) + " " +
                   d2s(b, 2) + " " + d2s(phi1, 2) + " " + d2s(phi2, 2)) !=
      rvG_OK)
    return rvCannotExport;
  if (FillEllipseArc(o1.p[0], o1.p[1], a, b, phi1, phi2 + phi1, m_Basic_Area,
                     bFillTriangle) != rvG_OK)
    return rvCannotExport;

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------
