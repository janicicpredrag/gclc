// TheoremProver.h: interface for the CTheoremProver class.
//
//////////////////////////////////////////////////////////////////////

#include <ostream>
#if !defined(THEOREMPROVER_H)
#define THEOREMPROVER_H

#include "ProverExpression.h"
#include <fstream>
#include <iomanip>
#include <list>
#include <sstream>
#include <string>

#include "../Utils/Timer.h"
#include "../Utils/Utils.h"

#define DEF_LENGHT 2048
#define PROVER_TIMEOUT 10000
#define PROOF_LIMIT 10000

void take_text(const std::string &sInput, int &nInputPos, std::string &sOutput);
void take_id(const std::string &sInput, int &nInputPos, std::string &sOutput);

enum eGCLC_prover_command {
  p_constant,
  p_point,
  p_line,
  p_inter,
  p_pratio,
  p_tratio,
  p_foot,
  online,
  midpoint,
  med,
  perp,
  parallel,
  // new commands for algebraic methods 08.2007
  p_circle,
  oncircle,
  p_interlc,
  p_intercc,
  p_bis
};

enum eGCLC_conjecture_status {
  e_idle,
  e_proved,
  e_disproved,
  e_conjecture_out_of_scope,
  e_construction_out_of_scope,
  e_unknown_timeout,
  e_unknown_toomanysteps,
  e_unknown
};

class CGCLCProverExpression;

class CGCLCProverCommand {
  friend class CTheoremProver;
  friend class CAreaMethod;
  friend class CAlgMethod;
  friend class CWuMethod;

public:
  CGCLCProverCommand();
  virtual ~CGCLCProverCommand();
  void CleanUp();

private:
  eGCLC_prover_command type;
  std::string arg[6];
  double x, y;
};

class CNDGC {
  friend class CTheoremProver;
  friend class CAreaMethod;

public:
  CNDGC();
  virtual ~CNDGC();
  void CleanUp();
  std::string sPrintLaTeX();
  std::string sPrintXML(int indent);

private:
  std::string m_sCondition;
  CGCLCProverExpression m_Left;
  CGCLCProverExpression m_Right;
  GCLCexpression_type m_type;
};

class CNewDef {
  friend class CTheoremProver;

public:
  CNewDef() {}
  virtual ~CNewDef() {}

private:
  std::string m_sDef;
};

class CTheoremProver {
public:
  CTheoremProver();
  virtual ~CTheoremProver();
  virtual void CleanUp();

  void Prove(std::ostream *LaTeXOutputProof, std::ostream *XMLOutputProof, double &Time,
             const std::string &theorem, eGCLC_conjecture_status &Status);
  virtual bool AddProverCommand(eGCLC_prover_command type,
                                const std::string &a1 = "", const std::string &a2 = "",
                                const std::string &a3 = "", const std::string &a4 = "",
                                const std::string &a5 = "");
  virtual bool SetProverConjecture(const std::string &conjecture);
  void SetProofLevel(int n) { m_iProofLevel = n; }
  int GetProofLevel() { return m_iProofLevel; }
  void SetProofLimit(int n) { m_iProofLimit = n; }
  int GetProofLimit() { return m_iProofLimit; }
  void SetProverTimeout(int n) {
    m_iProverTimeout = n;
  }                                                   // n miliseconds timeout
  int GetProverTimeout() { return m_iProverTimeout; } // n miliseconds timeout

  bool ValidConjecture() { return (m_bValidConjecture); }
  const CGCLCProverExpression* GetConjecture() { return m_pConjecture; }

  bool Timeout();

  virtual void PrintMethodSpecificOutput() = 0;
  virtual std::string GetMethodSpecificOutput() = 0;

  bool GetPointsOnLine(const std::string &sLineName, std::string &P1, std::string &P2);

  virtual const std::string &GetName() const = 0;

protected:
  virtual enum eGCLC_conjecture_status
  ProveConjecture(const CGCLCProverExpression &Conj) = 0;
  void PrintNDGConditions(bool itemize = false);
  virtual void PrintProofFooter(eGCLC_conjecture_status eRet);
  virtual void PrintProofTitleXML(const std::string &theoremName);
  virtual void PrintProofTitleLatex(const std::string &theoremName);

  bool ExistsPoint(const std::string &A);
  bool Collinear(const std::string &A, const std::string &B, const std::string &C);
  bool OnTheLine(const std::string &P, const std::string &l);

  bool AddNewDef(std::string &sDef);
  bool AddNDG(GCLCexpression_type type, const CGCLCProverExpression &Left,
              const CGCLCProverExpression &Right, const std::string &sCond);

  bool CalculateCoordinates(CGCLCProverCommand &Command) const;
  bool GetPointCoordinates(const std::string &sPoint, double &x1, double &y) const;

  void PrintLaTeX(const std::string &s);
  void PrintXML(const std::string &s);

  bool GetExpression(const std::string &conjecture, int &nPos,
                     CGCLCProverExpression &exp);

  CGCLCProverExpression m_InitialConjecture;
  CGCLCProverExpression* m_pConjecture;
  bool m_bValidConjecture;
  std::list<CNewDef> m_NewDefs;

  std::list<CGCLCProverCommand> m_ProverCommands;

  std::list<CNDGC> m_NDGCs;
  std::ostream *LaTeXOutputProofStream, *XMLOutputProofStream;

  int m_iProofLevel;
  int m_iProofLimit;
  int m_iProofDepth;
  int m_iProverTimeout;
  bool m_bXML;

  CTimer m_Timer;

private:
  bool AddProverCommandLowLevel(eGCLC_prover_command type,
                                const std::string &a1 = "", const std::string &a2 = "",
                                const std::string &a3 = "", const std::string &a4 = "",
                                const std::string &a5 = "", const std::string &a6 = "");
  bool PrintList();
  void PrintNGDCs();
  int m_iPointCounter;
};

#endif // !defined(THEOREMPROVER_H)
