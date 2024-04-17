// GCompiler.cpp: implementation of the GCompiler class.
//
//////////////////////////////////////////////////////////////////////

#include "GCompiler.h"
#include "../Export/GCLCOutput.h"
#include "../TheoremProver/AreaMethod.h"
#include "../TheoremProver/GroebnerMethod.h"
#include "../TheoremProver/WuMethod.h"
#include "IntermediateRepresentation.h"

// ----------------------------------------------------------------------------

CGCompiler::CGCompiler() {
  m_pPrim = new CIntermediateRepresentation;
  m_bExternIntermediateRepresentations = false;
  m_bExternProverCommands = false;
  m_pTheoremProver = NULL;

  m_dLastThickness = 0.16;
  m_r = 0;
  m_g = 0;
  m_b = 0;
}

// ----------------------------------------------------------------------------

CGCompiler::CGCompiler(CIntermediateRepresentation *pPrim) {
  m_pPrim = pPrim;
  m_bExternIntermediateRepresentations = true;
  m_bExternProverCommands = false;
  m_pTheoremProver = NULL;

  m_dLastThickness = 0.16;
  m_r = 0;
  m_g = 0;
  m_b = 0;
}

// ----------------------------------------------------------------------------

CGCompiler::~CGCompiler() { CleanUp(); }

// ----------------------------------------------------------------------------

void CGCompiler::CleanUp() {
  if (!m_bExternIntermediateRepresentations && m_pPrim != NULL) {
    delete m_pPrim;
    m_pPrim = NULL;
  }
  if (!m_bExternProverCommands && m_pTheoremProver != NULL) {
    delete m_pTheoremProver;
    m_pTheoremProver = NULL;
  }
  m_FixedPoints.clear();
  m_TracePoints.clear();
}

// ----------------------------------------------------------------------------

GReturnValue CGCompiler::SetPixel(double x1, double y1, GCLC_area &area) {
  return m_pPrim->SetPixel(x1, y1, area);
}

// ----------------------------------------------------------------------------

GReturnValue CGCompiler::DrawSegment(double x1, double y1, double x2, double y2,
                                     bool sensitive, bool dashed,
                                     GCLC_area &area) {
  return m_pPrim->DrawSegment(x1, y1, x2, y2, sensitive, dashed, area);
}

// ----------------------------------------------------------------------------

GReturnValue CGCompiler::EmptyCircle(double x1, double y1, double x2, double y2,
                                     GCLC_area &area) {
  return m_pPrim->EmptyCircle(x1, y1, x2, y2, area);
}

// ----------------------------------------------------------------------------

GReturnValue CGCompiler::FillTriangle(double x1, double y1, double x2,
                                      double y2, double x3, double y3,
                                      GCLC_area &area) {
  return m_pPrim->FillTriangle(x1, y1, x2, y2, x3, y3, area);
}

// ----------------------------------------------------------------------------

GReturnValue CGCompiler::FillRectangle(double x1, double y1, double x2,
                                       double y2, GCLC_area &area) {
  return m_pPrim->FillRectangle(x1, y1, x2, y2, area);
}

// ----------------------------------------------------------------------------

GReturnValue CGCompiler::FillEllipseArc(double x1, double y1, double a,
                                        double b, double phi1, double phi2,
                                        GCLC_area &area, bool bFillTriangle) {
  return m_pPrim->FillEllipseArc(x1, y1, a, b, phi1, phi2, area, bFillTriangle);
}

// ----------------------------------------------------------------------------

GReturnValue CGCompiler::SetLayer(unsigned int l) {
  return m_pPrim->SetLayer(l);
}

// ----------------------------------------------------------------------------

GReturnValue CGCompiler::HideLayer(unsigned int l) {
  return m_pPrim->HideLayer(l);
}

// ----------------------------------------------------------------------------

GReturnValue CGCompiler::HideLayersFrom(unsigned int l) {
  return m_pPrim->HideLayersFrom(l);
}

// ----------------------------------------------------------------------------

GReturnValue CGCompiler::HideLayersTo(unsigned int l) {
  return m_pPrim->HideLayersTo(l);
}

// ----------------------------------------------------------------------------

