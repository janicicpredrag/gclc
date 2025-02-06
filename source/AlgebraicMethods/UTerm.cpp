#include "Object.h"
#include "UTerm.h"
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <memory>

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

std::shared_ptr<UTerm> UTerm::Clone()
{
	return std::shared_ptr<UTerm>(Clone_impl());
}

UTerm *UTerm::Clone_impl() const
{
	std::unique_ptr<UTerm> utClone =
	  std::unique_ptr<UTerm>(new UTerm(this->GetCoeff()));

	uint count = this->GetPowerCount();
    for (unsigned int ii = 0; ii < count; ii++)
	{
		std::shared_ptr<Power> uwClone = this->GetPower(ii)->Clone();
		utClone->AddPower(uwClone);
	}

	return utClone.release();
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
void UTerm::AddPower(std::shared_ptr<Power> power)
{
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
// print real number without trailing zeros
//
static void PrintREAL(REAL f)
{
	static char dst[37];
	sprintf(dst, "%.20lf", f);

	int len = 0;
	while (dst[len])
	{
		len ++;
	}
	--len;

	while (dst[len] == '0')
	{
		len --;
	}

	if (dst[len] == '.')
	{
		len --;
	}
	dst[len + 1] = 0;

	Log::PrintLogF(0, "%s", dst);
}

//
// {coeff, UP1, UP2, ...}
void UTerm::PrettyPrint() const
{
	Log::PrintLogF(0, "{");
	PrintREAL(_coeff);
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
// stringise a real, trimming trailing zeros
//
static void print_real(std::ostream &os, REAL value)
{
	char dst[37] = {0};
    snprintf(dst, sizeof(dst), "%.3lf", value);

    size_t len = strlen(dst) - 1;

	while (dst[len] == '0')
	{
		len --;
	}

	if (dst[len] == '.')
	{
		len --;
	}
	dst[len + 1] = 0;

	os << dst;
}

//
// multiplication of coeff and upowers
//
void UTerm::PrintLatex(std::ostream &os) const
{
	if (this->GetPowerCount() == 0 || this->GetCoeff() * this->GetCoeff() != 1)
	{
		//Object::PrintREAL(this->GetCoeff());
		print_real(os, this->GetCoeff());
	}
	else if (this->GetCoeff() == -1)
	{
		//Log::PrintLogF(0, " - ");
		os << '-';
	}

	uint size = this->GetPowerCount();
	for (uint ii = 0; ii < size; ii++)
	{
		this->GetPower(ii)->PrintLatex(os);
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
