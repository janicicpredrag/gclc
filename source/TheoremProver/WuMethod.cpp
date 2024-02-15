// WuMethod.cpp: implementation of the CWuMethod class.
//
//////////////////////////////////////////////////////////////////////

#include "WuMethod.h"

CWuMethod::CWuMethod() { _prover = &wu_prover; /* new Wu(); */ }

CWuMethod::~CWuMethod() { /* delete _prover; */
}

const std::string _wuName = "Wu's";
const std::string &CWuMethod::GetName() const { return _wuName; }
