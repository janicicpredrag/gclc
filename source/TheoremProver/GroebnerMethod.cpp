// GroebnerMethod.cpp: implementation of the CGroebnerMethod class.
//
//////////////////////////////////////////////////////////////////////

#include "GroebnerMethod.h"

CGroebnerMethod::CGroebnerMethod() {
  _prover = &gb_prover; /* _prover = new Groebner(); */
}

CGroebnerMethod::~CGroebnerMethod() { /* delete _prover; */
}

const string _grName = "Groebner bases";
const string &CGroebnerMethod::GetName() const { return _grName; }
