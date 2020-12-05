// DummyLog.h: interface for the CDummyLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(DUMMYLOG_H)
#define DUMMYLOG_H

#include "GCLCLog.h"
#include <string>


class CDummyLog : public CGCLCLog
{
public: 
	CDummyLog();
    ~CDummyLog();

    void Reset();
    void AddText(const string& sText);
};

#endif // !defined(DUMMYLOG_H)


