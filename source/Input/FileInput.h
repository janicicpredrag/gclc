// FileInput.h: interface for the CFileInput class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(FILEINPUT_H)
#define FILEINPUT_H

#include "GCLCInput.h"
#include <fstream>

using namespace std;

class CFileInput : public CGCLCInput {
public:
  CFileInput(ifstream &h);
  virtual ~CFileInput();

  GReturnValue ReadChar(unsigned char &c);
  bool EndOfInputData();

private:
  ifstream &m_h;
};

#endif // !defined(FILEINPUT_H)
