#include "UPolynomial.h"
#include <iostream>

class UPolynomialFraction : public Object
{
	// numerator
	UPolynomial* _num;

	// denominator
	UPolynomial* _den;

public:
	UPolynomialFraction();
	UPolynomialFraction(REAL cf);

	~UPolynomialFraction();

	UPolynomialFraction* Clone();

	UPolynomial* GetNumerator() const;
	void SetNumerator(UPolynomial* up);

	UPolynomial* GetDenominator() const;
	void SetDenominator(UPolynomial* up);

	// arithmetic operations
	int Mul(UPolynomialFraction* uf);
	int Inverse();
	int Negation();
	void SimpleReduction();

	// printing
	void PrettyPrint() const;
	void PrintLatex(std::ostream &os) const;
};
