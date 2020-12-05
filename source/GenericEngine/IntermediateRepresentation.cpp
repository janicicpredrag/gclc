// ListOfPrimitives.cpp: implementation of the CIntermediateRepresentation
// class.
//
//////////////////////////////////////////////////////////////////////

#include "IntermediateRepresentation.h"
#include "../Export/GCLCOutput.h"
#include "../Utils/Utils.h"
#include <cmath>

void clip_arc_x(bool bLeft, double x, GCLC_area &area, GCLC_Arc *arcs,
                int *number_of_arcs);
void clip_arc_y(bool bBottom, double y, GCLC_area &area, GCLC_Arc *arcs,
                int *number_of_arcs);

// ----------------------------------------------------------------------------

CGCLCPrimitive::CGCLCPrimitive() {}

// ----------------------------------------------------------------------------

CGCLCPrimitive::~CGCLCPrimitive() { CleanUp(); }

// ----------------------------------------------------------------------------

void CGCLCPrimitive::CleanUp() {}

// ----------------------------------------------------------------------------

CIntermediateRepresentation::CIntermediateRepresentation() {}

// ----------------------------------------------------------------------------

CIntermediateRepresentation::~CIntermediateRepresentation() { CleanUp(); }

// ----------------------------------------------------------------------------

void CIntermediateRepresentation::CleanUp() {
  m_ListOfPrimitives.clear();
  m_EmptyCircles.clear();

  for (int i = 0; i < NUMBER_OF_LAYERS; i++)
    m_aSkipLayer[i] = false;
}

// ----------------------------------------------------------------------------

GReturnValue CIntermediateRepresentation::SetPixel(double x1, double y1,
                                                   GCLC_area &area) {
  CGCLCPrimitive Command;
  Command.type = setpixel;
  Command.arg[0] = x1;
  Command.arg[1] = y1;
  Command.area = area;
  return AddPrimitiveCommand(Command);
}

// ----------------------------------------------------------------------------

GReturnValue CIntermediateRepresentation::DrawLineSensitive(
    double x1, double y1, double x2, double y2, bool dashed, GCLC_area &area) {
  CGCLCPrimitive Command;
  Command.type = drawlinesensitive;
  Command.arg[0] = x1;
  Command.arg[1] = y1;
  Command.arg[2] = x2;
  Command.arg[3] = y2;
  Command.dashed = dashed;
  Command.area = area;
  return AddPrimitiveCommand(Command);
}

// ----------------------------------------------------------------------------

GReturnValue CIntermediateRepresentation::DrawSegment(double x1, double y1,
                                                      double x2, double y2,
                                                      bool sensitive,
                                                      bool dashed,
                                                      GCLC_area &area) {
  CGCLCPrimitive Command;
  Command.type = drawsegment;
  Command.arg[0] = x1;
  Command.arg[1] = y1;
  Command.arg[2] = x2;
  Command.arg[3] = y2;
  Command.sensitive = sensitive;
  Command.dashed = dashed;
  Command.area = area;
  return AddPrimitiveCommand(Command);
}

// ----------------------------------------------------------------------------

GReturnValue CIntermediateRepresentation::EmptyCircle(double x1, double y1,
                                                      double x2, double y2,
                                                      GCLC_area &area) {
  // added the argument area, 31.12.2006
  CGCLCPrimitive Command;
  Command.type = emptycircle;
  Command.arg[0] = x1;
  Command.arg[1] = y1;
  Command.arg[2] = x2;
  Command.arg[3] = y2;
  Command.area = area;
  return AddPrimitiveCommand(Command);
}

// ----------------------------------------------------------------------------

GReturnValue CIntermediateRepresentation::FillTriangle(double x1, double y1,
                                                       double x2, double y2,
                                                       double x3, double y3,
                                                       GCLC_area &area) {
  CGCLCPrimitive Command;
  Command.type = filltriangle;
  Command.arg[0] = x1;
  Command.arg[1] = y1;
  Command.arg[2] = x2;
  Command.arg[3] = y2;
  Command.arg[4] = x3;
  Command.arg[5] = y3;
  Command.area = area;
  return AddPrimitiveCommand(Command);
}

// ----------------------------------------------------------------------------

GReturnValue CIntermediateRepresentation::FillRectangle(double x1, double y1,
                                                        double x2, double y2,
                                                        GCLC_area &area) {
  CGCLCPrimitive Command;
  Command.type = fillrectangle;
  Command.arg[0] = x1;
  Command.arg[1] = y1;
  Command.arg[2] = x2;
  Command.arg[3] = y2;
  Command.area = area;
  return AddPrimitiveCommand(Command);
}

// ----------------------------------------------------------------------------

GReturnValue CIntermediateRepresentation::FillEllipseArc(
    double x1, double y1, double a, double b, double phi1, double phi2,
    GCLC_area &area, bool bFillTriangle) {
  CGCLCPrimitive Command;
  if (bFillTriangle)
    Command.type = fillellipsearc;
  else
    Command.type = fillellipsearc_without_triangle;
  Command.arg[0] = x1;
  Command.arg[1] = y1;
  Command.arg[2] = a;
  Command.arg[3] = b;
  Command.arg[4] = phi1;
  Command.arg[5] = phi2;
  Command.area = area;
  return AddPrimitiveCommand(Command);
}

// ----------------------------------------------------------------------------

