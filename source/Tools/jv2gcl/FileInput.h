// FileInput.h: interface for the CFileInput class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(FILEINPUT_H)
#define FILEINPUT_H

#include <stdio.h>
#include "GCLCInput.h"


class CFileInput : public CGCLCInput 
{
public:
	CFileInput(FILE* h);
	virtual ~CFileInput() = default;
	
	GReturnValue ReadChar(char* c);
	bool EndOfInputData();

private:	
	FILE *m_h;

};

#endif // !defined(FILEINPUT_H)
