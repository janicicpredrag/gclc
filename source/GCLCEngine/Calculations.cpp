#include "../ExpressionParser/parser.h"
#include "../Input/StringInput.h"
#include "GCLC.h"
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <string>

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_x() {
  std::string sNumberName;
  GCLC_object o1;
  GCLCError iRv;

  if (ReadToken(sNumberName) != rvGCLCOK)
    return rvGCLCIdExpected;
  if ((iRv = ReadObject(GCLC_POINT, o1)) != rvGCLCOK)
    return iRv;

  return Let(sNumberName, GCLC_NUMBER, o1.p[0], 0.00, 0.00, 0.00, 0.00, 0.00);
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_y() {
  std::string sNumberName;
  GCLC_object o1;
  GCLCError iRv;

  if (ReadToken(sNumberName) != rvGCLCOK)
    return rvGCLCIdExpected;
  if ((iRv = ReadObject(GCLC_POINT, o1)) != rvGCLCOK)
    return iRv;

  return Let(sNumberName, GCLC_NUMBER, o1.p[1], 0.00, 0.00, 0.00, 0.00, 0.00);
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_distance() {
  std::string sNumberName;
  GCLC_object o1, o2;
  GCLCError iRv;
  double d;

  if (ReadToken(sNumberName) != rvGCLCOK)
    return rvGCLCIdExpected;
  if ((iRv = ReadObject(GCLC_POINT, o1)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadObject(GCLC_POINT, o2)) != rvGCLCOK)
    return iRv;

  d = distance2d(o1, o2);
  return Let(sNumberName, GCLC_NUMBER, d, 0.00, 0.00, 0.00, 0.00, 0.00);
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_angle(bool orientation) {
  GCLC_object o1, o2, o3;
  double a, b, c, angle;
  std::string sNumberName;
  GCLCError iRv;

  if (ReadToken(sNumberName) != rvGCLCOK)
    return rvGCLCIdExpected;
  if ((iRv = ReadObject(GCLC_POINT, o1)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadObject(GCLC_POINT, o2)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadObject(GCLC_POINT, o3)) != rvGCLCOK)
    return iRv;

  c = distance2d(o1, o2);
  a = distance2d(o2, o3);
  b = distance2d(o1, o3);

  if (a < EPS) {
    if (ProvingTheorem()) {
      // run-time deduction check
      SetProverConjecture("identical " + o3.name + " " + o2.name);
    }
    return rvGCLCBadAngle; /* a>0 */
  }
  if (c < EPS) {
    if (ProvingTheorem()) {
      // run-time deduction check
      SetProverConjecture("identical " + o1.name + " " + o3.name);
    }
    return rvGCLCBadAngle; /* c>0 */
  }

  double ee = (a * a + c * c - b * b) / (2 * a * c);
  angle = (fabs(ee + 1.0) < EPS) ? -PI : acos(ee);

  if (orientation) {
    if ((o2.p[0] - o1.p[0]) * (o3.p[1] - o1.p[1]) -
            (o3.p[0] - o1.p[0]) * (o2.p[1] - o1.p[1]) >
        0)
      angle = -1 * angle;
  }

  return Let(sNumberName, GCLC_NUMBER, 360.00 * angle / (2 * PI), 0.00, 0.00,
             0.00, 0.00, 0.00);
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_random() {
  std::string sNumberName;

  if (ReadToken(sNumberName) != rvGCLCOK)
    return rvGCLCIdExpected;

  return Let(sNumberName, GCLC_NUMBER, (double)rand() / (double)RAND_MAX, 0.00,
             0.00, 0.00, 0.00, 0.00);
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_expression() {
  std::string sNumberName;
  double dNumber;
  GCLCError iRv;
  std::string text;

  if (ReadToken(sNumberName) != rvGCLCOK)
    return rvGCLCIdExpected;
  if ((iRv = take_text(text)) != rvGCLCOK)
    return iRv;
  if ((iRv = calc_expression(text, dNumber)) != rvGCLCOK)
    return iRv;

  //-------- Support for input exported to XML form ---------
  if (m_bXMLOutput && m_hXMLOutput.good()) {
    ChangeCurrentXMLGroup(eXMLdefine);
    Print(m_hXMLOutput, "\t\t<fixed_number value=\"" + d2s(dNumber, -1) +
                            "\">" + sNumberName + "</fixed_number>\n");
    m_CurrentXMLgroup = eXMLdefine;
  }
  //-------- End of support for XML form ---------

  return Let(sNumberName, GCLC_NUMBER, dNumber, 0.00, 0.00, 0.00, 0.00, 0.00);
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::calc_expression(const std::string &text, double &dNumber) {
  // in order to enable arrays EvaluateIndex(identifier);
  std::string inputLine = text;
  EvaluateIndex(inputLine);

  try {
    // remove backspaces/leading/trailing spaces
    inputLine = trim(removeBackspaces(inputLine));

    if (inputLine.empty())
      return rvGCLCInvalidExpression;

    // create instance of parser, using the entered input line
    Parser p(inputLine);

    p.SetGCLC(this);

    // parse entered expression, return result into "value
    dNumber = p.Evaluate();

  } // end of try block

  // catch parse and runtime errors
  catch (std::exception &e) {
    return rvGCLCInvalidExpression;
  }

  if (std::isnan(dNumber))
    return rvGCLCInvalidExpression;

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_array() {
  std::string sArrayName;
  double dNumber;
  int iDim[10];
  unsigned char c;
  GCLCError iRv;

  if (ReadToken(sArrayName) != rvGCLCOK)
    return rvGCLCIdExpected;

  do {
    GReturnValue rv = m_Input->ReadChar(c);
    if (rv != rvG_OK)
      return rvGCLCLBracketExpected;
    if (rv == rvG_InvalidInput)
      return rvGCLCInvalidInput;
  } while ((c == ' ') || (c == '\n') || (c == 9) || (c == '\t') || (c == '\r'));

  if (c != '{')
    return rvGCLCLBracketExpected;

  int i;
  for (i = 0; i < 10; i++) {
    iRv = ReadToken();
    if (LastToken() == "}") {
      i--;
      break;
    }

    if ((iRv != rvGCLCOK) && LastToken().empty())
      return rvGCLCIdOrNumberExpected;

    iRv = calc_expression(LastToken(), dNumber);
    if (iRv != rvGCLCOK)
      return iRv;
    iDim[i] = (int)dNumber;
    if (iDim[i] < 1)
      iDim[i] = 1;
  }

  if (i > 0)
    generate_array_elements(iDim, 0, i, sArrayName);

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::generate_array_elements(int iDim[], int current_index,
                                         int max_index, const std::string &sName) {
  GCLCError iRv;
  if (current_index > max_index) {
    if ((iRv = Let(sName, GCLC_NUMBER, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00)) !=
        rvGCLCOK)
      return iRv;
  } else {
    for (int i = 1; i <= iDim[current_index]; i++)
      generate_array_elements(iDim, current_index + 1, max_index,
                              sName + "[" + i2s(i) + "]");
  }
  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::EvaluateIndex(std::string &sObjectName) {
  GCLCError iRv;
  std::string expression;
  double dNumber;
  size_t found1 = -1, found2;

  while (1) {
    if ((found1 = sObjectName.find_first_of('[', found1 + 1)) == std::string::npos)
      break;

    int iBrackets = 1;
    found2 = sObjectName.find_first_of("[]", found1 + 1);
    {
      if (sObjectName[found2] == '[')
        iBrackets++;
      if (sObjectName[found2] == ']')
        iBrackets--;

      if (sObjectName[found2] == ']' && iBrackets == 0) {
        expression = sObjectName.substr(found1 + 1, found2 - found1 - 1);
        if ((iRv = calc_expression(expression, dNumber)) != rvGCLCOK)
          return iRv;
        sObjectName = sObjectName.substr(0, found1) + "[" + d2s(dNumber, -1) +
                      "]" + sObjectName.substr(found2 + 1);
      }
    }
  }
  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_while() {
  GCLCError iRv;
  double dNumber;
  std::string block_text;
  std::string cond_text;

  if ((iRv = take_text(cond_text)) != rvGCLCOK)
    return iRv;
  if ((iRv = take_text(block_text)) != rvGCLCOK)
    return iRv;

  if ((iRv = calc_expression(cond_text, dNumber)) != rvGCLCOK)
    return iRv;

  int iExecutions = 0;
  while (dNumber == 1.0) {
    iExecutions++;
    if (iExecutions > MAX_NUMBER_OF_LOOP_EXECUTIONS)
      return rvGCLCToManyWhileBlockExecutions;

    // CGCLC C(this->m_pPrim,this->m_pTheoremProver,this->m_pProcedures); //
    // changed on 11.2015.
    CStringInput input(block_text);
    GCLC_env e;
    this->GetEnv(e);
    CGCLC C(input, this->m_Log, this->m_pPrim, this->m_pProcedures,
            m_pSymbolTable, this->m_bXMLOutput, this->m_hXMLOutput);
    C.SetEnv(e);

    bool bSupressWarnings = m_bSupressWarnings;
    if (m_bXMLOutput && m_hXMLOutput.good())
      ChangeCurrentXMLGroup(eXMLnone);
    if (C.Import() != rvG_OK) {
      m_bSupressWarnings = bSupressWarnings;
      return rvGCLCInvalidWhileBlock;
    }

    if ((iRv = calc_expression(cond_text, dNumber)) != rvGCLCOK)
      return iRv;
  }

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_if_then_else() {
  GCLCError iRv;
  double dNumber;
  std::string then_block_text;
  std::string else_block_text;
  std::string cond_text;

  if ((iRv = take_text(cond_text)) != rvGCLCOK)
    return iRv;
  if ((iRv = take_text(then_block_text)) != rvGCLCOK)
    return iRv;
  if ((iRv = take_text(else_block_text)) != rvGCLCOK)
    return iRv;

  if ((iRv = calc_expression(cond_text, dNumber)) != rvGCLCOK)
    return iRv;

  CStringInput input(dNumber == 1 ? then_block_text : else_block_text);
  GCLC_env e;
  this->GetEnv(e);
  CGCLC C(input, this->m_Log, this->m_pPrim, this->m_pProcedures,
          m_pSymbolTable, this->m_bXMLOutput,
          this->m_hXMLOutput); // changed 11.2015.
  C.SetEnv(e);

  bool bSupressWarnings = m_bSupressWarnings;

  if (m_bXMLOutput && m_hXMLOutput.good())
    ChangeCurrentXMLGroup(eXMLnone);
  if (C.Import() != rvG_OK) {
    m_bSupressWarnings = bSupressWarnings;
    return rvGCLCInvalidIfBlock;
  }

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_procedure() {
  if (m_bWhileBlockOrProcedureCall)
    return rvGCLCNoProceduresInBlocks;

  GCLCError iRv;
  std::string sProcedureName;
  std::string params_text;
  std::string block_text;

  if ((iRv = ReadToken(sProcedureName)) != rvGCLCOK)
    return rvGCLCIdExpected;
  if ((iRv = take_text(params_text)) != rvGCLCOK)
    return iRv;
  if ((iRv = take_text(block_text)) != rvGCLCOK)
    return iRv;

  AddNewProcedure(sProcedureName, params_text, block_text);
  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::AddNewProcedure(std::string name, std::string params, std::string block) {
  GCLCprocedure proc;
  proc.sName = name;
  proc.sParameters = params;
  proc.sBlock = block;
  m_pProcedures->insert(std::pair<std::string, GCLCprocedure>(name, proc));
  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_call_procedure() {
  GCLCError iRv;
  std::string sProcedureName;
  std::string params_text;
  std::string sArgName;
  std::string sFormalArgName;
  GCLC_object o;

  if (ReadToken(sProcedureName) != rvGCLCOK)
    return rvGCLCIdExpected;
  if ((iRv = take_text(params_text)) != rvGCLCOK)
    return iRv;

  std::map<std::string, GCLCprocedure>::iterator it = m_pProcedures->find(sProcedureName);
  if (it == m_pProcedures->end())
    return rvGCLCUnknownprocedure;

  CStringInput input(it->second.sBlock);
  GCLC_env e;
  this->GetEnv(e);
  CGCLC C(input, this->m_Log, this->m_pPrim, m_pProcedures, NULL,
          this->m_bXMLOutput, this->m_hXMLOutput);

  std::string &formal_params = it->second.sParameters;
  std::string &actual_params = params_text;
  bool bSupressWarnings = m_bSupressWarnings;
  m_bSupressWarnings = true;

  unsigned int ai = 0, fi = 0;
  int alen = 0, flen = 0;
  bool bMoreArgs = true;
  while (bMoreArgs) {
    alen = 0, flen = 0;
    while (ai <= actual_params.size() && is_blank(actual_params[ai]))
      ai++;
    while (ai + alen <= actual_params.size() &&
           !is_blank(actual_params[ai + alen]))
      alen++;
    while (fi <= formal_params.size() && is_blank(formal_params[fi]))
      fi++;
    while (fi + flen <= formal_params.size() &&
           !is_blank(formal_params[fi + flen]))
      flen++;

    if ((ai < actual_params.length())) {
      sArgName = actual_params.substr(ai, alen);
      ai = ai + alen;

      if (fi < formal_params.length()) {
        sFormalArgName = formal_params.substr(fi, flen);
        fi = fi + flen;

        EvaluateIndex(sArgName);
        if ((iRv = Value(sArgName, o)) != rvGCLCOK) {
          if (convert(sArgName, o.p[0])) // argument is a number
          {
            o.type = GCLC_NUMBER;
            o.p[1] = o.p[2] = o.p[3] = o.p[4] = o.p[5] = 0;
          } else {
            iRv = Let(sArgName, GCLC_NUMBER, 0, 0, 0, 0, 0, 0);
            o.type = GCLC_NUMBER;
            o.p[0] = o.p[1] = o.p[2] = o.p[3] = o.p[4] = o.p[5] = 0;
            if (iRv != rvGCLCOK) {
              m_bSupressWarnings = bSupressWarnings;
              return iRv;
            }
          }
        }

        if ((iRv = C.Let(sFormalArgName, o.type, o.p[0], o.p[1], o.p[2], o.p[3],
                         o.p[4], o.p[5])) != rvGCLCOK) {
          m_bSupressWarnings = bSupressWarnings;
          return iRv;
        }
      } else {
        m_bSupressWarnings = bSupressWarnings;
        return rvGCLCParametersNotMatched;
      }
    } else
      bMoreArgs = false;
  }

  if (m_bXMLOutput && m_hXMLOutput.good())
    ChangeCurrentXMLGroup(eXMLnone);

  C.SetEnv(e);
  if (C.Import() != rvG_OK) {
    m_bSupressWarnings = bSupressWarnings;
    return rvGCLCInvalidProcedure;
  }

  formal_params = it->second.sParameters;
  actual_params = params_text;

  bMoreArgs = true;
  ai = 0;
  fi = 0;
  alen = 0;
  flen = 0;
  while (bMoreArgs) {
    alen = 0, flen = 0;
    while (ai <= actual_params.size() && is_blank(actual_params[ai]))
      ai++;
    while (ai + alen <= actual_params.size() &&
           !is_blank(actual_params[ai + alen]))
      alen++;
    while (fi < formal_params.size() && is_blank(formal_params[fi]))
      fi++;
    while (fi + flen <= formal_params.size() &&
           !is_blank(formal_params[fi + flen]))
      flen++;

    if ((ai < actual_params.size())) {
      sArgName = actual_params.substr(ai, alen);
      ai = ai + alen;

      if (fi < formal_params.size()) {
        sFormalArgName = formal_params.substr(fi, flen);
        fi = fi + flen;

        if ((iRv = C.Value(sFormalArgName, o)) != rvGCLCOK) {
          m_bSupressWarnings = bSupressWarnings;
          return iRv;
        }

        double d;
        if (!convert(sArgName, d)) // argument is not a number
        {
          if ((iRv = Let(sArgName, o.type, o.p[0], o.p[1], o.p[2], o.p[3],
                         o.p[4], o.p[5])) != rvGCLCOK) {
            m_bSupressWarnings = bSupressWarnings;
            return iRv;
          }
        }
      } else {
        m_bSupressWarnings = bSupressWarnings;
        return rvGCLCParametersNotMatched;
      }
    } else
      bMoreArgs = false;
  }

  m_bSupressWarnings = bSupressWarnings;
  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_include() {
  if (ReadToken() != rvGCLCOK)
    return rvGCLCIdExpected;

  std::ifstream Input(LastToken().c_str());
  if (!Input.is_open())
    return rvGCLCInvalidIncludeFile;
  std::string sIncludeFile((std::istreambuf_iterator<char>(Input)),
                      std::istreambuf_iterator<char>());

  CStringInput input(sIncludeFile);

  bool bSupressWarnings = m_bSupressWarnings;
  m_bSupressWarnings = true;

  int iLine = m_Input->GetCurrentLine();
  int iPos = m_Input->GetCurrentPosition();

  CGCLCInput *I_tmp = m_Input;
  m_Input = &input;

  GCLCError m_iErrorCode = Execute();

  m_iWarnLine = iLine;
  m_iWarnPos = iPos;

  m_Input = I_tmp;
  m_bSupressWarnings = bSupressWarnings;

  if (m_iErrorCode != rvGCLCOK)
    return rvGCLCInvalidIncludeFile;

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------
