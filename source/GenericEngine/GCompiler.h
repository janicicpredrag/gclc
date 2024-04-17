// GCompiler.h: interface for the GCompiler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(GCOMPILER_H)
#define GCOMPILER_H

/* ********************************************************
The class CGCompiler provides an abstract/generic interface
between specific geometry engine and the main application,
through a limited set of methods. Some of them aim at
dymanic geometry features and are not relevant for the
console version. The class CGCompiler is inherited by
the class CGCLC (which implements all methods), while it
can be inherited by some other similar class. So, any
geometry engine can be used as long as it fits in the
given interface.
******************************************************** */

#include "../TheoremProver/TheoremProver.h"
#include "../Utils/Common.h"
#include <map>
#include <memory>

class CGCLCInput;
class CGCLCOutput;
class CGCLCLog;
class CIntermediateRepresentation;

typedef struct GCLCobjecttag {
  std::string name;
  int type;
  double p[6];
} GCLC_object;

typedef struct GCLC_areatag { double x_lb, y_lb, x_rt, y_rt; } GCLC_area;

enum eTheoremProvingMethod {
  tpAreaMethod,
  tpWuMethod,
  tpGroebnerMethod,
  tpNone
};

struct prover_config {
  std::string m_sTheoremName;
  std::string sTheoremFileName;
  eTheoremProvingMethod TheoremProvingMethod;
  bool bDeductionControl;
  bool bLaTeX;
  bool bXML;
};

class CFixedPoint {
public:
  CFixedPoint() {}
  CFixedPoint(const CFixedPoint &p) { *this = p; }

  CFixedPoint(double x, double y, double x1, double y1, const std::string &sName,
              int iStartLine, int iStartPosition, int iEndLine,
              int iEndPosition, int iEndDestLine, int iEndDestPosition) {
    this->x = x;
    this->y = y;
    this->x1 = x1;
    this->y1 = y1;
    this->iStartLine = iStartLine;
    this->iStartPosition = iStartPosition;
    this->iEndLine = iEndLine;
    this->iEndPosition = iEndPosition;
    this->iEndDestLine = iEndDestLine;
    this->iEndDestPosition = iEndDestPosition;

    this->sName = sName;
  }

  CFixedPoint &operator=(const CFixedPoint &p) {
    x = p.x;
    y = p.y;
    x1 = p.x1;
    y1 = p.y1;
    iStartLine = p.iStartLine;
    iStartPosition = p.iStartPosition;
    iEndLine = p.iEndLine;
    iEndPosition = p.iEndPosition;
    iEndDestLine = p.iEndDestLine;
    iEndDestPosition = p.iEndDestPosition;
    sName = p.sName;
    return *this;
  }

  const std::string &GetName() { return sName; }

  std::string sName;
  int iStartLine;
  int iStartPosition;
  int iEndLine;
  int iEndPosition;
  int iEndDestLine;
  int iEndDestPosition;
  double x, y, x1, y1;
};

class CTracePoint {
public:
  CTracePoint() {}
  CTracePoint(const CTracePoint &p) { *this = p; }

  CTracePoint(unsigned char r, unsigned char g, unsigned char b,
              const std::string &sName) {
    this->r = r;
    this->g = g;
    this->b = b;
    this->sName = sName;
  }

  CTracePoint &operator=(const CTracePoint &p) {
    r = p.r;
    g = p.g;
    b = p.b;
    sName = p.sName;
    return *this;
  }

  std::string &GetName() { return sName; }
  unsigned char r, g, b;
  std::string sName;
};

class CGCompiler {
public:
  CGCompiler();
  CGCompiler(CIntermediateRepresentation *pPrim);
  virtual ~CGCompiler();
  void CleanUp();

  virtual GReturnValue Import(eGCLC_conjecture_status &prover_output,
                              double &prover_time) = 0;
  virtual GReturnValue GetError(int &iErrorCode, std::string &sErrMessage, int &line,
                                int &pos) = 0;
  GReturnValue Export(CGCLCOutput &Output);

  virtual GReturnValue GetValue(const std::string &sVarName, std::string &sValue) = 0;
  virtual GReturnValue GetPointValue(const std::string &sVarName, double &x,
                                     double &y) = 0;
  bool FixedPointExists(const std::string &sName);

