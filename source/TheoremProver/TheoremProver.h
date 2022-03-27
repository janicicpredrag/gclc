// TheoremProver.h: interface for the CTheoremProver class.
//
//////////////////////////////////////////////////////////////////////

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

using namespace std;

void take_text(const string &sInput, int &nInputPos, string &sOutput);
void take_id(const string &sInput, int &nInputPos, string &sOutput);

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
  string arg[6];
  double x, y;
};

class CNDGC {
  friend class CTheoremProver;
  friend class CAreaMethod;

public:
  CNDGC();
  virtual ~CNDGC();
  void CleanUp();
  string sPrintLaTeX();
  string sPrintXML(int indent);

private:
  string m_sCondition;
  CGCLCProverExpression m_Left;
  CGCLCProverExpression m_Right;
  GCLCexperssion_type m_type;
};

class CNewDef {
  friend class CTheoremProver;

public:
  CNewDef() {}
  virtual ~CNewDef() {}

private:
  string m_sDef;
};

class CTheoremProver {
public:
  CTheoremProver();
  virtual ~CTheoremProver();
  virtual void CleanUp();

  bool Prove(const string &sLaTeXProof, const string &sXMLProof, double &Time,
             const string &theorem, eGCLC_conjecture_status &Status);
  virtual bool AddProverCommand(eGCLC_prover_command type,
                                const string &a1 = "", const string &a2 = "",
                                const string &a3 = "", const string &a4 = "",
                                const string &a5 = "");
  virtual bool SetProverConjecture(const string &conjecture);
  void SetProofLevel(int n) { m_iProofLevel = n; }
  int GetProofLevel() { return m_iProofLevel; }
  void SetProofLimit(int n) { m_iProofLimit = n; }
  int GetProofLimit() { return m_iProofLimit; }
  void SetProverTimeout(int n) {
    m_iProverTimeout = n;
  }                                                   // n miliseconds timeout
  int GetProverTimeout() { return m_iProverTimeout; } // n miliseconds timeout

  bool ValidConjecture() { return (m_bValidConjecture); }
  CGCLCProverExpression GetConjecture() { return m_Conjecture; }

  bool Timeout();

  virtual void PrintMethodSpecificOutput() = 0;
  virtual string GetMethodSpecificOutput() = 0;

  bool GetPointsOnLine(const string &sLineName, string &P1, string &P2);

  virtual const string &GetName() const = 0;

protected:
  virtual enum eGCLC_conjecture_status
  ProveConjecture(const CGCLCProverExpression &Conj) = 0;
  void PrintNDGConditions(bool itemize = false);
  virtual void PrintProofFooter(eGCLC_conjecture_status eRet);
  virtual void PrintProofTitleXML(const string &theoremName);
  virtual void PrintProofTitleLatex(const string &theoremName);

  bool ExistsPoint(const string &A);
  bool Collinear(const string &A, const string &B, const string &C);
  bool OnTheLine(const string &P, const string &l);

  bool AddNewDef(string &sDef);
  bool AddNDG(GCLCexperssion_type type, const CGCLCProverExpression &Left,
              const CGCLCProverExpression &Right, const string &sCond);

  bool CalculateCoordinates(CGCLCProverCommand &Command) const;
  bool GetPointCoordinates(const string &sPoint, double &x1, double &y) const;

  void PrintLaTeX(const string &s);
  void PrintXML(const string &s);

  bool GetExpression(const string &conjecture, int &nPos,
                     CGCLCProverExpression &exp);

  CGCLCProverExpression m_Conjecture;
  bool m_bValidConjecture;
  list<CNewDef> m_NewDefs;

  list<CGCLCProverCommand> m_ProverCommands;

  list<CNDGC> m_NDGCs;
  ofstream m_hLaTeXOutputProof, m_hXMLOutputProof;

  int m_iProofLevel;
  int m_iProofLimit;
  int m_iProofDepth;
  int m_iProverTimeout;
  bool m_bXML;

  CTimer m_Timer;

private:
  bool AddProverCommandLowLevel(eGCLC_prover_command type,
                                const string &a1 = "", const string &a2 = "",
                                const string &a3 = "", const string &a4 = "",
                                const string &a5 = "", const string &a6 = "");
  bool PrintList();
  void PrintNGDCs();
  int m_iPointCounter;
};

#endif // !defined(THEOREMPROVER_H)
