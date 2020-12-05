#pragma once

#include "Prover.h"

/*************************************************************
*
* Wu.h
*
* Algorithms and methods for performing
* Wu's method for proving geometric theorems
*
**************************************************************/

class Wu : public Prover {
private:
#if 0
	bool _Triangulate(vxp& vxps, vector<int>& vars);
	bool _IsTriangular(vxp& vxps, vector<int>& vars);
#endif
  bool _FinalRemainder(vxp &vxps, vector<int> &vars, XPolynomial *xpConclusion,
                       int conjectureIndex);
  bool _IsLinearSystem(vxp &vxps);

public:
  PROVER_STATUS Prove(vxp &vxps, XPolynomial *xpConclusion, int conjectureIndex,
                      double timeout);
  const string Description();
};
