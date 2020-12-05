// GCLCInput.h: interface for the CGCLCInput class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(GCLCINPUT_H)
#define GCLCINPUT_H

#include "GCompiler.h"


class CGCLCInput  
{
public:
	CGCLCInput();
	virtual ~CGCLCInput();

	// start read from the begining of the input
	virtual void Reset() { 	m_iInputLine = 1; m_iPosition = 0; m_iGlobalPosition=1; };	
	virtual GReturnValue ReadChar(char* c)=0;
	virtual bool EndOfInputData()=0;

	int GetCurrentLine()	{	return m_iInputLine;	}
	int GetCurrentPosition()	{	return m_iPosition;	}
	int GetCurrentGlobalPosition()	{	return m_iGlobalPosition;	}

protected:	
	int m_iInputLine, m_iPosition, m_iGlobalPosition;
};


#endif // !defined(GCLCINPUT_H)
