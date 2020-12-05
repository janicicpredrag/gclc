#include "../TheoremProver/TheoremProver.h"
#include "ITimeout.h"
#include "Log.h"

bool ITimeout::_session = false;
double ITimeout::_timeout = -1; // infinite timeout by default
double ITimeout::_startTime = 0;

// check timeout
// raise exception if timeout is reached
bool ITimeout::CheckTimeout() {
  if (_session && _timeout >= 0 && Log::ElapsedTime(_startTime) > _timeout) {
    throw E_TIMEOUT;
  }
  return true;
}

// check timeout
// return false if timeout is reached
bool ITimeout::CheckTimeoutSafe() {
  if (_session && _timeout >= 0 && Log::ElapsedTime(_startTime) > _timeout) {
    return false;
  }
  return true;
}

// start new session
void ITimeout::StartSession() {
  if (_session) {
    throw E_TIMEOUTERROR;
  }
  if (_timeout >= 0) {
    _session = true;
    _startTime = Log::ElapsedTime(0);
  }
}

// end active session
void ITimeout::EndSession() { _session = false; }

// set new timeout
void ITimeout::SetTimeout(double timeout) {
  if (_session) {
    throw E_TIMEOUTERROR;
  }
  _timeout = timeout;
}
