#include "StringBuilder.h"
#include "string.h"

StringBuilder::StringBuilder()
{
	_bufferSize = STRING_BUILDER_CHUNK;

    // _buffer = new char[_bufferSize];
    _buffer = (char*) malloc(_bufferSize); // 10.2015.
	if (_buffer == NULL)
	{
        // out of memory
		throw -2;
	}

	_usedSize = 0;
}

StringBuilder::~StringBuilder()
{
    // delete [] _buffer;
    free (_buffer); // 10.2015.
}

void StringBuilder::_AllocSize(int size)
{
	if (_usedSize + size > _bufferSize)
	{
		_bufferSize = ((_usedSize + size) / STRING_BUILDER_CHUNK) * STRING_BUILDER_CHUNK + STRING_BUILDER_CHUNK;
		char* newBuffer = (char*)realloc(_buffer, _bufferSize);
		if (newBuffer == NULL)
		{
            // delete [] _buffer;
            free(_buffer); // 10.2015.
			throw 0x8007000EL;
		}
		_buffer = newBuffer;
	}
}

void StringBuilder::AddREAL(REAL cf)
{
	char dst[37] = {0};
	sprintf(dst, "%.3lf", cf);

	int len = 0;
	while (dst[len])
	{
		len ++;
	}
	--len;

	while (dst[len] == '0')
	{
		len --;
	}

	if (dst[len] == '.')
	{
		len --;
	}
	dst[len + 1] = 0;

	AddString(dst, len + 1);
}

void StringBuilder::AddChar(char c)
{
	_AllocSize(1);

	_buffer[_usedSize] = c;
	_usedSize ++;
}

void StringBuilder::AddString(const char* str, int len)
{
	_AllocSize(len);

	for (int ii = 0; ii < len; ii++)
	{
		_buffer[_usedSize] = str[ii];
		_usedSize ++;
	}
}

void StringBuilder::AddInt(int n)
{
	static char dst[37];
	sprintf(dst, "%d", n);

	AddString(dst, strlen(dst));
}

char* StringBuilder::GetString() const
{
	return _buffer;
}

char* StringBuilder::GetStringCopy() const
{
	char* str = new char[_usedSize + 1];
	if (str == NULL)
	{
		throw 0x8007000EL;
	}	

	for (int ii = 0; ii < _usedSize; ii++)
	{
		str[ii] = _buffer[ii];
	}
	str[_usedSize] = 0;

	return str;
}
