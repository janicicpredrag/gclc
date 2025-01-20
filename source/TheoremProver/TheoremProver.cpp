// TheoremProver.cpp: implementation of the CTheoremProver class.
//
//////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <cmath>
#include <ctime>

#include "ProverExpression.h"
#include "TheoremProver.h"
#include "../Utils/Utils.h"
#include <sstream>
#include <string>

#define EPSILON 0.000005

CGCLCProverCommand::CGCLCProverCommand() {}

CGCLCProverCommand::~CGCLCProverCommand() { CleanUp(); }

void CGCLCProverCommand::CleanUp() {}

// ----------------------------------------------------------------------------

CNDGC::CNDGC() {}

CNDGC::~CNDGC() { CleanUp(); }

// ----------------------------------------------------------------------------

void CNDGC::CleanUp() {}

// ----------------------------------------------------------------------------

std::string CNDGC::sPrintLaTeX() {
  if (m_type == ep_equality)
    return m_Left.sPrintLaTeX() + "=" + m_Right.sPrintLaTeX();
  else
    return m_Left.sPrintLaTeX() + "\\neq" + m_Right.sPrintLaTeX();
}

// ----------------------------------------------------------------------------

std::string CNDGC::sPrintXML(int indent) {
  std::string s;

  s += make_indent(indent + 1);
  if (m_type == ep_equality)
    s += "<equality>\n";
  else
    s += "<inequality>\n";

  s += make_indent(indent + 2) + "<expression>\n";
  s += m_Left.sPrintXML(indent + 3);
  s += make_indent(indent + 2) + "</expression>\n";

  s += make_indent(indent + 2) + "<expression>\n";
  s += m_Right.sPrintXML(indent + 3);
  s += make_indent(indent + 2) + "</expression>\n";

  s += make_indent(indent + 1);
  if (m_type == ep_equality)
    s += "</equality>\n";
  else
    s += "</inequality>\n";

  return s;
}

// ----------------------------------------------------------------------------

CTheoremProver::CTheoremProver() {
  m_bValidConjecture = false;
  m_iPointCounter = 0;

  m_iProofLevel = 1;
  m_iProofLimit = PROOF_LIMIT;
  m_iProofDepth = -1;
  m_iProverTimeout = PROVER_TIMEOUT; // proof timeout in milliseconds; default is
                                     // 10 seconds; -1 stands for infinite
                                     // timeout
}

// ----------------------------------------------------------------------------

CTheoremProver::~CTheoremProver() { CleanUp(); }

// ----------------------------------------------------------------------------

void CTheoremProver::CleanUp() {
  m_bValidConjecture = false;
  m_iPointCounter = 0;

  m_iProofDepth = -1;
  m_iProofLevel = 1;
}

// ----------------------------------------------------------------------------

