#include "GCLC.h"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

// ----------------------------------------------------------------------------

GCLCError CGCLC::ang3d_picture() {
  GCLCError iRv;

  if ((iRv = ReadNumber(m_3D_Area.x_lb)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(m_3D_Area.y_lb)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(m_3D_Area.x_rt)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(m_3D_Area.y_rt)) != rvGCLCOK)
    return iRv;

  ANG3D_SCALE_Y = 1.00;
  ANG3D_SCALE_Z = 1.00;
  ANG3D_LOG = 1;
  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::ang3d_origin() {
  double dNumber;
  GCLCError iRv;

  if ((iRv = ReadNumber(X3D_ORIGIN)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(Y3D_ORIGIN)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(dNumber)) != rvGCLCOK)
    return iRv;
  SIN_ALPHA3D = sin(dNumber);
  COS_ALPHA3D = cos(dNumber);

  if ((iRv = ReadNumber(dNumber)) != rvGCLCOK)
    return iRv;

  SIN_BETA3D = sin(dNumber);
  COS_BETA3D = cos(dNumber);

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::ang3d_unit() { return ReadNumber(UNIT3D); }

// ----------------------------------------------------------------------------

GCLCError CGCLC::ang3d_scale() {
  double d;
  GCLCError iRv;

  if ((iRv = ReadNumber(d)) != rvGCLCOK)
    return iRv;

  ANG3D_LOG = (int)d;
  if (ANG3D_LOG < 1)
    ANG3D_LOG = 1;
  if (ANG3D_LOG > 2)
    ANG3D_LOG = 2;

  if ((iRv = ReadNumber(d)) != rvGCLCOK)
    return iRv;

  if (d <= 0)
    d = 1;
  ANG3D_SCALE_Y = d;

  if ((iRv = ReadNumber(d)) != rvGCLCOK)
    return iRv;

  if (d <= 0)
    d = 1;
  ANG3D_SCALE_Z = d;

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::ang3d_axes_drawing_range() {
  GCLCError iRv;

  if ((iRv = ReadNumber(X3D_MIN)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(X3D_MAX)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(Y3D_MIN)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(Y3D_MAX)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(Z3D_MIN)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(Z3D_MAX)) != rvGCLCOK)
    return iRv;
  ANG3D_AXES_LIMITED = true;

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::ang3d_draw_system_p() {
  GCLCError iRv;
  double number;
  int mark_integers, mark_axes, arrows, x_step, y_step, z_step;

  if ((iRv = ReadNumber(number)) != rvGCLCOK)
    return iRv;

  mark_integers = (int)number;
  if (mark_integers < 1)
    mark_integers = 1;
  if (mark_integers > 3)
    mark_integers = 3;

  if ((iRv = ReadNumber(number)) != rvGCLCOK)
    return iRv;

  x_step = (int)number;
  if (x_step < 1)
    x_step = 1;

  if ((iRv = ReadNumber(number)) != rvGCLCOK)
    return iRv;

  y_step = (int)number;
  if (y_step < 1)
    y_step = 1;

  if ((iRv = ReadNumber(number)) != rvGCLCOK)
    return iRv;

  z_step = (int)number;
  if (z_step < 1)
    z_step = 1;

  if ((iRv = ReadNumber(number)) != rvGCLCOK)
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

  return ang3d_draw_system(mark_integers, x_step, y_step, z_step, mark_axes,
                           arrows);
}

// ------------------------------------------------------------------------------

GCLCError CGCLC::ang3d_draw_system(int mark_integers, int x_step, int y_step,
                                   int z_step, int mark_axes, int arrows) {
  // char name[16];
  double x, y, z;
  double x1, y1, x2, y2;
  int i;
  double dash_halflen = 0.75;
  bool bOutAngArea;
  double cp, cn;
  bool bReachedMax;

  if ((m_3D_Area.x_lb == 0) && (m_3D_Area.x_rt == 0) && (m_3D_Area.y_lb == 0) &&
      (m_3D_Area.y_rt == 0))
    return rvGCLCOK;

  if (PrintComment("Drawing 3D Cartesian system") != rvG_OK)
    return rvCannotExport;

  GCLCError iRv;

  switch (mark_integers) {
  case 1: {
    i = 0;
    z = (ANG3D_LOG == 1 ? 0 : 1);
    bOutAngArea = false;
    do {
      x = ang3d_x(i, 0, z);
      y = ang3d_y(i, 0, z);
      if (!(i % x_step)) {
        if ((x >= m_3D_Area.x_lb) && (x <= m_3D_Area.x_rt) &&
            (y >= m_3D_Area.y_lb) && (y <= m_3D_Area.y_rt)) {
          if (i == 0)
            ang3d_cmark(x, y, GCLC_DIR_LB, i2s(i));
          else
            ang3d_cmark(x, y, GCLC_DIR_B, i2s(i));
        } else
          bOutAngArea = true;
      }
      i++;

      if (ANG3D_AXES_LIMITED)
        bReachedMax = (i > X3D_MAX);
      else
        bReachedMax = (i > 10.0 * (m_3D_Area.x_rt - m_3D_Area.x_lb) / UNIT3D);
    } while (!bOutAngArea && !bReachedMax);

    i = -1;
    bOutAngArea = false;
    do {
      x = ang3d_x(i, 0, z);
      y = ang3d_y(i, 0, z);
      if ((x >= m_3D_Area.x_lb) && (x <= m_3D_Area.x_rt) &&
          (y >= m_3D_Area.y_lb) && (y <= m_3D_Area.y_rt)) {
        if (!(i % x_step))
          ang3d_cmark(x, y, GCLC_DIR_B, i2s(i));
      } else
        bOutAngArea = true;
      i--;

      if (ANG3D_AXES_LIMITED)
        bReachedMax = (i < X3D_MIN);
      else
        bReachedMax = (i < -10.0 * (m_3D_Area.x_rt - m_3D_Area.x_lb) / UNIT3D);
    } while (!bOutAngArea && !bReachedMax);

    i = 0;
    bOutAngArea = false;
    do {
      x = ang3d_x(0, i, z);
      y = ang3d_y(0, i, z);
      if ((x >= m_3D_Area.x_lb) && (x <= m_3D_Area.x_rt) &&
          (y >= m_3D_Area.y_lb) && (y <= m_3D_Area.y_rt)) {
        if (!(i % y_step)) {
          if (i == 0)
            ang3d_cmark(x, y, GCLC_DIR_LB, i2s(i));
          else
            ang3d_cmark(x, y, GCLC_DIR_B, i2s(i));
        }
      } else
        bOutAngArea = true;
      i++;

      if (ANG3D_AXES_LIMITED)
        bReachedMax = (i > Y3D_MAX);
      else
        bReachedMax = (i > 10.0 * (m_3D_Area.y_rt - m_3D_Area.y_lb) / UNIT3D);
    } while (!bOutAngArea && !bReachedMax);

    i = -1;
    bOutAngArea = false;
    do {
      x = ang3d_x(0, i, z);
      y = ang3d_y(0, i, z);
      if ((x >= m_3D_Area.x_lb) && (x <= m_3D_Area.x_rt) &&
          (y >= m_3D_Area.y_lb) && (y <= m_3D_Area.y_rt)) {
        if (!(i % y_step))
          ang3d_cmark(x, y, GCLC_DIR_B, i2s(i));
      } else
        bOutAngArea = true;
      i--;

      if (ANG3D_AXES_LIMITED)
        bReachedMax = (i < Y3D_MIN);
      else
        bReachedMax = (i < -10.0 * (m_3D_Area.y_rt - m_3D_Area.y_lb) / UNIT3D);
    } while (!bOutAngArea && !bReachedMax);

    i = 0;
    bOutAngArea = false;
    z = 1;
    do {
      if (ANG3D_LOG == 1)
        i++;
      else
        i = (i == 0 ? 10 : i * 10);

      x = ang3d_x(0, 0, i);
      y = ang3d_y(0, 0, i);

      if (ANG3D_AXES_LIMITED)
        bReachedMax = (i > Z3D_MAX);
      else
        bReachedMax =
            (i > (ANG3D_LOG == 1
                      ? 10.0 * (m_3D_Area.x_rt - m_3D_Area.x_lb) / UNIT3D
                      : 1000000000));

      if ((x >= m_3D_Area.x_lb) && (x <= m_3D_Area.x_rt) &&
          (y >= m_3D_Area.y_lb) && (y <= m_3D_Area.y_rt) && !bReachedMax) {
        if (((ANG3D_LOG == 1) && !(i % z_step)) ||
            ((ANG3D_LOG == 2) && !(log_div(i, z_step)))) {
          ang3d_cmark(x, y, GCLC_DIR_L, i2s(i));
        }
      } else
        bOutAngArea = true;

      if (ANG3D_AXES_LIMITED)
        bReachedMax = (i + 1 > Z3D_MAX);
      else
        bReachedMax =
            (i + 1 > (ANG3D_LOG == 1
                          ? 10.0 * (m_3D_Area.x_rt - m_3D_Area.x_lb) / UNIT3D
                          : 1000000000));
    } while (!bOutAngArea && !bReachedMax);

    i = 0;
    if (ANG3D_LOG == 1)
      z = -1;
    else
      z = 0.1;
    bOutAngArea = false;
    do {
      if (ANG3D_LOG == 1) {
        i++;
        z = -i;
      } else {
        i = (i == 0 ? 10 : i * 10);
        z = 1.0 / i;
      }

      x = ang3d_x(0, 0, z);
      y = ang3d_y(0, 0, z);

      if (ANG3D_AXES_LIMITED)
        bReachedMax = (i > -Z3D_MIN);
      else
        bReachedMax =
            (i > (ANG3D_LOG == 1
                      ? 10.0 * (m_3D_Area.x_rt - m_3D_Area.x_lb) / UNIT3D
                      : 1000000000));

      if ((x >= m_3D_Area.x_lb) && (x <= m_3D_Area.x_rt) &&
          (y >= m_3D_Area.y_lb) && (y <= m_3D_Area.y_rt) && !bReachedMax) {
        if (((ANG3D_LOG == 1) && !(i % z_step)) ||
            ((ANG3D_LOG == 2) && !(log_div(i, z_step)))) {
          string name;
          if (ANG3D_LOG == 1)
            name = i2s(-i);
          else {
            name = d2s(1.0 / i, -1);
            trimrightzeros(name);
          }
          ang3d_cmark(x, y, GCLC_DIR_L, name);
        }
      } else
        bOutAngArea = true;

      if (ANG3D_AXES_LIMITED)
        bReachedMax = (i + 1 > -Z3D_MIN);
      else
        bReachedMax =
            (i + 1 > (ANG3D_LOG == 1
                          ? 10.0 * (m_3D_Area.x_rt - m_3D_Area.x_lb) / UNIT3D
                          : 1000000000));
    } while (!bOutAngArea && !bReachedMax);
  } break;

  case 2: {
    i = 0;
    if (ANG3D_LOG == 1)
      z = 0;
    else
      z = 1;
    bOutAngArea = false;
    do {
      if (!(i % x_step)) {
        x = ang3d_x(i, 0, z);
        y = ang3d_y(i, 0, z);
        if ((x >= m_3D_Area.x_lb) && (x <= m_3D_Area.x_rt) &&
            (y >= m_3D_Area.y_lb) && (y <= m_3D_Area.y_rt)) {
          if (i == 0)
            ang3d_mark(x, y, GCLC_DIR_LB, i2s(i));
          else
            ang3d_mark(x, y, GCLC_DIR_B, i2s(i));
          iRv = ang3d_fsegment(x, y - dash_halflen, x, y + dash_halflen);
          if (iRv != rvGCLCOK)
            return iRv;
        } else
          bOutAngArea = true;
      }
      i++;

      if (ANG3D_AXES_LIMITED)
        bReachedMax = (i > X3D_MAX);
      else
        bReachedMax = (i > 10.0 * (m_3D_Area.x_rt - m_3D_Area.x_lb) / UNIT3D);
    } while (!bOutAngArea && !bReachedMax);

    i = -1;
    bOutAngArea = false;
    do {
      if (!(i % x_step)) {
        x = ang3d_x(i, 0, z);
        y = ang3d_y(i, 0, z);

        if ((x >= m_3D_Area.x_lb) && (x <= m_3D_Area.x_rt) &&
            (y >= m_3D_Area.y_lb) && (y <= m_3D_Area.y_rt)) {
          ang3d_mark(x, y, GCLC_DIR_B, i2s(i));
          iRv = ang3d_fsegment(x, y - dash_halflen, x, y + dash_halflen);
          if (iRv != rvGCLCOK)
            return iRv;
        } else
          bOutAngArea = true;
      }

      i--;
      if (ANG3D_AXES_LIMITED)
        bReachedMax = (i < X3D_MIN);
      else
        bReachedMax = (i < -10.0 * (m_3D_Area.x_rt - m_3D_Area.x_lb) / UNIT3D);
    } while (!bOutAngArea && !bReachedMax);

    i = 1;
    bOutAngArea = false;
    do {
      if (!(i % y_step)) {
        x = ang3d_x(0, i, z);
        y = ang3d_y(0, i, z);
        if ((x >= m_3D_Area.x_lb) && (x <= m_3D_Area.x_rt) &&
            (y >= m_3D_Area.y_lb) && (y <= m_3D_Area.y_rt)) {
          ang3d_mark(x, y, GCLC_DIR_B, i2s(i));
          iRv = ang3d_fsegment(x, y - dash_halflen, x, y + dash_halflen);
          if (iRv != rvGCLCOK)
            return iRv;
        } else
          bOutAngArea = true;
      }
      i++;

      if (ANG3D_AXES_LIMITED)
        bReachedMax = (i > Y3D_MAX);
      else
        bReachedMax = (i > 10.0 * (m_3D_Area.x_rt - m_3D_Area.x_lb) / UNIT3D);
    } while (!bOutAngArea && !bReachedMax);

    i = -1;
    bOutAngArea = false;
    do {
      if (!(i % y_step)) {
        x = ang3d_x(0, i, z);
        y = ang3d_y(0, i, z);

        if ((x >= m_3D_Area.x_lb) && (x <= m_3D_Area.x_rt) &&
            (y >= m_3D_Area.y_lb) && (y <= m_3D_Area.y_rt)) {
          ang3d_mark(x, y, GCLC_DIR_B, i2s(i));
          iRv = ang3d_fsegment(x, y - dash_halflen, x, y + dash_halflen);
          if (iRv != rvGCLCOK)
            return iRv;
        } else
          bOutAngArea = true;
      }
      i--;
      if (ANG3D_AXES_LIMITED)
        bReachedMax = (i < Y3D_MIN);
      else
        bReachedMax = (i < -10.0 * (m_3D_Area.x_rt - m_3D_Area.x_lb) / UNIT3D);
    } while (!bOutAngArea && !bReachedMax);

    i = 0;
    z = 1;
    bOutAngArea = false;
    do {
      if (ANG3D_LOG == 1)
        i++;
      else
        i = (i == 0 ? 1 : i * 10);

      z = i;
      x = ang3d_x(0, 0, z);
      y = ang3d_y(0, 0, z);

      if ((x >= m_3D_Area.x_lb) && (x <= m_3D_Area.x_rt) &&
          (y >= m_3D_Area.y_lb) && (y <= m_3D_Area.y_rt)) {
        if (((ANG_LOG == 1) && !(i % z_step)) ||
            ((ANG_LOG == 2) && !(log_div(i, z_step)))) {
          ang3d_mark(x, y, GCLC_DIR_L, i2s(i));
          iRv = ang3d_fsegment(x - dash_halflen, y, x + dash_halflen, y);
          if (iRv != rvGCLCOK)
            return iRv;
        }
      } else
        bOutAngArea = true;

      if (ANG3D_AXES_LIMITED)
        bReachedMax = (i + 1 > Z3D_MAX);
      else
        bReachedMax =
            (i + 1 > (ANG3D_LOG == 1
                          ? 10.0 * (m_3D_Area.x_rt - m_3D_Area.x_lb) / UNIT3D
                          : 1000000000));
    } while (!bOutAngArea && !bReachedMax);

    i = 0;
    if (ANG3D_LOG == 1)
      z = -1;
    else
      z = 0.1;
    bOutAngArea = false;
    do {
      if (ANG3D_LOG == 1) {
        i++;
        z = -i;
      } else {
        i = (i == 0 ? 10 : i * 10);
        z = 1.0 / i;
      }
      x = ang3d_x(0, 0, z);
      y = ang3d_y(0, 0, z);

      if ((x >= m_3D_Area.x_lb) && (x <= m_3D_Area.x_rt) &&
          (y >= m_3D_Area.y_lb) && (y <= m_3D_Area.y_rt)) {
        if (((ANG3D_LOG == 1) && !(i % z_step)) ||
            ((ANG3D_LOG == 2) && !(log_div(i, z_step)))) {
          string name;
          if (ANG3D_LOG == 1)
            name = i2s(-i);
          else {
            name = d2s(1.0 / i, -1);
            trimrightzeros(name);
          }
          ang3d_mark(x, y, GCLC_DIR_L, name);
          iRv = ang3d_fsegment(x - dash_halflen, y, x + dash_halflen, y);
          if (iRv != rvGCLCOK)
            return iRv;
        }
      } else
        bOutAngArea = true;

      if (ANG3D_AXES_LIMITED)
        bReachedMax = (i + 1 > -Z3D_MIN);
      else
        bReachedMax =
            (i + 1 > (ANG3D_LOG == 1
                          ? 10.0 * (m_3D_Area.x_rt - m_3D_Area.x_lb) / UNIT3D
                          : 1000000000));
    } while (!bOutAngArea && !bReachedMax);

  } break;

  case 3:
    break;

  default:
    break;
  }

  switch (mark_axes) {
  case 1:
    // denoting axes
    if (ANG3D_LOG == 1)
      z = 0;
    else
      z = 1;

    if (ANG3D_AXES_LIMITED) {
      x = ang3d_x(X3D_MAX + 0.2, 0, z);
      y = ang3d_y(X3D_MAX + 0.2, 0, z);
      if ((x >= m_3D_Area.x_lb) && (x <= m_3D_Area.x_rt) &&
          (y >= m_3D_Area.y_lb) && (y <= m_3D_Area.y_rt))
        ang3d_mark(x, y, GCLC_DIR_B, "x");
      else {
        if (ang3d_clip_x_axis(x1, y1, x2, y2)) {
          cp = ang3d_x(1, 0, z);
          cn = ang3d_x(-1, 0, z);
          if (fabs(x1 - cp) < fabs(x1 - cn))
            ang3d_mark(x1, y1, GCLC_DIR_B, "x");
          else
            ang3d_mark(x2, y2, GCLC_DIR_B, "x");
        }
      }

      x = ang3d_x(0, Y3D_MAX, z);
      y = ang3d_y(0, Y3D_MAX, z);
      if ((x >= m_3D_Area.x_lb) && (x <= m_3D_Area.x_rt) &&
          (y >= m_3D_Area.y_lb) && (y <= m_3D_Area.y_rt))
        ang3d_mark(x, y, GCLC_DIR_B, "y");
      else {
        if (ang3d_clip_y_axis(x1, y1, x2, y2)) {
          cp = ang3d_y(0, 1, z);
          cn = ang3d_y(0, -1, z);
          if (fabs(y1 - cp) < fabs(y1 - cn))
            ang3d_mark(x1, y1, GCLC_DIR_B, "y");
          else
            ang3d_mark(x2, y2, GCLC_DIR_B, "y");
        }
      }

      x = ang3d_x(0, 0, Z3D_MAX);
      y = ang3d_y(0, 0, Z3D_MAX);
      if ((x >= m_3D_Area.x_lb) && (x <= m_3D_Area.x_rt) &&
          (y >= m_3D_Area.y_lb) && (y <= m_3D_Area.y_rt))
        ang3d_mark(x, y, GCLC_DIR_L, "z");
      else {
        if (ang3d_clip_z_axis(x1, y1, x2, y2)) {
          cp = ang3d_y(0, 0, 1);
          cn = ang3d_y(0, 0, -1);
          if (fabs(y1 - cp) < fabs(y1 - cn))
            ang3d_mark(x1, y1, GCLC_DIR_L, "z");
          else
            ang3d_mark(x2, y2, GCLC_DIR_L, "z");
        }
      }
    } else {
      if (ang3d_clip_x_axis(x1, y1, x2, y2)) {
        cp = ang3d_x(1, 0, z);
        cn = ang3d_x(-1, 0, z);
        if (fabs(x1 - cp) < fabs(x1 - cn))
          ang3d_mark(x1, y1, GCLC_DIR_B, "x");
        else
          ang3d_mark(x2, y2, GCLC_DIR_B, "x");
      }
      if (ang3d_clip_y_axis(x1, y1, x2, y2)) {
        cp = ang3d_y(0, 1, z);
        cn = ang3d_y(0, -1, z);
        if (fabs(y1 - cp) < fabs(y1 - cn))
          ang3d_mark(x1, y1, GCLC_DIR_B, "y");
        else
          ang3d_mark(x2, y2, GCLC_DIR_B, "y");
      }
      if (ang3d_clip_z_axis(x1, y1, x2, y2)) {
        cp = ang3d_y(0, 0, 1);
        cn = ang3d_y(0, 0, -1);
        if (fabs(y1 - cp) < fabs(y1 - cn))
          ang3d_mark(x1, y1, GCLC_DIR_L, "z");
        else
          ang3d_mark(x2, y2, GCLC_DIR_L, "z");
      }
    }

    break;
  case 2:
    break;
  default:
    break;
  }

  if (ANG3D_LOG == 1)
    z = 0;
  else
    z = 1;
  switch (arrows) {
  case 1:
    if (ANG3D_AXES_LIMITED) {
      x1 = ang3d_x(X3D_MAX, 0, z);
      y1 = ang3d_y(X3D_MAX, 0, z);
      x2 = ang3d_x(X3D_MIN, 0, z);
      y2 = ang3d_y(X3D_MIN, 0, z);
      if (clip_segment(x1, y1, x2, y2, m_3D_Area.x_lb, m_3D_Area.y_lb,
                       m_3D_Area.x_rt, m_3D_Area.y_rt, x1, y1, x2, y2)) {
        DrawSegment(x1, y1, x2, y2, true, false, m_3D_Area);
        ang3d_arrow(x1, y1, x2, y2);
        ang3d_arrow(x2, y2, x1, y1);
      }

      x1 = ang3d_x(0, Y3D_MAX, z);
      y1 = ang3d_y(0, Y3D_MAX, z);
      x2 = ang3d_x(0, Y3D_MIN, z);
      y2 = ang3d_y(0, Y3D_MIN, z);
      if (clip_segment(x1, y1, x2, y2, m_3D_Area.x_lb, m_3D_Area.y_lb,
                       m_3D_Area.x_rt, m_3D_Area.y_rt, x1, y1, x2, y2)) {
        DrawSegment(x1, y1, x2, y2, true, false, m_3D_Area);
        ang3d_arrow(x1, y1, x2, y2);
        ang3d_arrow(x2, y2, x1, y1);
      }

      x1 = ang3d_x(0, 0, Z3D_MAX);
      y1 = ang3d_y(0, 0, Z3D_MAX);
      x2 = ang3d_x(0, 0, Z3D_MIN);
      y2 = ang3d_y(0, 0, Z3D_MIN);
      if (clip_segment(x1, y1, x2, y2, m_3D_Area.x_lb, m_3D_Area.y_lb,
                       m_3D_Area.x_rt, m_3D_Area.y_rt, x1, y1, x2, y2)) {
        DrawSegment(x1, y1, x2, y2, true, false, m_3D_Area);
        ang3d_arrow(x1, y1, x2, y2);
        ang3d_arrow(x2, y2, x1, y1);
      }
    } else {
      if (ang3d_clip_x_axis(x1, y1, x2, y2)) {
        DrawSegment(x1, y1, x2, y2, true, false, m_3D_Area);
        ang3d_arrow(x1, y1, x2, y2);
        ang3d_arrow(x2, y2, x1, y1);
      }
      if (ang3d_clip_y_axis(x1, y1, x2, y2)) {
        DrawSegment(x1, y1, x2, y2, true, false, m_3D_Area);
        ang3d_arrow(x1, y1, x2, y2);
        ang3d_arrow(x2, y2, x1, y1);
      }
      if (ang3d_clip_z_axis(x1, y1, x2, y2)) {
        DrawSegment(x1, y1, x2, y2, true, false, m_3D_Area);
        ang3d_arrow(x1, y1, x2, y2);
        ang3d_arrow(x2, y2, x1, y1);
      }
    }
    break;

  case 2:
    if (ANG3D_AXES_LIMITED) {
      x1 = ang3d_x(X3D_MAX, 0, z);
      y1 = ang3d_y(X3D_MAX, 0, z);
      x2 = ang3d_x(X3D_MIN, 0, z);
      y2 = ang3d_y(X3D_MIN, 0, z);
      if (clip_segment(x1, y1, x2, y2, m_3D_Area.x_lb, m_3D_Area.y_lb,
                       m_3D_Area.x_rt, m_3D_Area.y_rt, x1, y1, x2, y2)) {
        DrawSegment(x1, y1, x2, y2, true, false, m_3D_Area);
        cp = ang3d_x(1, 0, 0);
        cn = ang3d_x(-1, 0, 0);
        if (fabs(x1 - cp) < fabs(x1 - cn))
          ang3d_arrow(x2, y2, x1, y1);
        else
          ang3d_arrow(x1, y1, x2, y2);
      }

      x1 = ang3d_x(0, Y3D_MAX, z);
      y1 = ang3d_y(0, Y3D_MAX, z);
      x2 = ang3d_x(0, Y3D_MIN, z);
      y2 = ang3d_y(0, Y3D_MIN, z);
      if (clip_segment(x1, y1, x2, y2, m_3D_Area.x_lb, m_3D_Area.y_lb,
                       m_3D_Area.x_rt, m_3D_Area.y_rt, x1, y1, x2, y2)) {
        DrawSegment(x1, y1, x2, y2, true, false, m_3D_Area);
        cp = ang3d_y(0, 1, 0);
        cn = ang3d_y(0, -1, 0);
        if (fabs(y1 - cp) < fabs(y1 - cn))
          ang3d_arrow(x2, y2, x1, y1);
        else
          ang3d_arrow(x1, y1, x2, y2);
      }

      x1 = ang3d_x(0, 0, Z3D_MAX);
      y1 = ang3d_y(0, 0, Z3D_MAX);
      x2 = ang3d_x(0, 0, Z3D_MIN);
      y2 = ang3d_y(0, 0, Z3D_MIN);
      if (clip_segment(x1, y1, x2, y2, m_3D_Area.x_lb, m_3D_Area.y_lb,
                       m_3D_Area.x_rt, m_3D_Area.y_rt, x1, y1, x2, y2)) {
        DrawSegment(x1, y1, x2, y2, true, false, m_3D_Area);
        cp = ang3d_y(0, 0, 1);
        cn = ang3d_y(0, 0, -1);
        if (fabs(y1 - cp) < fabs(y1 - cn))
          ang3d_arrow(x2, y2, x1, y1);
        else
          ang3d_arrow(x1, y1, x2, y2);
      }
    } else {
      if (ang3d_clip_x_axis(x1, y1, x2, y2)) {
        DrawSegment(x1, y1, x2, y2, true, false, m_3D_Area);
        cp = ang3d_x(1, 0, 0);
        cn = ang3d_x(-1, 0, 0);
        if (fabs(x1 - cp) < fabs(x1 - cn))
          ang3d_arrow(x2, y2, x1, y1);
        else
          ang3d_arrow(x1, y1, x2, y2);
      }
      if (ang3d_clip_y_axis(x1, y1, x2, y2)) {
        DrawSegment(x1, y1, x2, y2, true, false, m_3D_Area);
        cp = ang3d_y(0, 1, 0);
        cn = ang3d_y(0, -1, 0);
        if (fabs(y1 - cp) < fabs(y1 - cn))
          ang3d_arrow(x2, y2, x1, y1);
        else
          ang3d_arrow(x1, y1, x2, y2);
      }
      if (ang3d_clip_z_axis(x1, y1, x2, y2)) {
        DrawSegment(x1, y1, x2, y2, true, false, m_3D_Area);
        cp = ang3d_y(0, 0, 1);
        cn = ang3d_y(0, 0, -1);
        if (fabs(y1 - cp) < fabs(y1 - cn))
          ang3d_arrow(x2, y2, x1, y1);
        else
          ang3d_arrow(x1, y1, x2, y2);
      }
    }
    break;

  case 3:
    if (ANG3D_AXES_LIMITED) {
      x1 = ang3d_x(X3D_MAX, 0, z);
      y1 = ang3d_y(X3D_MAX, 0, z);
      x2 = ang3d_x(X3D_MIN, 0, z);
      y2 = ang3d_y(X3D_MIN, 0, z);
      if (clip_segment(x1, y1, x2, y2, m_3D_Area.x_lb, m_3D_Area.y_lb,
                       m_3D_Area.x_rt, m_3D_Area.y_rt, x1, y1, x2, y2))
        DrawSegment(x1, y1, x2, y2, true, false, m_3D_Area);

      x1 = ang3d_x(0, Y3D_MAX, z);
      y1 = ang3d_y(0, Y3D_MAX, z);
      x2 = ang3d_x(0, Y3D_MIN, z);
      y2 = ang3d_y(0, Y3D_MIN, z);
      if (clip_segment(x1, y1, x2, y2, m_3D_Area.x_lb, m_3D_Area.y_lb,
                       m_3D_Area.x_rt, m_3D_Area.y_rt, x1, y1, x2, y2))
        DrawSegment(x1, y1, x2, y2, true, false, m_3D_Area);

      x1 = ang3d_x(0, 0, Z3D_MAX);
      y1 = ang3d_y(0, 0, Z3D_MAX);
      x2 = ang3d_x(0, 0, Z3D_MIN);
      y2 = ang3d_y(0, 0, Z3D_MIN);
      if (clip_segment(x1, y1, x2, y2, m_3D_Area.x_lb, m_3D_Area.y_lb,
                       m_3D_Area.x_rt, m_3D_Area.y_rt, x1, y1, x2, y2))
        DrawSegment(x1, y1, x2, y2, true, false, m_3D_Area);
    } else {
      if (ang3d_clip_x_axis(x1, y1, x2, y2))
        DrawSegment(x1, y1, x2, y2, true, false, m_3D_Area);
      if (ang3d_clip_y_axis(x1, y1, x2, y2))
        DrawSegment(x1, y1, x2, y2, true, false, m_3D_Area);
      if (ang3d_clip_z_axis(x1, y1, x2, y2))
        DrawSegment(x1, y1, x2, y2, true, false, m_3D_Area);
    }
    break;

  default:
    break;
  }

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::ang3d_point() {
  string sPointName;
  double x, y, z, x2d, y2d;
  GCLCError iRv;

  if (ReadToken(sPointName) != rvGCLCOK)
    return rvGCLCIdExpected;
  if ((iRv = ReadNumber(x)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(y)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(z)) != rvGCLCOK)
    return iRv;

  x2d = ang3d_x(x, y, z);
  y2d = ang3d_y(x, y, z);

  return Let(sPointName, GCLC_POINT, x2d, y2d, x, y, z,
             0.00); // 3D coordinates are also stored
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::ang3d_getx() {
  string sNumberName;
  GCLC_object o1;
  GCLCError iRv;

  if (ReadToken(sNumberName) != rvGCLCOK)
    return rvGCLCIdExpected;
  if ((iRv = ReadObject(GCLC_POINT, o1)) != rvGCLCOK)
    return iRv;

  return Let(sNumberName, GCLC_NUMBER, o1.p[2], 0.00, 0.00, 0.00, 0.00, 0.00);
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::ang3d_gety() {
  string sNumberName;
  GCLC_object o1;
  GCLCError iRv;

  if (ReadToken(sNumberName) != rvGCLCOK)
    return rvGCLCIdExpected;
  if ((iRv = ReadObject(GCLC_POINT, o1)) != rvGCLCOK)
    return iRv;

  return Let(sNumberName, GCLC_NUMBER, o1.p[3], 0.00, 0.00, 0.00, 0.00, 0.00);
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::ang3d_getz() {
  string sNumberName;
  GCLC_object o1;
  GCLCError iRv;

  if (ReadToken(sNumberName) != rvGCLCOK)
    return rvGCLCIdExpected;
  if ((iRv = ReadObject(GCLC_POINT, o1)) != rvGCLCOK)
    return iRv;

  return Let(sNumberName, GCLC_NUMBER, o1.p[4], 0.00, 0.00, 0.00, 0.00, 0.00);
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::ang3d_draw_line_p() {
  GCLC_object o1, o2;
  GCLCError iRv;

  if ((iRv = ReadObject(GCLC_POINT, o1)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadObject(GCLC_POINT, o2)) != rvGCLCOK)
    return iRv;

  if (PrintComment("Drawing 2D ang line " + o1.name + " " + o2.name) != rvG_OK)
    return rvCannotExport;

  return ang3d_fline(o1.p[0], o1.p[1], o2.p[0], o2.p[1]);
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::ang3d_draw_parametric_surface() {
  string sNumberName1, sNumberName2;
  GCLCError iRv;
  double dNumber1, dNumber2;
  // string text;
  string init_text1;
  string cond_text1;
  string iter_text1;

  string init_text2;
  string cond_text2;
  string iter_text2;

  string x_text;
  string y_text;
  string z_text;

  double x2d_prev[1024], y2d_prev[1024];
  bool bDefined_prev[1024];

  double x, y, z;
  double x2d, y2d, x2d_old = 0, y2d_old = 0;

  if ((iRv = ReadToken(sNumberName1)) != rvGCLCOK)
    return rvGCLCIdExpected;
  if ((iRv = ReadToken(sNumberName2)) != rvGCLCOK)
    return rvGCLCIdExpected;

  bool bSupressWarnings = m_bSupressWarnings;
  m_bSupressWarnings = true;

  if ((iRv = Let(sNumberName1, GCLC_NUMBER, 0, 0.00, 0.00, 0.00, 0.00, 0.00)) !=
      rvGCLCOK) {
    m_bSupressWarnings = bSupressWarnings;
    return iRv;
  }
  if ((iRv = Let(sNumberName2, GCLC_NUMBER, 0, 0.00, 0.00, 0.00, 0.00, 0.00)) !=
      rvGCLCOK) {
    m_bSupressWarnings = bSupressWarnings;
    return iRv;
  }

  string sText;
  if ((iRv = take_text(sText)) != rvGCLCOK) {
    m_bSupressWarnings = bSupressWarnings;
    return iRv;
  }

  if (sText.find(';') != string::npos) {
    init_text1 = sText.substr(0, sText.find(';'));
    sText = sText.substr(sText.find(';') + 1);
  } else {
    m_bSupressWarnings = bSupressWarnings;
    return rvGCLCSeparatorExpected;
  }
  if (sText.find(';') != string::npos) {
    cond_text1 = sText.substr(0, sText.find(';'));
    sText = sText.substr(sText.find(';') + 1);
  } else {
    m_bSupressWarnings = bSupressWarnings;
    return rvGCLCSeparatorExpected;
  }
  iter_text1 = sText;

  //----second loop
  if ((iRv = take_text(sText)) != rvGCLCOK) {
    m_bSupressWarnings = bSupressWarnings;
    return iRv;
  }

  if (sText.find(';') != string::npos) {
    init_text2 = sText.substr(0, sText.find(';'));
    sText = sText.substr(sText.find(';') + 1);
  } else {
    m_bSupressWarnings = bSupressWarnings;
    return rvGCLCSeparatorExpected;
  }
  if (sText.find(';') != string::npos) {
    cond_text2 = sText.substr(0, sText.find(';'));
    sText = sText.substr(sText.find(';') + 1);
  } else {
    m_bSupressWarnings = bSupressWarnings;
    return rvGCLCSeparatorExpected;
  }
  iter_text2 = sText;

  if ((iRv = take_text(sText)) != rvGCLCOK) {
    m_bSupressWarnings = bSupressWarnings;
    return iRv;
  }

  if (sText.find(';') != string::npos) {
    x_text = sText.substr(0, sText.find(';'));
    sText = sText.substr(sText.find(';') + 1);
  } else {
    m_bSupressWarnings = bSupressWarnings;
    return rvGCLCSeparatorExpected;
  }
  if (sText.find(';') != string::npos) {
    y_text = sText.substr(0, sText.find(';'));
    sText = sText.substr(sText.find(';') + 1);
  } else {
    m_bSupressWarnings = bSupressWarnings;
    return rvGCLCSeparatorExpected;
  }
  z_text = sText;

  if (PrintComment("Drawing 3D parametric surface (" + x_text + "," + y_text +
                   "," + z_text + ")") != rvG_OK)
    return rvCannotExport;

  double v;
  if ((iRv = calc_expression(init_text1, v)) != rvGCLCOK) {
    m_bSupressWarnings = bSupressWarnings;
    return iRv;
  }
  if ((iRv = Let(sNumberName1, GCLC_NUMBER, v, 0.00, 0.00, 0.00, 0.00, 0.00)) !=
      rvGCLCOK) {
    m_bSupressWarnings = bSupressWarnings;
    return iRv;
  }
  if ((iRv = calc_expression(cond_text1, dNumber1)) != rvGCLCOK) {
    m_bSupressWarnings = bSupressWarnings;
    return iRv;
  }

  int iCounter;
  bool bFirst1 = true;
  while (dNumber1 == 1.0) {
    if ((iRv = calc_expression(init_text2, v)) != rvGCLCOK) {
      m_bSupressWarnings = bSupressWarnings;
      return iRv;
    }
    if ((iRv = Let(sNumberName2, GCLC_NUMBER, v, 0.00, 0.00, 0.00, 0.00,
                   0.00)) != rvGCLCOK) {
      m_bSupressWarnings = bSupressWarnings;
      return iRv;
    }
    if ((iRv = calc_expression(cond_text2, dNumber2)) != rvGCLCOK) {
      m_bSupressWarnings = bSupressWarnings;
      return iRv;
    }

    bool bDefined;
    bool bFirst2 = true;

    iCounter = 0;
    while (dNumber2 == 1.0) {
      bDefined = true;

      if ((iRv = calc_expression(x_text, x)) != rvGCLCOK)
        bDefined = false;
      if ((iRv = calc_expression(y_text, y)) != rvGCLCOK)
        bDefined = false;
      if ((iRv = calc_expression(z_text, z)) != rvGCLCOK)
        bDefined = false;

      if (!bFirst2)
        if ((y2d_old < m_3D_Area.y_lb) && (y2d_old > m_3D_Area.y_rt))
          bDefined = false;

      x2d = ang3d_x(x, y, z);
      y2d = ang3d_y(x, y, z);
      if ((y2d < m_3D_Area.y_lb) && (y2d > m_3D_Area.y_rt))
        bDefined = false;

      if (!bFirst2 && bDefined) {
        // connect to the previous one
        if ((iRv = ang3d_fsegment(x2d, y2d, x2d_old, y2d_old)) != rvGCLCOK) {
          m_bSupressWarnings = bSupressWarnings;
          return iRv;
        }
      }

      bFirst2 = !bDefined;

      x2d_old = x2d;
      y2d_old = y2d;

      if (!bFirst1 && bDefined) {
        if (bDefined_prev[iCounter])
          // connect with the corresponding one for the previous column
          iRv =
              ang3d_fsegment(x2d, y2d, x2d_prev[iCounter], y2d_prev[iCounter]);
        if (iRv != rvGCLCOK) {
          m_bSupressWarnings = bSupressWarnings;
          return iRv;
        }
      }

      x2d_prev[iCounter] = x2d;
      y2d_prev[iCounter] = y2d;
      bDefined_prev[iCounter] = bDefined;
      iCounter++;

      if ((iRv = calc_expression(iter_text2, v)) != rvGCLCOK) {
        m_bSupressWarnings = bSupressWarnings;
        return iRv;
      }

      if (fabs(v) < EPS)
        v = 0;

      if ((iRv = Let(sNumberName2, GCLC_NUMBER, v, 0.00, 0.00, 0.00, 0.00,
                     0.00)) != rvGCLCOK) {
        m_bSupressWarnings = bSupressWarnings;
        return iRv;
      }
      if ((iRv = calc_expression(cond_text2, dNumber2)) != rvGCLCOK) {
        m_bSupressWarnings = bSupressWarnings;
        return iRv;
      }
    }

    if ((iRv = calc_expression(iter_text1, v)) != rvGCLCOK) {
      m_bSupressWarnings = bSupressWarnings;
      return iRv;
    }

    if (fabs(v) < EPS)
      v = 0;

    if ((iRv = Let(sNumberName1, GCLC_NUMBER, v, 0.00, 0.00, 0.00, 0.00,
                   0.00)) != rvGCLCOK) {
      m_bSupressWarnings = bSupressWarnings;
      return iRv;
    }
    if ((iRv = calc_expression(cond_text1, dNumber1)) != rvGCLCOK) {
      m_bSupressWarnings = bSupressWarnings;
      return iRv;
    }
    bFirst1 = false;
  }

  m_bSupressWarnings = bSupressWarnings;
  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::ang3d_draw_parametric_curve() {
  string sNumberName;
  GCLCError iRv;
  double dNumber;
  string init_text;
  string cond_text;
  string iter_text;

  string x_text;
  string y_text;
  string z_text;

  double x, y, z;
  double x2d, y2d, x2d_old = 0, y2d_old = 0;

  iRv = ReadToken(sNumberName);
  if (iRv != rvGCLCOK)
    return rvGCLCIdExpected;

  bool bSupressWarnings = m_bSupressWarnings;
  m_bSupressWarnings = true;

  if ((iRv = Let(sNumberName, GCLC_NUMBER, 0, 0.00, 0.00, 0.00, 0.00, 0.00)) !=
      rvGCLCOK) {
    m_bSupressWarnings = bSupressWarnings;
    return iRv;
  }

  if ((iRv = Let(sNumberName, GCLC_NUMBER, 0, 0.00, 0.00, 0.00, 0.00, 0.00)) !=
      rvGCLCOK) {
    m_bSupressWarnings = bSupressWarnings;
    return iRv;
  }

  string sText;
  if ((iRv = take_text(sText)) != rvGCLCOK) {
    m_bSupressWarnings = bSupressWarnings;
    return iRv;
  }

  if (sText.find(';') != string::npos) {
    init_text = sText.substr(0, sText.find(';'));
    sText = sText.substr(sText.find(';') + 1);
  } else {
    m_bSupressWarnings = bSupressWarnings;
    return rvGCLCSeparatorExpected;
  }
  if (sText.find(';') != string::npos) {
    cond_text = sText.substr(0, sText.find(';'));
    sText = sText.substr(sText.find(';') + 1);
  } else {
    m_bSupressWarnings = bSupressWarnings;
    return rvGCLCSeparatorExpected;
  }
  iter_text = sText;

  if ((iRv = take_text(sText)) != rvGCLCOK) {
    m_bSupressWarnings = bSupressWarnings;
    return iRv;
  }

  if (sText.find(';') != string::npos) {
    x_text = sText.substr(0, sText.find(';'));
    sText = sText.substr(sText.find(';') + 1);
  } else {
    m_bSupressWarnings = bSupressWarnings;
    return rvGCLCSeparatorExpected;
  }

  if (sText.find(';') != string::npos) {
    y_text = sText.substr(0, sText.find(';'));
    sText = sText.substr(sText.find(';') + 1);
  } else {
    m_bSupressWarnings = bSupressWarnings;
    return rvGCLCSeparatorExpected;
  }
  z_text = sText;

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
  if (PrintComment("Drawing 3D parametric curve (" + x_text + "," + y_text +
                   "," + z_text + ")") != rvG_OK)
    return rvCannotExport;

  bool bFirst = true;
  bool bDefined;
  while (dNumber == 1.0) {
    bDefined = true;

    if ((iRv = calc_expression(x_text, x)) != rvGCLCOK)
      bDefined = false;
    if ((iRv = calc_expression(y_text, y)) != rvGCLCOK)
      bDefined = false;
    if ((iRv = calc_expression(z_text, z)) != rvGCLCOK)
      bDefined = false;

    if (!bFirst)
      if ((y2d_old < m_3D_Area.y_lb) && (y2d_old > m_3D_Area.y_rt))
        bDefined = false;

    x2d = ang3d_x(x, y, z);
    y2d = ang3d_y(x, y, z);
    if ((y2d < m_3D_Area.y_lb) && (y2d > m_3D_Area.y_rt))
      bDefined = false;

    if (!bFirst && bDefined) {
      // connect to the previous one
      if ((iRv = ang3d_fsegment(x2d, y2d, x2d_old, y2d_old)) != rvGCLCOK) {
        m_bSupressWarnings = bSupressWarnings;
        return iRv;
      }
    }

    bFirst = !bDefined;

    x2d_old = x2d;
    y2d_old = y2d;

    if ((calc_expression(iter_text, v)) != rvGCLCOK) {
      m_bSupressWarnings = bSupressWarnings;
      return iRv;
    }

    if (fabs(v) < EPS)
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

double CGCLC::ang3d_x(double x, double y, double /* z */) {
  return X3D_ORIGIN - x * COS_ALPHA3D * UNIT3D +
         y * ANG3D_SCALE_Y * SIN_ALPHA3D * UNIT3D;
}

// ----------------------------------------------------------------------------

double CGCLC::ang3d_y(double x, double y, double z) {
  if (ANG3D_LOG == 1)
    return Y3D_ORIGIN - x * SIN_ALPHA3D * UNIT3D * SIN_BETA3D -
           y * ANG3D_SCALE_Y * COS_ALPHA3D * UNIT3D * SIN_BETA3D +
           z * ANG3D_SCALE_Z * COS_BETA3D * UNIT3D;
  else
    return Y3D_ORIGIN - x * SIN_ALPHA3D * UNIT3D * SIN_BETA3D -
           y * ANG3D_SCALE_Y * COS_ALPHA3D * UNIT3D * SIN_BETA3D +
           log10(z) * ANG3D_SCALE_Z * COS_BETA3D * UNIT3D;
}

// ----------------------------------------------------------------------------

bool CGCLC::ang3d_clip_x_axis(double &x1, double &y1, double &x2, double &y2) {
  x1 = ang3d_x(0, 0, 0);
  y1 = ang3d_y(0, 0, 0);
  x2 = ang3d_x(1, 0, 0);
  y2 = ang3d_y(1, 0, 0);
  return clip_line(x1, y1, x2, y2, m_3D_Area.x_lb, m_3D_Area.y_lb,
                   m_3D_Area.x_rt, m_3D_Area.y_rt, x1, y1, x2, y2);
}

// ----------------------------------------------------------------------------

bool CGCLC::ang3d_clip_y_axis(double &x1, double &y1, double &x2, double &y2) {
  x1 = ang3d_x(0, 0, 0);
  y1 = ang3d_y(0, 0, 0);
  x2 = ang3d_x(0, 1, 0);
  y2 = ang3d_y(0, 1, 0);
  return clip_line(x1, y1, x2, y2, m_3D_Area.x_lb, m_3D_Area.y_lb,
                   m_3D_Area.x_rt, m_3D_Area.y_rt, x1, y1, x2, y2);
}

// ----------------------------------------------------------------------------

bool CGCLC::ang3d_clip_z_axis(double &x1, double &y1, double &x2, double &y2) {
  x1 = ang3d_x(0, 0, 0);
  y1 = ang3d_y(0, 0, 0);
  x2 = ang3d_x(0, 0, 1);
  y2 = ang3d_y(0, 0, 1);
  return clip_line(x1, y1, x2, y2, m_3D_Area.x_lb, m_3D_Area.y_lb,
                   m_3D_Area.x_rt, m_3D_Area.y_rt, x1, y1, x2, y2);
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::ang3d_mark(double x0, double y0, int dir, const string &name) {
  return ang_2D_3D_mark(x0, y0, dir, name, m_3D_Area);
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::ang3d_cmark(double x0, double y0, int dir,
                             const string &name) {
  return ang_2D_3D_cmark(x0, y0, dir, name, m_3D_Area);
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::ang3d_arrow(double x1, double y1, double x2, double y2) {
  return ang_2D_3D_arrow(x1, y1, x2, y2, m_3D_Area);
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::ang3d_fline(double x1, double y1, double x2, double y2) {
  if ((m_3D_Area.x_lb == 0) && (m_3D_Area.x_rt == 0) && (m_3D_Area.y_lb == 0) &&
      (m_3D_Area.y_rt == 0))
    return rvGCLCOK;

  if (DrawLineSensitive(x1, y1, x2, y2, false, m_3D_Area) != rvG_OK)
    return rvCannotExport;
  else
    return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::ang3d_fsegment(double x1, double y1, double x2, double y2) {
  if ((m_3D_Area.x_lb == 0) && (m_3D_Area.x_rt == 0) && (m_3D_Area.y_lb == 0) &&
      (m_3D_Area.y_rt == 0))
    return rvGCLCOK;

  if (DrawSegment(x1, y1, x2, y2, true, false, m_3D_Area) != rvG_OK)
    return rvCannotExport;
  else
    return rvGCLCOK;
}

// ----------------------------------------------------------------------------
