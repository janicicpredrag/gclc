#pragma once

#include "Polynomial.h"
#include "XTerm.h"
#include <iostream>
#include <string>

class XPolynomial : public Polynomial
{
    void _ResReplace(std::string& res, char l1, char l2, char r1, bool nnCond) const;
	bool _PseudoRemainder(XPolynomial* xp, int index, bool free, XPolynomial* xpDivisionResult);
    friend class Reduce;

public:
	XPolynomial();
	~XPolynomial();
	XPolynomial(REAL x);
	XPolynomial(bool free, int index);
	XPolynomial* Clone() override;

	TERM_TYPE Type() const override;

	static int maxt();

	// helper methods to create polynomial conditions
	static XPolynomial* CreatePolynomialCondition(
		bool f1, uint index1,
		bool f2, uint index2,
		bool f3, uint index3,
		bool f4, uint index4);

	uint GetTotalTermCount();

	// arithmetic operations
	void SPol(XPolynomial* xp);
	void SimpleReduction();
	void PseudoRemainder(XPolynomial* xp, int index, bool free = false, XPolynomial* xpDivisionResult = NULL);

	// printing
	void PrintLatex(std::ostream &os) const override;
	std::string PrintLatex() const;
};

typedef std::vector<XPolynomial*> vxp;
