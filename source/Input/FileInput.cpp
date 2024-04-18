// FileInput.cpp: implementation of the CFileInput class.
//
//////////////////////////////////////////////////////////////////////

#include "FileInput.h"
#include <fstream>
#include <iostream>

// ----------------------------------------------------------------------------

CFileInput::CFileInput(std::ifstream &h) : m_h(h) {}

// ----------------------------------------------------------------------------

GReturnValue CFileInput::ReadChar(unsigned char &c) {
  if (!m_h.good())
    return rvG_InvalidInput;
  c = m_h.get();
  if (m_h.eof())
    return rvG_EndOfData;
  if (c == '\n') {
    m_iInputLine++;
    m_iPosition = 0;
  } else
    m_iPosition++;

  return rvG_OK;
}

// ----------------------------------------------------------------------------

bool CFileInput::EndOfInputData() { return (!m_h.good() || m_h.eof()); }

// ----------------------------------------------------------------------------