GReturnValue CIntermediateRepresentation::SetLayer(unsigned int l) {
  CGCLCPrimitive Command;
  Command.type = setlayer;
  Command.arg[0] = l;
  return AddPrimitiveCommand(Command);
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CIntermediateRepresentation::HideLayer(unsigned int l) {
  m_aSkipLayer[l] = true;
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CIntermediateRepresentation::HideLayersFrom(unsigned int l) {
  for (unsigned int i = l; i < NUMBER_OF_LAYERS; i++)
    m_aSkipLayer[i] = true;
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CIntermediateRepresentation::HideLayersTo(unsigned int l) {
  for (unsigned int i = 0; i <= l; i++)
    m_aSkipLayer[i] = true;
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CIntermediateRepresentation::SetThickness(double t) {
  CGCLCPrimitive Command;
  Command.type = setthickness;
  Command.arg[0] = t;
  return AddPrimitiveCommand(Command);
}

// ----------------------------------------------------------------------------

GReturnValue CIntermediateRepresentation::SetDashLen(double d1, double d2,
                                                     double d3, double d4) {
  CGCLCPrimitive Command;
  Command.type = setdashlen;
  Command.arg[0] = d1;
  Command.arg[1] = d2;
  Command.arg[2] = d3;
  Command.arg[3] = d4;
  return AddPrimitiveCommand(Command);
}

// ----------------------------------------------------------------------------

GReturnValue CIntermediateRepresentation::PrintText(double x, double y,
                                                    const string &position,
                                                    const string &text,
                                                    GCLC_area &area) {
  CGCLCPrimitive Command;
  Command.type = printtext;
  Command.arg[0] = x;
  Command.arg[1] = y;
  Command.pos = position;
  Command.text = text;
  Command.area = area;
  return AddPrimitiveCommand(Command);
}

// ----------------------------------------------------------------------------

GReturnValue CIntermediateRepresentation::PrintComment(const string &text) {
  CGCLCPrimitive Command;
  Command.type = printcomment;
  Command.text = text;
  return AddPrimitiveCommand(Command);
}

// ----------------------------------------------------------------------------

GReturnValue CIntermediateRepresentation::DirectExport(const string &text,
                                                       export_type format) {
  CGCLCPrimitive Command;
  Command.type = directexport;
  Command.text = text;
  Command.export_format = format;
  return AddPrimitiveCommand(Command);
}

// ----------------------------------------------------------------------------

GReturnValue CIntermediateRepresentation::SetDim(double x, double y) {
  CGCLCPrimitive Command;
  Command.type = dim;
  Command.arg[0] = x;
  Command.arg[1] = y;
  return AddPrimitiveCommand(Command);
}

// ----------------------------------------------------------------------------

GReturnValue CIntermediateRepresentation::SetColor(unsigned char r,
                                                   unsigned char g,
                                                   unsigned char b) {
  CGCLCPrimitive Command;
  Command.type = setcolor;
  Command.arg[0] = r;
  Command.arg[1] = g;
  Command.arg[2] = b;
  return AddPrimitiveCommand(Command);
}

// ----------------------------------------------------------------------------

GReturnValue CIntermediateRepresentation::SetBackgroundColor(unsigned char r,
                                                             unsigned char g,
                                                             unsigned char b) {
  CGCLCPrimitive Command;
  Command.type = setbgcolor;
  Command.arg[0] = r;
  Command.arg[1] = g;
  Command.arg[2] = b;
  return AddPrimitiveCommand(Command);
}

// ----------------------------------------------------------------------------

GReturnValue CIntermediateRepresentation::SetFontSize(unsigned char size) {
  CGCLCPrimitive Command;
  Command.type = setfontsize;
  Command.arg[0] = size;
  return AddPrimitiveCommand(Command);
}

// ----------------------------------------------------------------------------

GReturnValue CIntermediateRepresentation::DrawArc(double x1, double y1,
                                                  double x2, double y2,
                                                  double phi, bool sensitive,
                                                  bool dashed, GCLC_area &area,
                                                  int precision) {
  CGCLCPrimitive Command;
  Command.type = drawarc;
  Command.arg[0] = x1;
  Command.arg[1] = y1;
  Command.arg[2] = x2;
  Command.arg[3] = y2;
  Command.arg[4] = phi;
  Command.sensitive = sensitive;
  Command.dashed = dashed;
  Command.area = area;
  Command.precision = precision;
  return AddPrimitiveCommand(Command);
}

// ----------------------------------------------------------------------------

GReturnValue
CIntermediateRepresentation::AddPrimitiveCommand(CGCLCPrimitive &C) {
  m_ListOfPrimitives.push_back(C);
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CIntermediateRepresentation::Export(CGCLCOutput &Out) {
  GReturnValue iRv;
  bool founddim = false;
  GCLC_area area;
  GCLC_Arc clipped_arcs[5];
  int number_of_clipped_arcs;

  double x1, y1, x2, y2, d;
  double x1_start, x2_start, y1_start, y2_start;
  double r, sinn, cosn, x3, y3, phi;
  double x1c, x2c, y1c, y2c;
  bool bDone;
  int num, i;
  double angle;

  for (list<CGCLCPrimitive>::iterator it = m_ListOfPrimitives.begin();
       it != m_ListOfPrimitives.end() && !founddim; it++) {
    if (it->type == dim) {
      iRv = Out.Init((int)it->arg[0], (int)it->arg[1]);
      if (iRv != rvG_OK)
        return iRv;
      founddim = true;

      Default_Area.x_lb = 0;
      Default_Area.y_lb = 0;
      Default_Area.x_rt = (int)it->arg[0];
      Default_Area.y_rt = (int)it->arg[1];
    }
  }
  if (!founddim) {
    iRv = Out.Init(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    if (iRv != rvG_OK)
      return iRv;

    Default_Area.x_lb = 0;
    Default_Area.y_lb = 0;
    Default_Area.x_rt = DEFAULT_WIDTH;
    Default_Area.y_rt = DEFAULT_HEIGHT;
  }

  for (list<CGCLCPrimitive>::iterator it = m_ListOfPrimitives.begin();
       it != m_ListOfPrimitives.end(); it++) {
    if (it->type == setbgcolor) {
      iRv = Out.SetBackgroundColor((unsigned char)it->arg[0],
                                   (unsigned char)it->arg[1],
                                   (unsigned char)it->arg[2]);
      if (iRv != rvG_OK)
        return iRv;
    }
  }

  m_dDashLen1 = DEFAULT_DASH; // default DASH 16.01.2006.
  m_dDashLen2 = DEFAULT_DASH / 2;
  m_dDashLen3 = DEFAULT_DASH;
  m_dDashLen4 = DEFAULT_DASH / 2;

  bool bSkipLayer = false;

  for (list<CGCLCPrimitive>::iterator it = m_ListOfPrimitives.begin();
       it != m_ListOfPrimitives.end(); it++) {
    if (it->type == setlayer)
      bSkipLayer = SkipLayer((unsigned int)it->arg[0]);
    else if (!bSkipLayer) {
      if (it->type == emptycircle) {
        // only if there is no area or if the point is within area 31.12.2006
        x1 = it->arg[0];
        y1 = it->arg[1];

        x2 = it->arg[2];
        y2 = it->arg[3];

        d = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));

        if ((it->area.x_lb != 0) || (it->area.x_rt != 0) ||
            (it->area.y_lb != 0) || (it->area.y_rt != 0)) {
          if ((x1 >= it->area.x_lb - d) && (x1 <= it->area.x_rt + d) &&
              (y1 >= it->area.y_lb - d) && (y1 <= it->area.y_rt + d))
            m_EmptyCircles.push_back(*it);
        } else
          m_EmptyCircles.push_back(*it);
      }
    }
  }

  bSkipLayer = false;
  for (list<CGCLCPrimitive>::iterator it = m_ListOfPrimitives.begin();
       it != m_ListOfPrimitives.end(); it++) {
    if (it->type == setlayer)
      bSkipLayer = SkipLayer((unsigned int)it->arg[0]);
    else if (!bSkipLayer) {
      switch (it->type) {
      case setpixel:
        x1 = it->arg[0];
        y1 = it->arg[1];

        if ((it->area.x_lb != 0) || (it->area.x_rt != 0) ||
            (it->area.y_lb != 0) || (it->area.y_rt != 0))
          area = it->area;
        else
          area = Default_Area;

        if ((area.x_lb != 0) || (area.x_rt != 0) || (area.y_lb != 0) ||
            (area.y_rt != 0)) {
          iRv = rvG_OK;
          if ((x1 >= area.x_lb) && (x1 <= area.x_rt) && (y1 >= area.y_lb) &&
              (y1 <= area.y_rt))
            iRv = this->ClipPixelByEmptyCircles(x1, y1, Out,
                                                m_EmptyCircles.begin());
        } else
          iRv = this->ClipPixelByEmptyCircles(x1, y1, Out,
                                              m_EmptyCircles.begin());
        if ((iRv != rvG_OK) && (iRv != rvG_EmptySet))
          return iRv;
        break;

      // ------------------------------------------------------------

      case drawsegment:

        x1 = it->arg[0];
        y1 = it->arg[1];
        x2 = it->arg[2];
        y2 = it->arg[3];

        if ((it->area.x_lb != 0) || (it->area.x_rt != 0) ||
            (it->area.y_lb != 0) || (it->area.y_rt != 0))
          area = it->area;
        else
          area = Default_Area;

        if (ClipSegmentByArea(x1, y1, x2, y2, area) == rvG_OK) {
          if (it->dashed) {
            d = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));

            if (d < EPS)
              return rvG_OK;

            double dx1, dy1, dx2, dy2, dx3, dy3, dx4, dy4;
            dx1 = (m_dDashLen1 * (x2 - x1)) / d;
            dy1 = (m_dDashLen1 * (y2 - y1)) / d;

            dx2 = (m_dDashLen2 * (x2 - x1)) / d;
            dy2 = (m_dDashLen2 * (y2 - y1)) / d;

            dx3 = (m_dDashLen3 * (x2 - x1)) / d;
            dy3 = (m_dDashLen3 * (y2 - y1)) / d;

            dx4 = (m_dDashLen4 * (x2 - x1)) / d;
            dy4 = (m_dDashLen4 * (y2 - y1)) / d;

            bDone = false;

            int iDashCounter = 1;
            do {
              iDashCounter++;

              if (iDashCounter % 2) {
                x2 = x1 + dx1;
                y2 = y1 + dy1;
              } else {
                x2 = x1 + dx3;
                y2 = y1 + dy3;
              }

              if (ClipSegmentByArea(x1, y1, x2, y2, area) == rvG_OK) {
                if (bet(it->arg[0], it->arg[1], x2, y2, it->arg[2],
                        it->arg[3])) {
                  if (it->sensitive)
                    iRv = this->ClipSegmentByEmptyCircles(
                        x1, y1, x2, y2, Out, m_EmptyCircles.begin());
                  else
                    iRv = Out.DrawSegment(x1, y1, x2, y2);

                  if ((iRv != rvG_OK) && (iRv != rvG_EmptySet))
                    return iRv;
                } else {
                  if (it->sensitive)
                    iRv = this->ClipSegmentByEmptyCircles(
                        x1, y1, it->arg[2], it->arg[3], Out,
                        m_EmptyCircles.begin());
                  else
                    iRv = Out.DrawSegment(x1, y1, it->arg[2], it->arg[3]);

                  if ((iRv != rvG_OK) && (iRv != rvG_EmptySet))
                    return iRv;
                  bDone = true;
                }

                if (iDashCounter % 2) {
                  x1 = x2 + dx2;
                  y1 = y2 + dy2;
                } else {
                  x1 = x2 + dx4;
                  y1 = y2 + dy4;
                }

                if (!bet(it->arg[0], it->arg[1], x1, y1, it->arg[2],
                         it->arg[3]))
                  bDone = true;
              } else
                bDone = true;
            } while (!bDone);
          } else {
            if (it->sensitive)
              iRv = this->ClipSegmentByEmptyCircles(x1, y1, x2, y2, Out,
                                                    m_EmptyCircles.begin());
            else
              iRv = Out.DrawSegment(x1, y1, x2, y2);

            if ((iRv != rvG_OK) && (iRv != rvG_EmptySet))
              return iRv;
          }
        }

        break;

      // ------------------------------------------------------------

      case drawlinesensitive:

        if ((it->area.x_lb != 0) || (it->area.x_rt != 0) ||
            (it->area.y_lb != 0) || (it->area.y_rt != 0))
          area = it->area;
        else
          area = Default_Area;

        if ((area.x_lb == 0) && (area.x_rt == 0) && (area.y_lb == 0) &&
            (area.y_rt == 0))
          break;

        x1 = it->arg[0];
        y1 = it->arg[1];
        x2 = it->arg[2];
        y2 = it->arg[3];

        iRv = ClipLineByArea(x1, y1, x2, y2, area);
        if (iRv == rvG_OK) {
          if (!it->dashed) {
            iRv = this->ClipSegmentByEmptyCircles(x1, y1, x2, y2, Out,
                                                  m_EmptyCircles.begin());
            if ((iRv != rvG_OK) && (iRv != rvG_EmptySet))
              return iRv;
          } else {
            x1_start = x1;
            y1_start = y1;
            x2_start = x2;
            y2_start = y2;

            d = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));

            if (d < EPS)
              return rvG_OK;

            double dx1, dy1, dx2, dy2, dx3, dy3, dx4, dy4;
            dx1 = (m_dDashLen1 * (x2 - x1)) / d;
            dy1 = (m_dDashLen1 * (y2 - y1)) / d;

            dx2 = (m_dDashLen2 * (x2 - x1)) / d;
            dy2 = (m_dDashLen2 * (y2 - y1)) / d;

            dx3 = (m_dDashLen3 * (x2 - x1)) / d;
            dy3 = (m_dDashLen3 * (y2 - y1)) / d;

            dx4 = (m_dDashLen4 * (x2 - x1)) / d;
            dy4 = (m_dDashLen4 * (y2 - y1)) / d;

            bDone = false;
            int iDashCounter = 1;
            do {
              iDashCounter++;

              if (iDashCounter % 2) {
                x2 = x1 + dx1;
                y2 = y1 + dy1;
              } else {
                x2 = x1 + dx3;
                y2 = y1 + dy3;
              }

              if (bet(x1_start, y1_start, x2, y2, x2_start, y2_start)) {
                iRv = this->ClipSegmentByEmptyCircles(x1, y1, x2, y2, Out,
                                                      m_EmptyCircles.begin());
                if ((iRv != rvG_OK) && (iRv != rvG_EmptySet))
                  return iRv;
              } else {
                iRv = this->ClipSegmentByEmptyCircles(
                    x1, y1, x2_start, y2_start, Out, m_EmptyCircles.begin());
                if ((iRv != rvG_OK) && (iRv != rvG_EmptySet))
                  return iRv;
                bDone = true;
              }

              if (iDashCounter % 2) {
                x1 = x2 + dx2;
                y1 = y2 + dy2;
              } else {
                x1 = x2 + dx4;
                y1 = y2 + dy4;
              }

              if (!bet(x1_start, y1_start, x1, y1, x2_start, y2_start))
                bDone = true;
            } while (!bDone);
          }
        } else if (iRv != rvG_EmptySet)
          return iRv;

        break;

      // ------------------------------------------------------------

      case drawarc:

        x1 = it->arg[0];
        y1 = it->arg[1];
        x2 = it->arg[2];
        y2 = it->arg[3];
        phi = it->arg[4];

        while (phi > 360)
          phi -= 360;

        if ((it->area.x_lb != 0) || (it->area.x_rt != 0) ||
            (it->area.y_lb != 0) || (it->area.y_rt != 0))
          area = it->area;
        else
          area = Default_Area;

        r = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));

        if (it->dashed) {
          num = (int)((it->precision * r) / 10);
          if (num < it->precision)
            num = it->precision;

          sinn = sin(2 * PI / num);
          cosn = cos(2 * PI / num);

          if (phi >= 0) {
            for (i = 1; ((360.00 * (double)i) / num) <= phi; i++) {
              x3 = x1 + (x2 - x1) * cosn - (y2 - y1) * sinn;
              y3 = y1 + (y2 - y1) * cosn + (x2 - x1) * sinn;
              if (!it->dashed || i % 3) {
                x1c = x2;
                y1c = y2;
                x2c = x3;
                y2c = y3;
                if (ClipArcByArea(x1, y1, x1c, y1c, 360.0 / num, area,
                                  clipped_arcs,
                                  &number_of_clipped_arcs) == rvG_OK) {
                  int i;
                  for (i = 0; i < number_of_clipped_arcs; i++) {
                    x1 = clipped_arcs[i].xc;
                    y1 = clipped_arcs[i].yc;
                    x1c = clipped_arcs[i].x1;
                    y1c = clipped_arcs[i].y1;
                    angle = clipped_arcs[i].angle;

                    if (it->sensitive)
                      iRv = this->ClipArcByEmptyCircles(
                          x1, y1, x1c, y1c, angle, Out, m_EmptyCircles.begin(),
                          it->precision);
                    else
                      iRv = Out.DrawArc(x1, y1, x1c, y1c, angle, it->precision);
                  }
                }
              }
              x2 = x3;
              y2 = y3;
            }
          } else { // This should never be reached: arc with negative angles
                   // should be transformed into arcs with positive angles
            // *** bug fixed in Dec 2004
            // Didn't work for negative angles
            for (i = -1; ((360.00 * (double)i) / num) >= phi; i--) {
              x3 = x1 + (x2 - x1) * cosn + (y2 - y1) * sinn;
              y3 = y1 + (y2 - y1) * cosn - (x2 - x1) * sinn;
              if (!it->dashed || i % 3) {
                x1c = x2;
                y1c = y2;
                x2c = x3;
                y2c = y3;

                // if (ClipArcByArea(x1, y1, x1c, y1c, angle, area,
                // clipped_arcs,
                if (ClipArcByArea(x1, y1, x1c, y1c, -360.0 / num, area,
                                  clipped_arcs,
                                  &number_of_clipped_arcs) == rvG_OK) {
                  int i;
                  for (i = 0; i < number_of_clipped_arcs; i++) {
                    x1 = clipped_arcs[i].xc;
                    y1 = clipped_arcs[i].yc;
                    x1c = clipped_arcs[i].x1;
                    y1c = clipped_arcs[i].y1;
                    angle = clipped_arcs[i].angle;

                    if (it->sensitive)
                      iRv = this->ClipArcByEmptyCircles(
                          x1, y1, x1c, y1c, angle, Out, m_EmptyCircles.begin(),
                          it->precision);
                    else
                      iRv = Out.DrawArc(x1, y1, x1c, y1c, angle, it->precision);
                  }
                }
              }
              x2 = x3;
              y2 = y3;
            }
          }

          // *** added on august 1997. in order to connect the
          // last small arc with the last point on the big arc
          x3 = x1 + (it->arg[2] - x1) * cos(2 * PI * phi / 360) -
               (it->arg[3] - y1) * sin(2 * PI * phi / 360);
          y3 = y1 + (it->arg[3] - y1) * cos(2 * PI * phi / 360) +
               (it->arg[2] - x1) * sin(2 * PI * phi / 360);
          if (!it->dashed || i % 3) {
            x1c = x2;
            y1c = y2;
            x2c = x3;
            y2c = y3;
            angle = compute_angle_in_degrees(x1c, y1c, x1, y1, x2c, y2c);

            if (ClipArcByArea(x1, y1, x1c, y1c, angle, area, clipped_arcs,
                              &number_of_clipped_arcs) == rvG_OK) {
              int i;
              for (i = 0; i < number_of_clipped_arcs; i++) {
                x1 = clipped_arcs[i].xc;
                y1 = clipped_arcs[i].yc;
                x1c = clipped_arcs[i].x1;
                y1c = clipped_arcs[i].y1;
                angle = clipped_arcs[i].angle;

                if (it->sensitive)
                  iRv = this->ClipArcByEmptyCircles(x1, y1, x1c, y1c, angle,
                                                    Out, m_EmptyCircles.begin(),
                                                    it->precision);
                else
                  iRv = Out.DrawArc(x1, y1, x1c, y1c, angle, it->precision);
              }
            }
          }
        } else // not dashed
        {
          angle = it->arg[4];
          if (ClipArcByArea(x1, y1, x2, y2, angle, area, clipped_arcs,
                            &number_of_clipped_arcs) == rvG_OK) {
            int i;
            for (i = 0; i < number_of_clipped_arcs; i++) {
              x1 = clipped_arcs[i].xc;
              y1 = clipped_arcs[i].yc;
              x2 = clipped_arcs[i].x1;
              y2 = clipped_arcs[i].y1;
              angle = clipped_arcs[i].angle;

              if (it->sensitive)
                iRv = this->ClipArcByEmptyCircles(x1, y1, x2, y2, angle, Out,
                                                  m_EmptyCircles.begin(),
                                                  it->precision);
              else
                iRv = Out.DrawArc(x1, y1, x2, y2, angle, it->precision);

              if ((iRv != rvG_OK) && (iRv != rvG_EmptySet))
                return iRv;
            }
          }
        }

        break;

      // ------------------------------------------------------------

      case emptycircle:
        break;

      // ------------------------------------------------------------

      case filltriangle:
        iRv = Out.FillTriangle(it->arg[0], it->arg[1], it->arg[2], it->arg[3],
                               it->arg[4], it->arg[5]);
        if (iRv != rvG_OK)
          return iRv;

        break;

      // ------------------------------------------------------------

      case fillrectangle:
        iRv = Out.FillRectangle(it->arg[0], it->arg[1], it->arg[2], it->arg[3]);
        if (iRv != rvG_OK)
          return iRv;

        break;

      // ------------------------------------------------------------

      case fillellipsearc:
        iRv = Out.FillEllipseArc(it->arg[0], it->arg[1], it->arg[2], it->arg[3],
                                 it->arg[4], it->arg[5], true);
        if (iRv != rvG_OK)
          return iRv;

        break;

      // ------------------------------------------------------------

      case fillellipsearc_without_triangle:
        iRv = Out.FillEllipseArc(it->arg[0], it->arg[1], it->arg[2], it->arg[3],
                                 it->arg[4], it->arg[5], false);
        if (iRv != rvG_OK)
          return iRv;

        break;

      // ------------------------------------------------------------

      case setthickness:
        iRv = Out.SetThickness(it->arg[0]);
        if (iRv != rvG_OK)
          return iRv;
        break;

      // ------------------------------------------------------------

      case setdashlen:

        m_dDashLen1 = it->arg[0];
        m_dDashLen2 = it->arg[1];
        m_dDashLen3 = it->arg[2];
        m_dDashLen4 = it->arg[3];
        break;

      // ------------------------------------------------------------

      case setcolor:
        iRv = Out.SetColor((unsigned char)it->arg[0], (unsigned char)it->arg[1],
                           (unsigned char)it->arg[2]);
        if (iRv != rvG_OK)
          return iRv;
        break;

      // ------------------------------------------------------------

      case setfontsize:
        iRv = Out.SetFontSize((unsigned char)it->arg[0]);
        if (iRv != rvG_OK)
          return iRv;
        break;

      // ------------------------------------------------------------

      case printtext:
        if ((it->area.x_lb != 0) || (it->area.x_rt != 0) ||
            (it->area.y_lb != 0) || (it->area.y_rt != 0))
          area = it->area;
        else
          area = Default_Area;

        if ((area.x_lb != 0) || (area.x_rt != 0) || (area.y_lb != 0) ||
            (area.y_rt != 0)) {
          if ((it->arg[0] >= area.x_lb) && (it->arg[0] <= area.x_rt) &&
              (it->arg[1] >= area.y_lb) && (it->arg[1] <= area.y_rt)) {
            iRv = Out.PrintText(it->arg[0], it->arg[1], it->pos, it->text);
            if (iRv != rvG_OK)
              return iRv;
          }
        } else {
          iRv = Out.PrintText(it->arg[0], it->arg[1], it->pos, it->text);
          if ((iRv != rvG_OK) && (iRv != rvG_EmptySet))
            return iRv;
        }
        break;

      // ------------------------------------------------------------

      case printcomment:

        iRv = Out.PrintComment(it->text);
        if (iRv != rvG_OK)
          return iRv;
        break;

      // ------------------------------------------------------------

      case directexport:

        iRv = Out.Export(it->text, it->export_format);

        if (iRv != rvG_OK)
          return iRv;
        break;

      // ------------------------------------------------------------

      default:
        break;
      }
    }
  }

  Out.Close();
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CIntermediateRepresentation::ClipSegmentByEmptyCircles(
    double x1, double y1, double x2, double y2, CGCLCOutput &Out,
    list<CGCLCPrimitive>::iterator it) {
  GReturnValue iRv;
  double a, b, c, d, x, xx, y, yy, r;
  double x1c, y1c, x2c, y2c;
  bool bIntersect;

  if (fabs(x1 - x2) < EPS && fabs(y1 - y2) < EPS)
    return rvG_OK;

  // new version, trying to avoid some unnecessary recursion calls; 08.06.2008.
  while (it != m_EmptyCircles.end()) {
    bIntersect = true;

    a = y2 - y1;
    b = x1 - x2;
    c = x2 * y1 - y2 * x1;

    x = it->arg[0];
    y = it->arg[1];
    xx = it->arg[2];
    yy = it->arg[3];
    r = sqrt((xx - x) * (xx - x) + (yy - y) * (yy - y));

    c = c + a * x + b * y;

    if (fabs(a) > EPS) {
      d = a * a * r * r + b * b * r * r - c * c;
      if (d < EPS)
        bIntersect = false;
      else {
        y1c = (-b * c + fabs(a) * sqrt(d)) / (a * a + b * b) + y;
        x1c = -(c + b * (y1c - y)) / a + x;
        y2c = (-b * c - fabs(a) * sqrt(d)) / (a * a + b * b) + y;
        x2c = -(c + b * (y2c - y)) / a + x;
      }
    } else {
      d = a * a * r * r + b * b * r * r - c * c;
      if (d < EPS)
        bIntersect = false;
      else {
        x1c = (-a * c + fabs(b) * sqrt(d)) / (a * a + b * b) + x;
        y1c = -(c + a * (x1c - x)) / b + y;
        x2c = (-a * c - fabs(b) * sqrt(d)) / (a * a + b * b) + x;
        y2c = -(c + a * (x2c - x)) / b + y;
      }
    }

    if (bIntersect &&
        (fabs(x1c - x2c) > EPS || fabs(y1c - y2c) > EPS)) // fixed 14.08.2008.
    {
      if (bet(x1, y1, x1c, y1c, x2, y2)) // B(p1,p1c,p2)
      {
        if (bet(x1, y1, x2c, y2c, x2, y2)) // B(p1,p2c,p2)
        {
          list<CGCLCPrimitive>::iterator jt = it;
          if (bet(x1, y1, x1c, y1c, x2c, y2c)) // B(p1,p1c,p2c,p2)
          {
            iRv = ClipSegmentByEmptyCircles(x1, y1, x1c, y1c, Out, jt++);
            if (iRv != rvG_OK)
              return iRv;
            else {
              x1 = x2c;
              y1 = y2c;
            }
          } else // B(p1,p2c,p1c,p2)
          {
            iRv = ClipSegmentByEmptyCircles(x1, y1, x2c, y2c, Out, jt++);
            if (iRv != rvG_OK)
              return iRv;
            else {
              x1 = x1c;
              y1 = y1c;
            }
          }
        } else // ~B(p1,p2c,p2)
        {
          if (bet(x2c, y2c, x1, y1, x1c, y1c)) // B(p2c,p1,p1c,p2)
          {
            x1 = x1c;
            y1 = y1c;
          } else // B(p1,p1c,p2,p2c)
          {
            x2 = x1c;
            y2 = y1c;
          }
        }
      } else // ~B(p1,p1c,p2)
      {
        if (bet(x1, y1, x2c, y2c, x2, y2)) // B(p1,p2c,p2)
        {
          if (bet(x1c, y1c, x1, y1, x2c, y2c)) // B(p1c,p1,p2c,p2)
          {
            x1 = x2c;
            y1 = y2c;
          } else // B(p1,p2c,p2,p1c)
          {
            x2 = x2c;
            y2 = y2c;
          }
        } else // ~B(p1,p2c,p2)
        {
          if (bet(x1c, y1c, x1, y1, x2c, y2c) ||
              bet(x1c, y1c, x2, y2, x2c, y2c)) // B(p1c,p1,p2,p2c)
            return rvG_OK;
          if (fabs(x1c - x1) < EPS && fabs(y1c - y1) < EPS &&
              fabs(x2c - x2) < EPS && fabs(y2c - y2) < EPS)
            return rvG_OK; // added 29.08.2009
          if (fabs(x1c - x2) < EPS && fabs(y1c - y2) < EPS &&
              fabs(x2c - x1) < EPS && fabs(y2c - y1) < EPS)
            return rvG_OK; // added 29.08.2009
        }
      }
    }
    it++;
  }

  return Out.DrawSegment(x1, y1, x2, y2); // pCommand == NULL
}

