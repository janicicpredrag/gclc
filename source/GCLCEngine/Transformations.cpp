#include "GCLC.h"
#include <cmath>
#include <sstream>

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_translate() {
  std::string sPointName;
  GCLC_object o1, o2, o3;
  GCLCError iRv;

  if ((iRv = ReadToken(sPointName)) != rvGCLCOK)
    return rvGCLCIdExpected;
  if ((iRv = ReadObject(GCLC_POINT, o1)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadObject(GCLC_POINT, o2)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadObject(GCLC_POINT, o3)) != rvGCLCOK)
    return iRv;

  //-------- Support for the prover ---------
  if (ProvingTheorem())
    AddProverCommand(p_pratio, sPointName, o3.name, o1.name, o2.name, "1");
  //-------- End of support for the prover --

  //-------- Support for input exported to XML form ---------
  if (m_bXMLOutput && m_hXMLOutput.good()) {
    ChangeCurrentXMLGroup(eXMLconstruct);
    Print(m_hXMLOutput, "\t\t<translate><new_point>" + sPointName +
                            "</new_point><vector><point>" + o1.name +
                            "</point><point>" + o2.name +
                            "</point></vector><point>" + o3.name +
                            "</point></translate>\n");
    m_CurrentXMLgroup = eXMLconstruct;
  }
  //-------- End of support for XML form ---------

  return Let(sPointName, GCLC_POINT, o3.p[0] + o2.p[0] - o1.p[0],
             o3.p[1] + o2.p[1] - o1.p[1], 0.00, 0.00, 0.00, 0.00);
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_rotate() {
  std::string sPointName;
  GCLC_object o1, o3;
  GCLCError iRv;
  double x1, x3, y1, y3, sinn, cosn, angle;

  if ((iRv = ReadToken(sPointName)) != rvGCLCOK)
    return rvGCLCIdExpected;
  if ((iRv = ReadObject(GCLC_POINT, o1)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(angle)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadObject(GCLC_POINT, o3)) != rvGCLCOK)
    return iRv;

  //-------- Support for input exported to XML form ---------
  if (m_bXMLOutput && m_hXMLOutput.good()) {
    ChangeCurrentXMLGroup(eXMLconstruct);
    Print(m_hXMLOutput,
          "\t\t<rotate><new_point>" + sPointName + "</new_point><center>" +
              o1.name + "</center><angle>" + d2s(angle, 2) + "</angle><point>" +
              o3.name + "</point></rotate>\n");
    m_CurrentXMLgroup = eXMLconstruct;
  }
  //-------- End of support for XML form ---------

  angle = 2 * PI * (angle / 360.00); /* conversion from deg to rad */

  sinn = sin(angle);
  cosn = cos(angle);
  x1 = o1.p[0];
  y1 = o1.p[1];
  x3 = o3.p[0];
  y3 = o3.p[1];

  return Let(sPointName, GCLC_POINT, (x3 - x1) * cosn - (y3 - y1) * sinn + x1,
             (y3 - y1) * cosn + (x3 - x1) * sinn + y1, 0.00, 0.00, 0.00, 0.00);
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_rotateonellipse() {
  std::string sPointName;
  GCLC_object o1, o2, o3, o4;
  GCLCError iRv;
  double phi, phi1, angle, a, b, pom, x, y, x1, y1, x2, y2, x3, y3, x4, y4,
      sina, cosa;

  if ((iRv = ReadToken(sPointName)) != rvGCLCOK)
    return rvGCLCIdExpected;
  if ((iRv = ReadObject(GCLC_POINT, o1)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadObject(GCLC_POINT, o2)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadObject(GCLC_POINT, o3)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(angle)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadObject(GCLC_POINT, o4)) != rvGCLCOK)
    return iRv;

  // the ellipse is determined by o1, o2, o3
  x1 = o1.p[0];
  y1 = o1.p[1];
  x2 = o2.p[0];
  y2 = o2.p[1];
  x3 = o3.p[0];
  y3 = o3.p[1];
  x4 = o4.p[0];
  y4 = o4.p[1];
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

  phi = 180 * atan2(y2 - y1, x2 - x1) / PI;
  phi = 180 * atan2(y4 - y1, x4 - x1) / PI - phi;
  phi1 = transform_ellipse_angle(a, b, phi + angle);

  double xx = x1 + a * cos(PI * phi1 / 180);
  double yy = y1 + b * sin(PI * phi1 / 180);
  x = x1 + (xx - x1) * cosa - (yy - y1) * sina;
  y = y1 + (yy - y1) * cosa + (xx - x1) * sina;

  return Let(sPointName, GCLC_POINT, x, y, 0.00, 0.00, 0.00, 0.00);
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_sim() {
  std::string sPointName;
  GCLC_object o1, o2;
  GCLCError iRv;
  double x, y, x0, y0, a, b, c, r, k;

  if ((iRv = ReadToken(sPointName)) != rvGCLCOK)
    return rvGCLCIdExpected;
  if ((iRv = ReadNextObject(o1)) != rvGCLCOK)
    return iRv;
  if (o1.type == GCLC_NUMBER)
    return rvGCLCWrongType;

  if ((iRv = ReadNextObject(o2)) != rvGCLCOK)
    return iRv;
  if (o2.type != GCLC_POINT)
    return rvGCLCWrongType;

  if (o1.type == GCLC_POINT) {
    //-------- Support for input exported to XML form ---------
    if (m_bXMLOutput && m_hXMLOutput.good()) {
      ChangeCurrentXMLGroup(eXMLconstruct);
      Print(m_hXMLOutput, "\t\t<half_turn><new_point>" + sPointName +
                              "</new_point><center>" + o1.name +
                              "</center><point>" + o2.name +
                              "</point></half_turn>\n");
      m_CurrentXMLgroup = eXMLconstruct;
    }
    //-------- End of support for XML form ---------

    return Let(sPointName, GCLC_POINT, 2 * o1.p[0] - o2.p[0], 2 * o1.p[1] - o2.p[1],
               0.00, 0.00, 0.00, 0.00);
  }

  if (o1.type == GCLC_LINE) {
    //-------- Support for input exported to XML form ---------
    if (m_bXMLOutput && m_hXMLOutput.good()) {
      ChangeCurrentXMLGroup(eXMLconstruct);
      Print(m_hXMLOutput, "\t\t<line_reflection><new_point>" + sPointName +
                              "</new_point><line>" + o1.name +
                              "</line><point>" + o2.name +
                              "</point></line_reflection>\n");
      m_CurrentXMLgroup = eXMLconstruct;
    }
    //-------- End of support for XML form ---------

    a = o1.p[0];
    b = o1.p[1];
    c = o1.p[2];
    x0 = o2.p[0];
    y0 = o2.p[1];
    x = (b * b * x0 - a * b * y0 - a * c) / (a * a + b * b);
    y = (a * a * y0 - a * b * x0 - b * c) / (a * a + b * b);
    return Let(sPointName, GCLC_POINT, 2 * x - x0, 2 * y - y0, 0.00, 0.00, 0.00,
               0.00);
  }

  if (o1.type == GCLC_CIRCLE) {
    //-------- Support for input exported to XML form ---------
    if (m_bXMLOutput && m_hXMLOutput.good()) {
      ChangeCurrentXMLGroup(eXMLconstruct);
      Print(m_hXMLOutput, "\t\t<inversion><new_point>" + sPointName +
                              "</new_point><circle>" + o1.name +
                              "</circle><point>" + o2.name +
                              "</point></inversion>\n");
      m_CurrentXMLgroup = eXMLconstruct;
    }
    //-------- End of support for XML form ---------

    a = o1.p[0];
    b = o1.p[1];
    r = o1.p[2];
    x0 = o2.p[0];
    y0 = o2.p[1];
    k = r * r / ((x0 - a) * (x0 - a) + (y0 - b) * (y0 - b));
    return Let(sPointName, GCLC_POINT, k * (x0 - a) + a, k * (y0 - b) + b, 0.00,
               0.00, 0.00, 0.00);
  }

  return rvGCLCWrongType;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_turtle() {
  std::string sPointName;
  GCLC_object o1, o2;
  GCLCError iRv;
  double x1, x2, y1, y2, sinn, cosn, angle, d, d0;

  if ((iRv = ReadToken(sPointName)) != rvGCLCOK)
    return rvGCLCIdExpected;
  if ((iRv = ReadObject(GCLC_POINT, o1)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadObject(GCLC_POINT, o2)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(angle)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(d)) != rvGCLCOK)
    return iRv;

  angle = 2 * PI * (angle / 360.00); /* conversion from deg to rad */

  sinn = sin(angle);
  cosn = cos(angle);
  x1 = o1.p[0];
  y1 = o1.p[1];
  x2 = o2.p[0];
  y2 = o2.p[1];
  d0 = distance2d(o1, o2);

  return Let(sPointName, GCLC_POINT,
             ((x1 - x2) * cosn - (y1 - y2) * sinn) * (d / d0) + x2,
             ((y1 - y2) * cosn + (x1 - x2) * sinn) * (d / d0) + y2, 0.00, 0.00,
             0.00, 0.00);
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_towards() {
  std::string sPointName;
  GCLC_object o1, o2;
  GCLCError iRv;
  double x1, x2, y1, y2, k;

  if ((iRv = ReadToken(sPointName)) != rvGCLCOK)
    return rvGCLCIdExpected;
  if ((iRv = ReadObject(GCLC_POINT, o1)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadObject(GCLC_POINT, o2)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(k)) != rvGCLCOK)
    return iRv;

  x1 = o1.p[0];
  y1 = o1.p[1];
  x2 = o2.p[0];
  y2 = o2.p[1];

  //-------- Support for the prover ---------
  if (ProvingTheorem())
    AddProverCommand(p_pratio, sPointName, o1.name, o1.name, o2.name,
                     d2s(k, -1));
  //-------- End of support for the prover --

  //-------- Support for input exported to XML form ---------
  if (m_bXMLOutput && m_hXMLOutput.good()) {
    ChangeCurrentXMLGroup(eXMLconstruct);
    Print(m_hXMLOutput, "\t\t<towards><new_point>" + sPointName +
                            "</new_point><vector><point>" + o1.name +
                            "</point><point>" + o2.name +
                            "</point></vector><coefficient>" + d2s(k, -1) +
                            "</coefficient></towards>\n");
    m_CurrentXMLgroup = eXMLconstruct;
  }
  //-------- End of support for XML form ---------

  return Let(sPointName, GCLC_POINT, x1 + k * (x2 - x1), y1 + k * (y2 - y1), 0.00,
             0.00, 0.00, 0.00);
}

// ----------------------------------------------------------------------------
