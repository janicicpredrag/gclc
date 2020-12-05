// AreaMethod.h: interface for the CAreaMethod class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AREAMETHOD_H)
#define AREAMETHOD_H

#include "TheoremProver.h"

enum GCLCgeometric_rule {
  er_Orient,
  er_GeometricRuleMax,
};

enum eGCLC_proof_step {
  eps_statement,
  eps_geometric_elimination,
  eps_algebraic,
  eps_grouped_algebraic,
  eps_geometric_simplification,
  eps_grouped_geometric
};

enum GCLCpoint_type {
  e_IntersectionPoint,
  e_PratioPoint,
  e_TratioPoint,
  e_FootPoint,
};

class CAreaMethod : public CTheoremProver {
public:
  CAreaMethod();
  virtual ~CAreaMethod();
  const string &GetName() const;

private:
  enum eGCLC_conjecture_status
  ProveConjecture(const CGCLCProverExpression &Conj);

  void OutputStep(const CGCLCProverExpression &exp, const string &explanation,
                  eGCLC_proof_step type);
  void OutputLaTeXStep(const CGCLCProverExpression &exp,
                       const string &explanation, eGCLC_proof_step type);
  void OutputXMLStep(const CGCLCProverExpression &exp,
                     const string &explanation, eGCLC_proof_step type);
  void PrintMethodSpecificOutput();
  string GetMethodSpecificOutput();

  bool EliminatePoint(GCLCpoint_type ptype, const string &sPoint,
                      list<CGCLCProverCommand>::iterator pCommand,
                      CGCLCProverExpression &exp);
  bool EliminateIntersectionPoint(const string &sPoint,
                                  list<CGCLCProverCommand>::iterator pCommand,
                                  CGCLCProverExpression &Conjecture);
  bool EliminatePratioPoint(const string &sPoint,
                            list<CGCLCProverCommand>::iterator pCommand,
                            CGCLCProverExpression &Conjecture);
  bool EliminateTratioPoint(const string &sPoint,
                            list<CGCLCProverCommand>::iterator pCommand,
                            CGCLCProverExpression &exp);
  bool EliminateFootPoint(const string &sPoint,
                          list<CGCLCProverCommand>::iterator pCommand,
                          CGCLCProverExpression &exp);

  bool ApplyOneGeometricRule(CGCLCProverExpression &exp,
                             GCLCgeometric_rule rule);

  bool AlgebraicSimplification(int exceptlast, eGCLC_conjecture_status *status);

  bool Orient(const string &sPoint, CGCLCProverExpression &exp);
  bool Company(const CGCLCProverExpression &exp,
               CGCLCProverExpression &top_exp);
  bool FindingCompany(CGCLCProverExpression &exp,
                      const CGCLCProverExpression &target_exp);
  bool ZeroGeometryQuantity(CGCLCProverExpression &exp);
  bool ApplyAssumptions(CGCLCProverExpression &exp);

  bool PointOccurs(const CGCLCProverExpression &exp, const string &A) const;
  int PointOccurences(const string &A, const CGCLCProverExpression &exp) const;
  eGCLC_conjecture_status ProveCollinear(const string &A, const string &B,
                                         const string &C);

  bool ExistsS3(const CGCLCProverExpression &exp, const string &A,
                const string &B, const string &C) const;
  bool ExistsS3similar(const CGCLCProverExpression &exp, const string &A,
                       const string &B, const string &C) const;
  bool ExistsP3similar(const CGCLCProverExpression &exp, const string &A,
                       const string &B, const string &C) const;
  bool ExistsSegmentsimilar(const CGCLCProverExpression &exp, const string &A,
                            const string &B) const;

  bool CancelMult(CGCLCProverExpression &exp);

  eGCLC_conjecture_status
  ZeroByAssumption(const CGCLCProverExpression &exp) const;

  double EvaluateExpression(const CGCLCProverExpression &exp) const;
  bool GetConstant(const string &sConstant, double &r) const;

  bool S4_to_S3(CGCLCProverExpression &exp);
  bool P4_to_P3(CGCLCProverExpression &exp);
  bool P3_to_segments(CGCLCProverExpression &exp);
  bool H4points(CGCLCProverExpression &exp);

  int m_iNumberOfEliminationSteps;
  int m_iNumberOfGeometricSteps;
  int m_iNumberOfAlgebraicSteps;
};

#endif // !defined(AREAMETHOD_H)