// ----------------------------------------------------------------------------

GReturnValue CIntermediateRepresentation::ClipArcByEmptyCircles(
    double x1, double y1, double xr1, double yr1, double angle,
    CGCLCOutput &Out, list<CGCLCPrimitive>::iterator it, int precision) {
  double a, b, c, d, e, f;
  double x2, y2, xr2, yr2;
  double xi1, yi1, xi2, yi2;
  double r1, r2;
  bool bIntersect;

  if (fabs(x1 - xr1) < EPS && fabs(y1 - yr1) < EPS)
    return rvG_OK;

  // new version, trying to avoid unnecessary recursion calls; 08.06.2008.
  while (it != m_EmptyCircles.end()) {
    bIntersect = true;

    r1 = sqrt((x1 - xr1) * (x1 - xr1) + (y1 - yr1) * (y1 - yr1));

    x2 = it->arg[0];
    y2 = it->arg[1];
    xr2 = it->arg[2];
    yr2 = it->arg[3];
    r2 = sqrt((x2 - xr2) * (x2 - xr2) + (y2 - yr2) * (y2 - yr2));

    d = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));

    if (r1 >= r2) {
      if ((d < r1 - r2) || (d > r1 + r2))
        bIntersect = false;
    } else {
      if ((d < r2 - r1) || (d > r1 + r2))
        bIntersect = false;
    }

    d = r1 * r1 - r2 * r2 + (x2 * x2 + y2 * y2) - (x1 * x1 + y1 * y1);

    if ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) < EPS)
      bIntersect = false;

    if (bIntersect) {
      if (fabs(x1 - x2) < EPS) {
        e = d / (2 * y2 - 2 * y1);
        f = (2 * x1 - 2 * x2) / (2 * y2 - 2 * y1);
        a = 1 + f * f;
        b = 2 * e * f - 2 * x1 - 2 * f * y1;
        c = e * e - 2 * e * y1 + x1 * x1 + y1 * y1 - r1 * r1;
        xi1 = (-b - sqrt(b * b - 4 * a * c)) / (2 * a);
        yi1 = e + f * xi1;
        xi2 = (-b + sqrt(b * b - 4 * a * c)) / (2 * a);
        yi2 = e + f * xi2;
      } else {
        e = d / (2 * x2 - 2 * x1);
        f = (2 * y1 - 2 * y2) / (2 * x2 - 2 * x1);
        a = 1 + f * f;
        b = 2 * e * f - 2 * y1 - 2 * f * x1;
        c = e * e - 2 * e * x1 + x1 * x1 + y1 * y1 - r1 * r1;
        yi1 = (-b - sqrt(b * b - 4 * a * c)) / (2 * a);
        xi1 = e + f * yi1;
        yi2 = (-b + sqrt(b * b - 4 * a * c)) / (2 * a);
        xi2 = e + f * yi2;
      }
      if (fabs(xi1 - xi2) < EPS && fabs(yi1 - yi2) < EPS)
        bIntersect = false;
    }

    if (bIntersect) {
      double angle1, angle2;

      angle1 = compute_angle_in_degrees(xr1, yr1, x1, y1, xi1, yi1);
      angle2 = compute_angle_in_degrees(xr1, yr1, x1, y1, xi2, yi2);

      if (fabs(angle1) < EPS)
        angle1 = 0;

      if (fabs(angle2) < EPS)
        angle2 = 0;

      if (angle1 < 0)
        angle1 += 360;

      if (angle2 < 0)
        angle2 += 360;

      bool bInside = false;
      if ((x2 - xr1) * (x2 - xr1) + (y2 - yr1) * (y2 - yr1) < r2 * r2 + EPS)
        bInside = true;

      if (bInside) {
        if (angle2 > angle1) {
          if (angle2 > angle) {
            if (angle > angle1) {
              xr1 = xi1;
              yr1 = yi1;
              angle = angle - angle1;
            } else
              return rvG_OK;
          } else {
            if (angle > angle1) {
              xr1 = xi1;
              yr1 = yi1;
              angle = angle2 - angle1;
            } else
              return rvG_OK;
          }
        } else {
          if (angle1 > angle) {
            if (angle > angle2) {
              xr1 = xi2;
              yr1 = yi2;
              angle = angle - angle2;
            } else
              return rvG_OK;
          } else if (angle > angle2) {
            xr1 = xi2;
            yr1 = yi2;
            angle = angle1 - angle2;
          } else
            return rvG_OK;
        }
      } else {
        list<CGCLCPrimitive>::iterator jt = it;
        if (angle2 > angle1) {

          if (angle > angle1) {
            ClipArcByEmptyCircles(x1, y1, xr1, yr1, angle1, Out, ++jt,
                                  precision);
            if (angle > angle2) {
              xr1 = xi2;
              yr1 = yi2;
              angle = angle - angle2;
            } else
              return rvG_OK;
          }
        } else if (angle > angle2) {
          ClipArcByEmptyCircles(x1, y1, xr1, yr1, angle2, Out, ++jt, precision);
          if (angle > angle1) {
            xr1 = xi1;
            yr1 = yi1;
            angle = angle - angle1;
          } else
            return rvG_OK;
        }
      }
    }
    it++;
  }

  return Out.DrawArc(x1, y1, xr1, yr1, angle, precision); // pCommand == NULL
}

