// WuMethod.cpp: implementation of the CWuMethod class.
//
//////////////////////////////////////////////////////////////////////

#include "WuMethod.h"

CWuMethod::CWuMethod() { _prover = &wu_prover; /* new Wu(); */ }

CWuMethod::~CWuMethod() { /* delete _prover; */
}

const string _wuName = "Wu's";
const string &CWuMethod::GetName() const { return _wuName; }
