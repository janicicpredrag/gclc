#pragma once

#include "Object.h"
#include "StringBuilder.h"

/*************************************************************
*
* Power.h
*
* Base Power class. Shared properties and methods for
* XPower and UPower classes.
*
**************************************************************/

enum VAR_TYPE
{
	VAR_TYPE_U = 1,
	VAR_TYPE_X = 2
};

class Power : public Object
{
protected:
	// index of variable
	uint _index;

	// degree of variable
	uint _degree;

	// which variable
	VAR_TYPE _vType;

public:
	Power(uint index, uint degree, VAR_TYPE varType);
	void Construct(uint index, uint degree, VAR_TYPE varType);

	~Power();
	void Dispose();

	Power* Clone();

	uint GetIndex() const;
	void SetDegree(uint degree);
	uint GetDegree() const;

	// multiplication
	void Mul(Power* pow);

	// comparison
	int Compare(Power* pow) const;

	// printint
	void PrettyPrint() const;
	void PrintLatex(StringBuilder* sb) const;
};
