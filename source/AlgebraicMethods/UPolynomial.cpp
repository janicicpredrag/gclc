#include "UPolynomial.h"
#include <memory>

UPolynomial::UPolynomial()
{
	COSTR("upoly");
}

UPolynomial::~UPolynomial()
{
	DESTR("upoly");
}

//
// constructor of upolynomial as a real constant
//
UPolynomial::UPolynomial(REAL cf)
{
	std::shared_ptr<UTerm> ut = std::make_shared<UTerm>(cf);
	this->AddTerm(ut);

	COSTR("upoly");
}

UPolynomial* UPolynomial::Clone()
{
	UPolynomial* upClone = new UPolynomial();

	_terms->EnumReset();
	while (_terms->EnumMoveNext())
	{
		UTerm* ut = (UTerm*)_terms->EnumGetCurrent();
		std::shared_ptr<Term> utClone = ut->Clone();
		upClone->AddTerm(utClone);
	}

	return upClone;
}

TERM_TYPE UPolynomial::Type() const
{
	return UTERM;
}

bool UPolynomial::IsUnit() const
{
	return this->GetTermCount() == 1 && ((UTerm*)this->GetTerm(0))->IsUnit();
}

//
// latex output
//
void UPolynomial::PrintLatex(std::ostream &os) const
{
	uint size = this->GetTermCount();
	for (uint ii = 0; ii < size; ii++)
	{
		if (ii > 0 && ((UTerm*)this->GetTerm(ii))->GetCoeff() > 0)
		{
			//Log::PrintLogF(0, " + ");
			os << '+';
		}
		this->GetTerm(ii)->PrintLatex(os);
	}
}
