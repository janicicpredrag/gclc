#include "XTerm.h"
#include "XPolynomial.h"
#include "PolyReader.h"
#include <iostream>

XTerm::XTerm()
: _frac(NULL)
{
	COSTR("xterm");
}

XTerm::~XTerm()
{
	if (_frac)
	{
		_frac->Dispose();
	}

	DESTR("xterm");
}

Term* XTerm::Clone()
{
	XTerm* xtClone = new XTerm();

	// fraction
	UPolynomialFraction* ufClone = _frac->Clone();
	xtClone->SetUFraction(ufClone);
	ufClone->Dispose();

	// powers
	for (uint ii = 0; ii < this->GetPowerCount(); ii++)
	{
		Power* xwClone = this->GetPower(ii)->Clone();
		xtClone->AddPower(xwClone);
		xwClone->Dispose();
	}

	return xtClone;
}

Term* XTerm::ClonePowers()
{
	XTerm* xtClone = new XTerm();

	// powers
	for (uint ii = 0; ii < this->GetPowerCount(); ii++)
	{
		Power* xwClone = this->GetPower(ii)->Clone();
		xtClone->AddPower(xwClone);
		xwClone->Dispose();
	}

	return xtClone;
}

TERM_TYPE XTerm::Type() const
{
	return XTERM;
}

//
// return p1p2
//
XTerm* XTerm::CreatePolynomialConditionTerm(bool f1, uint index1, bool f2, uint index2)
{
	if ((f1 && index1 == 0) || (f2 && index2 == 0))
	{
		return NULL;
	}

	if (f1 == f2 && index1 < index2)
	{
		return CreatePolynomialConditionTerm(f2, index2, f1, index1);
	}

	XTerm* xt = new XTerm();
	Power *p1, *p2;

	UPolynomialFraction* upf;
	if (f1 || f2)
	{
		// there will be non-empty fraction
		upf = new UPolynomialFraction();
		UPolynomial* up = new UPolynomial();
		upf->SetNumerator(up);
		up->Dispose();
		up = new UPolynomial(1);
		upf->SetDenominator(up);
		up->Dispose();
	}
	else
	{
		// unit fraction
		upf = new UPolynomialFraction(1);
	}
	xt->SetUFraction(upf);
	upf->Dispose();

	if (f1)
	{
#ifdef POWER_BANK
		p1 = PowerBank::PowerFactory.AcquirePower(index1, 1, VAR_TYPE_U);
#else
		p1 = new Power(index1, 1, VAR_TYPE_U);
#endif
	}
	else
	{
#ifdef POWER_BANK
		p1 = PowerBank::PowerFactory.AcquirePower(index1, 1, VAR_TYPE_X);
#else
		p1 = new Power(index1, 1, VAR_TYPE_X);
#endif
	}
	if (f2)
	{
#ifdef POWER_BANK
		p2 = PowerBank::PowerFactory.AcquirePower(index2, 1, VAR_TYPE_U);
#else
		p2 = new Power(index2, 1, VAR_TYPE_U);
#endif
	}
	else
	{
#ifdef POWER_BANK
		p2 = PowerBank::PowerFactory.AcquirePower(index2, 1, VAR_TYPE_X);
#else
		p2 = new Power(index2, 1, VAR_TYPE_X);
#endif
	}

	if (!f1)
	{
		xt->AddPower(p1);
	}
	else
	{
#ifdef UTERMS_BANK
		UTerm* ut = UTermsBank::UTermsFactory.AcquireUTerm();
#else
		UTerm* ut = new UTerm();
#endif
		ut->AddPower(p1);
		xt->GetUFraction()->GetNumerator()->AddTerm(ut);
		ut->Dispose();
	}
	if (!f2)
	{
		if (f1 == f2 && index1 == index2)
		{
			xt->GetPower(0)->SetDegree(2);
		}
		else
		{
			xt->AddPower(p2);
		}
	}
	else
	{
		if (f1)
		{
			// add power
			UTerm* ut = (UTerm*)xt->GetUFraction()->GetNumerator()->GetTerm(0);
			if (f1 == f2 && index1 == index2)
			{
				ut->GetPower(0)->SetDegree(2);
			}
			else
			{
				ut->AddPower(p2);
			}
		}
		else
		{
			// add term
#ifdef UTERMS_BANK
			UTerm* ut = UTermsBank::UTermsFactory.AcquireUTerm();
#else
			UTerm* ut = new UTerm();
#endif
			ut->AddPower(p2);
			xt->GetUFraction()->GetNumerator()->AddTerm(ut);
			ut->Dispose();
		}
	}

#if 0
	if (xt)
	{
		Log::PrintLogF(1, "CreatePolynomialConditionTerm(f1 = %d, index1 = %d, f2 = %d, index2 = %d)\n", f1, index1, f2, index2);
		XPolynomial* xp = new XPolynomial();
		xp->AddTerm((XTerm*)xt->Clone());
		PolyReader::PrintPolynomial(xp);
		xp->Dispose();
	}
#endif

	if (p1)
	{
		p1->Dispose();
	}
	if (p2)
	{
		p2->Dispose();
	}

	return xt;
}


