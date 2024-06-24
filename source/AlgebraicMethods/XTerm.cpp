#include "XTerm.h"
#include "XPolynomial.h"
#include "PolyReader.h"
#include <iostream>
#include <memory>

XTerm::XTerm()
{
	COSTR("xterm");
}

XTerm::~XTerm()
{
	DESTR("xterm");
}

std::shared_ptr<Term> XTerm::Clone()
{
	std::shared_ptr<XTerm> xtClone = std::make_shared<XTerm>();

	// fraction
	std::shared_ptr<UPolynomialFraction> ufClone = _frac->Clone();
	xtClone->SetUFraction(ufClone);

	// powers
	for (uint ii = 0; ii < this->GetPowerCount(); ii++)
	{
		std::shared_ptr<Power> xwClone = this->GetPower(ii)->Clone();
		xtClone->AddPower(xwClone);
	}

	return xtClone;
}

std::shared_ptr<XTerm> XTerm::ClonePowers()
{
	std::shared_ptr<XTerm> xtClone = std::make_shared<XTerm>();

	// powers
	for (uint ii = 0; ii < this->GetPowerCount(); ii++)
	{
		std::shared_ptr<Power> xwClone = this->GetPower(ii)->Clone();
		xtClone->AddPower(xwClone);
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
std::shared_ptr<XTerm> XTerm::CreatePolynomialConditionTerm(bool f1,
                                                            uint index1,
                                                            bool f2,
                                                            uint index2) {
	if ((f1 && index1 == 0) || (f2 && index2 == 0))
	{
		return NULL;
	}

	if (f1 == f2 && index1 < index2)
	{
		return CreatePolynomialConditionTerm(f2, index2, f1, index1);
	}

	std::shared_ptr<XTerm> xt = std::make_shared<XTerm>();

	std::shared_ptr<UPolynomialFraction> upf;
	if (f1 || f2)
	{
		// there will be non-empty fraction
		upf = std::make_shared<UPolynomialFraction>();
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
		upf = std::make_shared<UPolynomialFraction>(1);
	}
	xt->SetUFraction(upf);

	std::shared_ptr<Power> p1 =
	  std::make_shared<Power>(index1, 1, f1 ? VAR_TYPE_U : VAR_TYPE_X);
	std::shared_ptr<Power> p2 =
	  std::make_shared<Power>(index2, 1, f2 ? VAR_TYPE_U : VAR_TYPE_X);

	if (!f1)
	{
		xt->AddPower(p1);
	}
	else
	{
		std::shared_ptr<UTerm> ut = std::make_shared<UTerm>();
		ut->AddPower(p1);
		xt->GetUFraction()->GetNumerator()->AddTerm(ut);
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
			std::shared_ptr<UTerm> ut = std::make_shared<UTerm>();
			ut->AddPower(p2);
			xt->GetUFraction()->GetNumerator()->AddTerm(ut);
		}
	}

#if 0
	if (xt)
	{
		Log::PrintLogF(1, "CreatePolynomialConditionTerm(f1 = %d, index1 = %d, f2 = %d, index2 = %d)\n", f1, index1, f2, index2);
		XPolynomial* xp = new XPolynomial();
		xp->AddTerm(xt->Clone());
		PolyReader::PrintPolynomial(xp);
		xp->Dispose();
	}
#endif

	return xt;
}


UPolynomialFraction* XTerm::GetUFraction() const
{
	return _frac.get();
}

void XTerm::SetUFraction(std::shared_ptr<UPolynomialFraction> uf)
{
	_frac = uf;
}

//
// add xpower at the end
// deserialization method
//
void XTerm::AddPower(std::shared_ptr<Power> xp)
{
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
	UPolynomial* n2clone = n2->Clone();
	n2clone->Mul(d1);

	// add n2*d1 to the n1
	n1->Add(n2clone);
	n2clone->Dispose();

	// multiply d1 with d2
	d1->Mul(d2);
}