// ----------------------------------------------------------------------------

GReturnValue CIntermediateRepresentation::ClipPixelByEmptyCircles(
    double x1, double y1, CGCLCOutput &Out, list<CGCLCPrimitive>::iterator it) {
  // new, non-recursive version - 08.06.2008.
  double x, xx, y, yy, r;
  int ii = 0;

  while (it != m_EmptyCircles.end()) {

    ii++;
    if (it->type == emptycircle) {
      x = it->arg[0];
      y = it->arg[1];
      xx = it->arg[2];
      yy = it->arg[3];
      r = sqrt((xx - x) * (xx - x) + (yy - y) * (yy - y));

      if ((x1 - x) * (x1 - x) + (y1 - y) * (y1 - y) <= r * r)
        return rvG_OK;
    }
    it++;
  }
  return Out.SetPixel(x1, y1);
}

// ----------------------------------------------------------------------------

GReturnValue CIntermediateRepresentation::ClipLineByArea(double &x1, double &y1,
                                                         double &x2, double &y2,
                                                         GCLC_area &area) {
  if (clip_line(x1, y1, x2, y2, area.x_lb, area.y_lb, area.x_rt, area.y_rt, x1,
                y1, x2, y2) == true)
    return rvG_OK;

  return rvG_EmptySet;
}