UPolynomialFraction* XTerm::GetUFraction() const
{
	return _frac;
}

void XTerm::SetUFraction(UPolynomialFraction* uf)
{
	if (uf)
	{
		uf->AddRef();
	}
	if (_frac)
	{
		_frac->Dispose();
	}
	_frac = uf;
}

//
// add xpower at the end
// deserialization method
//
void XTerm::AddPower(Power* xp)
{
	xp->AddRef();
	_powers.push_back(xp);
}

// merge powers and mul fractions
int XTerm::Mul(Term* t)
{
	int status = 0;

	if (t->Type() != XTERM)
	{
		Log::PrintLogF(1, "XTerm::Mul, argument is not XTERM!\n");
		throw -1;
	}
	XTerm* xt = (XTerm*)t;

	// multiply fractions
	UPolynomialFraction* uf1 = this->GetUFraction();
	UPolynomialFraction* uf2 = xt->GetUFraction();

	status = uf1->GetNumerator()->Mul(uf2->GetNumerator());
	if (status == 0)
	{
		status = uf1->GetDenominator()->Mul(uf2->GetDenominator());\
	}

	// merge powers
	if (status == 0)
	{
		this->MergePowers(xt, true);
	}

	return status;
}

// mul fraction numerator only
int XTerm::Mul(REAL r)
{
	return this->GetUFraction()->GetNumerator()->Mul(r);
}

// merge powers and divide fractions
int XTerm::Divide(Term* t)
{
	int status = 0;
	XTerm* xt = (XTerm*)t;

	// multiply fractions
	UPolynomialFraction* uf1 = this->GetUFraction();
	UPolynomialFraction* uf2 = xt->GetUFraction();

	status = uf1->GetNumerator()->Mul(uf2->GetDenominator());
	if (status == 0)
	{
		status = uf1->GetDenominator()->Mul(uf2->GetNumerator());
	}

	// merge powers
	if (status == 0)
	{
		this->MergePowers(xt, false);
	}

	return status;
}

//
// divide only monoms
// example: x2^3x3 / x2x3^2 = x2
void XTerm::DivideMonoms(XTerm* t)
{
    for (unsigned int ii = 0; ii < this->GetPowerCount(); ii++)
    {
        Power* p = this->GetPower(ii);

        // is there power with same index in other term
        Power* p1 = t->GetPowerByIndex(p->GetIndex());

        if (p1)
        {
            if (p1->GetDegree() >= p->GetDegree())
            {
                this->RemovePower(ii);
            }
            else
            {
                p->SetDegree(p->GetDegree() - p1->GetDegree());
            }
        }
    }
}

//
// Negation
//
int XTerm::Inverse()
{
	UPolynomial* unum = this->GetUFraction()->GetNumerator();
	return unum->Inverse();
}

bool XTerm::IsZero() const
{
	return this->GetUFraction()->GetNumerator()->IsZero();
}

// printing

//
// {UF, XP1, XP2, ...}
//
void XTerm::PrettyPrint() const
{
	Log::PrintLogF(0, "{");

	this->GetUFraction()->PrettyPrint();
	Log::PrintLogF(0, ", ");

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
// multiplication of fraction and powers
//
void XTerm::PrintLatex(std::ostream &os) const
{
	this->GetUFraction()->PrintLatex(os);

	uint size = this->GetPowerCount();
	for (uint ii = 0; ii < size; ii++)
	{
		this->GetPower(ii)->PrintLatex(os);
	}
}

int XTerm::Compare(Term* t) const
{
	XTerm* xt = (XTerm*)t;

	uint c1 = this->GetPowerCount();
	uint c2 = xt->GetPowerCount();

	uint ii = 0;
	while (ii < c1 && ii < c2)
	{
		int cmp = this->GetPower(ii)->Compare(xt->GetPower(ii));
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
void XTerm::Merge(Term* t)
{
	XTerm* xt = (XTerm*)t;

	// powers are same, must sum fractions
	// n1/d1 + n2/d2 = (n1*d2 + n2*d1) / (d1*d2)
	// n1 = n1 * d2 + n2 * d1
	// d1 = d1 * d2

	// n1 = n1 * d2 + n2 * d1
	// n1 *= d2
	// n1 += n2 * d1
	UPolynomialFraction* xtf = xt->GetUFraction();

	UPolynomial* n1 = this->GetUFraction()->GetNumerator();
	UPolynomial* d1 = this->GetUFraction()->GetDenominator();

	UPolynomial* n2 = xtf->GetNumerator();
	UPolynomial* d2 = xtf->GetDenominator();

	// multiply n1 with d2
	n1->Mul(d2);

	// create second sumand, n2*d1
	UPolynomial* n2clone = (UPolynomial*)n2->Clone();
	n2clone->Mul(d1);

	// add n2*d1 to the n1
	n1->Add(n2clone);
	n2clone->Dispose();

	// multiply d1 with d2
	d1->Mul(d2);
}
