// StringInput.h: interface for the CStringInput class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(STRINGINPUT_H)
#define STRINGINPUT_H

#include "GCLCInput.h"
#include <string>

class CStringInput : public CGCLCInput {
public:
  CStringInput(const std::string &sData);
  virtual ~CStringInput() = default;

  GReturnValue ReadChar(unsigned char &c);
  bool EndOfInputData();

private:
  const std::string m_sData;
  int m_iDataLen, m_iTotalPos;
};

#endif // !defined(STRINGINPUT_H)
