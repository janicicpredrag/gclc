// GroebnerMethod.h: interface for the CGroebnerMethod class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(GROEBNERMETHOD_H)
#define GROEBNERMETHOD_H

#include "../AlgebraicMethods/Groebner.h"
#include "AlgMethod.h"

class CGroebnerMethod : public CAlgMethod {
public:
  CGroebnerMethod();
  virtual ~CGroebnerMethod();

  const string &GetName() const;

  Groebner gb_prover;
};

#endif // !defined(GROEBNERMETHOD_H)
