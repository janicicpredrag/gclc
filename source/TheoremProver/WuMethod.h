// WuMethod.h: interface for the CWuMethod class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(WUMETHOD_H)
#define WUMETHOD_H

#include "../AlgebraicMethods/Wu.h"
#include "AlgMethod.h"

class CWuMethod : public CAlgMethod {
public:
  CWuMethod();
  virtual ~CWuMethod();

  const string &GetName() const;

  Wu wu_prover;
};

#endif // !defined(WUMETHOD_H)
