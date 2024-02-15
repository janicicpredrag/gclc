// GroebnerMethod.cpp: implementation of the CGroebnerMethod class.
//
//////////////////////////////////////////////////////////////////////

#include "GroebnerMethod.h"

using namespace std;

CGroebnerMethod::CGroebnerMethod() {
  _prover = &gb_prover; /* _prover = new Groebner(); */
}

CGroebnerMethod::~CGroebnerMethod() { /* delete _prover; */
}

const std::string _grName = "Groebner bases";
const std::string &CGroebnerMethod::GetName() const { return _grName; }
