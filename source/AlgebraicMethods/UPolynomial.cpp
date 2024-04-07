#include "UPolynomial.h"

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
#ifdef UTERMS_BANK
	UTerm* ut = UTermsBank::UTermsFactory.AcquireUTerm(cf);
#else
	UTerm* ut = new UTerm(cf);
#endif
	this->AddTerm(ut);
	ut->Dispose();

	COSTR("upoly");
}

UPolynomial* UPolynomial::Clone()
{
	UPolynomial* upClone = new UPolynomial();

	_terms->EnumReset();
	while (_terms->EnumMoveNext())
	{
		UTerm* ut = (UTerm*)_terms->EnumGetCurrent();
		UTerm* utClone = (UTerm*)ut->Clone();
		upClone->AddTerm(utClone);
		utClone->Dispose();
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
