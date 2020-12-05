// StringInput.h: interface for the CStringInput class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(STRINGINPUT_H)
#define STRINGINPUT_H

#include "GCLCInput.h"
#include <string>

using namespace std;

class CStringInput : public CGCLCInput {
public:
  CStringInput(const string &sData);
  virtual ~CStringInput();

  GReturnValue ReadChar(unsigned char &c);
  bool EndOfInputData();

private:
  const string m_sData;
  int m_iDataLen, m_iTotalPos;
};

#endif // !defined(STRINGINPUT_H)
