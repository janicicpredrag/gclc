#pragma once

#include "Term.h"
#include "Power.h"
#include "UPolynomialFraction.h"
#include <iostream>
#include <memory>

class XTerm : public Term
{
	// U fraction (analogue to real factor of U term)
	std::shared_ptr<UPolynomialFraction> _frac;

public:
	XTerm();
	~XTerm();

	std::shared_ptr<XTerm> Clone() const;
	std::shared_ptr<XTerm> ClonePowers();

	TERM_TYPE Type() const override;

	static std::shared_ptr<XTerm> CreatePolynomialConditionTerm(bool f1,
	                                                            uint index1,
	                                                            bool f2,
	                                                            uint index2);

	UPolynomialFraction* GetUFraction() const;
	void SetUFraction(std::shared_ptr<UPolynomialFraction> uf);
	void AddPower(std::shared_ptr<Power> xp);

	// arithmetic operations
	int Mul(Term* ut) override;
	int Mul(REAL r) override;
	int Divide(Term* ut) override;
    void DivideMonoms(XTerm* t);
	int Inverse() override;
	bool IsZero() const override;

	// printing
	void PrettyPrint() const override;
	void PrintLatex(std::ostream &os) const override;

	// overridden methods
	int Compare(Term* t) const override;
	void Merge(Term* t) override;

private:
	XTerm *Clone_impl() const override;
};
