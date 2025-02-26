#pragma once

#include "stdinc.h"
#include "XPolynomial.h"
#include <memory>

class PolyReader
{
private:
	static int _GotoOpenBracket(char* stream, int pos, int end);
	static int _GotoCloseBracket(char* stream, int s, int e);
	static int _GotoSeparator(char* stream, int s, int e);
	static char _GotoNextChar(char* stream, int s);


	static void _Print(char* stream, int s, int e);
    static void _Error(const char* msg);
    static void _Assert(bool assert, const char* msg);

	static std::shared_ptr<UPolynomialFraction> _ReadUFraction(char* stream, int s, int e);
	static UPolynomial* _ReadUPolynomial(char* stream, int s, int e);
	static std::shared_ptr<UTerm> _ReadUTerm(char* stream, int s, int e);
	static std::shared_ptr<Power> _ReadUPower(char* stream, int s, int e);
	static std::shared_ptr<Power> _ReadXPower(char* stream, int s, int e);


public:
	static void PrintPolynomials(std::vector<XPolynomial*>& vpols, int level = 0, int size = -1);
	static void PrintPolynomial(XPolynomial* xp, int level = 0, int index = -1, bool tabular = false);
};
