#pragma once

#include "Polynomial.h"
#include "UTerm.h"

class UPolynomial : public Polynomial
{
public:
	UPolynomial();
	~UPolynomial();

	UPolynomial(REAL cf);
	Polynomial* Clone();

	TERM_TYPE Type() const;

	bool IsUnit() const;

	// printint
	void PrintLatex(StringBuilder* sb) const;
};
