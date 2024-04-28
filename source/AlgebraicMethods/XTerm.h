#pragma once

#include "Term.h"
#include "Power.h"
#include "UPolynomialFraction.h"
#include <iostream>

class XTerm : public Term
{
	// U fraction (analouge to real factor of U term)
	UPolynomialFraction* _frac;

public:
	XTerm();
	~XTerm();

	XTerm* Clone() override;
	XTerm* ClonePowers();

	TERM_TYPE Type() const override;

	static XTerm* CreatePolynomialConditionTerm(bool f1, uint index1, bool f2, uint index2);

	UPolynomialFraction* GetUFraction() const;
	void SetUFraction(UPolynomialFraction* uf);
	void AddPower(Power* xp);

	// aritmetic operations
	int Mul(Term* ut) override;
	int Mul(REAL r) override;
	int Divide(Term* ut) override;
    void DivideMonoms(XTerm* t);
	int Inverse() override;
	bool IsZero() const override;

	// printing
	void PrettyPrint() const override;
	void PrintLatex(std::ostream &os) const override;

	// overriden methods
	int Compare(Term* t) const override;
	void Merge(Term* t) override;
};
