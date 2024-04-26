#include "Polynomial.h"
#include <memory>

Polynomial::Polynomial()
{
	_terms = Term::CreateTermStorage();
	COSTR("poly");
}

Polynomial::~Polynomial()
{
	DESTR("poly");
}

uint Polynomial::GetTermCount() const
{
	return _terms->Count();
}

Term* Polynomial::GetTerm(uint index) const
{
	return _terms->GetTerm(index);
}

bool Polynomial::IsZero() const
{
	return this->GetTermCount() == 0;
}

//
// add at the end
// deserialization method
//
int Polynomial::AddTerm(Term* t)
{
	if (t->Type() != this->Type())
	{
		Log::PrintLogF(0, "Polynomial::AddTerm error: Types are not Equal!\n");
		throw -1;
	}
	return _terms->AddTerm(t);
}

//
// remove term from collection
//
void Polynomial::RemoveTerm(Term* t)
{
	_terms->RemoveTerm(t);
	t->Dispose();
}

// aritmetic operations

//
// Multiply with another polynomial
//
int Polynomial::Mul(Polynomial* p)
{
	int status = 0;

	// case when one of them is zero!
	if (this->IsZero())
	{
		return status;
	}
	if (p->IsZero())
	{
		_terms = Term::CreateTermStorage();
	}

	std::shared_ptr<TermStorage> tmpTerms = Term::CreateTermStorage();

	uint cnt1 = this->GetTermCount();

	for (uint ii = 0; ii < cnt1 && status == 0; ii++)
	{
		Term* t1 = this->GetTerm(ii);

		// multiply with this term
		uint cnt2 = p->GetTermCount();
		for (uint jj = 0; jj < cnt2 && status == 0; jj++)
		{
			Term* t2clone = p->GetTerm(jj)->Clone();

			t2clone->Mul(t1);
			status = tmpTerms->AddTerm(t2clone);
			t2clone->Dispose();
		}
	}

	// now replace term storages
	_terms = tmpTerms;

	return status;
}

//
// Multiply with single term
//
int Polynomial::Mul(Term* t)
{
	int status = 0;
	uint cnt1 = this->GetTermCount();

	for (uint ii = 0; ii < cnt1 && status == 0; ii++)
	{
		Term* t1 = this->GetTerm(ii);
		status = t1->Mul(t);
	}

	return status;
}
//
// Multiply with real number
//
int Polynomial::Mul(REAL r)
{
	int status = 0;

	if (r == 0)
	{
		// reset polynomial
		_terms = Term::CreateTermStorage();
	}
	else
	{
		uint cnt1 = this->GetTermCount();

		for (uint ii = 0; ii < cnt1 && status == 0; ii++)
		{
			Term* t1 = this->GetTerm(ii);
			status = t1->Mul(r);
		}
	}

	return status;
}


//
// Add another polynomial
//
int Polynomial::Add(Polynomial* p)
{
	int status = 0;
	uint count = p->GetTermCount();

	for (uint ii = 0; ii < count && status == 0; ii ++)
	{
		Term* t = p->GetTerm(ii)->Clone();
		status = this->AddTerm(t);
		t->Dispose();
	}

	return status;
}

//
// Subtract another polynomial
//
int Polynomial::Subtract(Polynomial* up)
{
	int status = 0;

	Polynomial* upClone = up->Clone();
	upClone->Inverse();
	status = this->Add(upClone);
	upClone->Dispose();

	return status;
}

//
// Negation
//
int Polynomial::Inverse()
{
	int status = 0;

	uint count = this->GetTermCount();
	for (uint ii = 0; ii < count && status == 0; ii ++)
	{
		Term* t = this->GetTerm(ii);
		status = t->Inverse();
	}

	return status;
}

//
// Leading term over variable with given index
//
Term* Polynomial::LeadTerm(int index, bool free) const
{
	Term* retv = NULL;
	int maxDegree = 0;

	for (int ii = 0, size = this->GetTermCount(); ii < size; ii++)
	{
		Term* t = this->GetTerm(ii);

		int deg = t->VariableDeg(index, free);
		if (deg > maxDegree)
		{
			maxDegree = deg;
			retv = t;
		}
	}

	return retv;
}

// printing
//
// {UT1, UT2, ...}
void Polynomial::PrettyPrint() const
{
	Log::PrintLogF(0, "{");

	uint size = this->GetTermCount();
	for (uint ii = 0; ii < size; ii++)
	{
		if (ii > 0)
		{
			Log::PrintLogF(0, ", ");
		}
		this->GetTerm(ii)->PrettyPrint();
	}

	Log::PrintLogF(0, "}");
}
