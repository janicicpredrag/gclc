// FileInput.cpp: implementation of the CFileInput class.
//
//////////////////////////////////////////////////////////////////////
#include "FileInput.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileInput::CFileInput(FILE* h)
{
	m_h = h;
}


GReturnValue CFileInput::ReadChar(char* c)
{
	if (m_h == NULL) 
		return rvG_InvalidInput;

	int ch;
	ch = fgetc(m_h); 
	if (ch == EOF) 
		return rvG_EndOfData;
	else 
	{
		*c = (char)ch;
		if ((*c) == '\n') 
		{
			m_iInputLine++; 
			m_iPosition=0;
		}
		else
			m_iPosition++;
		return 	rvG_OK;
	}
}



bool CFileInput::EndOfInputData()
{
	if (m_h == NULL) 
		return true;
	if (feof(m_h)) 
		return true;
	else 
		return false;
}

