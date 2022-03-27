#include "GCLC.h"
#include <cmath>
#include <cstdlib>
#include <sstream>

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_intersec() {
  string sPointName;
  GCLC_object o1, o2, o3, o4;
  double dDet;
  GCLCError iRv;

  if (ReadToken(sPointName) != rvGCLCOK)
    return rvGCLCIdExpected;
  if ((iRv = ReadNextObject(o1)) != rvGCLCOK)
    return iRv;

  if (o1.type == GCLC_LINE) {
    if ((iRv = ReadObject(GCLC_LINE, o2)) != rvGCLCOK)
      return iRv;

    if ((fabs(o1.p[0] - o2.p[0]) <= EPS) && (fabs(o1.p[1] - o2.p[1]) <= EPS)) {
      if (ProvingTheorem()) {
        // run-time deduction check
        string P1, P2, P3, P4;
        if (GetPointsOnLine(o1.name, P1, P2) &&
            GetPointsOnLine(o2.name, P3, P4))
          SetProverConjecture("parallel " + P1 + " " + P2 + " " + P3 + " " +
                              P4);
      }
      return rvGCLCBadIntersection;
    }

    dDet = o1.p[0] * o2.p[1] - o2.p[0] * o1.p[1];
    if (fabs(dDet) <= EPS) {
      if (ProvingTheorem()) {
        // run-time deduction check
        string P1, P2, P3, P4;
        if (GetPointsOnLine(o1.name, P1, P2) &&
            GetPointsOnLine(o2.name, P3, P4))
          SetProverConjecture("parallel " + P1 + " " + P2 + " " + P3 + " " +
                              P4);
      }
      return rvGCLCBadIntersection;
    }

    //-------- Support for the prover ----------
    if (ProvingTheorem())
      AddProverCommand(p_inter, sPointName, o1.name, o2.name);
    //-------- End of support for the prover ---

    //-------- Support for input exported to XML form ---------
    if (m_bXMLOutput && m_hXMLOutput.good()) {
      ChangeCurrentXMLGroup(eXMLconstruct);
      Print(m_hXMLOutput, "\t\t<intersection><new_point>" + sPointName +
                              "</new_point><line>" + o1.name + "</line><line>" +
                              o2.name + "</line></intersection>\n");
      m_CurrentXMLgroup = eXMLconstruct;
    }
    //-------- End of support for XML form ---------

    return Let(
        sPointName, GCLC_POINT, (o1.p[1] * o2.p[2] - o2.p[1] * o1.p[2]) / dDet,
        (o2.p[0] * o1.p[2] - o1.p[0] * o2.p[2]) / dDet, 0.00, 0.00, 0.00, 0.00);
  } else if (o1.type == GCLC_POINT) {
    if ((iRv = ReadObject(GCLC_POINT, o2)) != rvGCLCOK)
      return iRv;
    if ((iRv = ReadObject(GCLC_POINT, o3)) != rvGCLCOK)
      return iRv;
    if ((iRv = ReadObject(GCLC_POINT, o4)) != rvGCLCOK)
      return iRv;

    double a1, b1, c1, a2, b2, c2, k;
    if (fabs(o1.p[0] - o2.p[0]) <= EPS) {
      a1 = 1.0;
      b1 = 0.0;
      c1 = -(o1.p[0]);
    } else if (fabs(o1.p[1] - o2.p[1]) <= EPS) {
      a1 = 0.0;
      b1 = 1.0;
      c1 = -(o1.p[1]);
    } else {
      k = (o2.p[0] - o1.p[0]) / (o1.p[1] - o2.p[1]);
      a1 = 1.0;
      b1 = k;
      c1 = -(o1.p[0] + k * (o1.p[1]));
    }

    if (fabs(o3.p[0] - o4.p[0]) <= EPS) {
      a2 = 1.0;
      b2 = 0.0;
      c2 = -(o3.p[0]);
    } else if (fabs(o3.p[1] - o4.p[1]) <= EPS) {
      a2 = 0.0;
      b2 = 1.0;
      c2 = -(o3.p[1]);
    } else {
      k = (o4.p[0] - o3.p[0]) / (o3.p[1] - o4.p[1]);
      a2 = 1.0;
      b2 = k;
      c2 = -(o3.p[0] + k * (o3.p[1]));
    }

    if ((fabs(a1 - a2) <= EPS) && (fabs(b1 - b2) <= EPS)) {
      if (ProvingTheorem()) {
        // run-time deduction check
        SetProverConjecture("parallel " + o1.name + " " + o2.name + " " +
                            o3.name + " " + o4.name);
      }
      return rvGCLCBadIntersection;
    }

    dDet = a1 * b2 - a2 * b1;
    if (fabs(dDet) <= EPS) {
      if (ProvingTheorem()) {
        // run-time deduction check
        SetProverConjecture("parallel " + o1.name + " " + o2.name + " " +
                            o3.name + " " + o4.name);
      }
      return rvGCLCBadIntersection;
    }

    //-------- Support for the prover ----------
    if (ProvingTheorem())
      AddProverCommand(p_inter, sPointName, o1.name, o2.name, o3.name, o4.name);
    //-------- End of support for the prover ---

    //-------- Support for input exported to XML form ---------
    if (m_bXMLOutput && m_hXMLOutput.good()) {
      ChangeCurrentXMLGroup(eXMLconstruct);
      Print(m_hXMLOutput, "\t\t<intersection><new_point>" + sPointName +
                              "</new_point><line>" + o1.name + "-" + o2.name +
                              "</line><line>" + o3.name + "-" + o4.name +
                              "</line></intersection>\n");
      m_CurrentXMLgroup = eXMLconstruct;
    }
    //-------- End of support for XML form ---------

    return Let(sPointName, GCLC_POINT, (b1 * c2 - b2 * c1) / dDet,
               (a2 * c1 - a1 * c2) / dDet, 0.00, 0.00, 0.00, 0.00);
  } else
    return rvGCLCWrongType;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_intersec2() {
  string sPointName1, sPointName2;
  GCLC_object o1, o2;
  GCLCError iRv;
  double x1, y1, x2, y2, x, y, r, r1, r2, a, b, c, d, e, f;

  if (ReadToken(sPointName1) != rvGCLCOK)
    return rvGCLCIdExpected;
  if (ReadToken(sPointName2) != rvGCLCOK)
    return rvGCLCIdExpected;

  if ((iRv = ReadNextObject(o1)) != rvGCLCOK)
    return iRv;
  if ((o1.type != GCLC_LINE) && (o1.type != GCLC_CIRCLE))
    return rvGCLCWrongType;

  if ((iRv = ReadNextObject(o2)) != rvGCLCOK)
    return iRv;
  if ((o2.type != GCLC_LINE) && (o2.type != GCLC_CIRCLE))
    return rvGCLCWrongType;

  if ((o1.type != GCLC_CIRCLE) && (o2.type != GCLC_CIRCLE))
    return rvGCLCWrongType;

  if ((o1.type == GCLC_CIRCLE) && (o2.type == GCLC_CIRCLE)) {
    x1 = o1.p[0];
    y1 = o1.p[1];
    r1 = o1.p[2];
    x2 = o2.p[0];
    y2 = o2.p[1];
    r2 = o2.p[2];
    d = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    if (r1 >= r2) {
      if ((d < r1 - r2) || (d > r1 + r2)) {
        // TODO: perform deductive check
        return rvGCLCBadIntersection;
      }
    } else {
      if ((d < r2 - r1) || (d > r1 + r2)) {
        // TODO: perform deductive check
        return rvGCLCBadIntersection;
      }
    }

    if ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) < EPS) {
      // TODO: perform deductive check
      return rvGCLCBadIntersection;
    } else {
      d = r1 * r1 - r2 * r2 + (x2 * x2 + y2 * y2) - (x1 * x1 + y1 * y1);
      if (fabs(x1 - x2) < EPS) {
        e = d / (2 * y2 - 2 * y1);
        f = (2 * x1 - 2 * x2) / (2 * y2 - 2 * y1);
        a = 1 + f * f;
        b = 2 * e * f - 2 * x1 - 2 * f * y1;
        c = e * e - 2 * e * y1 + x1 * x1 + y1 * y1 - r1 * r1;
        x1 = (-b - sqrt(b * b - 4 * a * c)) / (2 * a);
        y1 = e + f * x1;
        x2 = (-b + sqrt(b * b - 4 * a * c)) / (2 * a);
        y2 = e + f * x2;
      } else {
        e = d / (2 * x2 - 2 * x1);
        f = (2 * y1 - 2 * y2) / (2 * x2 - 2 * x1);
        a = 1 + f * f;
        b = 2 * e * f - 2 * y1 - 2 * f * x1;
        c = e * e - 2 * e * x1 + x1 * x1 + y1 * y1 - r1 * r1;
        y1 = (-b - sqrt(b * b - 4 * a * c)) / (2 * a);
        x1 = e + f * y1;
        y2 = (-b + sqrt(b * b - 4 * a * c)) / (2 * a);
        x2 = e + f * y2;
      }
    }

    //-------- Support for input exported to XML form ---------
    if (m_bXMLOutput && m_hXMLOutput.good()) {
      ChangeCurrentXMLGroup(eXMLconstruct);
      Print(m_hXMLOutput, "\t\t<intersection_cc><new_point>" + sPointName1 +
                              "</new_point><new_point>" + sPointName2 +
                              "</new_point><circle>" + o1.name +
                              "</circle><circle>" + o2.name +
                              "</circle></intersection_cc>\n");
      m_CurrentXMLgroup = eXMLconstruct;
    }
    //-------- End of support for XML form ---------

    //-------- Support for the prover ----------
    // p_intercc is not supported, this call will be rejected by the prover
    if (ProvingTheorem())
      AddProverCommand(p_intercc, sPointName1, sPointName2, o1.name, o2.name);
    //-------- End of support for the prover ---


  } else {
    if (o1.type == GCLC_LINE) {
      //-------- Support for input exported to XML form ---------
      if (m_bXMLOutput && m_hXMLOutput.good()) {
        ChangeCurrentXMLGroup(eXMLconstruct);
        Print(m_hXMLOutput, "\t\t<intersection_cl><new_point>" + sPointName1 +
                                "</new_point><new_point>" + sPointName2 +
                                "</new_point><circle>" + o2.name +
                                "</circle><line>" + o1.name +
                                "</line></intersection_cl>\n");
        m_CurrentXMLgroup = eXMLconstruct;
      }
      //-------- End of support for XML form ---------

      a = o1.p[0];
      b = o1.p[1];
      c = o1.p[2];
      x = o2.p[0];
      y = o2.p[1];
      r = o2.p[2];
    } else {
      //-------- Support for input exported to XML form ---------
      if (m_bXMLOutput && m_hXMLOutput.good()) {
        ChangeCurrentXMLGroup(eXMLconstruct);
        Print(m_hXMLOutput, "\t\t<intersection_cl><new_point>" + sPointName1 +
                                "</new_point><new_point>" + sPointName2 +
                                "</new_point><circle>" + o1.name +
                                "</circle><line>" + o2.name +
                                "</line></intersection_cl>\n");
        m_CurrentXMLgroup = eXMLconstruct;
      }
      //-------- End of support for XML form ---------

      a = o2.p[0];
      b = o2.p[1];
      c = o2.p[2];
      x = o1.p[0];
      y = o1.p[1];
      r = o1.p[2];
    }
    c = c + a * x + b * y;
    if (fabs(a) > EPS) {
      d = a * a * r * r + b * b * r * r - a * a * c * c;
      if (d < 0) {
        // TODO
        // perform deductive check
        return rvGCLCBadIntersection;
      }
      y1 = (-b * c + fabs(a) * sqrt(d)) / (a * a + b * b) + y;
      x1 = -(c + b * (y1 - y)) / a + x;
      y2 = (-b * c - fabs(a) * sqrt(d)) / (a * a + b * b) + y;
      x2 = -(c + b * (y2 - y)) / a + x;
    } else {
      d = a * a * r * r + b * b * r * r - b * b * c * c;
      if (d < 0) {
        // TODO
        // perform deductive check
        return rvGCLCBadIntersection;
      }
      x1 = (-a * c + fabs(b) * sqrt(d)) / (a * a + b * b) + x;
      y1 = -(c + a * (x1 - x)) / b + y;
      x2 = (-a * c - fabs(b) * sqrt(d)) / (a * a + b * b) + x;
      y2 = -(c + a * (x2 - x)) / b + y;
    }

    //-------- Support for the prover ----------
    if (ProvingTheorem())
      AddProverCommand(p_interlc, sPointName1, sPointName2, o1.name, o2.name);
    //-------- End of support for the prover ---
  }

  if ((iRv = Let(sPointName1, GCLC_POINT, x1, y1, 0.00, 0.00, 0.00, 0.00)) !=
      rvGCLCOK)
    return iRv;

  return Let(sPointName2, GCLC_POINT, x2, y2, 0.00, 0.00, 0.00, 0.00);
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_midpoint() {
  string sPointName;
  GCLC_object o1, o2;
  GCLCError iRv;

  if (ReadToken(sPointName) != rvGCLCOK)
    return rvGCLCIdExpected;
  if ((iRv = ReadObject(GCLC_POINT, o1)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadObject(GCLC_POINT, o2)) != rvGCLCOK)
    return iRv;

  //-------- Support for the prover ---------
  if (ProvingTheorem())
    AddProverCommand(midpoint, sPointName, o1.name, o2.name);
  //-------- End of support for the prover --

  //-------- Support for input exported to XML form ---------
  if (m_bXMLOutput && m_hXMLOutput.good()) {
    ChangeCurrentXMLGroup(eXMLconstruct);
    Print(m_hXMLOutput,
          "\t\t<midpoint><new_point>" + sPointName + "</new_point><point>" +
              o1.name + "</point><point>" + o2.name + "</point></midpoint>\n");
    m_CurrentXMLgroup = eXMLconstruct;
  }
  //-------- End of support for XML form ---------

  return Let(sPointName, GCLC_POINT, (o1.p[0] + o2.p[0]) / 2,
             (o1.p[1] + o2.p[1]) / 2, 0, 0.00, 0.00, 0.00);
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_med() {
  string sLineName;
  GCLC_object o1, o2;
  double x1, y1, x2, y2;
  GCLCError iRv;

  if (ReadToken(sLineName) != rvGCLCOK)
    return rvGCLCIdExpected;
  if ((iRv = ReadObject(GCLC_POINT, o1)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadObject(GCLC_POINT, o2)) != rvGCLCOK)
    return iRv;

  x1 = o1.p[0];
  y1 = o1.p[1];
  x2 = o2.p[0];
  y2 = o2.p[1];

  /* err=let(name,GCLC_LINE,2*x2-2*x1,2*y2-2*y1,x1*x1+y1*y1-x2*x2-y2*y2); */
  /* changed 18.07.1997; normalisation of parameters in order        */
  /* to prevent overflow/underflow and induced convertor errors      */

  //-------- Support for the prover ---------
  if (ProvingTheorem())
    AddProverCommand(med, sLineName, o1.name, o2.name);
  //-------- End of support for the prover --

  //-------- Support for input exported to XML form ---------
  if (m_bXMLOutput && m_hXMLOutput.good()) {
    ChangeCurrentXMLGroup(eXMLconstruct);
    Print(m_hXMLOutput, "\t\t<segment_bisector><new_line>" + sLineName +
                            "</new_line><point>" + o1.name + "</point><point>" +
                            o2.name + "</point></segment_bisector>\n");
    m_CurrentXMLgroup = eXMLconstruct;
  }
  //-------- End of support for XML form ---------

  if (fabs(2 * x2 - 2 * x1) > EPS)
    return Let(sLineName, GCLC_LINE, 1, (2 * y2 - 2 * y1) / (2 * x2 - 2 * x1),
               (x1 * x1 + y1 * y1 - x2 * x2 - y2 * y2) / (2 * x2 - 2 * x1),
               0.00, 0.00, 0.00);
  else {
    if (fabs(2 * y2 - 2 * y1) > EPS)
      return Let(sLineName, GCLC_LINE, (2 * x2 - 2 * x1) / (2 * y2 - 2 * y1), 1,
                 (x1 * x1 + y1 * y1 - x2 * x2 - y2 * y2) / (2 * y2 - 2 * y1),
                 0.00, 0.00, 0.00);
    else {
      if (ProvingTheorem()) {
        // run-time deduction check
        SetProverConjecture("identical " + o1.name + " " + o2.name);
      }
      return rvGCLCBadLine;
    }
  }
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_bis() {
  string sLineName;
  GCLC_object o1, o2, o3;
  GCLCError iRv;
  double x, y, k, d1, d2, a, b;

  if (ReadToken(sLineName) != rvGCLCOK)
    return rvGCLCIdExpected;
  if ((iRv = ReadObject(GCLC_POINT, o1)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadObject(GCLC_POINT, o2)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadObject(GCLC_POINT, o3)) != rvGCLCOK)
    return iRv;

  d1 = sqrt((o1.p[0] - o2.p[0]) * (o1.p[0] - o2.p[0]) +
            (o1.p[1] - o2.p[1]) * (o1.p[1] - o2.p[1]));
  d2 = sqrt((o3.p[0] - o2.p[0]) * (o3.p[0] - o2.p[0]) +
            (o3.p[1] - o2.p[1]) * (o3.p[1] - o2.p[1]));

  if (d1 <= EPS) {
    if (ProvingTheorem()) {
      // run-time deduction check
      SetProverConjecture("identical " + o1.name + " " + o2.name);
    }
    return rvGCLCBadLine;
  }

  if (d2 <= EPS) {
    if (ProvingTheorem()) {
      // run-time deduction check
      SetProverConjecture("identical " + o3.name + " " + o2.name);
    }
    return rvGCLCBadLine;
  }

  //-------- Support for input exported to XML form ---------
  if (m_bXMLOutput && m_hXMLOutput.good()) {
    ChangeCurrentXMLGroup(eXMLconstruct);
    Print(m_hXMLOutput, "\t\t<angle_bisector><new_line>" + sLineName +
                            "</new_line><point>" + o1.name + "</point><point>" +
                            o2.name + "</point><point>" + o3.name +
                            "</point></angle_bisector>\n");
    m_CurrentXMLgroup = eXMLconstruct;
  }
  //-------- End of support for XML form ---------

  //-------- Support for the prover ---------
  if (ProvingTheorem())
    AddProverCommand(p_bis, sLineName, o1.name, o2.name, o3.name);

  if (bet(o1.p[0], o1.p[1], o2.p[0], o2.p[1], o3.p[0], o3.p[1])) {

    if (fabs(o1.p[0] - o2.p[0]) <= EPS) {
      a = 1.00;
      b = 0.00;
      // c = -(o1.p[0]);
    } else if (fabs(o1.p[1] - o2.p[1]) <= EPS) {
      a = 0.00;
      b = 1.00;
      // c = -(o1.p[1]);
    } else {
      k = (o2.p[0] - o1.p[0]) / (o1.p[1] - o2.p[1]);
      a = 1.00;
      b = k;
      // c = -(o1.p[0]+k*(o1.p[1]));
    }

    if (fabs(b) > EPS)
      return Let(sLineName, GCLC_LINE, 1, a / (-b),
                 (o2.p[0] * b - o2.p[1] * a) / (-b), 0.00, 0.00, 0.00);
    else
      return Let(sLineName, GCLC_LINE, -b / a, 1, (o2.p[0] * b - o2.p[1] * a) / a,
                 0.00, 0.00, 0.00);
  }

  if (bet(o2.p[0], o2.p[1], o1.p[0], o1.p[1], o3.p[0], o3.p[1]) ||
      bet(o2.p[0], o2.p[1], o3.p[0], o3.p[1], o1.p[0], o1.p[1])) {
    if (fabs(o1.p[0] - o2.p[0]) <= EPS)
      return Let(sLineName, GCLC_LINE, 1.00, 0.00, -(o1.p[0]), 0.00, 0.00, 0.00);
    else if (fabs(o1.p[1] - o2.p[1]) <= EPS)
      return Let(sLineName, GCLC_LINE, 0.00, 1.00, -(o1.p[1]), 0.00, 0.00, 0.00);
    else {
      k = (o2.p[0] - o1.p[0]) / (o1.p[1] - o2.p[1]);
      return Let(sLineName, GCLC_LINE, 1.00, k, -(o1.p[0] + k * (o1.p[1])), 0.00,
                 0.00, 0.00);
    }
  }

  x = (d2 * o1.p[0] + d1 * o3.p[0]) / (d1 + d2);
  y = (d2 * o1.p[1] + d1 * o3.p[1]) / (d1 + d2);

  if (fabs(x - o2.p[0]) <= EPS)
    return Let(sLineName, GCLC_LINE, 1.00, 0.00, -x, 0.00, 0.00, 0.00);

  if (fabs(y - o2.p[1]) <= EPS)
    return Let(sLineName, GCLC_LINE, 0.00, 1.00, -y, 0.00, 0.00, 0.00);
  else {
    k = (o2.p[0] - x) / (y - o2.p[1]);
    return Let(sLineName, GCLC_LINE, 1.00, k, -(x + k * y), 0.00, 0.00, 0.00);
  }
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_perp() {
  string sLineName;
  GCLC_object o1, o2;
  GCLCError iRv;

  if (ReadToken(sLineName) != rvGCLCOK)
    return rvGCLCIdExpected;
  if ((iRv = ReadObject(GCLC_POINT, o1)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadObject(GCLC_LINE, o2)) != rvGCLCOK)
    return iRv;

  /* err=let(name,GCLC_LINE,-o2.p[1],o2.p[0],o1.p[0]*o2.p[1]-o1.p[1]*o2.p[0]); */
  /*                                                          */
  /* changed on august 1997                                   */
  /* line parameters are normalized in order to get           */
  /* more precise intersections etc.                          */

  //-------- Support for the prover ---------
  if (ProvingTheorem())
    AddProverCommand(perp, sLineName, o1.name, o2.name);
  //-------- Support for the prover ---------

  //-------- Support for input exported to XML form ---------
  if (m_bXMLOutput && m_hXMLOutput.good()) {
    ChangeCurrentXMLGroup(eXMLconstruct);
    Print(m_hXMLOutput, "\t\t<perpendicular><new_line>" + sLineName +
                            "</new_line><point>" + o1.name + "</point><line>" +
                            o2.name + "</line></perpendicular>\n");
    m_CurrentXMLgroup = eXMLconstruct;
  }
  //-------- End of support for XML form ---------

  if (fabs(o2.p[1]) > EPS)
    return Let(sLineName, GCLC_LINE, 1, o2.p[0] / (-o2.p[1]),
               (o1.p[0] * o2.p[1] - o1.p[1] * o2.p[0]) / (-o2.p[1]), 0.00, 0.00,
               0.00);
  else
    return Let(sLineName, GCLC_LINE, -o2.p[1] / o2.p[0], 1,
               (o1.p[0] * o2.p[1] - o1.p[1] * o2.p[0]) / o2.p[0], 0.00, 0.00,
               0.00);
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_foot() {
  string sPointName;
  GCLC_object o1, o2;
  GCLCError iRv;
  double a1, a2, b1, b2, c1, c2, dDet;

  if (ReadToken(sPointName) != rvGCLCOK)
    return rvGCLCIdExpected;
  if ((iRv = ReadObject(GCLC_POINT, o1)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadObject(GCLC_LINE, o2)) != rvGCLCOK)
    return iRv;

  a1 = o2.p[0];
  b1 = o2.p[1];
  c1 = o2.p[2];

  if (b1 > EPS) {
    a2 = 1;
    b2 = a1 / -b1;
    c2 = (o1.p[0] * b1 - o1.p[1] * a1) / -b1;
  } else {
    a2 = -b1 / a1;
    b2 = 1;
    c2 = (o1.p[0] * b1 - o1.p[1] * a1) / a1;
  }

  dDet = a1 * b2 - a2 * b1;

  //-------- Support for the prover ---------
  if (ProvingTheorem())
    AddProverCommand(p_foot, sPointName, o1.name, o2.name);
  //-------- End support for the prover ---------

  //-------- Support for input exported to XML form ---------
  if (m_bXMLOutput && m_hXMLOutput.good()) {
    ChangeCurrentXMLGroup(eXMLconstruct);
    Print(m_hXMLOutput, "\t\t<foot><new_point>" + sPointName +
                            "</new_point><point>" + o1.name + "</point><line>" +
                            o2.name + "</line></foot>\n");
    m_CurrentXMLgroup = eXMLconstruct;
  }
  //-------- End of support for XML form ---------

  return Let(sPointName, GCLC_POINT, (b1 * c2 - b2 * c1) / dDet,
             (a2 * c1 - a1 * c2) / dDet, 0.00, 0.00, 0.00, 0.00);
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_parallel() {
  string sLineName;
  GCLC_object o1, o2;
  GCLCError iRv;

  if (ReadToken(sLineName) != rvGCLCOK)
    return rvGCLCIdExpected;
  if ((iRv = ReadObject(GCLC_POINT, o1)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadObject(GCLC_LINE, o2)) != rvGCLCOK)
    return iRv;

  //-------- Support for the prover ---------
  if (ProvingTheorem())
    AddProverCommand(parallel, sLineName, o1.name, o2.name);
  //-------- Support for the prover ---------

  //-------- Support for input exported to XML form ---------
  if (m_bXMLOutput && m_hXMLOutput.good()) {
    ChangeCurrentXMLGroup(eXMLconstruct);
    Print(m_hXMLOutput, "\t\t<parallel><new_line>" + sLineName +
                            "</new_line><point>" + o1.name + "</point><line>" +
                            o2.name + "</line></parallel>\n");
    m_CurrentXMLgroup = eXMLconstruct;
  }
  //-------- End of support for XML form ---------

  return Let(sLineName, GCLC_LINE, o2.p[0], o2.p[1],
             -o2.p[0] * o1.p[0] - o2.p[1] * o1.p[1], 0.00, 0.00, 0.00);
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_getcenter() {
  string sPointName;
  GCLC_object o;
  GCLCError iRv;

  if (ReadToken(sPointName) != rvGCLCOK)
    return rvGCLCIdExpected;
  if ((iRv = ReadObject(GCLC_CIRCLE, o)) != rvGCLCOK)
    return iRv;

  return Let(sPointName, GCLC_POINT, o.p[0], o.p[1], 0.00, 0.00, 0.00, 0.00);
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_onsegment() {
  string sPointName;
  GCLC_object o1, o2;
  GCLCError iRv;
  double x1, x2, y1, y2, k;

  if (ReadToken(sPointName) != rvGCLCOK)
    return rvGCLCIdExpected;
  if ((iRv = ReadObject(GCLC_POINT, o1)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadObject(GCLC_POINT, o2)) != rvGCLCOK)
    return iRv;

  x1 = o1.p[0];
  y1 = o1.p[1];
  x2 = o2.p[0];
  y2 = o2.p[1];
  k = ((double)rand() / RAND_MAX);

  return Let(sPointName, GCLC_POINT, x1 + k * (x2 - x1), y1 + k * (y2 - y1), 0.00,
             0.00, 0.00, 0.00);
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_online() {
  string sPointName;
  GCLC_object o1, o2;
  GCLCError iRv;
  double x1, x2, y1, y2, k;

  if (ReadToken(sPointName) != rvGCLCOK)
    return rvGCLCIdExpected;
  if ((iRv = ReadObject(GCLC_POINT, o1)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadObject(GCLC_POINT, o2)) != rvGCLCOK)
    return iRv;

  x1 = o1.p[0];
  y1 = o1.p[1];
  x2 = o2.p[0];
  y2 = o2.p[1];

  k = 3 * ((double)rand() / RAND_MAX) - 1;

  //-------- Support for the prover ---------
  if (ProvingTheorem())
    AddProverCommand(online, sPointName, o1.name, o2.name, d2s(k, -1));
  //-------- Support for the prover ---------

  //-------- Support for input exported to XML form ---------
  if (m_bXMLOutput && m_hXMLOutput.good()) {
    ChangeCurrentXMLGroup(eXMLconstruct);
    Print(m_hXMLOutput, "\t\t<random_point_on_line><new_point>" + sPointName +
                            "</new_point><point>" + o1.name +
                            "</point><point>" + o2.name +
                            "</point></random_point_on_line>\n");
    m_CurrentXMLgroup = eXMLconstruct;
  }
  //-------- End of support for XML form ---------

  return Let(sPointName, GCLC_POINT, x1 + k * (x2 - x1), y1 + k * (y2 - y1), 0.00,
             0.00, 0.00, 0.00);
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_oncircle() {
  string sPointName;
  GCLC_object o1, o2;
  GCLCError iRv;
  double x1, x2, y1, y2, k, x, y;

  if (ReadToken(sPointName) != rvGCLCOK)
    return rvGCLCIdExpected;
  if ((iRv = ReadObject(GCLC_POINT, o1)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadObject(GCLC_POINT, o2)) != rvGCLCOK)
    return iRv;

  x1 = o1.p[0];
  y1 = o1.p[1];
  x2 = o2.p[0];
  y2 = o2.p[1];

  k = 2 * PI * ((double)rand() / RAND_MAX);

  //-------- Support for the prover ---------
  if (ProvingTheorem())
    AddProverCommand(oncircle, sPointName, o1.name, o2.name, d2s(k, -1));
  //-------- Support for the prover ---------

  x = (x2 - x1) * cos(k) - (y2 - y1) * sin(k) + x1,
  y = (y2 - y1) * cos(k) + (x2 - x1) * sin(k) + y1;

  return Let(sPointName, GCLC_POINT, x, y, 0.00, 0.00, 0.00, 0.00);
}

// ----------------------------------------------------------------------------
