#pragma once

#include "Polynomial.h"
#include "UTerm.h"
#include <iostream>

class UPolynomial : public Polynomial
{
public:
	UPolynomial();
	~UPolynomial();

	UPolynomial(REAL cf);
	UPolynomial* Clone() override;

	TERM_TYPE Type() const override;

	bool IsUnit() const;

	// printint
	void PrintLatex(std::ostream &os) const override;
};
