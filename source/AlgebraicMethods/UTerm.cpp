#include "UTerm.h"

UTerm::UTerm(REAL coeff)
: _coeff(coeff)
{
	COSTR("uterm");
}

void UTerm::Construct(REAL coeff)
{
	_coeff = coeff;

	COSTR("reused uterm");
}

UTerm::~UTerm()
{
	DESTR("uterm");
}

void UTerm::Dispose()
{
	_refCount --;

	if (_refCount == 0)
	{
#ifdef UTERMS_BANK
		for (int ii = 0, size = _powers.size(); ii < size; ii++)
		{
			_powers[ii]->Dispose();
		}
		_powers.clear();

		UTermsBank::UTermsFactory.ReleaseObject(this);
#else
		delete this;
#endif
	}
}

Term* UTerm::Clone()
{
#ifdef UTERMS_BANK
	UTerm* utClone = UTermsBank::UTermsFactory.AcquireUTerm(this->GetCoeff());
#else
	UTerm* utClone = new UTerm(this->GetCoeff());
#endif

	uint count = this->GetPowerCount();
    for (unsigned int ii = 0; ii < count; ii++)
	{
		Power* uwClone = this->GetPower(ii)->Clone();
		utClone->AddPower(uwClone);
		uwClone->Dispose();
	}

	return utClone;
}

TERM_TYPE UTerm::Type() const
{
	return UTERM;
}

REAL UTerm::GetCoeff() const
{
	return _coeff;
}
void UTerm::SetCoeff(REAL coeff)
{
	_coeff = coeff;
}

//
// add on the end
// this method is used in deserialization
//
void UTerm::AddPower(Power* power)
{
	power->AddRef();
	_powers.push_back(power);
}

bool UTerm::IsUnit() const
{
	return _powers.size() == 0 && _coeff == 1;
}

// aritmetic operations

// merge powers and mul coefficients
int UTerm::Mul(Term* t)
{
	UTerm* ut = (UTerm*)t;

	// coefficients are non-zero by design!
	this->SetCoeff(this->GetCoeff() * ut->GetCoeff());

	// merge powers
	this->MergePowers(ut, true);

	return 0;
}

// simple multiplication
// cannot mul with zero!
int UTerm::Mul(REAL r)
{
	// coefficients are non-zero by design!
	this->SetCoeff(this->GetCoeff() * r);
	return 0;
}

// merge powers and divide coefficients
int UTerm::Divide(Term* t)
{
	UTerm* ut = (UTerm*)t;

	// coefficients are non-zero by design!
	this->SetCoeff(this->GetCoeff() / ut->GetCoeff());

	// merge powers
	this->MergePowers(ut, false);
	return 0;
}

//
// Negation
//
int UTerm::Inverse()
{
	this->SetCoeff(- this->GetCoeff());
	return 0;
}

// printing

//
// {coeff, UP1, UP2, ...}
void UTerm::PrettyPrint() const
{
	Log::PrintLogF(0, "{");
	Object::PrintREAL(_coeff);
	Log::PrintLogF(0, ", ", _coeff);

	uint size = this->GetPowerCount();
	for (uint ii = 0; ii < size; ii++)
	{
		if (ii > 0)
		{
			Log::PrintLogF(0, ", ");
		}
		this->GetPower(ii)->PrettyPrint();
	}

	Log::PrintLogF(0, "}");
}

//
// multiplication of coeff and upowers
//
void UTerm::PrintLatex(StringBuilder* sb) const
{
	if (this->GetPowerCount() == 0 || this->GetCoeff() * this->GetCoeff() != 1)
	{
		//Object::PrintREAL(this->GetCoeff());
		sb->AddREAL(this->GetCoeff());
	}
	else if (this->GetCoeff() == -1)
	{
		//Log::PrintLogF(0, " - ");
		sb->AddChar('-');
	}

	uint size = this->GetPowerCount();
	for (uint ii = 0; ii < size; ii++)
	{
		this->GetPower(ii)->PrintLatex(sb);
	}
}

int UTerm::Compare(Term* other) const
{
	UTerm* ut = (UTerm*)other;

	uint c1 = this->GetPowerCount();
	uint c2 = ut->GetPowerCount();

	uint ii = 0;
	while (ii < c1 && ii < c2)
	{
		int cmp = this->GetPower(ii)->Compare(ut->GetPower(ii));
		if (cmp != 0)
		{
			return cmp;
		}
		ii ++;
	}

	return (c1 < c2 ? -1 : (c1 > c2 ? 1 : 0));
}

//
// Merge with another term
// this is actualy Sum operation
//
void UTerm::Merge(Term* t)
{
	UTerm* ut = (UTerm*)t;

	// powers are same, must sum real coefficients
	this->SetCoeff(this->GetCoeff() + ut->GetCoeff());
}

//
// check zeroness
//
bool UTerm::IsZero() const
{
	// exact zero or in [-epsilon, epsilon] range
	//return this->GetCoeff() == 0;
	return fabs(this->GetCoeff()) < 0.000001;
}
