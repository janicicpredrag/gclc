#pragma once

#include "stdinc.h"

const int STRING_BUILDER_CHUNK = 100;

/*************************************************************
*
* StringBuilder.h
*
* Efficient class for creating long strings.
*
**************************************************************/

class StringBuilder
{
	char* _buffer;
	int _bufferSize;
	int _usedSize;

	void _AllocSize(int size);

public:

	StringBuilder();
	~StringBuilder();

	void AddREAL(REAL cf);
	void AddChar(char c);
    void AddString(const char* str, int len);
	void AddInt(int n);

	char* GetString() const;
	char* GetStringCopy() const;
};