  virtual bool GetAnimationFrames(int &iFrames, int &iSpeed) = 0;
  GReturnValue DrawTraceSegment(double x1, double y1, double x2, double y2);
  GReturnValue SetTraceColorWidth(unsigned char r, unsigned char g,
                                  unsigned char b, double thickness);
  GReturnValue ResetTraceColorWidth();

  void InitTheoremProver(eTheoremProvingMethod &method);
  bool ProvingTheorem() {
    return (m_pTheoremProver != NULL
            /*&& m_ProverConfig.TheoremProvingMethod != tpNone*/);
  }
  bool GetPointsOnLine(const std::string &sLineName, std::string &P1, std::string &P2);
  GReturnValue
  AddProverCommand(eGCLC_prover_command type, const std::string &arg1 = "",
                   const std::string &arg2 = "", const std::string &arg3 = "",
                   const std::string &arg4 = "", const std::string &arg5 = "");
  GReturnValue SetProverConjecture(const std::string &conjecture);
  void SetProofLevel(int level);
  int GetProofLevel();
  void SetProofLimit(int limit);
  int GetProofLimit();
  void SetProverTimeout(int timeout);
  int GetProverTimeout();
  GReturnValue Prove(const std::string &sLaTeXProof, const std::string &sXMLProof,
                     double &Time, const std::string &sTheoremName,
                     eGCLC_conjecture_status &Status);
  void CleanUpProver();
  bool ValidConjecture() {
    return m_pTheoremProver && m_pTheoremProver->ValidConjecture();
  }
  const CGCLCProverExpression* GetConjecture() {
    return m_pTheoremProver->GetConjecture();
  }
  std::string GetMethodSpecificOutput();

protected:
  GReturnValue SetPixel(double x1, double y1, GCLC_area &area);
  GReturnValue DrawSegment(double x1, double y1, double x2, double y2,
                           bool sensitive, bool dashed, GCLC_area &area);
  GReturnValue DrawLineSensitive(double x1, double y1, double x2, double y2,
                                 bool dashed, GCLC_area &area);
  GReturnValue DrawCircle(double x1, double y1, double x2, double y2,
                          bool sensitive, bool dashed, GCLC_area &area,
                          int precision);
  GReturnValue DrawArcSensitive(double x1, double y1, double x2, double y2,
                                double angle, bool dashed, GCLC_area &area,
                                int precision);
  GReturnValue PrintText(double x, double y, const std::string &position,
                         const std::string &text, GCLC_area &area);
  GReturnValue PrintComment(const std::string &text);
  GReturnValue DirectExport(const std::string &text, export_type format);
  GReturnValue EmptyCircle(double x1, double y1, double x2, double y2,
                           GCLC_area &area);
  GReturnValue FillTriangle(double x1, double y1, double x2, double y2,
                            double x3, double y3, GCLC_area &area);
  GReturnValue FillRectangle(double x1, double y1, double x2, double y2,
                             GCLC_area &area);
  GReturnValue FillEllipseArc(double x1, double y1, double a, double b,
                              double phi1, double phi2, GCLC_area &area,
                              bool bFillTriangle);

  GReturnValue SetLayer(unsigned int l);
  GReturnValue HideLayer(unsigned int l);
  GReturnValue HideLayersFrom(unsigned int l);
  GReturnValue HideLayersTo(unsigned int l);

  GReturnValue SetThickness(double t);
  GReturnValue SetDashLen(double d1, double d2, double d3, double d4);
  GReturnValue SetColor(unsigned char r, unsigned char g, unsigned char b);
  GReturnValue SetBackgroundColor(unsigned char r, unsigned char g,
                                  unsigned char b);
  GReturnValue SetFontSize(unsigned char size);
  GReturnValue SetDim(double x, double y);

  GReturnValue AddFixedPoint(double x, double y, double x1, double y1,
                             const std::string &sName, int iStartLine,
                             int iStartPosition, int iEndLine, int iEndPosition,
                             int iStartDestLine, int iEndDestPos);
  GReturnValue AddTracePoint(unsigned char r, unsigned char g, unsigned char b,
                             const std::string &sName);

  CIntermediateRepresentation *m_pPrim;

private:
  bool m_bExternIntermediateRepresentations;

  std::map<std::string, CFixedPoint> m_FixedPoints;
  std::map<std::string, CTracePoint> m_TracePoints;

  double m_dLastThickness;
  unsigned char m_r, m_g, m_b;

  // prover_config m_ProverConfig;
  std::unique_ptr<CTheoremProver> m_pTheoremProver;
};

#endif // !defined(GCOMPILER_H)
