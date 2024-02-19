#pragma once

#include "TermStorage.h"

//
// TermStorage class implemented as a vector
// Constant access time
// O(n) insertion time
//
class TermStorageVector : public TermStorage
{
private:
	std::vector<Term*> _terms;
	int _enumIndex;

public:
	TermStorageVector();
	~TermStorageVector();

	int AddTerm(Term* term);
	uint Count() const;

	Term* GetTerm(uint index) const;

	// enumeration
	void EnumReset();
	bool EnumMoveNext();
	Term* EnumGetCurrent() const;
};