// ----------------------------------------------------------------------------

GReturnValue CIntermediateRepresentation::ClipSegmentByArea(
    double &x1, double &y1, double &x2, double &y2, GCLC_area &area) {
  if (clip_segment(x1, y1, x2, y2, area.x_lb, area.y_lb, area.x_rt, area.y_rt,
                   x1, y1, x2, y2) == true)
    return rvG_OK;

  return rvG_EmptySet;
}

// ----------------------------------------------------------------------------

GReturnValue CIntermediateRepresentation::ClipArcByArea(
    double x1, double y1, double x2, double y2, double angle, GCLC_area &area,
    GCLC_Arc *arcs, int *number_of_arcs) {
  arcs[0].xc = x1;
  arcs[0].yc = y1;
  arcs[0].x1 = x2;
  arcs[0].y1 = y2;
  arcs[0].angle = angle;
  *number_of_arcs = 1;

  clip_arc_x(true, area.x_lb, area, arcs, number_of_arcs);
  clip_arc_x(false, area.x_rt, area, arcs, number_of_arcs);
  clip_arc_y(true, area.y_lb, area, arcs, number_of_arcs);
  clip_arc_y(false, area.y_rt, area, arcs, number_of_arcs);

  return rvG_OK;
}

// ----------------------------------------------------------------------------

