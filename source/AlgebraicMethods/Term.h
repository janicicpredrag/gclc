#pragma once

#include "Object.h"
#include "Power.h"
#include <iostream>
#include <memory>

class Term;
class TermStorage;
typedef Term* TermKeyType;

/*************************************************************
*
* Term.h
*
* Base Term class with shared properties and methods for
* UTerm and XTerm classes.
*
**************************************************************/

enum TERM_TYPE
{
	UTERM,
	XTERM
};

class Term : public Object
{
protected:
	// list of powers
	std::vector<Power*> _powers;

public:
	Term();
	virtual ~Term();

	// comparison of two terms
	virtual int Compare(Term* other) const;

	virtual Term* Clone() = 0;

	virtual TERM_TYPE Type() const = 0;

	// if two terms are equal, then they should
	// be merged
	virtual void Merge(Term* other);

	// delete power from the term (after division or gcd operation)
	void RemovePower(uint index);

	// key used by TermStorage
	TermKeyType Key();

	static std::shared_ptr<TermStorage> CreateTermStorage();

	// powers
	uint GetPowerCount() const;
	Power* GetPower(uint index) const;

	// arithmetic operations
	void MergePowers(Term* t, bool add);
	virtual int Mul(Term* ut) = 0;
	virtual int Mul(REAL r) = 0;
	virtual int Divide(Term* ut) = 0;
	virtual int Inverse() = 0;
	bool Divisible(Term* t) const;
	void GCDWith(const Term* t);

	virtual bool IsZero() const = 0;

	int VariableDeg(int index, bool free = false) const;
	Power* GetPowerByIndex(int index) const;
	int GetIndexOfIndex(int index) const;
	void ChangePowerDegree(int index, int change);

	virtual void PrettyPrint() const = 0;
	virtual void PrintLatex(std::ostream &os) const = 0;
};
