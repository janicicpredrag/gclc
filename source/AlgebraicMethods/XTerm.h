#pragma once

#include "Term.h"
#include "Power.h"
#include "UPolynomialFraction.h"
#include "ObjectBank.h"
#include <iostream>

class XTerm : public Term
{
	// U fraction (analouge to real factor of U term)
	UPolynomialFraction* _frac;

public:
	XTerm();
	~XTerm();

	XTerm* Clone() override;
	Term* ClonePowers();

	TERM_TYPE Type() const;

	static XTerm* CreatePolynomialConditionTerm(bool f1, uint index1, bool f2, uint index2);

	UPolynomialFraction* GetUFraction() const;
	void SetUFraction(UPolynomialFraction* uf);
	void AddPower(Power* xp);

	// aritmetic operations
	int Mul(Term* ut);
	int Mul(REAL r);
	int Divide(Term* ut);
    void DivideMonoms(XTerm* t);
	int Inverse();
	bool IsZero() const;

	// printing
	void PrettyPrint() const;
	void PrintLatex(std::ostream &os) const;

	// overriden methods
	int Compare(Term* t) const;
	void Merge(Term* t);
};