bool CIntermediateRepresentation::SkipLayer(unsigned int l) {
  return m_aSkipLayer[l];
}

// ----------------------------------------------------------------------------

void clip_arc_x(bool bLeft, double x, GCLC_area & /*area*/, GCLC_Arc *arcs,
                int *number_of_arcs) {
  double xc, yc, xr, yr, angle;
  double d, D, r;
  int i, j;
  bool bCond;

  for (i = 0; i < (*number_of_arcs); i++) {
    xc = arcs[i].xc;
    yc = arcs[i].yc;
    xr = arcs[i].x1;
    yr = arcs[i].y1;
    angle = arcs[i].angle;

    r = sqrt((xr - xc) * (xr - xc) + (yr - yc) * (yr - yc));

    D = r * r - (x - xc) * (x - xc);
    if (D > 0) {
      double xi1, yi1, xi2, yi2, angle1, angle2;

      d = sqrt(D);

      xi1 = x;
      yi1 = yc + d;

      xi2 = x;
      yi2 = yc - d;

      if (fabs(xi1 - xi2) > EPS || fabs(yi1 - yi2) > EPS) {

        angle1 = compute_angle_in_degrees(xr, yr, xc, yc, xi1, yi1);
        angle2 = compute_angle_in_degrees(xr, yr, xc, yc, xi2, yi2);

        if (angle1 < 0)
          angle1 += 360;

        if (angle2 < 0)
          angle2 += 360;

        if (angle2 < angle1) {
          double xt, yt, anglet;

          xt = xi1;
          yt = yi1;
          anglet = angle1;

          xi1 = xi2;
          yi1 = yi2;
          angle1 = angle2;

          xi2 = xt;
          yi2 = yt;
          angle2 = anglet;
        }

        if (bLeft)
          bCond = (xr > x);
        else
          bCond = (xr < x);

        if (bCond) {
          if (angle > angle1) {
            arcs[i].xc = xc;
            arcs[i].yc = yc;
            arcs[i].x1 = xr;
            arcs[i].y1 = yr;
            arcs[i].angle = angle1;

            if (angle > angle2) {
              arcs[*number_of_arcs].xc = xc;
              arcs[*number_of_arcs].yc = yc;
              arcs[*number_of_arcs].x1 = xi2;
              arcs[*number_of_arcs].y1 = yi2;
              arcs[*number_of_arcs].angle = angle - angle2;
              (*number_of_arcs)++;
            }
          }
        } else // xr>x
        {
          if (angle1 < EPS &&
              (((yi1 < yi2) && !bLeft) || ((yi2 < yi1) && bLeft))) {
            arcs[i].xc = xc;
            arcs[i].yc = yc;
            arcs[i].x1 = xi2;
            arcs[i].y1 = yi2;

            if (angle > angle2)
              arcs[i].angle = angle - angle2;
            else {
              for (j = i; j < (*number_of_arcs); j++)
                arcs[j] = arcs[j + 1];
              (*number_of_arcs)--;
              i--;
            }
          } else {
            if (angle > angle1) {
              arcs[i].xc = xc;
              arcs[i].yc = yc;
              arcs[i].x1 = xi1;
              arcs[i].y1 = yi1;

              if (angle > angle2)
                arcs[i].angle = angle2 - angle1;
              else
                arcs[i].angle = angle - angle1;
            } else {
              for (j = i; j < (*number_of_arcs); j++)
                arcs[j] = arcs[j + 1];
              (*number_of_arcs)--;
              i--;
            }
          }
        }
      }
    } else {
      bCond = bLeft ? xc < x : xc > x;
      if (bCond) {
        for (j = i; j < (*number_of_arcs); j++)
          arcs[j] = arcs[j + 1];
        (*number_of_arcs)--;
        i--;
      }
    }
  }
}

