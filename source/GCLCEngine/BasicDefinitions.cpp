#include "GCLC.h"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_number() {
  std::string sNumberName;
  double dNumber;
  GCLCError iRv;

  if (ReadToken(sNumberName) != rvGCLCOK)
    return rvGCLCIdExpected;
  if ((iRv = ReadNumber(dNumber)) != rvGCLCOK)
    return iRv;

  if (ProvingTheorem())
    AddProverCommand(p_constant, sNumberName, d2s(dNumber, -1));

  return Let(sNumberName, GCLC_NUMBER, dNumber, 0.00, 0.00, 0.00, 0.00, 0.00);
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_point(int &iArgs) {
  std::string sPointName;
  double x, y, x1, y1;
  GCLCError iRv;

  if (ReadToken(sPointName) != rvGCLCOK)
    return rvGCLCIdExpected;
  if ((iRv = ReadNumber(x)) != rvGCLCOK)
    return iRv;

  int iStartLine = m_iLineBeforeLastToken;
  int iStartPos = m_iPositionBeforeLastToken;

  if ((iRv = ReadNumber(y)) != rvGCLCOK)
    return iRv;

  //-------- Support for the prover ---------
  if (ProvingTheorem())
    AddProverCommand(p_point, sPointName, d2s(x, -1), d2s(y, -1));
  //-------- End of support for the prover ---------

  //-------- Support for input exported to XML form ---------
  if (m_bXMLOutput && m_hXMLOutput.good()) {
    ChangeCurrentXMLGroup(eXMLdefine);
    Print(m_hXMLOutput, "\t\t<fixed_point x=\"" + d2s(x, -1) + "\" y=\"" +
                            d2s(y, -1) + "\">" + sPointName +
                            "</fixed_point>\n");

    m_CurrentXMLgroup = eXMLdefine;
  }
  //-------- End of support for XML form ---------

  int iEndLine = m_iLastReadLine;
  int iEndPos = m_iLastReadPosition;

  iArgs = 2;

  iRv = ReadToken();

  if ((LastToken().empty()) ||
      (choose_command(LastToken()) != gclc_unknowncommand) ||
      (iRv == rvGCLCEndOfInput)) {
    iArgs = 2;
    if (!FixedPointExists(sPointName) &&
        AddFixedPoint(x, y, x, y, sPointName, iStartLine, iStartPos, iEndLine,
                      iEndPos, iEndLine, iEndPos) != rvG_OK)
      return rvGCLCOutOfMemory;
    return Let(sPointName, GCLC_POINT, x, y, 0.00, 0.00, 0.00, 0.00);
  }

  if (EndOfInputData())
    return rvGCLCOK; // 06.09.2009.

  if (iRv == rvGCLCEndOfInput)
    return rvGCLCOK;
  else if (iRv != rvGCLCOK)
    return iRv; // 06.09.2009.

  iArgs = 4;

  if (!convert(LastToken(), x1)) {
    GCLC_object o;

    if ((iRv = Value(LastToken(), o)) != rvGCLCOK)
      return rvGCLCNumberExpected;
    if (o.type != GCLC_NUMBER)
      return rvGCLCWrongType;
    x1 = o.p[0];
  }

  if ((iRv = ReadNumber(y1)) != rvGCLCOK)
    return iRv;
  int iEndDestLine = m_iLastReadLine;
  int iEndDestPos = m_iLastReadPosition;

  if (!FixedPointExists(sPointName) &&
      AddFixedPoint(x, y, x1, y1, sPointName, iStartLine, iStartPos, iEndLine,
                    iEndPos, iEndDestLine, iEndDestPos) != rvG_OK)
    return rvGCLCOutOfMemory;

  return Let(sPointName, GCLC_POINT, x, y, 0.00, 0.00, 0.00, 0.00);
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_line() {
  std::string sLineName;
  GCLC_object o1, o2;
  double k;
  GCLCError iRv;

  if (ReadToken(sLineName) != rvGCLCOK)
    return rvGCLCIdExpected;
  if ((iRv = ReadObject(GCLC_POINT, o1)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadObject(GCLC_POINT, o2)) != rvGCLCOK)
    return iRv;

  if ((fabs(o1.p[0] - o2.p[0]) <= EPS) && (fabs(o1.p[1] - o2.p[1]) <= EPS)) {
    if (ProvingTheorem()) {
      // run-time deduction check
      // EQUALPOINTS(o1.name,o2.name)
      SetProverConjecture("identical " + o1.name + " " + o2.name);
    }
    return rvGCLCBadLine;
  }

  if (fabs(o1.p[0] - o2.p[0]) <= EPS) {
    iRv = Let(sLineName, GCLC_LINE, 1.00, 0.00, -(o1.p[0]), 0.00, 0.00, 0.00);
    if (iRv != rvGCLCOK)
      return iRv;
  } else if (fabs(o1.p[1] - o2.p[1]) <= EPS) {
    iRv = Let(sLineName, GCLC_LINE, 0.00, 1.00, -(o1.p[1]), 0.00, 0.00, 0.00);
    if (iRv != rvGCLCOK)
      return iRv;
  } else {
    k = (o2.p[0] - o1.p[0]) / (o1.p[1] - o2.p[1]);
    iRv = Let(sLineName, GCLC_LINE, 1.00, k, -(o1.p[0] + k * (o1.p[1])), 0.00, 0.00,
              0.00);
    if (iRv != rvGCLCOK)
      return iRv;
  }

  //-------- Support for the prover ---------
  if (ProvingTheorem())
    AddProverCommand(p_line, sLineName, o1.name, o2.name);

  //-------- Support for input exported to XML form ---------
  if (m_bXMLOutput && m_hXMLOutput.good()) {
    ChangeCurrentXMLGroup(eXMLconstruct);
    Print(m_hXMLOutput, "\t\t<ruler><new_line>" + sLineName +
                            "</new_line><point>" + o1.name + "</point><point>" +
                            o2.name + "</point></ruler>\n");
    m_CurrentXMLgroup = eXMLconstruct;
  }
  //-------- End of support for XML form ---------

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_circle() {
  std::string sCircleName;
  GCLC_object o1, o2;
  double k;
  GCLCError iRv;

  if (ReadToken(sCircleName) != rvGCLCOK)
    return rvGCLCIdExpected;
  if ((iRv = ReadObject(GCLC_POINT, o1)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadObject(GCLC_POINT, o2)) != rvGCLCOK)
    return iRv;

  if ((fabs(o1.p[0] - o2.p[0]) <= EPS) && (fabs(o1.p[1] - o2.p[1]) <= EPS)) {
    if (ProvingTheorem()) {
      // run-time deduction check
      // EQUALPOINTS(o1.name,o2.name)
      SetProverConjecture("identical " + o1.name + " " + o2.name);
    }
    return rvGCLCBadCircle;
  }

  //-------- Support for the prover ---------
  if (ProvingTheorem())
    AddProverCommand(p_circle, sCircleName, o1.name, o2.name);

  k = sqrt((o1.p[0] - o2.p[0]) * (o1.p[0] - o2.p[0]) +
           (o1.p[1] - o2.p[1]) * (o1.p[1] - o2.p[1]));

  //-------- Support for input exported to XML form ---------
  if (m_bXMLOutput && m_hXMLOutput.good()) {
    ChangeCurrentXMLGroup(eXMLconstruct);
    Print(m_hXMLOutput,
          "\t\t<compass><new_circle>" + sCircleName + "</new_circle><center>" +
              o1.name + "</center><point>" + o2.name + "</point></compass>\n");
    m_CurrentXMLgroup = eXMLconstruct;
  }
  //-------- End of support for XML form ---------

  return Let(sCircleName, GCLC_CIRCLE, o1.p[0], o1.p[1], k, 0.00, 0.00, 0.00);
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::set_equal() {
  std::string sLeftObjectName;
  GCLC_object o;
  GCLCError iRv;

  if (ReadToken(sLeftObjectName) != rvGCLCOK)
    return rvGCLCIdExpected;
  if ((iRv = ReadNextObject(o)) != rvGCLCOK)
    return iRv;

  return Let(sLeftObjectName, o.type, o.p[0], o.p[1], o.p[2], o.p[3], o.p[4],
             o.p[5]);
}

// ----------------------------------------------------------------------------
