#pragma once

#include "Power.h"
#include "Term.h"
#include <iostream>
#include <memory>

class UTerm : public Term
{
	// real factor of U term
	REAL _coeff;

public:
	UTerm(REAL coeff = 1);
	void Construct(REAL coeff = 1);

	~UTerm();

	std::shared_ptr<UTerm> Clone() const;

	TERM_TYPE Type() const override;

	REAL GetCoeff() const;
	void SetCoeff(REAL coeff);

	void AddPower(std::shared_ptr<Power> power);

	bool IsUnit() const;

	// arithmetic operations
	int Mul(Term* ut) override;
	int Mul(REAL r) override;
	int Divide(Term* ut) override;
	int Inverse() override;
	bool IsZero() const override;

	// printing
	void PrettyPrint() const override;
	void PrintLatex(std::ostream &os) const override;

	// overridden methods
	int Compare(Term* other) const override;
	void Merge(Term* t) override;

private:
	UTerm *Clone_impl() const override;
};