// ----------------------------------------------------------------------------

void clip_arc_y(bool bBottom, double y, GCLC_area & /*area*/, GCLC_Arc *arcs,
                int *number_of_arcs) {
  double xc, yc, xr, yr, angle;
  double d, D, r;
  int i, j;
  bool bCond;

  for (i = 0; i < (*number_of_arcs); i++) {
    xc = arcs[i].xc;
    yc = arcs[i].yc;
    xr = arcs[i].x1;
    yr = arcs[i].y1;
    angle = arcs[i].angle;

    r = sqrt((xr - xc) * (xr - xc) + (yr - yc) * (yr - yc));

    D = r * r - (y - yc) * (y - yc);
    if (D > 0) {
      double xi1, yi1, xi2, yi2, angle1, angle2;

      d = sqrt(D);

      xi1 = xc + d;
      yi1 = y;

      xi2 = xc - d;
      yi2 = y;

      if (fabs(xi1 - xi2) > EPS || fabs(yi1 - yi2) > EPS) {
        angle1 = compute_angle_in_degrees(xr, yr, xc, yc, xi1, yi1);
        angle2 = compute_angle_in_degrees(xr, yr, xc, yc, xi2, yi2);

        if (angle1 < 0)
          angle1 += 360;

        if (angle2 < 0)
          angle2 += 360;

        if (angle2 < angle1) {
          double xt, yt, anglet;

          xt = xi1;
          yt = yi1;
          anglet = angle1;

          xi1 = xi2;
          yi1 = yi2;
          angle1 = angle2;

          xi2 = xt;
          yi2 = yt;
          angle2 = anglet;
        }

        if (bBottom)
          bCond = (yr > y);
        else
          bCond = (yr < y);

        if (bCond) {
          if (angle > angle1) {
            arcs[i].xc = xc;
            arcs[i].yc = yc;
            arcs[i].x1 = xr;
            arcs[i].y1 = yr;
            arcs[i].angle = angle1;

            if (angle > angle2) {
              arcs[*number_of_arcs].xc = xc;
              arcs[*number_of_arcs].yc = yc;
              arcs[*number_of_arcs].x1 = xi2;
              arcs[*number_of_arcs].y1 = yi2;
              arcs[*number_of_arcs].angle = angle - angle2;
              (*number_of_arcs)++;
            }
          }
        } else // xr>x
        {
          if (angle1 < EPS &&
              (((xi1 < xi2) && bBottom) || ((xi2 < xi1) && !bBottom))) {
            arcs[i].xc = xc;
            arcs[i].yc = yc;
            arcs[i].x1 = xi2;
            arcs[i].y1 = yi2;

            if (angle > angle2)
              arcs[i].angle = angle - angle2;
            else {
              for (j = i; j < (*number_of_arcs); j++)
                arcs[j] = arcs[j + 1];
              (*number_of_arcs)--;
              i--;
            }
          } else {
            if (angle > angle1) {
              arcs[i].xc = xc;
              arcs[i].yc = yc;
              arcs[i].x1 = xi1;
              arcs[i].y1 = yi1;

              if (angle > angle2)
                arcs[i].angle = angle2 - angle1;
              else
                arcs[i].angle = angle - angle1;
            } else {
              for (j = i; j < (*number_of_arcs); j++)
                arcs[j] = arcs[j + 1];
              (*number_of_arcs)--;
              i--;
            }
          }
        }
      }
    } else {
      bCond = bBottom ? yc < y : yc > y;
      if (bCond) {
        for (j = i; j < (*number_of_arcs); j++)
          arcs[j] = arcs[j + 1];
        (*number_of_arcs)--;
        i--;
      }
    }
  }
}

// ----------------------------------------------------------------------------
