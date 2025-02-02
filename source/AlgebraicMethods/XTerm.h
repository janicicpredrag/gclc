#pragma once

#include "Term.h"
#include "Power.h"
#include "UPolynomialFraction.h"
#include <iostream>
#include <memory>

class XTerm : public Term
{
	// U fraction (analouge to real factor of U term)
	std::shared_ptr<UPolynomialFraction> _frac;

public:
	XTerm();
	~XTerm();

	std::shared_ptr<XTerm> Clone();
	std::shared_ptr<XTerm> ClonePowers();

	TERM_TYPE Type() const override;

	static std::shared_ptr<XTerm> CreatePolynomialConditionTerm(bool f1,
	                                                            uint index1,
	                                                            bool f2,
	                                                            uint index2);

	UPolynomialFraction* GetUFraction() const;
	void SetUFraction(std::shared_ptr<UPolynomialFraction> uf);
	void AddPower(std::shared_ptr<Power> xp);

	// aritmetic operations
	int Mul(Term* ut) override;
	int Mul(REAL r) override;
	int Divide(Term* ut) override;
    void DivideMonoms(XTerm* t);
	int Inverse() override;
	bool IsZero() const override;

	// printing
	void PrettyPrint() const override;
	void PrintLatex(std::ostream &os) const override;

	// overriden methods
	int Compare(Term* t) const override;
	void Merge(Term* t) override;

private:
	XTerm *Clone_impl() override;
};