GReturnValue CGCompiler::SetThickness(double t) {
  m_dLastThickness = t;
  return m_pPrim->SetThickness(t);
}

// ----------------------------------------------------------------------------

GReturnValue CGCompiler::SetDashLen(double d1, double d2, double d3,
                                    double d4) {
  return m_pPrim->SetDashLen(d1, d2, d3, d4);
}

// ----------------------------------------------------------------------------

GReturnValue CGCompiler::PrintText(double x, double y, const std::string &position,
                                   const std::string &text, GCLC_area &area) {
  return m_pPrim->PrintText(x, y, position, text, area);
}

// ----------------------------------------------------------------------------

GReturnValue CGCompiler::PrintComment(const std::string &text) {
  return m_pPrim->PrintComment(text);
}

// ----------------------------------------------------------------------------

GReturnValue CGCompiler::DirectExport(const std::string &text, export_type format) {
  return m_pPrim->DirectExport(text, format);
}

// ----------------------------------------------------------------------------

GReturnValue CGCompiler::SetDim(double x, double y) {
  return m_pPrim->SetDim(x, y);
}

// ----------------------------------------------------------------------------

GReturnValue CGCompiler::SetColor(unsigned char r, unsigned char g,
                                  unsigned char b) {
  m_r = r;
  m_g = g;
  m_b = b;
  return m_pPrim->SetColor(r, g, b);
}

// ----------------------------------------------------------------------------

GReturnValue CGCompiler::SetBackgroundColor(unsigned char r, unsigned char g,
                                            unsigned char b) {
  return m_pPrim->SetBackgroundColor(r, g, b);
}

// ----------------------------------------------------------------------------

GReturnValue CGCompiler::SetFontSize(unsigned char size) {
  return m_pPrim->SetFontSize(size);
}

// ----------------------------------------------------------------------------

GReturnValue CGCompiler::Export(CGCLCOutput &Output) {
  return m_pPrim->Export(Output);
}

// ----------------------------------------------------------------------------

GReturnValue CGCompiler::DrawLineSensitive(double x1, double y1, double x2,
                                           double y2, bool dashed,
                                           GCLC_area &area) {
  return m_pPrim->DrawLineSensitive(x1, y1, x2, y2, dashed, area);
}

// ----------------------------------------------------------------------------

bool CGCompiler::FixedPointExists(const std::string &sName) {
  return (m_FixedPoints.find(sName) != m_FixedPoints.end());
}

// ----------------------------------------------------------------------------

GReturnValue CGCompiler::AddFixedPoint(double x, double y, double x1, double y1,
                                       const std::string &sName, int iStartLine,
                                       int iStartPosition, int iEndLine,
                                       int iEndPosition, int iEndDestLine,
                                       int iEndDestPos) {
  m_FixedPoints.insert(std::pair<std::string, CFixedPoint>(
      sName, CFixedPoint(x, y, x1, y1, sName, iStartLine, iStartPosition,
                         iEndLine, iEndPosition, iEndDestLine, iEndDestPos)));
  return rvG_OK;
}

// ----------------------------------------------------------------------------

GReturnValue CGCompiler::AddTracePoint(unsigned char r, unsigned char g,
                                       unsigned char b, const std::string &sName) {
  m_TracePoints.insert(
      std::pair<std::string, CTracePoint>(sName, CTracePoint(r, g, b, sName)));
  return rvG_OK;
}

// ----------------------------------------------------------------------------

void CGCompiler::InitTheoremProver(eTheoremProvingMethod &method) {
  if (m_pTheoremProver != NULL) {
    delete m_pTheoremProver;
    m_pTheoremProver = NULL;
  }
  if (method == tpAreaMethod)
    m_pTheoremProver = new CAreaMethod;
  else if (method == tpWuMethod)
    m_pTheoremProver = new CWuMethod;
  else if (method == tpGroebnerMethod)
    m_pTheoremProver = new CGroebnerMethod;
  if (m_pTheoremProver != nullptr)
    m_bExternProverCommands = true;
}

// ----------------------------------------------------------------------------

