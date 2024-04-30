#pragma once

#include "TermStorage.h"
#include <memory>

//
// TermStorage class implemented as a vector
// Constant access time
// O(n) insertion time
//
class TermStorageVector : public TermStorage
{
private:
	std::vector<std::shared_ptr<Term>> _terms;
	int _enumIndex;

public:
	TermStorageVector();
	virtual ~TermStorageVector() = default;

	int AddTerm(std::shared_ptr<Term> term);
	uint Count() const;

	Term* GetTerm(uint index) const;

	// enumeration
	void EnumReset();
	bool EnumMoveNext();
	Term* EnumGetCurrent() const;
};
