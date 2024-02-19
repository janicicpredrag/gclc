#pragma once

#include "Prover.h"

/*************************************************************
*
* Groebner.h
*
* Algorithms and methods for computing
* Groebner basis of the polynomial set
*
**************************************************************/

class Groebner : public Prover {
private:
  bool ReduceAll(vxp &vxps);
  bool FullReduce(vxp &vxps, XPolynomial *xp, int exclude);
  bool Reduce(XPolynomial *xp1, XPolynomial *xp2);
  int CanReduce(XPolynomial *xp1, XPolynomial *xp2);
  bool GroebnerBasis(vxp &vxps);
  bool GroebnerBasis2(vxp &vxps);
  bool _bTimeout;
  bool GCDCondition(XPolynomial *xp1, XPolynomial *xp2);

public:
  PROVER_STATUS Prove(vxp &vxps, XPolynomial *xpConclusion, int conjectureIndex,
                      double timeout);
  const std::string Description();
};
