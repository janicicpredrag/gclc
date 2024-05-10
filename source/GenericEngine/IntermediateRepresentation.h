// ListOfPrimitives.h: interface for the CIntermediateRepresentation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(LISTOFPRIMITIVES_H)
#define LISTOFPRIMITIVES_H

#include "GCompiler.h"
#include <list>

#define DEFAULT_WIDTH 140
#define DEFAULT_HEIGHT 100
#define NUMBER_OF_LAYERS 1001
#define DEFAULT_DASH 1.5 // default dash length

enum GCLCprimitivecommands {
  setpixel,
  drawsegment,
  drawlinesensitive,
  drawarc,
  drawdashedarc,
  emptycircle,
  filltriangle,
  fillrectangle,
  fillellipsearc,
  fillellipsearc_without_triangle,
  printtext,
  directexport,
  printcomment,
  setthickness,
  setdashlen,
  setcolor,
  setbgcolor,
  setfontsize,
  setlayer,
  dim
};

typedef struct ArcTag {
  double xc;
  double yc;
  double x1;
  double y1;
  double angle;
} GCLC_Arc;

class CGCLCOutput;

class CGCLCPrimitive {
  friend class CIntermediateRepresentation;

public:
  CGCLCPrimitive();

private:
  GCLCprimitivecommands type;
  double arg[6];
  std::string pos;
  std::string text;
  bool dashed;
  bool sensitive;
  GCLC_area area;
  int precision;
  export_type export_format;
};

class CIntermediateRepresentation {
public:
  CIntermediateRepresentation();
  virtual ~CIntermediateRepresentation();
  void CleanUp();

  GReturnValue SetPixel(double x1, double y1, GCLC_area &area);
  GReturnValue DrawSegment(double x1, double y1, double x2, double y2,
                           bool sensitive, bool dashed, GCLC_area &area);
  GReturnValue DrawLineSensitive(double x1, double y1, double x2, double y2,
                                 bool dashed, GCLC_area &area);
  GReturnValue DrawArc(double x1, double y1, double x2, double y2, double r,
                       bool sensitive, bool dashed, GCLC_area &area,
                       int precision);
  GReturnValue EmptyCircle(double x1, double y1, double x2, double y2,
                           GCLC_area &area);
  GReturnValue FillTriangle(double x1, double y1, double x2, double y2,
                            double x3, double y3, GCLC_area &area);
  GReturnValue FillRectangle(double x1, double y1, double x2, double y2,
                             GCLC_area &area);
  GReturnValue FillEllipseArc(double x1, double y1, double a, double b,
                              double phi1, double phi2, GCLC_area &area,
                              bool bFillTriangle);

  GReturnValue PrintText(double x, double y, const std::string &position,
                         const std::string &text, GCLC_area &area);
  GReturnValue PrintComment(const std::string &text);
  GReturnValue DirectExport(const std::string &text, export_type format);

  GReturnValue SetLayer(unsigned int l);
  GReturnValue HideLayer(unsigned int l);
  GReturnValue HideLayersFrom(unsigned int l);
  GReturnValue HideLayersTo(unsigned int l);

  GReturnValue SetFontSize(unsigned char size);
  GReturnValue SetDim(double x, double y);
  GReturnValue SetColor(unsigned char r, unsigned char g, unsigned char b);
  GReturnValue SetBackgroundColor(unsigned char r, unsigned char g,
                                  unsigned char b);
  GReturnValue SetThickness(double t);
  GReturnValue SetDashLen(double d1, double d2, double d3, double d4);

  GReturnValue Export(CGCLCOutput &Output);

  bool SkipLayer(unsigned int l);

private:
  std::list<CGCLCPrimitive> m_ListOfPrimitives, m_EmptyCircles;

  GCLC_area Default_Area;
  bool m_aSkipLayer[NUMBER_OF_LAYERS];

  double m_dDashLen1, m_dDashLen2, m_dDashLen3, m_dDashLen4;

  GReturnValue AddPrimitiveCommand(CGCLCPrimitive &C);
  GReturnValue ClipPixelByEmptyCircles(double x1, double y1, CGCLCOutput &Out,
                                       std::list<CGCLCPrimitive>::iterator it);
  GReturnValue ClipSegmentByEmptyCircles(double x1, double y1, double x2,
                                         double y2, CGCLCOutput &Out,
                                         std::list<CGCLCPrimitive>::iterator it);
  GReturnValue ClipArcByEmptyCircles(double x1, double y1, double x2, double y2,
                                     double angle, CGCLCOutput &Out,
                                     std::list<CGCLCPrimitive>::iterator it,
                                     int precision);
  GReturnValue ClipSegmentByArea(double &x1, double &y1, double &x2, double &y2,
                                 GCLC_area &area);
  GReturnValue ClipLineByArea(double &x1, double &y1, double &x2, double &y2,
                              GCLC_area &area);
  GReturnValue ClipArcByArea(double x1, double y1, double x2, double y2,
                             double angle, GCLC_area &area, GCLC_Arc *arcs,
                             int *number_of_arcs);
};

#endif // !defined(LISTOFPRIMITIVES_H)
