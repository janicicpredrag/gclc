#pragma once

#include "ITimeout.h"
#include "XPolynomial.h"
#include "stdinc.h"
#include <string>


/*************************************************************
*
* Prover.h
*
* Base geometric theorems prover class
*
**************************************************************/

enum PROVER_STATUS {
  PROVER_STATUS_INPROGRESS = 0,
  PROVER_STATUS_PROVED = 1,
  PROVER_STATUS_DISPROVED = 2,
  PROVER_STATUS_NONE = 3,
  PROVER_STATUS_TIMEOUT = 4,
  PROVER_STATUS_COMPLEX = 5,
  PROVER_STATUS_OTHER = 6
};

class Prover {
private:
protected:
  uint _maxt;
  void _ProofInitialize(double timeout);
  void _ProofDeInitialize() const;

public:
  //
  // input parameters:
  // 1) vxps            - hyphotheses system
  // 2) xpConclusion    - conjecture to prove
  // 3) conjectureIndex - index of conjecture (if there is more than one
  // conjecture)
  // 4) timeout         - if positive, time in miliseconds after which proof
  // should be stopped
  //
  // return value:
  // prover status - status of the proof process
  //
  virtual PROVER_STATUS Prove(vxp &vxps, XPolynomial *xpConclusion,
                              int conjectureIndex, double timeout) = 0;
  virtual const std::string Description() = 0;
  uint GetMaxTerms() const;
};