GReturnValue
CGCompiler::AddProverCommand(eGCLC_prover_command type, const std::string &arg1,
                             const std::string &arg2, const std::string &arg3,
                             const std::string &arg4, const std::string &arg5) {
  if (m_pTheoremProver->AddProverCommand(type, arg1, arg2, arg3, arg4, arg5))
    return rvG_OK;
  else
    return rvG_InvalidInput;
}

// ----------------------------------------------------------------------------

GReturnValue CGCompiler::SetProverConjecture(const std::string &conjecture) {
  if (m_pTheoremProver->SetProverConjecture(conjecture))
    return rvG_OK;
  else
    return rvG_InvalidInput;
}

// ----------------------------------------------------------------------------

void CGCompiler::SetProofLevel(int level) {
  m_pTheoremProver->SetProofLevel(level);
}

// ----------------------------------------------------------------------------

int CGCompiler::GetProofLevel() { return m_pTheoremProver->GetProofLevel(); }

// ----------------------------------------------------------------------------

void CGCompiler::SetProofLimit(int limit) {
  m_pTheoremProver->SetProofLimit(limit);
}

// ----------------------------------------------------------------------------

int CGCompiler::GetProofLimit() { return m_pTheoremProver->GetProofLimit(); }

// ----------------------------------------------------------------------------

void CGCompiler::SetProverTimeout(int timeout) {
  m_pTheoremProver->SetProverTimeout(timeout);
}

// ----------------------------------------------------------------------------

bool CGCompiler::GetPointsOnLine(const std::string &sLineName, std::string &P1,
                                 std::string &P2) {
  return m_pTheoremProver->GetPointsOnLine(sLineName, P1, P2);
}

// ----------------------------------------------------------------------------

GReturnValue CGCompiler::Prove(const std::string &sLaTeXProof,
                               const std::string &sXMLProof, double &Time,
                               const std::string &sTheoremName,
                               eGCLC_conjecture_status &Status) {
  if (!m_pTheoremProver->Prove(sLaTeXProof, sXMLProof, Time, sTheoremName,
                               Status))
    return rvG_CannotOpenOutputFile;
  if (Status == e_proved)
    return rvG_OK;
  return rvG_ProverFailed;
}

// ----------------------------------------------------------------------------

std::string CGCompiler::GetMethodSpecificOutput() {
  return m_pTheoremProver->GetMethodSpecificOutput();
}

// ----------------------------------------------------------------------------

void CGCompiler::CleanUpProver() {
  if (!m_bExternIntermediateRepresentations)
    if (m_pTheoremProver != NULL)
      m_pTheoremProver->CleanUp();
}

// ----------------------------------------------------------------------------

GReturnValue CGCompiler::DrawArcSensitive(double x1, double y1, double x2,
                                          double y2, double r, bool dashed,
                                          GCLC_area &area, int precision) {
  return m_pPrim->DrawArc(x1, y1, x2, y2, r, true, dashed, area, precision);
}

// ----------------------------------------------------------------------------

GReturnValue CGCompiler::DrawCircle(double x1, double y1, double x2, double y2,
                                    bool sensitive, bool dashed,
                                    GCLC_area &area, int precision) {
  return m_pPrim->DrawArc(x1, y1, x2, y2, 360, sensitive, dashed, area,
                          precision);
}

// ----------------------------------------------------------------------------

GReturnValue CGCompiler::DrawTraceSegment(double x1, double y1, double x2,
                                          double y2) {
  GCLC_area area = {0, 0, 0, 0};
  return m_pPrim->DrawSegment(x1, y1, x2, y2, true, false, area);
}

// ----------------------------------------------------------------------------

GReturnValue CGCompiler::SetTraceColorWidth(unsigned char r, unsigned char g,
                                            unsigned char b, double thickness) {
  GReturnValue iRv = m_pPrim->SetThickness(thickness);
  if (iRv != rvG_OK)
    return iRv;

  return m_pPrim->SetColor(r, g, b);
}

// ----------------------------------------------------------------------------

GReturnValue CGCompiler::ResetTraceColorWidth() {
  GReturnValue iRv = m_pPrim->SetThickness(m_dLastThickness);
  if (iRv != rvG_OK)
    return iRv;

  return m_pPrim->SetColor(m_r, m_g, m_b);
}

// ----------------------------------------------------------------------------
