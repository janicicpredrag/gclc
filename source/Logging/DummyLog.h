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
	virtual ~CDummyLog() = default;

    void Reset();
    void AddText(const std::string& sText);
};

#endif // !defined(DUMMYLOG_H)


