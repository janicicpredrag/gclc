#pragma once

#include "Term.h"
#include "TermStorage.h"
#include "ITimeout.h"
#include <iostream>
#include <memory>

/*************************************************************
*
* Polynomial.h
*
* Base Polynomial class, shared properties and methods for
* XPolynomial and UPolynomial classes.
*
**************************************************************/
class Polynomial : public Object
{
protected:

	//
	// list of terms
	// TermStorage may be implemented as a vector, list, tree or
	// something else.
	// Crucial for performance.
	//
	std::shared_ptr<TermStorage> _terms;

public:
	Polynomial();
	virtual ~Polynomial();

	virtual Polynomial* Clone() = 0;

	virtual TERM_TYPE Type() const = 0;

	// term operations
	uint GetTermCount() const;
	Term* GetTerm(uint index) const;

	int AddTerm(Term* t);
	void RemoveTerm(Term* t);

	// aritmetic operations
	int Mul(Polynomial* up);
	int Mul(Term* t);
	int Mul(REAL r);
	int Add(Polynomial* up);
	int Subtract(Polynomial* up);
	// aka negation
	int Inverse();
	bool IsZero() const;

	// helper methods/accessors
	Term* LeadTerm(int index, bool free = false) const;

	// printint
	void PrettyPrint() const;
	virtual void PrintLatex(std::ostream &os) const = 0;
};
