#include "Log.h"
#include "TermStorageVector.h"

TermStorageVector::TermStorageVector()
{
}

TermStorageVector::~TermStorageVector()
{
	uint size = (uint)_terms.size();
	for (uint ii = 0; ii < size; ii++)
	{
		_terms[ii]->Dispose();
	}
}

int TermStorageVector::AddTerm(Term* term)
{
	term->AddRef();
	_terms.push_back(term);
	return 0;
}

uint TermStorageVector::Count() const
{
	return (uint)_terms.size();
}

Term* TermStorageVector::GetTerm(uint index) const
{
    if (/*index < 0 ||*/ index >= _terms.size())
	{
		Log::PrintLogF(0, "index out of range!");
		throw -1;
	}

	return _terms[index];
}

// enumeration
void TermStorageVector::EnumReset()
{
	_enumIndex = -1;
}

bool TermStorageVector::EnumMoveNext()
{
	_enumIndex ++;
	return _enumIndex < (int)_terms.size();
}

Term* TermStorageVector::EnumGetCurrent() const
{
	if (_enumIndex < 0 || _enumIndex >= (int)_terms.size())
	{
		Log::PrintLogF(0, "index out of range!");
		throw -1;
	}

	return _terms[_enumIndex];
}