bool CTheoremProver::Prove(const std::string &sLaTeXProof, const std::string &sXMLProof,
                           double &Time, const std::string &theorem,
                           eGCLC_conjecture_status &Status) {

  if (!sLaTeXProof.empty()) {
    m_hLaTeXOutputProof.open(sLaTeXProof.c_str());
    if (!m_hLaTeXOutputProof.good())
      return false;
  }

  if (!sXMLProof.empty()) {
    m_hXMLOutputProof.open(sXMLProof.c_str());
    if (!m_hXMLOutputProof.good())
      return false;
  }

  if (!m_bValidConjecture) {
    CleanUp();
    Status = e_conjecture_out_of_scope;
    return true;
  }

  m_Timer.StartMeasuringTime();

  // LaTeX output
  PrintLaTeX("\\documentclass[a4paper]{article}\n");
  PrintLaTeX("\\usepackage{gclc_proof}\n\n\n");
  PrintLaTeX("\\begin{document}\n\n");

  // XML output
  PrintXML("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
#ifndef XML_DISABLE_DTD
  PrintXML("<!DOCTYPE main_proof SYSTEM \"geocons_proof.dtd\">\n");
  PrintXML(
      "<?xml-stylesheet href=\"GeoCons_proof.xsl\" type=\"text/xsl\"?>\n\n");
#endif
  PrintXML("<main_proof>\n");

  PrintProofTitleXML(theorem);
  PrintProofTitleLatex(theorem);

  Status = ProveConjecture(*m_pConjecture);

  Time = m_Timer.ElapsedTime();
  PrintProofFooter(Status);

  if (m_hLaTeXOutputProof.is_open())
    m_hLaTeXOutputProof.close();

  if (m_hXMLOutputProof.is_open())
    m_hXMLOutputProof.close();

  return true;
}

// ----------------------------------------------------------------------------

void CTheoremProver::PrintProofTitleXML(const std::string &theoremName) {
  // XML output
  std::string t = theoremName;
  if (t.empty())
    t = "[not named]";
  else
    replace(t.begin(), t.end(), '_', '-');
  PrintXML("<proof_title>GCLC Prover Output for conjecture " + t + " (" +
           GetName() + " method used)</proof_title>\n");

  // XML output
  PrintXML("\n\t<definitions>\n");

  for (std::list<CNewDef>::iterator it = m_NewDefs.begin(); it != m_NewDefs.end();
       it++) {
    // LaTeX output
    PrintLaTeX("\n\t" + it->m_sDef + "\n");

    // XML output
    PrintXML("\t\t<definition>\n");
    PrintXML("\t\t\t" + it->m_sDef + "\n");
    PrintXML("\t\t</definition>\n");
  }

  // XML output
  PrintXML("\t</definitions>\n\n");
}

// ----------------------------------------------------------------------------

void CTheoremProver::PrintProofTitleLatex(const std::string &theoremName) {
  if (m_NewDefs.size() != 0) {
    // if (m_pNewDefs != NULL) {
    // LaTeX output
    PrintLaTeX("\n\\vspace*{2mm} \\hrule \\vspace*{2mm} \n");
  }
  // LaTeX output
  std::string t = theoremName;
  if (t.empty())
    t = "[not named]";
  else
    replace(t.begin(), t.end(), '_', '-');
  PrintLaTeX("\\title{GCLC Prover Output for conjecture ``" + t + "'' }\n\n");
  PrintLaTeX("\\author{" + GetName() + " method used}\n\n");
  PrintLaTeX("\\maketitle\n\n");
}

// ----------------------------------------------------------------------------

void CTheoremProver::PrintProofFooter(eGCLC_conjecture_status /* eRet */) {
  double sec = m_Timer.ElapsedTime()/(double)1000;

  PrintNDGConditions();
  PrintMethodSpecificOutput();

  // LaTeX output
  PrintLaTeX("\n\nTime spent by the prover: " + d2s(sec, 3) + " seconds\n");
  PrintLaTeX("\n\n\\end{document}\n\n");

  // XML output
  PrintXML("\n\n</main_proof>\n\n");
}

// ----------------------------------------------------------------------------

void CTheoremProver::PrintNDGConditions(bool itemize) {
  if (m_NDGCs.empty()) {
    // LaTeX output
    PrintLaTeX("There are no ndg conditions.\n\n");

    // XML output
    PrintXML("\n\t<NDGconditions></NDGconditions>\n");
  } else {
    std::string sLaTeXCond, sXMLCond;
    // LaTeX output
    PrintLaTeX((std::string) "\n\n" + (itemize ? "\\item[" : "") + "NDG conditions" +
               (itemize ? "]" : "") + " are:\n\n");
    if (itemize)
      PrintLaTeX("\\begin{itemize}\n");
    // XML output
    PrintXML("\n\t<NDGconditions>\n");

    for (std::list<CNDGC>::iterator it = m_NDGCs.begin(); it != m_NDGCs.end();
         it++) {
      if (itemize)
        PrintLaTeX("\\item\n");
      // XML output
      PrintXML("\n\t\t<NDGcondition>\n");

      // LaTeX output
      sLaTeXCond = it->sPrintLaTeX();
      PrintLaTeX("$" + sLaTeXCond + "$ ");

      // XML output
      sXMLCond = it->sPrintXML(2);
      PrintXML(sXMLCond);

      if (it->m_type == ep_inequality && it->m_Right.GetType() == ep_number &&
          it->m_Right.GetNumber() == 0) {
        switch (it->m_Left.GetType()) {
        case ep_constant:
          // LaTeX output
          sLaTeXCond =
              "i.e., constant $" + it->m_Left.GetName() + "$ is non-zero";
          // XML output
          sXMLCond = "\t\t<explanation>constant " + it->m_Left.GetName() +
                     "is non-zero";
          break;

        case ep_s3:
          // LaTeX output
          sLaTeXCond = "i.e., points $" + it->m_Left.GetArgName(0) + "$, $" +
                       it->m_Left.GetArgName(1) + "$ and $" +
                       it->m_Left.GetArgName(2) + "$ are not collinear";
          // XML output
          sXMLCond = "\t\t<explanation>points " + it->m_Left.GetArgName(0) +
                     ", " + it->m_Left.GetArgName(1) + " and " +
                     it->m_Left.GetArgName(2) + "are not collinear";
          break;

        case ep_p3:
          if (it->m_Left.GetArgName(0) == it->m_Left.GetArgName(2)) {
            // LaTeX output
            sLaTeXCond = "i.e., points $" + it->m_Left.GetArgName(0) +
                         "$ and $" + it->m_Left.GetArgName(1) +
                         "$ are not identical";
            // XML output
            sXMLCond = "\t\t<explanation>points " + it->m_Left.GetArgName(0) +
                       " and " + it->m_Left.GetArgName(1) +
                       " are not identical";
          } else {
            // LaTeX output
            sLaTeXCond = "i.e., angle $" + it->m_Left.GetArgName(0) +
                         it->m_Left.GetArgName(1) + it->m_Left.GetArgName(2) +
                         "$ is not right angle";
            // XML output
            sXMLCond = "\t\t<explanation>angle " + it->m_Left.GetArgName(0) +
                       it->m_Left.GetArgName(1) + it->m_Left.GetArgName(2) +
                       " is not right angle";
          }

          break;

        default:
          // LaTeX output
          sLaTeXCond = it->m_Left.GetName();
          // XML output
          sXMLCond = "\t\t<explanation>" + it->m_Left.GetName();
          break;
        }
      } else if (it->m_Left.GetType() == ep_s3 &&
                 it->m_Right.GetType() == ep_s3) {
        // LaTeX output
        sLaTeXCond = "i.e., lines $" + it->m_Left.GetArgName(0) +
                     it->m_Right.GetArgName(0) + "$ and $" +
                     it->m_Right.GetArgName(1) + it->m_Right.GetArgName(2) +
                     "$ are not parallel";
        // XML output
        sXMLCond = "\t\t\t<explanation>lines " + it->m_Left.GetArgName(0) +
                   it->m_Right.GetArgName(0) + " and " +
                   it->m_Right.GetArgName(1) + it->m_Right.GetArgName(2) +
                   " are not parallel";
      } else {
        sLaTeXCond = "";
        sXMLCond = "";
      }

      // LaTeX output
      PrintLaTeX(sLaTeXCond + " ");
      PrintLaTeX(it->m_sCondition);
      if (itemize)
        PrintLaTeX(".");
      PrintLaTeX("\n\n");

      // XML output
      PrintXML(sXMLCond + " ");
      PrintXML(it->m_sCondition);
      PrintXML("</explanation>\n");

      // XML output
      PrintXML("\t\t</NDGcondition>\n");
    }

    if (itemize)
      PrintLaTeX("\\end{itemize}\n");
    else
      PrintLaTeX("\n\\vspace*{2mm} \\hrule \\vspace*{2mm} \n");

    // XML output
    PrintXML("\n\t</NDGconditions>\n\n");
  }
}

// ----------------------------------------------------------------------------

bool CTheoremProver::AddNDG(GCLCexpression_type type,
                            const CGCLCProverExpression &Left,
                            const CGCLCProverExpression &Right,
                            const std::string &sCond) {
  bool bAlreadyExists = false;

  for (std::list<CNDGC>::iterator it = m_NDGCs.begin();
       it != m_NDGCs.end() && !bAlreadyExists; it++) {
    // do nothing if already exists
    if (it->m_Left == Left && (it->m_Right == Right) && (it->m_type == type))
      bAlreadyExists = true;

    // do not add P_ABA \= 0 if AB \= 0 already exists
    if ((Left.GetType() == ep_p3) && (it->m_Left.GetType() == ep_segment) &&
        (Left.GetArgName(0) == Left.GetArgName(2)) &&
        (it->m_Left.GetArgName(0) == Left.GetArgName(0)) &&
        (it->m_Left.GetArgName(1) == Left.GetArgName(1)) &&
        (Right.GetType() == ep_number) && (Right.GetNumber() == 0) &&
        (it->m_Right.GetType() == ep_number) &&
        (it->m_Right.GetNumber() == 0) && (it->m_type == type))
      bAlreadyExists = true;

    // do not add AB \= 0 if P_ABA \= 0 already exists
    if ((it->m_Left.GetType() == ep_p3) && (Left.GetType() == ep_segment) &&
        (it->m_Left.GetArgName(0) == it->m_Left.GetArgName(2)) &&
        (it->m_Left.GetArgName(0) == Left.GetArgName(0)) &&
        (it->m_Left.GetArgName(1) == Left.GetArgName(1)) &&
        (Right.GetType() == ep_number) && (Right.GetNumber() == 0) &&
        (it->m_Right.GetType() == ep_number) &&
        (it->m_Right.GetNumber() == 0) && (it->m_type == type))
      bAlreadyExists = true;
  }

  if (bAlreadyExists)
    return true;

  CNDGC Ndgc;
  Ndgc.m_type = type;
  Ndgc.m_Left = Left;
  Ndgc.m_Right = Right;
  Ndgc.m_sCondition = sCond;
  m_NDGCs.push_back(Ndgc);

  return true;
}

// ----------------------------------------------------------------------------

bool CTheoremProver::AddProverCommand(eGCLC_prover_command type,
                                      const std::string &a1, const std::string &a2,
                                      const std::string &a3, const std::string &a4,
                                      const std::string &a5) {
  std::string s1, s2, s3, s4;

  if (type == p_point) {
    if (ExistsPoint(a1))
      return true;
    else
      return AddProverCommandLowLevel(type, a1, a2, a3, a4, a5);
  }

  if (type == p_inter) {
    if (!a4.empty()) // form: intersec X A B C D
    {
      CGCLCProverExpression Left(ep_s3, a2, a4, a5);
      CGCLCProverExpression Right(ep_s3, a3, a4, a5);
      if (!AddNDG(ep_inequality, Left, Right,
                  "(construction based assumption)"))
        return false;

      return AddProverCommandLowLevel(type, a1, a2, a3, a4, a5);
    } else { // form: intersec X a b
      for (std::list<CGCLCProverCommand>::iterator it = m_ProverCommands.begin();
           it != m_ProverCommands.end(); it++) {
        if (it->type == p_line) {
          if (a2 == it->arg[0]) {
            s1 = it->arg[1];
            s2 = it->arg[2];
          }

          if (a3 == it->arg[0]) {
            s3 = it->arg[1];
            s4 = it->arg[2];
          }

          if ((!s1.empty()) && (!s3.empty())) {
            CGCLCProverExpression Left(ep_s3, s1, s3, s4);
            CGCLCProverExpression Right(ep_s3, s2, s3, s4);
            if (!AddNDG(ep_inequality, Left, Right,
                        "(construction based assumption)"))
              return false;

            return AddProverCommandLowLevel(type, a1, s1, s2, s3, s4);
          }
        }
      }
      return false;
    }
  }

  if (type == midpoint) {
    return AddProverCommandLowLevel(p_pratio, a1, a2, a2, a3, "0.5");
  }

  if (type == online) {
    std::string sConstantName;
    sConstantName = "r_{" + i2s(m_iPointCounter++) + "}";

    double r;
    convert(a4, r);
    std::string s = "Let $" + sConstantName +
               "$ be the number such that {\\tt PRATIO} $" + a1 + "$ $" + a2 +
               "$ $" + a2 + "$ $" + a3 + "$ $" + sConstantName +
               "$ (for the concrete example $" + sConstantName + "$=" +
               d2s(r, -1) + ").";
    AddNewDef(s);

    return AddProverCommandLowLevel(p_pratio, a1, a2, a2, a3, sConstantName,
                                    d2s(r, -1));
  }

  if (type == med) {
    std::string sPointName1, sPointName2;
    sPointName1 = "M_{" + a1 + "}^{" + i2s(m_iPointCounter++) + "}";
    sPointName2 = "T_{" + a1 + "}^{" + i2s(m_iPointCounter++) + "}";

    std::string s = "Let $" + sPointName1 + "$ be the midpoint of the segment $" +
               a2 + a3 + "$.";
    AddNewDef(s);

    s = "Let $" + sPointName2 + "$ be the point on bisector of the segment $" +
        a2 + a3 + "$ (such that {\\tt TRATIO} $" + sPointName2 + "$ $" +
        sPointName1 + "$ $" + a2 + "$ " + "$1$).";
    AddNewDef(s);

    if (!AddProverCommand(p_pratio, sPointName1, a2, a2, a3, "0.5"))
      return false;
    if (!AddProverCommand(p_tratio, sPointName2, sPointName1, a2, "1"))
      return false;
    return AddProverCommand(p_line, a1, sPointName1, sPointName2);
  }

  if (type == perp) {
    for (std::list<CGCLCProverCommand>::iterator it = m_ProverCommands.begin();
         it != m_ProverCommands.end(); it++) {
      if (it->type == p_line) {
        if (it->arg[0] == a3) {
          s1 = it->arg[1];
          s2 = it->arg[2];
        }
      }
    }

    if (!s1.empty()) {
      std::string sPointName;
      if (Collinear(a2, s1, s2)) {
        sPointName = "T_{" + a1 + "}^{" + i2s(m_iPointCounter++) + "}";
        std::string s = "Let $" + sPointName +
                   "$ be the point on the normal from the point on the line $" +
                   a2 + "$ (such that {\\tt TRATIO} $" + a3 + "$ $" +
                   sPointName + "$ $" + a2 + "$ $" + s1 + "$ $1$).";

        AddNewDef(s);
        if (!AddProverCommand(p_tratio, sPointName, a2, s1, i2s(1)))
          return false;

        return AddProverCommand(p_line, a1, a2, sPointName);
      } else {
        sPointName = "F_{" + a1 + "}^{" + i2s(m_iPointCounter++) + "}";

        std::string s = "Let $" + sPointName +
                   "$ be the foot of the normal from the point $" + a2 +
                   "$ on the line $" + a3 + "$.";
        AddNewDef(s);
        if (!AddProverCommandLowLevel(p_foot, sPointName, a2, s1, s2))
          return false;

        CGCLCProverExpression Left(ep_s3, a2, s1, s2);
        CGCLCProverExpression Right(0.0);
        if (!AddNDG(ep_inequality, Left, Right, "(foot is not the point "
                                                "itself; construction based "
                                                "assumption)"))
          return false;

        return AddProverCommand(p_line, a1, a2, sPointName);
      }
    } else
      return false;
  }

  if (type == p_foot) {
    for (std::list<CGCLCProverCommand>::iterator it = m_ProverCommands.begin();
         it != m_ProverCommands.end(); it++) {
      if (it->type == p_line) {
        if (it->arg[0] == a3) {
          s1 = it->arg[1];
          s2 = it->arg[2];
        }
      }
    }

    if ((!s1.empty()) && (!s2.empty()))
      return AddProverCommandLowLevel(p_foot, a1, a2, s1, s2);
    else
      return false;
  }

  if (type == parallel) {
    for (std::list<CGCLCProverCommand>::iterator it = m_ProverCommands.begin();
         it != m_ProverCommands.end(); it++) {
      if (it->type == p_line) {
        if (it->arg[0] == a3) {
          s1 = it->arg[1];
          s2 = it->arg[2];
        }
      }
    }

    if (!s1.empty()) {
      std::string sPointName1 = "P_{" + a1 + "}^{" + i2s(m_iPointCounter++) + "}";
      std::string s = "Let $" + sPointName1 + "$ be the point such that lines $" +
                 sPointName1 + a2 + "$ and $" + s1 + s2 +
                 "$ are parallel (and {\\tt PRATIO} $" + sPointName1 + "$ $" +
                 a2 + "$ $" + s1 + "$ $" + s2 + "$ $1$).\n";
      AddNewDef(s);

      if (!AddProverCommand(p_pratio, sPointName1, a2, s1, s2, i2s(1)))
        return false;
      return AddProverCommand(p_line, a1, a2, sPointName1);
    } else
      return false;
  }

  // type=oncircle, type=inter_lc, commands not supported by the area method
  return AddProverCommandLowLevel(type, a1, a2, a3, a4, a5);
}

// ----------------------------------------------------------------------------

bool CTheoremProver::AddProverCommandLowLevel(
    eGCLC_prover_command type, const std::string &a1, const std::string &a2,
    const std::string &a3, const std::string &a4, const std::string &a5, const std::string &a6) {
  CGCLCProverCommand Command;
  Command.type = type;

  if (!a1.empty())
    Command.arg[0] = a1;

  if (type == p_point) {
    convert(a2, Command.x);
    convert(a3, Command.y);
  } else {
    if (!a2.empty())
      Command.arg[1] = a2;
    if (!a3.empty())
      Command.arg[2] = a3;
    if (!a4.empty())
      Command.arg[3] = a4;
    if (!a5.empty())
      Command.arg[4] = a5;
    if (!a6.empty())
      Command.arg[5] = a6;
  }

  CalculateCoordinates(Command);

  m_ProverCommands.push_back(Command);
  return true;
}

// ----------------------------------------------------------------------------

bool CTheoremProver::ExistsPoint(const std::string &A) {
  for (std::list<CGCLCProverCommand>::iterator it = m_ProverCommands.begin();
       it != m_ProverCommands.end(); it++) {
    if (it->type == p_point || it->type == p_inter || it->type == p_pratio ||
        it->type == p_tratio || it->type == p_foot || it->type == midpoint ||
        it->type == online || it->type == oncircle || // new
        it->type == p_interlc) {
      if (it->arg[0] == A) {
        return true;
      }
      // p_interlc contains two points
      else if (it->type == p_interlc && it->arg[1] == A) {
        return true;
      }
    }
  }

  return false;
}

// ----------------------------------------------------------------------------

bool CTheoremProver::Collinear(const std::string &A, const std::string &B,
                               const std::string &C) {
  if (A == B || A == C || B == C)
    return true;

  for (std::list<CGCLCProverCommand>::iterator it = m_ProverCommands.begin();
       it != m_ProverCommands.end(); it++) {
    if (it->type == p_line) {
      if (!OnTheLine(A, it->arg[0]))
        break;
      if (!OnTheLine(B, it->arg[0]))
        break;
      if (!OnTheLine(C, it->arg[0]))
        break;
      return true;
    }
  }

  return false;
}

// ----------------------------------------------------------------------------

bool CTheoremProver::OnTheLine(const std::string &P, const std::string &l) {
  std::string A, B;
  for (std::list<CGCLCProverCommand>::iterator it = m_ProverCommands.begin();
       it != m_ProverCommands.end(); it++) {
    switch (it->type) {
    case p_line:
      if (l == it->arg[0]) {
        A = it->arg[1];
        B = it->arg[2];
      }
      break;
    default:
      break;
    }
  }
  for (std::list<CGCLCProverCommand>::iterator it = m_ProverCommands.begin();
       it != m_ProverCommands.end(); it++) {
    switch (it->type) {
    case p_line:
      if (l == it->arg[0] && P == it->arg[1]) {
        return true;
      }
      if (l == it->arg[0] && P == it->arg[2]) {
        return true;
      }

      break;

    case p_inter:
      if (P == it->arg[0]) {
        if (A == it->arg[1] && B == it->arg[2])
          return true;
        if (A == it->arg[2] && B == it->arg[1])
          return true;
        if (A == it->arg[3] && B == it->arg[4])
          return true;
        if (A == it->arg[4] && B == it->arg[3])
          return true;
      }

    default:
      break;
    }
  }

  return false;
}

// ----------------------------------------------------------------------------

bool CTheoremProver::GetPointsOnLine(const std::string &sLineName, std::string &P1,
                                     std::string &P2) {
  for (std::list<CGCLCProverCommand>::iterator it = m_ProverCommands.begin();
       it != m_ProverCommands.end(); it++) {
    if (it->type == p_line) {
      if (it->arg[0] == sLineName) {
        P1 = it->arg[1];
        P2 = it->arg[2];
        return (!P1.empty() && !P2.empty());
      }
    }
  }
  return false;
}

// ----------------------------------------------------------------------------

bool CTheoremProver::CalculateCoordinates(CGCLCProverCommand &Command) const {
  double k, x1, y1, x2, y2, x3, y3, x4, y4, a1, b1, c1, a2, b2, c2;
  double dDet, r;

  switch (Command.type) {
  case p_point:
    break;

  case p_line:
    break;

  case p_inter:
    GetPointCoordinates(Command.arg[1], x1, y1);
    GetPointCoordinates(Command.arg[2], x2, y2);
    GetPointCoordinates(Command.arg[3], x3, y3);
    GetPointCoordinates(Command.arg[4], x4, y4);

    if ((fabs(x1 - x2) <= EPSILON) && (fabs(y1 - y2) <= EPSILON)) {
      a1 = 1.00;
      b1 = 0.00;
      c1 = -x1;
      return false;
    } else if (fabs(x1 - x2) <= EPSILON) {
      a1 = 1.00;
      b1 = 0.00;
      c1 = -x1;
    } else if (fabs(y1 - y2) <= EPSILON) {
      a1 = 0.00;
      b1 = 1.00;
      c1 = -y1;
    } else {
      k = (x2 - x1) / (y1 - y2);
      a1 = 1.00;
      b1 = k;
      c1 = -(x1 + k * y1);
    }

    if ((fabs(x3 - x4) <= EPSILON) && (fabs(y3 - y4) <= EPSILON)) {
      a2 = 0.00;
      b2 = 1.00;
      c2 = -y3;
      return false;
    } else if (fabs(x3 - x4) <= EPSILON) {
      a2 = 1.00;
      b2 = 0.00;
      c2 = -x3;
    } else if (fabs(y3 - y4) <= EPSILON) {
      a2 = 0.00;
      b2 = 1.00;
      c2 = -y3;
    } else {
      k = (x4 - x3) / (y3 - y4);
      a2 = 1.00;
      b2 = k;
      c2 = -(x3 + k * y3);
    }

    if ((fabs(a1 - a2) <= EPSILON) && (fabs(b1 - b2) <= EPSILON)) {
      Command.x = 0;
      Command.y = 0;
      return false;
    }

    dDet = a1 * b2 - a2 * b1;

    if (fabs(dDet) <= EPSILON) {
      Command.x = 0;
      Command.y = 0;
      return false;
    }

    Command.x = (b1 * c2 - b2 * c1) / dDet;
    Command.y = (a2 * c1 - a1 * c2) / dDet;
    break;

  case p_tratio:
    // "\nTRATIO %s %s %s
    // %s",pCommand->arg[0],pCommand->arg[1],pCommand->arg[2],pCommand->arg[3];

    GetPointCoordinates(Command.arg[1], x1, y1);
    GetPointCoordinates(Command.arg[2], x2, y2);
    convert(Command.arg[3], r);

    Command.x = x1 + y1 - y2;
    Command.y = y1 + x2 - x1;

    Command.x = x1 + r * (Command.x - x1);
    Command.y = y1 + r * (Command.y - y1);
    break;

  case p_pratio:
    // "\nPRATIO %s %s %s %s
    // %s",pCommand->arg[0],pCommand->arg[1],pCommand->arg[2],pCommand->arg[3],pCommand->arg[4];

    GetPointCoordinates(Command.arg[1], x1, y1);
    GetPointCoordinates(Command.arg[2], x2, y2);
    GetPointCoordinates(Command.arg[3], x3, y3);
    if (!Command.arg[5].empty())
      convert(Command.arg[5], r); // construction "online"
    else
      convert(Command.arg[4], r);

    Command.x = x1 + r * (x3 - x2);
    Command.y = y1 + r * (y3 - y2);
    break;

  case p_foot:
    // "\nFOOT %s %s %s
    // %s",pCommand->arg[0],pCommand->arg[1],pCommand->arg[2],pCommand->arg[3];

    GetPointCoordinates(Command.arg[1], x1, y1);
    GetPointCoordinates(Command.arg[2], x2, y2);
    GetPointCoordinates(Command.arg[3], x3, y3);

    if ((fabs(x2 - x3) <= EPSILON) && (fabs(y2 - y3) <= EPSILON)) {
      a1 = 1.00;
      b1 = 0.00;
      c1 = -x2;
      return false;
    } else if (fabs(x2 - x3) <= EPSILON) {
      a1 = 1.00;
      b1 = 0.00;
      c1 = -x2;
    } else if (fabs(y2 - y3) <= EPSILON) {
      a1 = 0.00;
      b1 = 1.00;
      c1 = -y2;
    } else {
      k = (x3 - x2) / (y2 - y3);
      a1 = 1.00;
      b1 = k;
      c1 = -(x2 + k * y2);
    }

    if (b1 > EPSILON) {
      a2 = 1;
      b2 = a1 / -b1;
      c2 = (x1 * b1 - y1 * a1) / -b1;
    } else {
      a2 = -b1 / a1;
      b2 = 1;
      c2 = (x1 * b1 - y1 * a1) / a1;
    }

    if ((fabs(a1 - a2) <= EPSILON) && (fabs(b1 - b2) <= EPSILON)) {
      Command.x = 0;
      Command.y = 0;
      return false;
    }

    dDet = a1 * b2 - a2 * b1;

    if (fabs(dDet) <= EPSILON) {
      Command.x = 0;
      Command.y = 0;
      return false;
    }

    Command.x = (b1 * c2 - b2 * c1) / dDet;
    Command.y = (a2 * c1 - a1 * c2) / dDet;
    break;

  default:
    break;
  }

  return true;
}

// ----------------------------------------------------------------------------

bool CTheoremProver::PrintList() {
  m_pConjecture->PrettyPrint();

  std::string s;
  for (std::list<CGCLCProverCommand>::iterator it = m_ProverCommands.begin();
       it != m_ProverCommands.end(); it++) {
    switch (it->type) {
    case p_point:
      s = "\nPOINT " + it->arg[0];
      break;

    case p_line:
      s = "\nLINE " + it->arg[0] + " " + it->arg[1] + " " + it->arg[2];
      break;

    case p_inter:
      s = "\nINTER  " + it->arg[0] + " " + it->arg[1] + " " + it->arg[2] + " " +
          it->arg[3] + " " + it->arg[4];
      break;

    case p_tratio:
      s = "\nTRATIO " + it->arg[0] + " " + it->arg[1] + " " + it->arg[2] + " " +
          it->arg[3] + " " + it->arg[4];
      break;

    case p_pratio:
      s = "\nPRATIO " + it->arg[0] + " " + it->arg[1] + " " + it->arg[2] + " " +
          it->arg[3] + " " + it->arg[4];
      break;

    case p_foot:
      s = "\nFOOT " + it->arg[0] + " " + it->arg[1] + " " + it->arg[2] + " " +
          it->arg[3];
      break;

    default:
      break;
    }
  }

  return true;
}

// ----------------------------------------------------------------------------

bool CTheoremProver::SetProverConjecture(const std::string &conjecture) {
  int nInputPos;
  if (!GetExpression(conjecture, nInputPos, m_InitialConjecture))
    return false;

  // Natural, original formulation (e.g. "parallel" instead of
  // a formulation in terms of geometric quantities
  if (!GetExpression(conjecture, nInputPos, m_NaturalExpressedConjecture))
      return false;

  m_pConjecture = &m_InitialConjecture;

  // 13.07. conjectures in natural form added
  m_pConjecture->ToGeometricQuantities();

  if (m_pConjecture->GetType() != ep_equality)
    return false;

  m_bValidConjecture = true;

  return true;
}

// ----------------------------------------------------------------------------

bool CTheoremProver::GetExpression(const std::string &conjecture, int &nPos,
                                   CGCLCProverExpression &exp) {
  std::string s;
  int nInputPos = 0;
  bool bBracket = false;
  std::string InputString;
  double n;
  unsigned i;

  nPos = 0;

  InputString = conjecture;
  take_id(InputString, nInputPos, s);

  // Inputstd::string += nInputPos;
  InputString = InputString.substr(nInputPos, InputString.size() - nInputPos);

  nPos += nInputPos;

  if (s == "{") {
    bBracket = true;
    take_id(InputString, nInputPos, s);
    InputString = InputString.substr(nInputPos, InputString.size() - nInputPos);
    nPos += nInputPos;
  }

  exp.SetType(ep_point);
  if (!ExistsPoint(s)) {

    if (convert(s, n)) {
      exp = n;
    } else {
      if (s == "equal")
        exp.SetType(ep_equality);
      else if (s == "sum")
        exp.SetType(ep_sum);
      else if (s == "mult")
        exp.SetType(ep_mult);
      else if (s == "ratio")
        exp.SetType(ep_ratio);
      else if (s == "sratio")
        exp.SetType(ep_segment_ratio);
      else if (s == "signed_area3")
        exp.SetType(ep_s3);
      else if (s == "signed_area4")
        exp.SetType(ep_s4);
      else if (s == "pythagoras_difference3")
        exp.SetType(ep_p3);
      else if (s == "pythagoras_difference4")
        exp.SetType(ep_p4);
      else if (s == "identical")
        exp.SetType(ep_identical);
      else if (s == "collinear")
        exp.SetType(ep_collinear);
      else if (s == "perpendicular")
        exp.SetType(ep_perpendicular);
      else if (s == "parallel")
        exp.SetType(ep_parallel);
      else if (s == "midpoint")
        exp.SetType(ep_midpoint);
      else if (s == "same_length")
        exp.SetType(ep_same_length);
      else if (s == "harmonic")
        exp.SetType(ep_harmonic);
      else if (s == "segment") // Predovic, 08.2007
        exp.SetType(ep_segment);
      else if (s == "alg_sum_zero3") // Predovic, 09.2007
        exp.SetType(ep_algsumzero3);
      else if (s == "angle") // Predovic, 09.2007
        exp.SetType(ep_angle);
      else if (!ExistsPoint(s)) // 07.2007.
        exp.SetType(ep_constant);
    }
  }

  switch (exp.GetType()) {

  case ep_identical:
    for (i = 0; i < arity(exp.GetType()); i++) {
      CGCLCProverExpression arg;
      if (!GetExpression(InputString, nInputPos, arg))
        return false;
      exp.SetArg(i, arg);
      InputString =
          InputString.substr(nInputPos, InputString.size() - nInputPos);
      nPos += nInputPos;
    }

    if (exp.GetArg(0).GetType() != ep_point ||
        exp.GetArg(1).GetType() != ep_point)
      return false;

    break;

  case ep_collinear:
  case ep_midpoint:
    for (i = 0; i < arity(exp.GetType()); i++) {
      CGCLCProverExpression arg;
      if (!GetExpression(InputString, nInputPos, arg))
        return false;
      exp.SetArg(i, arg);
      InputString =
          InputString.substr(nInputPos, InputString.size() - nInputPos);
      nPos += nInputPos;
    }

    if (exp.GetArg(0).GetType() != ep_point ||
        exp.GetArg(1).GetType() != ep_point ||
        exp.GetArg(2).GetType() != ep_point) {
      return false;
    }
    break;

  case ep_parallel:
  case ep_perpendicular:
  case ep_same_length:
  case ep_harmonic:
    for (i = 0; i < arity(exp.GetType()); i++) {
      CGCLCProverExpression arg;
      if (!GetExpression(InputString, nInputPos, arg))
        return false;
      exp.SetArg(i, arg);
      InputString =
          InputString.substr(nInputPos, InputString.size() - nInputPos);
      nPos += nInputPos;
    }

    if (exp.GetArg(0).GetType() != ep_point ||
        exp.GetArg(1).GetType() != ep_point ||
        exp.GetArg(2).GetType() != ep_point ||
        exp.GetArg(3).GetType() != ep_point) {
      return false;
    }
    break;

  case ep_point:
    if (!ExistsPoint(s)) {
      // "\n Point %s not defined\n",s;
      return false;
    }
    exp.SetName(s);
    break;

  case ep_constant:
    exp.SetName(s);
    break;

  case ep_equality:
  case ep_mult:
  case ep_ratio:
  case ep_sum:
    for (i = 0; i < arity(exp.GetType()); i++) {
      CGCLCProverExpression arg;
      if (!GetExpression(InputString, nInputPos, arg))
        return false;
      exp.SetArg(i, arg);
      InputString =
          InputString.substr(nInputPos, InputString.size() - nInputPos);
      nPos += nInputPos;
    }

    break;

  case ep_s3:
  case ep_p3:
    for (i = 0; i < arity(exp.GetType()); i++) {
      CGCLCProverExpression arg;
      if (!GetExpression(InputString, nInputPos, arg))
        return false;
      exp.SetArg(i, arg);
      InputString =
          InputString.substr(nInputPos, InputString.size() - nInputPos);
      nPos += nInputPos;
    }

    break;

  case ep_segment_ratio:
  case ep_s4:
  case ep_p4:

    for (i = 0; i < arity(exp.GetType()); i++) {
      CGCLCProverExpression arg;
      if (!GetExpression(InputString, nInputPos, arg))
        return false;
      exp.SetArg(i, arg);
      InputString =
          InputString.substr(nInputPos, InputString.size() - nInputPos);
      nPos += nInputPos;
    }

    if (exp.GetType() == ep_segment_ratio) {
      std::string N1 = exp.GetArgName(2);
      std::string N2 = exp.GetArgName(3);
      CGCLCProverExpression Left(ep_p3, N1, N2, N1);
      CGCLCProverExpression Right(0.0);
      if (!AddNDG(ep_inequality, Left, Right, "(conjecture based assumption)"))
        // return false;
        return NULL; // changed on 11.2015.
    }

    break;

  case ep_segment:
    // two arguments
    for (i = 0; i < arity(exp.GetType()); i++) {
      CGCLCProverExpression arg;
      if (!GetExpression(InputString, nInputPos, arg))
        return false;
      exp.SetArg(i, arg);
      InputString =
          InputString.substr(nInputPos, InputString.size() - nInputPos);
      nPos += nInputPos;
    }

    break;

  case ep_algsumzero3:
    // alg_sum_zero3 E1 E2 E3
    // algebraic sum of expressions E1, E2 and E3 is zero

    // three arguments
    for (i = 0; i < arity(exp.GetType()); i++) {
      CGCLCProverExpression arg;
      if (!GetExpression(InputString, nInputPos, arg))
        return false;
      exp.SetArg(i, arg);
      InputString =
          InputString.substr(nInputPos, InputString.size() - nInputPos);
      nPos += nInputPos;
    }
    break;
  case ep_angle:
    // angle B A C
    // tangens of ORIENTED angle

    // three arguments
    for (i = 0; i < arity(exp.GetType()); i++) {
      CGCLCProverExpression arg;
      if (!GetExpression(InputString, nInputPos, arg))
        return false;
      exp.SetArg(i, arg);
      InputString =
          InputString.substr(nInputPos, InputString.size() - nInputPos);
      nPos += nInputPos;
    }

    break;
  default:
    break;
  }

  if (bBracket) {
    take_text(InputString, nInputPos, s);

    InputString = InputString.substr(nInputPos, InputString.size() - nInputPos);
    nPos += nInputPos;
    if (!(s[0] == '}'))
      return false;
  }

  return true;
}

// ----------------------------------------------------------------------------

bool CTheoremProver::AddNewDef(std::string &sDef) {
  CNewDef d;
  d.m_sDef = sDef;
  m_NewDefs.push_back(d);
  return true;
}

// ----------------------------------------------------------------------------

bool CTheoremProver::Timeout() {
  return (m_iProverTimeout < 0 ||
          m_Timer.ElapsedTime() > m_iProverTimeout);
}

// ----------------------------------------------------------------------------

void CTheoremProver::PrintLaTeX(const std::string &s) {
  if (m_hLaTeXOutputProof.is_open())
    Print(m_hLaTeXOutputProof, s);
}

// ----------------------------------------------------------------------------

void CTheoremProver::PrintXML(const std::string &s) {
  if (m_hXMLOutputProof.is_open())
    Print(m_hXMLOutputProof, s);
}

// **************************************************************************
// miscellaneous
// **************************************************************************

void take_text(const std::string &sInput, int &nInputPos, std::string &sOutput) {
  char c;

  nInputPos = 0;
  sOutput.erase();

  do {
    c = sInput[nInputPos++];
  } while ((c == ' ') || (c == '\n') || (c == '\r') || (c == '\t'));

  do {
    sOutput.push_back(c);
    c = sInput[nInputPos++];
  } while (!((c == ' ') || (c == '\n') || (c == '\r') || (c == '\t') ||
             (c == 0) || (c == '}') || (c == '{')));

  if ((c == 0) || (c == '}') || (c == '{'))
    nInputPos--;
}

// ----------------------------------------------------------------------------

void take_id(const std::string &sInput, int &nInputPos, std::string &sOutput) {
  char c;
  int i = 0;
  int brackets = 0;

  nInputPos = 0;

  sOutput = "";

  bool skip;
  do {
    c = sInput[nInputPos];
    skip = ((c == ' ') || (c == '\n') || (c == '\r') ||
            (c == '\t') || (c == 9) || (c == -96));
    if (skip)
      nInputPos++;
  } while (skip);

  do {
    sOutput += std::string(1, c);
    i++;
    c = sInput[++nInputPos];

    if (c == '{')
      brackets++;
    if (c == '}')
      brackets--;

  } while (
      !((c == ' ') || (c == '\n') || (c == '\r') || (c == '\t') || (c == 9) ||
        (c == 0) || (c == '}' && brackets < 0) || (c == '{' && i == 0) ||
        (c == '{' && i > 0 && sOutput[i - 1] != '_' && sOutput[i - 1] != '^')));

  if ((c == '}' && brackets < 0) || (c == '{' && i == 0) ||
      (c == '{' && i > 0 && sOutput[i - 1] != '_' && sOutput[i - 1] != '^'))
    nInputPos--;
}

// **************************************************************************
// end of miscellaneous
// **************************************************************************
