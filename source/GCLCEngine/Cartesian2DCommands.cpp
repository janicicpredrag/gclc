#include "../Input/GCLCInput.h"
#include "GCLC.h"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

// ----------------------------------------------------------------------------

GCLCError CGCLC::ang_picture() {
  GCLCError iRv;

  if ((iRv = ReadNumber(m_2D_Area.x_lb)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(m_2D_Area.y_lb)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(m_2D_Area.x_rt)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(m_2D_Area.y_rt)) != rvGCLCOK)
    return iRv;

  ANG_SCALE = 1.00;
  ANG_LOG = 1;
  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::ang_origin() {
  GCLCError iRv;
  if ((iRv = ReadNumber(X_ORIGIN)) != rvGCLCOK)
    return iRv;
  return ReadNumber(Y_ORIGIN);
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::ang_unit() { return ReadNumber(UNIT); }

// ----------------------------------------------------------------------------

GCLCError CGCLC::ang_scale() {
  double d;
  GCLCError iRv;

  if ((iRv = ReadNumber(d)) != rvGCLCOK)
    return iRv;

  ANG_LOG = (int)d;
  if (ANG_LOG < 1)
    ANG_LOG = 1;
  if (ANG_LOG > 2)
    ANG_LOG = 2;

  if ((iRv = ReadNumber(d)) != rvGCLCOK)
    return iRv;

  if (d <= 0)
    d = 1;
  ANG_SCALE = d;
  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::ang_draw_system_p() {
  GCLCError iRv;
  double number;
  int mark_integers, mark_axes, arrows, x_step, y_step;

  iRv = ReadNumber(number);
  if (iRv != rvGCLCOK)
    return iRv;

  mark_integers = (int)number;
  if (mark_integers < 1)
    mark_integers = 1;
  if (mark_integers > 3)
    mark_integers = 3;

  iRv = ReadNumber(number);
  if (iRv != rvGCLCOK)
    return iRv;

  x_step = (int)number;
  if (x_step < 1)
    x_step = 1;

  iRv = ReadNumber(number);
  if (iRv != rvGCLCOK)
    return iRv;

  y_step = (int)number;
  if (y_step < 1)
    y_step = 1;

  iRv = ReadNumber(number);
  if (iRv != rvGCLCOK)
    return iRv;

  mark_axes = (int)number;
  if (mark_axes < 1)
    mark_axes = 1;
  if (mark_axes > 2)
    mark_axes = 2;

  if ((iRv = ReadNumber(number)) != rvGCLCOK)
    return iRv;

  arrows = (int)number;
  if (arrows < 1)
    arrows = 1;
  if (arrows > 3)
    arrows = 3;

  return ang_draw_system(mark_integers, x_step, y_step, mark_axes, arrows);
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::ang_draw_system(int t) {
  switch (t) {
  case 1:
    return ang_draw_system(1, 1, 1, 2, 1);
  case 2:
    return ang_draw_system(3, 1, 1, 2, 1);
  case 3:
    return ang_draw_system(2, 1, 1, 2, 1);
  case 101:
    return ang_draw_system(1, 1, 1, 1, 1);
  case 102:
    return ang_draw_system(3, 1, 1, 1, 1);
  case 103:
    return ang_draw_system(2, 1, 1, 1, 1);
  default:
    return ang_draw_system(1, 1, 1, 2, 1);
  }
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::ang_draw_system(int mark_integers, int x_step, int y_step,
                                 int mark_axes, int arrows) {
  // char name[16];
  double x, y;
  int i;
  double dash_halflen = 0.75;

  GCLCError iRv;
  std::string sComment = "Drawing 2D Cartesian system";

  if (PrintComment(sComment) != rvG_OK)
    return rvCannotExport;

  switch (mark_integers) {
  case 1: {
    i = 0;
    x = X_ORIGIN;
    if (ANG_LOG == 1)
      y = ang_y(0);
    else
      y = ang_y(1);
    if ((x >= m_2D_Area.x_lb) && (x <= m_2D_Area.x_rt) &&
        (y >= m_2D_Area.y_lb) && (y <= m_2D_Area.y_rt)) {
      if (!(i % x_step))
        ang_cmark(x, y, GCLC_DIR_LB, i2s(i));
    }
    x = x + UNIT;
    while (x <= m_2D_Area.x_rt) {
      i++;
      if (!(i % x_step)) {
        if ((x >= m_2D_Area.x_lb) && (x <= m_2D_Area.x_rt) &&
            (y >= m_2D_Area.y_lb) && (y <= m_2D_Area.y_rt))
          ang_cmark(x, y, GCLC_DIR_B, i2s(i));
      }
      x = x + UNIT;
    }

    i = 0;
    x = X_ORIGIN;
    x = x - UNIT;
    while (x >= m_2D_Area.x_lb) {
      i++;
      if (!(i % x_step)) {
        if ((x >= m_2D_Area.x_lb) && (x <= m_2D_Area.x_rt) &&
            (y >= m_2D_Area.y_lb) && (y <= m_2D_Area.y_rt))
          ang_cmark(x, y, GCLC_DIR_B, i2s(-i));
      }
      x = x - UNIT;
    }

    i = 0;
    x = X_ORIGIN;
    y = ang_y(1);
    while (y <= m_2D_Area.y_rt && i < 1000000000) {
      if (ANG_LOG == 1)
        i++;
      else
        i = (i == 0 ? 10 : i * 10);
      y = ang_y(i);
      if (((ANG_LOG == 1) && !(i % y_step)) ||
          ((ANG_LOG == 2) && !(log_div(i, y_step)))) {
        if ((x >= m_2D_Area.x_lb) && (x <= m_2D_Area.x_rt) &&
            (y >= m_2D_Area.y_lb) && (y <= m_2D_Area.y_rt))
          ang_cmark(x, y, GCLC_DIR_L, i2s(i));
      }
    }

    i = 0;
    x = X_ORIGIN;
    if (ANG_LOG == 1)
      y = ang_y(-1);
    else
      y = ang_y(0.1);
    while (y >= m_2D_Area.y_lb && i < 1000000000) {
      if (ANG_LOG == 1) {
        i++;
        y = ang_y(-i);
      } else {
        i = (i == 0 ? 10 : i * 10);
        y = ang_y(1.0 / i);
      }

      if (((ANG_LOG == 1) && !(i % y_step)) ||
          ((ANG_LOG == 2) && !(log_div(i, y_step)))) {
        std::string name;
        if (ANG_LOG == 1)
          name = i2s(-i);
        else {
          name = d2s(1.0 / i, -1);
          trimrightzeros(name);
        }
        if ((x >= m_2D_Area.x_lb) && (x <= m_2D_Area.x_rt) &&
            (y >= m_2D_Area.y_lb) && (y <= m_2D_Area.y_rt))
          ang_cmark(x, y, GCLC_DIR_L, name);
      }
    }
  } break;

  case 2: {
    i = 0;
    x = X_ORIGIN;
    if (ANG_LOG == 1)
      y = ang_y(0);
    else
      y = ang_y(1);
    if ((x >= m_2D_Area.x_lb) && (x <= m_2D_Area.x_rt) &&
        (y >= m_2D_Area.y_lb) && (y <= m_2D_Area.y_rt)) {
      if (!(i % x_step)) {
        ang_mark(x, y, GCLC_DIR_LB, i2s(i));
        iRv = ang_fsegment(x, y - dash_halflen, x, y + dash_halflen);
        if (iRv != rvGCLCOK)
          return iRv;
      }
    }
    x = x + UNIT;
    while (x <= m_2D_Area.x_rt) {
      i++;
      if (!(i % x_step)) {
        if ((x >= m_2D_Area.x_lb) && (x <= m_2D_Area.x_rt) &&
            (y >= m_2D_Area.y_lb) && (y <= m_2D_Area.y_rt)) {
          ang_mark(x, y, GCLC_DIR_B, i2s(i));
          iRv = ang_fsegment(x, y - dash_halflen, x, y + dash_halflen);
          if (iRv != rvGCLCOK)
            return iRv;
        }
      }
      x = x + UNIT;
    }

    i = 0;
    x = X_ORIGIN;
    y = Y_ORIGIN;
    x = x - UNIT;
    while (x >= m_2D_Area.x_lb) {
      i++;
      if (!(i % x_step)) {
        if ((x >= m_2D_Area.x_lb) && (x <= m_2D_Area.x_rt) &&
            (y >= m_2D_Area.y_lb) && (y <= m_2D_Area.y_rt)) {
          ang_mark(x, y, GCLC_DIR_B, i2s(-i));
          iRv = ang_fsegment(x, y - dash_halflen, x, y + dash_halflen);
          if (iRv != rvGCLCOK)
            return iRv;
        }
      }
      x = x - UNIT;
    }

    i = 0;
    x = X_ORIGIN;
    y = ang_y(1);
    while (y <= m_2D_Area.y_rt && i < 1000000000) {
      if (ANG_LOG == 1)
        i++;
      else
        i = (i == 0 ? 1 : i * 10);
      y = ang_y(i);
      if (((ANG_LOG == 1) && !(i % y_step)) ||
          ((ANG_LOG == 2) && !(log_div(i, y_step)))) {
        if ((x >= m_2D_Area.x_lb) && (x <= m_2D_Area.x_rt) &&
            (y >= m_2D_Area.y_lb) && (y <= m_2D_Area.y_rt)) {
          ang_mark(x, y, GCLC_DIR_L, i2s(i));
          iRv = ang_fsegment(x - dash_halflen, y, x + dash_halflen, y);
          if (iRv != rvGCLCOK)
            return iRv;
        }
      }
    }

    i = 0;
    x = X_ORIGIN;
    if (ANG_LOG == 1)
      y = ang_y(-1);
    else
      y = ang_y(0.1);
    while (y >= m_2D_Area.y_lb) {
      if (ANG_LOG == 1) {
        i++;
        y = ang_y(-i);
      } else {
        i = (i == 0 ? 10 : i * 10);
        y = ang_y(1.0 / i);
      }

      if (((ANG_LOG == 1) && !(i % y_step)) ||
          ((ANG_LOG == 2) && !(log_div(i, y_step)))) {
        std::string name;
        if (ANG_LOG == 1)
          name = i2s(-i);
        else {
          name = d2s(1.0 / i, -1);
          trimrightzeros(name);
        }
        ang_mark(x, y, GCLC_DIR_L, name);
        iRv = ang_fsegment(x - dash_halflen, y, x + dash_halflen, y);
        if (iRv != rvGCLCOK)
          return iRv;
      }
    }
  } break;

  case 3:
    break;

  default:
    break;
  }

  switch (mark_axes) {
  case 1: {
    // denoting axes
    ang_mark(m_2D_Area.x_rt, Y_ORIGIN, GCLC_DIR_B, "x");
    ang_mark(X_ORIGIN, m_2D_Area.y_rt, GCLC_DIR_L, "y");
  } break;
  case 2:
    break;
  default:
    break;
  }

  switch (arrows) {
  case 1:
    if ((Y_ORIGIN >= m_2D_Area.y_lb) && (Y_ORIGIN <= m_2D_Area.y_rt)) {
      DrawSegment(m_2D_Area.x_lb, Y_ORIGIN, m_2D_Area.x_rt, Y_ORIGIN, true,
                  false, m_2D_Area);
      ang_arrow(m_2D_Area.x_lb, Y_ORIGIN, m_2D_Area.x_rt, Y_ORIGIN);
      ang_arrow(m_2D_Area.x_rt, Y_ORIGIN, m_2D_Area.x_lb, Y_ORIGIN);
    }
    if ((X_ORIGIN >= m_2D_Area.x_lb) && (X_ORIGIN <= m_2D_Area.x_rt)) {
      DrawSegment(X_ORIGIN, m_2D_Area.y_lb, X_ORIGIN, m_2D_Area.y_rt, true,
                  false, m_2D_Area);
      ang_arrow(X_ORIGIN, m_2D_Area.y_lb, X_ORIGIN, m_2D_Area.y_rt);
      ang_arrow(X_ORIGIN, m_2D_Area.y_rt, X_ORIGIN, m_2D_Area.y_lb);
    }
    break;
  case 2:
    if ((Y_ORIGIN >= m_2D_Area.y_lb) && (Y_ORIGIN <= m_2D_Area.y_rt)) {
      DrawSegment(m_2D_Area.x_lb, Y_ORIGIN, m_2D_Area.x_rt, Y_ORIGIN, true,
                  false, m_2D_Area);
      ang_arrow(m_2D_Area.x_lb, Y_ORIGIN, m_2D_Area.x_rt, Y_ORIGIN);
    }
    if ((X_ORIGIN >= m_2D_Area.x_lb) && (X_ORIGIN <= m_2D_Area.x_rt)) {
      DrawSegment(X_ORIGIN, m_2D_Area.y_lb, X_ORIGIN, m_2D_Area.y_rt, true,
                  false, m_2D_Area);
      ang_arrow(X_ORIGIN, m_2D_Area.y_lb, X_ORIGIN, m_2D_Area.y_rt);
    }
    break;

  case 3:
    iRv = ang_fsegment(m_2D_Area.x_lb, Y_ORIGIN, m_2D_Area.x_rt, Y_ORIGIN);
    if (iRv != rvGCLCOK)
      return iRv;
    iRv = ang_fsegment(X_ORIGIN, m_2D_Area.y_lb, X_ORIGIN, m_2D_Area.y_rt);
    if (iRv != rvGCLCOK)
      return iRv;
    break;

  default:
    break;
  }

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::ang_point() {
  std::string sPointName;
  double x, y;
  GCLCError iRv;

  if (ReadToken(sPointName) != rvGCLCOK)
    return rvGCLCIdExpected;
  if ((iRv = ReadNumber(x)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(y)) != rvGCLCOK)
    return iRv;

  return Let(sPointName, GCLC_POINT, X_ORIGIN + UNIT * x, ang_y(y), 0.00, 0.00, 0.00,
             0.00);
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::ang_getx() {
  std::string sNumberName;
  GCLC_object o1;
  GCLCError iRv;

  if (ReadToken(sNumberName) != rvGCLCOK)
    return rvGCLCIdExpected;
  if ((iRv = ReadObject(GCLC_POINT, o1)) != rvGCLCOK)
    return iRv;

  return Let(sNumberName, GCLC_NUMBER, (o1.p[0] - X_ORIGIN) / UNIT, 0.00, 0.00,
             0.00, 0.00, 0.00);
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::ang_gety() {
  std::string sNumberName;
  GCLC_object o1;
  GCLCError iRv;

  if (ReadToken(sNumberName) != rvGCLCOK)
    return rvGCLCIdExpected;
  if ((iRv = ReadObject(GCLC_POINT, o1)) != rvGCLCOK)
    return iRv;

  return Let(sNumberName, GCLC_NUMBER, ang_y_inv(o1.p[1]), 0.00, 0.00, 0.00,
             0.00, 0.00);
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::ang_line() {
  std::string sLineName;
  double a, b, c;
  GCLCError iRv;

  if (ReadToken(sLineName) != rvGCLCOK)
    return rvGCLCIdExpected;
  if ((iRv = ReadNumber(a)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(b)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(c)) != rvGCLCOK)
    return iRv;

  return Let(sLineName, GCLC_LINE, a / UNIT, b / UNIT,
             (-a * X_ORIGIN - b * Y_ORIGIN) / UNIT + c, 0.00, 0.00, 0.00);
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::ang_conic() {
  std::string sConicName;
  GCLCError iRv;
  double a, b, c, d, e, f;

  if (ReadToken(sConicName) != rvGCLCOK)
    return rvGCLCIdExpected;
  if ((iRv = ReadNumber(a)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(b)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(c)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(d)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(e)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(f)) != rvGCLCOK)
    return iRv;

  return Let(sConicName, GCLC_CONIC, a, b, c, d, e, f);
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::ang_intersec2() {
  std::string sPointName1, sPointName2;
  GCLC_object o1, o2;
  GCLCError iRv;
  double x1, y1, x2, y2, a, b, c, d, e, f, a1, b1, c1, a0, b0, c0, d0;

  if (ReadToken(sPointName1) != rvGCLCOK)
    return rvGCLCIdExpected;
  if ((iRv = ReadToken(sPointName2)) != rvGCLCOK)
    return rvGCLCIdExpected;
  if ((iRv = ReadNextObject(o1)) != rvGCLCOK)
    return iRv;
  if ((o1.type != GCLC_LINE) && (o1.type != GCLC_CONIC))
    return rvGCLCWrongType;

  if ((iRv = ReadNextObject(o2)) != rvGCLCOK)
    return iRv;
  if ((o2.type != GCLC_LINE) && (o2.type != GCLC_CONIC))
    return rvGCLCWrongType;

  if (((o1.type != GCLC_CONIC) || (o2.type != GCLC_LINE)) &&
      ((o2.type != GCLC_CONIC) || (o1.type != GCLC_LINE)))
    return rvGCLCWrongType;

  if (o1.type == GCLC_CONIC) {
    a = o1.p[0];
    b = o1.p[1];
    c = o1.p[2];
    d = o1.p[3];
    e = o1.p[4];
    f = o1.p[5];
    a1 = o2.p[0];
    b1 = o2.p[1];
    c1 = o2.p[2];
  } else {
    a = o2.p[0];
    b = o2.p[1];
    c = o2.p[2];
    d = o2.p[3];
    e = o2.p[4];
    f = o2.p[5];
    a1 = o1.p[0];
    b1 = o1.p[1];
    c1 = o1.p[2];
  }

  if (fabs(b1) >
      EPS) // transform to the ang system, changes w.r.t. ang_scale 22.01.2006
  {
    x1 = 0;
    y1 = ang_y_inv(-(c1 + a1 * (x1 * UNIT + X_ORIGIN)) / b1);
    x2 = 1;
    y2 = ang_y_inv(-(c1 + a1 * (x2 * UNIT + X_ORIGIN)) / b1);

    b1 = 1.0;
    c1 = -y1;
    a1 = -c1 - y2;
  } else {
    c1 = -(-(c1 / a1) - X_ORIGIN) / UNIT;
    b1 = 0.0;
    a1 = 1.0;
  }

  if (fabs(a1) > EPS) {
    a0 = (a * b1 * b1 / (a1 * a1)) - (2 * b * b1 / a1) + c;
    b0 = (2 * a * b1 * c1 / (a1 * a1)) - (2 * b * c1 / a1) - (2 * d * b1 / a1) +
         2 * e;
    c0 = a * c1 * c1 / (a1 * a1) - (2 * d * c1 / a1) + f;

    if (fabs(a0) < EPS) {
      y1 = -c0 / b0;
      x1 = -(c1 + b1 * y1) / a1;
      x2 = x1;
      y2 = y1;
    } else {
      d0 = b0 * b0 - 4 * a0 * c0;
      if (d0 < 0)
        return rvGCLCBadIntersection;
      y1 = (-b0 + sqrt(d0)) / (2 * a0);
      x1 = -(c1 + b1 * y1) / a1;
      y2 = (-b0 - sqrt(d0)) / (2 * a0);
      x2 = -(c1 + b1 * y2) / a1;
    }

    x1 = x1 * UNIT + X_ORIGIN;
    y1 = ang_y(y1);
    x2 = x2 * UNIT + X_ORIGIN;
    y2 = ang_y(y2);

  } else {
    a0 = (b * a1 * a1 / (b1 * b1)) - (2 * b * a1 / b1) + a;
    b0 = (2 * c * a1 * c1 / (b1 * b1)) - (2 * b * c1 / b1) - (2 * e * a1 / b1) +
         2 * d;
    c0 = c * c1 * c1 / (b1 * b1) - (2 * e * c1 / b1) + f;

    if (fabs(a0) < EPS) {
      x1 = -c0 / b0;
      y1 = -(c1 + a1 * x1) / b1;
      x2 = x1;
      y2 = y1;
    } else {
      d0 = b0 * b0 - 4 * a0 * c0;
      if (d0 < 0)
        return rvGCLCBadIntersection;

      x1 = (-b0 + sqrt(d0)) / (2 * a0);
      y1 = -(c1 + a1 * x1) / b1;
      x2 = (-b0 - sqrt(d0)) / (2 * a0);
      y2 = -(c1 + a1 * x2) / b1;
    }

    x1 = x1 * UNIT + X_ORIGIN;
    y1 = ang_y(y1);
    x2 = x2 * UNIT + X_ORIGIN;
    y2 = ang_y(y2);
  }

  if ((iRv = Let(sPointName1, GCLC_POINT, x1, y1, 0.00, 0.00, 0.00, 0.00)) !=
      rvGCLCOK)
    return iRv;
  return Let(sPointName2, GCLC_POINT, x2, y2, 0.00, 0.00, 0.00, 0.00);
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::ang_tangent() {
  std::string sLineName;
  GCLC_object o1, o2;
  GCLCError iRv;
  double A, B, C, D, E, a, b, c;
  double x, y;
  double x1, y1, x2, y2;

  if (ReadToken(sLineName) != rvGCLCOK)
    return rvGCLCIdExpected;
  if ((iRv = ReadObject(GCLC_POINT, o1)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadObject(GCLC_CONIC, o2)) != rvGCLCOK)
    return iRv;

  x = (o1.p[0] - X_ORIGIN) / UNIT;
  y = ang_y_inv(o1.p[1]);

  A = o2.p[0];
  B = o2.p[1];
  C = o2.p[2];
  D = o2.p[3];
  E = o2.p[4];

  a = 2 * A * x + 2 * B * y + 2 * D;
  b = (2 * B * x + 2 * C * y + 2 * E);
  c = -a * x - b * y;

  if (fabs(b) > EPS) // changes w.r.t. ang_scale 22.01.2006
  {
    x1 = o1.p[0];
    y1 = ang_y(-(c + a * (x1 - X_ORIGIN) / UNIT) / b);

    x2 = (fabs(x1 - X_ORIGIN) < EPS) ? X_ORIGIN + UNIT : X_ORIGIN;
    y2 = ang_y(-(c + a * (x2 - X_ORIGIN) / UNIT) / b);

    a = -(y2 - y1) / (x2 - x1);
    b = 1.0;
    c = -a * x1 - y1;
  } else {
    c = -((-c / a) * UNIT + X_ORIGIN);
    a = 1.0;
    b = 0.0;
  }

  return Let(sLineName, GCLC_LINE, a, b, c, 0.00, 0.00, 0.00);
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::ang_draw_line() {
  GCLC_object o1;
  GCLCError iRv;

  if ((iRv = ReadObject(GCLC_LINE, o1)) != rvGCLCOK)
    return iRv;

  if (PrintComment("Drawing 2D ang line " + o1.name) != rvG_OK)
    return rvCannotExport;

  if (o1.p[1] != 0)
    return ang_fline(0.00, (-o1.p[2]) / o1.p[1], 1.00,
                     (-o1.p[0] - o1.p[2]) / o1.p[1]);
  else
    return ang_fline((-o1.p[2]) / o1.p[0], 0.00, (-o1.p[1] - o1.p[2]) / o1.p[0],
                     1.00);
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::ang_draw_line_p() {
  GCLC_object o1, o2;
  GCLCError iRv;

  if ((iRv = ReadObject(GCLC_POINT, o1)) != rvGCLCOK)
    return iRv;

  if ((iRv = ReadObject(GCLC_POINT, o2)) != rvGCLCOK)
    return iRv;

  if ((m_2D_Area.x_lb == 0) && (m_2D_Area.x_rt == 0) && (m_2D_Area.y_lb == 0) &&
      (m_2D_Area.y_rt == 0))
    return rvGCLCOK;

  if (PrintComment("Drawing 2D ang line " + o1.name + " " + o2.name) != rvG_OK)
    return rvCannotExport;

  return ang_fline(o1.p[0], o1.p[1], o2.p[0], o2.p[1]);
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::ang_draw_conic(int dashed) {
  GCLC_object o1;
  double A, B, C, D, E, F;
  double y1, y2, y3, y4, y5, y6, x0, x_scr, scr_unit, x1, x2, x5, x_aux;
  int r1, r2, r3, r, num, i;

  GCLCError iRv;

  if ((iRv = ReadObject(GCLC_CONIC, o1)) != rvGCLCOK)
    return iRv;
  A = o1.p[0];
  B = o1.p[1];
  C = o1.p[2];
  D = o1.p[3];
  E = o1.p[4];
  F = o1.p[5];

  if (PrintComment("Drawing 2D ang conic " + o1.name) != rvG_OK)
    return rvCannotExport;

  num = CONIC_PRECISION;

  scr_unit = (m_2D_Area.x_rt - m_2D_Area.x_lb) / (double)num;

  x0 = (-X_ORIGIN) / UNIT;
  solve(C, 2 * B * x0 + 2 * E, A * x0 * x0 + 2 * D * x0 + F, y3, y4, r2);
  x_scr = -scr_unit + m_2D_Area.x_lb;
  for (i = 0; i <= num; i++) {
    x_scr = x_scr + scr_unit;
    x0 = (x_scr - X_ORIGIN) / UNIT;
    y1 = 0;
    y2 = 0;
    r1 = 0;
    r = solve(C, 2 * B * x0 + 2 * E, A * x0 * x0 + 2 * D * x0 + F, y1, y2, r1);
    if (i >= 0) {
      if (r1 == 0) {
        if (r2 == 2) {
          solve(B * B - A * C, 2 * E * B - 2 * D * C, E * E - C * F, x1, x2, r);
          if (x1 > x2) {
            x_aux = x1;
            x1 = x2;
            x2 = x_aux;
          }
          if (x2 > x0) {
            r = solve(C, 2 * B * x1 + 2 * E, A * x1 * x1 + 2 * D * x1 + F, y5,
                      y6, r3);
            x5 = ((X_ORIGIN + UNIT * x1) + (x_scr - scr_unit)) / 2;
            if (!dashed || !(i % 2))
              ang_fsegment(X_ORIGIN + UNIT * x1, ang_y(y5), x5,
                           ang_y((y3 + y5) / 2));
            if (!dashed || (i % 2))
              ang_fsegment(x5, ang_y((y5 + y3) / 2), x_scr - scr_unit,
                           ang_y(y3));
            if (!dashed || (i % 2))
              ang_fsegment(X_ORIGIN + UNIT * x1, ang_y(y5), x5,
                           ang_y((y4 + y5) / 2));
            if (!dashed || !(i % 2))
              ang_fsegment(x5, ang_y((y5 + y4) / 2), x_scr - scr_unit,
                           ang_y(y4));
          } else {
            r = solve(C, 2 * B * x2 + 2 * E, A * x2 * x2 + 2 * D * x2 + F, y5,
                      y6, r3);
            x5 = ((X_ORIGIN + UNIT * x2) + (x_scr - scr_unit)) / 2;
            if (!dashed || !(i % 2))
              ang_fsegment(X_ORIGIN + UNIT * x2, ang_y(y5), x5,
                           ang_y((y3 + y5) / 2));
            if (!dashed || (i % 2))
              ang_fsegment(x5, ang_y((y5 + y3) / 2), x_scr - scr_unit,
                           ang_y(y3));
            if (!dashed || (i % 2))
              ang_fsegment(X_ORIGIN + UNIT * x2, ang_y(y5), x5,
                           ang_y((y4 + y5) / 2));
            if (!dashed || !(i % 2))
              ang_fsegment(x5, ang_y((y5 + y4) / 2), x_scr - scr_unit,
                           ang_y(y4));
          }
        }
      } else if (r1 == 1) {
        if (r2 == 2) {
          if (!dashed || (i % 2))
            ang_fsegment(x_scr - scr_unit, ang_y(y3), x_scr, ang_y(y1));
          if (!dashed || !(i % 2))
            ang_fsegment(x_scr - scr_unit, ang_y(y4), x_scr, ang_y(y1));
        } else if (r2 == 1) {
          if (!dashed || (i % 2))
            ang_fsegment(x_scr - scr_unit, ang_y(y3), x_scr, ang_y(y1));
        }
      } else if (r1 == 2) {
        if (r2 == 0) {
          solve(B * B - A * C, 2 * E * B - 2 * D * C, E * E - C * F, x1, x2, r);
          if (x1 > x2) {
            x_aux = x1;
            x1 = x2;
            x2 = x_aux;
          }
          if (x2 > x0) {
            r = solve(C, 2 * B * x1 + 2 * E, A * x1 * x1 + 2 * D * x1 + F, y5,
                      y6, r3);
            x5 = ((X_ORIGIN + UNIT * x1) + x_scr) / 2;
            if (!dashed || !(i % 2))
              ang_fsegment(X_ORIGIN + UNIT * x1, ang_y(y5), x5,
                           ang_y((y1 + y5) / 2));
            if (!dashed || (i % 2))
              ang_fsegment(x5, ang_y((y5 + y1) / 2), x_scr, ang_y(y1));
            if (!dashed || (i % 2))
              ang_fsegment(X_ORIGIN + UNIT * x1, ang_y(y5), x5,
                           ang_y((y2 + y5) / 2));
            if (!dashed || !(i % 2))
              ang_fsegment(x5, ang_y((y5 + y2) / 2), x_scr, ang_y(y2));
          } else {
            r = solve(C, 2 * B * x2 + 2 * E, A * x2 * x2 + 2 * D * x2 + F, y5,
                      y6, r3);
            x5 = ((X_ORIGIN + UNIT * x2) + x_scr) / 2;
            if (!dashed || !(i % 2))
              ang_fsegment(X_ORIGIN + UNIT * x2, ang_y(y5), x5,
                           ang_y((y1 + y5) / 2));
            if (!dashed || (i % 2))
              ang_fsegment(x5, ang_y((y5 + y1) / 2), x_scr, ang_y(y1));
            if (!dashed || (i % 2))
              ang_fsegment(X_ORIGIN + UNIT * x2, ang_y(y5), x5,
                           ang_y((y2 + y5) / 2));
            if (!dashed || !(i % 2))
              ang_fsegment(x5, ang_y((y5 + y2) / 2), x_scr, ang_y(y2));
          }
        } else if (r2 == 1) {
          if (!dashed || (i % 2))
            ang_fsegment(x_scr - scr_unit, ang_y(y3), x_scr, ang_y(y1));
          if (!dashed || !(i % 2))
            ang_fsegment(x_scr - scr_unit, ang_y(y3), x_scr, ang_y(y2));
        } else if (r2 == 2) {
          if (y3 > y4) {
            if (y1 > y2) {
              if (!dashed || (i % 2))
                ang_fsegment(x_scr - scr_unit, ang_y(y3), x_scr, ang_y(y1));
              if (!dashed || !(i % 2))
                ang_fsegment(x_scr - scr_unit, ang_y(y4), x_scr, ang_y(y2));
            } else {
              if (!dashed || !(i % 2))
                ang_fsegment(x_scr - scr_unit, ang_y(y4), x_scr, ang_y(y1));
              if (!dashed || (i % 2))
                ang_fsegment(x_scr - scr_unit, ang_y(y3), x_scr, ang_y(y2));
            }
          } else {
            if (y1 > y2) {
              if (!dashed || (i % 2))
                ang_fsegment(x_scr - scr_unit, ang_y(y4), x_scr, ang_y(y1));
              if (!dashed || !(i % 2))
                ang_fsegment(x_scr - scr_unit, ang_y(y3), x_scr, ang_y(y2));
            } else {
              if (!dashed || !(i % 2))
                ang_fsegment(x_scr - scr_unit, ang_y(y3), x_scr, ang_y(y1));
              if (!dashed || (i % 2))
                ang_fsegment(x_scr - scr_unit, ang_y(y4), x_scr, ang_y(y2));
            }
          }
        }
      } // r1==1
      r2 = r1;
      y3 = y1;
      y4 = y2;
    }
  }

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::ang_draw_parametric_curve() {
  std::string sNumberName;
  GCLCError iRv;
  double dNumber;
  std::string text;
  std::string init_text;
  std::string cond_text;
  std::string iter_text;
  std::string x_text;
  std::string y_text;
  double x, y, x_old = 0, y_old = 0;

  if (ReadToken(sNumberName) != rvGCLCOK)
    return rvGCLCIdExpected;

  bool bSupressWarnings = m_bSupressWarnings;
  m_bSupressWarnings = true;

  if ((iRv = Let(sNumberName, GCLC_NUMBER, 0, 0.00, 0.00, 0.00, 0.00, 0.00)) !=
      rvGCLCOK) {
    m_bSupressWarnings = bSupressWarnings;
    return iRv;
  }

  if ((iRv = take_text(text)) != rvGCLCOK) {
    m_bSupressWarnings = bSupressWarnings;
    return iRv;
  }

  if (text.find(';') != std::string::npos) {
    init_text = text.substr(0, text.find(';'));
    text = text.substr(text.find(';') + 1);
  } else {
    m_bSupressWarnings = bSupressWarnings;
    return rvGCLCSeparatorExpected;
  }

  if (text.find(';') != std::string::npos) {
    cond_text = text.substr(0, text.find(';'));
    text = text.substr(text.find(';') + 1);
  } else {
    m_bSupressWarnings = bSupressWarnings;
    return rvGCLCSeparatorExpected;
  }
  iter_text = text;

  if ((iRv = take_text(text)) != rvGCLCOK) {
    m_bSupressWarnings = bSupressWarnings;
    return iRv;
  }

  if (text.find(';') != std::string::npos) {
    x_text = text.substr(0, text.find(';'));
    text = text.substr(text.find(';') + 1);
  } else {
    m_bSupressWarnings = bSupressWarnings;
    return rvGCLCSeparatorExpected;
  }
  y_text = text;

  if (PrintComment("Drawing 2D parametric curve (" + x_text + "," + y_text +
                   ")") != rvG_OK)
    return rvCannotExport;

  bool bFirst = true;

  double v;
  if ((iRv = calc_expression(init_text, v)) != rvGCLCOK) {
    m_bSupressWarnings = bSupressWarnings;
    return iRv;
  }
  if ((iRv = Let(sNumberName, GCLC_NUMBER, v, 0.00, 0.00, 0.00, 0.00, 0.00)) !=
      rvGCLCOK) {
    m_bSupressWarnings = bSupressWarnings;
    return iRv;
  }
  if ((iRv = calc_expression(cond_text, dNumber)) != rvGCLCOK) {
    m_bSupressWarnings = bSupressWarnings;
    return iRv;
  }

  bool bDefined;
  while (dNumber == 1.0) {
    bDefined = true;

    if ((iRv = calc_expression(x_text, x)) != rvGCLCOK)
      bDefined = false;
    if ((iRv = calc_expression(y_text, y)) != rvGCLCOK)
      bDefined = false;
    if (!bFirst)
      if ((ang_y(y_old) < 1 * m_2D_Area.y_lb) &&
          (ang_y(y_old) > 1 * m_2D_Area.y_rt))
        bDefined = false;
    if ((ang_y(y) > 1 * m_2D_Area.y_rt) && (ang_y(y) < 1 * m_2D_Area.y_lb))
      bDefined = false;

    if (!bFirst && bDefined) {
      // connect to the previous one
      if ((iRv = ang_fsegment(X_ORIGIN + UNIT * x, ang_y(y),
                              X_ORIGIN + UNIT * x_old, ang_y(y_old))) !=
          rvGCLCOK) {
        m_bSupressWarnings = bSupressWarnings;
        return iRv;
      }
    }
    bFirst = !bDefined;

    x_old = x;
    y_old = y;

    if ((iRv = calc_expression(iter_text, v)) != rvGCLCOK) {
      m_bSupressWarnings = bSupressWarnings;
      return iRv;
    }
    if ((v > 0 && v < EPS) || (v < 0 && v > -EPS))
      v = 0;

    if ((iRv = Let(sNumberName, GCLC_NUMBER, v, 0.00, 0.00, 0.00, 0.00,
                   0.00)) != rvGCLCOK) {
      m_bSupressWarnings = bSupressWarnings;
      return iRv;
    }
    if ((iRv = calc_expression(cond_text, dNumber)) != rvGCLCOK) {
      m_bSupressWarnings = bSupressWarnings;
      return iRv;
    }
  }
  m_bSupressWarnings = bSupressWarnings;
  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_conicprecision() {
  double num;
  GCLCError iRv;
  if ((iRv = ReadNumber(num)) != rvGCLCOK)
    return iRv;
  CONIC_PRECISION = (int)num;
  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::ang_plot_data() {
  GReturnValue rv;
  GCLCError iRv;
  bool bMarkDots;
  bool bFirst = true;
  unsigned char c;
  double d, x, y, x_old, y_old;

  if ((iRv = ReadNumber(d)) != rvGCLCOK)
    return iRv;
  bMarkDots = (d != 0);

  do {
    if ((rv = m_Input->ReadChar(c)) != rvG_OK)
      return rvGCLCLBracketExpected;
    if (rv == rvG_InvalidInput)
      return rvGCLCInvalidInput;
  } while ((c == ' ') || (c == '\n') || (c == 9) || (c == '\t') || (c == '\r'));

  if (c != '{')
    return rvGCLCLBracketExpected;

  for (;;) {
    iRv = ReadToken();
    if (LastToken() == "}")
      return rvGCLCOK;

    if (iRv != rvGCLCOK)
      return rvGCLCIdOrNumberExpected;
    convert(LastToken(), x);

    if ((iRv = ReadToken()) != rvGCLCOK)
      return rvGCLCIdOrNumberExpected;
    convert(LastToken(), y);

    if (bMarkDots) {
      if ((iRv = ang_cmark(X_ORIGIN + UNIT * x, ang_y(y), 0, "")) != rvGCLCOK)
        return iRv;
    }

    if (bFirst)
      bFirst = false;
    else {
      // connect to the previous one
      if ((iRv = ang_fsegment(X_ORIGIN + UNIT * x, ang_y(y),
                              X_ORIGIN + UNIT * x_old, ang_y(y_old))) !=
          rvGCLCOK)
        return iRv;
    }
    x_old = x;
    y_old = y;
  }

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::ang_mark(double x0, double y0, int dir, const std::string &name) {
  return ang_2D_3D_mark(x0, y0, dir, name, m_2D_Area);
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::ang_cmark(double x0, double y0, int dir, const std::string &name) {
  return ang_2D_3D_cmark(x0, y0, dir, name, m_2D_Area);
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::ang_arrow(double x1, double y1, double x2, double y2) {
  return ang_2D_3D_arrow(x1, y1, x2, y2, m_2D_Area);
}

// ----------------------------------------------------------------------------

double CGCLC::ang_y(double y) {
  if (ANG_LOG == 1)
    return y * UNIT * ANG_SCALE + Y_ORIGIN;
  else
    return log10(y) * UNIT * ANG_SCALE + Y_ORIGIN;
}

// ----------------------------------------------------------------------------

double CGCLC::ang_y_inv(double y) {
  if (ANG_LOG == 1)
    return (y - Y_ORIGIN) / (UNIT * ANG_SCALE);
  else
    return pow(10.0, ((y - Y_ORIGIN) / (UNIT * ANG_SCALE)));
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::ang_fsegment(double x1, double y1, double x2, double y2) {
  if ((m_2D_Area.x_lb == 0) && (m_2D_Area.x_rt == 0) && (m_2D_Area.y_lb == 0) &&
      (m_2D_Area.y_rt == 0))
    return rvGCLCOK;

  if (DrawSegment(x1, y1, x2, y2, true, false, m_2D_Area) != rvG_OK)
    return rvCannotExport;
  else
    return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::ang_fline(double x1, double y1, double x2, double y2) {
  if ((m_2D_Area.x_lb == 0) && (m_2D_Area.x_rt == 0) && (m_2D_Area.y_lb == 0) &&
      (m_2D_Area.y_rt == 0))
    return rvGCLCOK;

  if (DrawLineSensitive(x1, y1, x2, y2, false, m_2D_Area) != rvG_OK)
    return rvCannotExport;
  else
    return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::ang_2D_3D_mark(double x0, double y0, int dir,
                                const std::string &name, GCLC_area &area) {
  std::string pos;
  double x, y;

  if ((area.x_lb == 0) && (area.x_rt == 0) && (area.y_lb == 0) &&
      (area.y_rt == 0))
    return rvGCLCOK;

  switch (dir) {
  case GCLC_DIR_LT:
    x = x0 - DM;
    y = y0 + DM;
    pos = "[rb]";
    break;
  case GCLC_DIR_L:
    x = x0 - DM;
    y = y0;
    pos = "[r]";
    break;
  case GCLC_DIR_LB:
    x = x0 - DM;
    y = y0 - DM;
    pos = "[rt]";
    break;
  case GCLC_DIR_T:
    x = x0;
    y = y0 + DM;
    pos = "[b]";
    break;
  case GCLC_DIR_B:
    x = x0;
    y = y0 - DM;
    pos = "[t]";
    break;
  case GCLC_DIR_RT:
    x = x0 + DM;
    y = y0 + DM;
    pos = "[lb]";
    break;
  case GCLC_DIR_R:
    x = x0 + DM;
    y = y0;
    pos = "[l]";
    break;
  case GCLC_DIR_RB:
    x = x0 + DM;
    y = y0 - DM;
    pos = "[lt]";
    break;
  default:
    x = x0;
    y = y0;
    pos = "";
    break;
  }

  if (dir != 0) {
    PrintText(x, y, pos, name, area);
  }
  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::ang_2D_3D_cmark(double x0, double y0, int dir,
                                 const std::string &name, GCLC_area &area) {
  std::string pos;
  double x, y;

  if ((area.x_lb == 0) && (area.x_rt == 0) && (area.y_lb == 0) &&
      (area.y_rt == 0))
    return rvGCLCOK;

  DrawCircle(x0, y0, x0 + MARK_RADIUS, y0, false, false, area,
             MARK_CIRCLE_PRECISION);
  EmptyCircle(x0, y0, x0 + MARK_RADIUS, y0, area);

  switch (dir) {
  case GCLC_DIR_LT:
    x = x0 - DM;
    y = y0 + DM;
    pos = "[rb]";
    break;
  case GCLC_DIR_L:
    x = x0 - DM;
    y = y0;
    pos = "[r]";
    break;
  case GCLC_DIR_LB:
    x = x0 - DM;
    y = y0 - DM;
    pos = "[rt]";
    break;
  case GCLC_DIR_T:
    x = x0;
    y = y0 + DM;
    pos = "[b]";
    break;
  case GCLC_DIR_B:
    x = x0;
    y = y0 - DM;
    pos = "[t]";
    break;
  case GCLC_DIR_RT:
    x = x0 + DM;
    y = y0 + DM;
    pos = "[lb]";
    break;
  case GCLC_DIR_R:
    x = x0 + DM;
    y = y0;
    pos = "[l]";
    break;
  case GCLC_DIR_RB:
    x = x0 + DM;
    y = y0 - DM;
    pos = "[lt]";
    break;
  default:
    x = x0;
    y = y0;
    pos = "";
    break;
  }

  if (dir != 0)
    PrintText(x, y, pos, name, area);

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::ang_2D_3D_arrow(double x1, double y1, double x2, double y2,
                                 GCLC_area &area) {
  GCLC_object o1, o2;
  double d, c, s, x, y, x0, y0;

  if ((area.x_lb == 0) && (area.x_rt == 0) && (area.y_lb == 0) &&
      (area.y_rt == 0))
    return rvGCLCOK;

  o1.p[0] = x1;
  o1.p[1] = y1;
  o2.p[0] = x2;
  o2.p[1] = y2;

  d = sqrt((o1.p[0] - o2.p[0]) * (o1.p[0] - o2.p[0]) +
           (o1.p[1] - o2.p[1]) * (o1.p[1] - o2.p[1]));
  x0 = (o1.p[0] - o2.p[0]) / d + o2.p[0];
  y0 = (o1.p[1] - o2.p[1]) / d + o2.p[1];

  c = cos(PI / 24);
  s = sin(PI / 24);
  x = (3 / d) * ((o1.p[0] - o2.p[0]) * c + (o1.p[1] - o2.p[1]) * s) + o2.p[0];
  y = (3 / d) * (-(o1.p[0] - o2.p[0]) * s + (o1.p[1] - o2.p[1]) * c) + o2.p[1];
  DrawSegment(x, y, o2.p[0], o2.p[1], true, false, area);
  DrawSegment(x, y, x0, y0, true, false, area);

  s = -s;
  x = (3 / d) * ((o1.p[0] - o2.p[0]) * c + (o1.p[1] - o2.p[1]) * s) + o2.p[0];
  y = (3 / d) * (-(o1.p[0] - o2.p[0]) * s + (o1.p[1] - o2.p[1]) * c) + o2.p[1];
  DrawSegment(x, y, o2.p[0], o2.p[1], true, false, area);
  DrawSegment(x, y, x0, y0, true, false, area);

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------
