// StringInput.cpp: implementation of the CStringInput class.
//
//////////////////////////////////////////////////////////////////////

#include "StringInput.h"

using namespace std;

// ----------------------------------------------------------------------------

CStringInput::CStringInput(const string &sData) : m_sData(sData) {
  m_iDataLen = sData.length();
  m_iInputLine = 1;
  m_iPosition = 0;
  m_iTotalPos = 0;
}

// ----------------------------------------------------------------------------

CStringInput::~CStringInput() {}

// ----------------------------------------------------------------------------

GReturnValue CStringInput::ReadChar(unsigned char &c) {
  if (m_iDataLen - 1 < m_iTotalPos)
    return rvG_EndOfData;

  c = m_sData[m_iTotalPos];
  m_iTotalPos++;
  if (c == '\n' || c == 9) {
    m_iInputLine++;
    m_iPosition = 0;
  } else
    m_iPosition++;

  return rvG_OK;
}

// ----------------------------------------------------------------------------

bool CStringInput::EndOfInputData() { return (m_iDataLen - 1 < m_iTotalPos); }

// ----------------------------------------------------------------------------
