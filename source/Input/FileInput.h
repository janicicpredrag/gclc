// FileInput.h: interface for the CFileInput class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(FILEINPUT_H)
#define FILEINPUT_H

#include "GCLCInput.h"
#include <fstream>

class CFileInput : public CGCLCInput {
public:
  CFileInput(std::ifstream &h);
  virtual ~CFileInput();

  GReturnValue ReadChar(unsigned char &c);
  bool EndOfInputData();

private:
  std::ifstream &m_h;
};

#endif // !defined(FILEINPUT_H)
