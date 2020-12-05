#pragma once

#include "stdinc.h"

/*************************************************************
*
* ITimeout.h
*
* Interface for timeout checking
* and running syncronous timeout sessions
*
**************************************************************/

class ITimeout
{
private:
	static double _timeout;     // timeout in miliseconds, negative is infinity
	static double _startTime;   // time when last timeout session was started
	static bool _session;       // flag that indicates is session in progress
public:
	static bool CheckTimeout(); // check timeout method which raise exception
	static bool CheckTimeoutSafe(); // check timeout method which returns true or false
	static void StartSession(); // start new session
	static void EndSession();   // end active session
	static void SetTimeout(double timeout); // set new timeout
};
