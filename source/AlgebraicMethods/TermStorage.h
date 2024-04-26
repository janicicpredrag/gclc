#pragma once

#include "Term.h"
#include "ITimeout.h"

//
// Abstract terms container class
//
// Term Container must provide quick access to
// its elements and quick addition of the new
// element while keeping elements in sorted order
//
class TermStorage
{
public:

	virtual int AddTerm(Term* term) = 0;
	virtual void RemoveTerm(Term* term) = 0;
	virtual uint Count() const = 0;

	virtual Term* GetTerm(uint index) const = 0;

	// enumeration
	virtual void EnumReset() = 0;
	virtual bool EnumMoveNext() = 0;
	virtual Term* EnumGetCurrent() const = 0;
};
