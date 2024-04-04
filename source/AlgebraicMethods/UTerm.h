#pragma once

#include "Power.h"
#include "Term.h"
#include "ObjectBank.h"
#include <iostream>

class UTerm : public Term
{
	// real factor of U term
	REAL _coeff;

public:
	UTerm(REAL coeff = 1);
	void Construct(REAL coeff = 1);

	~UTerm();

	void Dispose();

	Term* Clone();

	TERM_TYPE Type() const;

	REAL GetCoeff() const;
	void SetCoeff(REAL coeff);

	void AddPower(Power* power);

	bool IsUnit() const;

	// aritmetic operations
	int Mul(Term* ut);
	int Mul(REAL r);
	int Divide(Term* ut);
	int Inverse();
	bool IsZero() const;

	// printing
	void PrettyPrint() const;
	void PrintLatex(std::ostream &os) const;

	// overriden methods
	int Compare(Term* other) const;
	void Merge(Term* t);
};
