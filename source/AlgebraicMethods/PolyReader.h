#pragma once

#include "stdinc.h"
#include "XPolynomial.h"
#include <memory>

class PolyReader
{
private:
    static void _Error(const char* msg);

public:
	static void PrintPolynomials(std::vector<XPolynomial*>& vpols, int level = 0, int size = -1);
	static void PrintPolynomial(XPolynomial* xp, int level = 0, int index = -1, bool tabular = false);
};
