// AreaMethod.cpp: implementation of the CAreaMethod class.
//
//////////////////////////////////////////////////////////////////////

#include "AreaMethod.h"
#include <cmath>
#include <vector>

#define EPSILON 0.000005

// ----------------------------------------------------------------------------

CAreaMethod::CAreaMethod() {
  m_iNumberOfEliminationSteps = 0;
  m_iNumberOfGeometricSteps = 0;
  m_iNumberOfAlgebraicSteps = 0;
  m_ConjecturesCount = 0;
}

// ----------------------------------------------------------------------------

CAreaMethod::~CAreaMethod() {}

// ----------------------------------------------------------------------------

enum eGCLC_conjecture_status
CAreaMethod::ProveConjecture(const CGCLCProverExpression &Conj) {
  int o1, o2;

  eGCLC_conjecture_status status;
  Push(Conj);
  m_iProofDepth++;

  if (m_iProofDepth > 0) {
    if (m_iProofLevel >= 3) {
      PrintLaTeX("\\end{displayproof}\n\n");
      PrintLaTeX("\n\\vspace*{2mm} \\hrule \\vspace*{2mm} \n");
      PrintLaTeX("\n\nProving lemma...(level " + i2s(m_iProofDepth) + ")");
      PrintLaTeX("\n\n\\begin{displayproof}\n");

      PrintXML(std::string(2 * m_iProofDepth, '\t'));
      PrintXML("<lemma level=\"" + i2s(m_iProofDepth) + "\">\n");
      PrintXML(std::string(2 * m_iProofDepth, '\t'));
      PrintXML("<proof>\n");
    }
  } else {
    PrintLaTeX("\n\n\\begin{displayproof}\n");
    PrintXML("\t<proof>\n");
  }

  if ((m_iProofDepth == 0) || ((m_iProofDepth > 0) && (m_iProofLevel >= 3)))
    OutputStep(*m_pConjecture, "the statement", eps_statement);

  for (std::list<CGCLCProverCommand>::iterator it = m_ProverCommands.end();
       it != m_ProverCommands.begin();) {
    it--;

    //-Geometric simplifications ----------------------------------------
    bool bChanged_geo = false;

    while (ZeroGeometryQuantity(*m_pConjecture))
      bChanged_geo = true;
    while (Company(*m_pConjecture, *m_pConjecture))
      bChanged_geo = true;

    if ((it->type == p_inter) || (it->type == p_pratio) ||
        (it->type == p_tratio) || (it->type == p_foot))
      while (Orient(it->arg[0], *m_pConjecture))
        bChanged_geo = true;

    if (bChanged_geo)
      OutputStep(*m_pConjecture, "geometric simplifications",
                 eps_grouped_geometric);

    if (!AlgebraicSimplification(2, &status)) {
      Pop();
      return status;
    }

    //-Elimination stuff -----------------------------------------------
    bool bChanged = false;

    o1 = PointOccurences(it->arg[0], *m_pConjecture);

    bool bTimeOut = Timeout();
    if (Timeout() ||
        m_iNumberOfEliminationSteps + m_iNumberOfGeometricSteps +
                m_iNumberOfAlgebraicSteps >
            m_iProofLimit) {

      PrintLaTeX("\\end{displayproof}\n\n");
      if (bTimeOut)
        PrintLaTeX("\n\nTimeout; the proving procedure was stopped.");
      else
        PrintLaTeX("\n\nToo many proof steps (more than "
                   "the given proof limit); the proving "
                   "procedure was stopped.");
      PrintLaTeX("\n\n\\proveddisproved{2}\n"); // Failed to prove or disprove

      PrintXML(std::string(2 * m_iProofDepth + 1, '\t'));
      PrintXML("</proof>\n\n");
      if (bTimeOut)
        PrintXML("\n\nTimeout; the proving procedure was stopped.");
      else
        PrintXML("Too many proof steps (more than the "
                 "given proof limit); the proving "
                 "procedure was stopped.");
      PrintXML(std::string(2 * m_iProofDepth + 1, '\t'));
      PrintXML("<status value=\"failed\"></status>\n"); // Failed to prove or
                                                        // disprove
      Pop();
      if (bTimeOut)
        return e_unknown_timeout;
      else
        return e_unknown_toomanysteps;
    }

    switch (it->type) {
    case p_point:
      bChanged = false;
      break;

    case p_line:
      bChanged = false;
      break;

    case p_inter:
      bChanged =
          EliminatePoint(e_IntersectionPoint, it->arg[0], it, *m_pConjecture);
      break;

    case p_pratio:
      bChanged = EliminatePoint(e_PratioPoint, it->arg[0], it, *m_pConjecture);
      break;

    case p_tratio:
      bChanged = EliminatePoint(e_TratioPoint, it->arg[0], it, *m_pConjecture);
      break;

    case p_foot:
      bChanged = EliminatePoint(e_FootPoint, it->arg[0], it, *m_pConjecture);
      break;

    default:
      Pop();

      PrintLaTeX("\\end{displayproof}\n\n");
      PrintLaTeX("\n\nThe conjecture out of scope of the prover.");
      PrintLaTeX("\n\n\\proveddisproved{2}\n"); // Failed to prove or disprove

      PrintXML(std::string(2 * m_iProofDepth + 1, '\t'));
      PrintXML("</proof>\n\n");
      PrintXML("\n\nThe conjecture out of scope of the prover.");
      PrintXML("\t");
      PrintXML("<status value=\"failed\"></status>\n"); // Failed to prove or
                                                        // disprove
      return e_construction_out_of_scope;
    }

    o2 = PointOccurences(it->arg[0], *m_pConjecture);
    if (((it->type == p_inter) || (it->type == p_pratio) ||
         (it->type == p_tratio) || (it->type == p_foot))) {
      if (o2 != 0) {
        if ((!bChanged) || (o2 > 0 && o2 > o1 + 5)) {

          PrintLaTeX("\\end{displayproof}");
          PrintLaTeX("\n\nUnable to eliminate the point $" + it->arg[0] + "$.");
          PrintLaTeX("\n\nFailed to prove (or disprove) "
                     "the conjecture (required elimination "
                     "not available).\n\n");

          PrintXML(std::string(2 * m_iProofDepth + 1, '\t'));
          PrintXML("</proof>\n\n");
          PrintXML("Unable to eliminate the point $" + it->arg[0] + "$.\n\n");
          PrintXML("Failed to prove (or disprove) the "
                   "conjecture (required elimination not "
                   "available).\n\n");
          PrintXML("</main_proof>\n\n");
          Pop();
          return e_unknown;
        } else
          it++;
      }
    }
  }

  if (!AlgebraicSimplification(2, &status)) {
    Pop();
    return status;
  }

  //-Geometric simplifications ----------------------------------------
  bool bChanged_geo = false;

  if (S4_to_S3(*m_pConjecture))
    bChanged_geo = true;
  if (P4_to_P3(*m_pConjecture))
    bChanged_geo = true;
  while (H4points(*m_pConjecture))
    bChanged_geo = true;
  while (ZeroGeometryQuantity(*m_pConjecture))
    bChanged_geo = true;
  while (Company(*m_pConjecture, *m_pConjecture))
    bChanged_geo = true;

  if (bChanged_geo)
    OutputStep(*m_pConjecture, "geometric simplifications",
               eps_grouped_geometric);

  if (!AlgebraicSimplification(2, &status)) {
    Pop();
    return status;
  }

  if (P3_to_segments(*m_pConjecture))
    bChanged_geo = true;
  while (Company(*m_pConjecture, *m_pConjecture))
    bChanged_geo = true;

  if (bChanged_geo)
    OutputStep(*m_pConjecture, "geometric simplifications",
               eps_grouped_geometric);

  if (!AlgebraicSimplification(0, &status)) {
    Pop();
    return status;
  }

  eGCLC_conjecture_status s;
  if (m_pConjecture->GetArg(0) == m_pConjecture->GetArg(1))
    s = e_proved;
  else if ((m_pConjecture->GetArg(0).GetType() == ep_number &&
            m_pConjecture->GetArg(1).GetType() == ep_number &&
            m_pConjecture->GetArg(0).GetNumber() !=
                m_pConjecture->GetArg(1).GetNumber()))
    s = e_disproved;
  else
    s = e_unknown;

  if ((m_iProofDepth == 0) || ((m_iProofDepth > 0) && (m_iProofLevel >= 3))) {
    PrintLaTeX("\\end{displayproof}\n\n");
    PrintXML(std::string(2 * m_iProofDepth + 1, '\t'));
    PrintXML("</proof>\n\n");

    if (s == e_proved) {
      PrintLaTeX("\n\\proveddisproved{1}\n"); // Proved
      PrintXML(std::string(2 * m_iProofDepth + 1, '\t'));
      PrintXML("<status value=\"proved\"></status>\n"); // Proved
    } else if (s == e_disproved) {
      PrintLaTeX("\n\\proveddisproved{0}\n"); // Disproved
      PrintXML(std::string(2 * m_iProofDepth + 1, '\t'));
      PrintXML("<status value=\"disproved\"></status>\n"); // Disproved
    } else {
      PrintLaTeX("\n\\proveddisproved{2}\n"); // Failed to prove or disprove
      PrintXML(std::string(2 * m_iProofDepth + 1, '\t'));
      PrintXML("<status value=\"failed\"></status>\n"); // Failed to prove or
                                                        // disprove
    }

    // XML output
    PrintXML(std::string(2 * m_iProofDepth, '\t'));
    if (m_iProofDepth > 0)
      PrintXML("</lemma>\n\n");
    if (m_iProofDepth > 0 && m_iProofLevel >= 3) {
      PrintLaTeX("\n\n\\begin{displayproof}\n");
    }
  }

  Pop();
  m_iProofDepth--;
  return s;
}

// ----------------------------------------------------------------------------

void CAreaMethod::Push(const CGCLCProverExpression &Conj)
{
    m_ConjecturesStack[m_ConjecturesCount] = Conj;
    m_pConjecture = &m_ConjecturesStack[m_ConjecturesCount];
    m_ConjecturesCount++;
}

// ----------------------------------------------------------------------------

void CAreaMethod::Pop()
{
    m_ConjecturesCount--;
    if (m_ConjecturesCount == 0)
      m_pConjecture = NULL;
    else
      m_pConjecture = &m_ConjecturesStack[m_ConjecturesCount-1];
}

// ----------------------------------------------------------------------------

bool CAreaMethod::AlgebraicSimplification(int exceptlast,
                                          eGCLC_conjecture_status *status) {
  std::string sRuleApplied;

  bool bChanged_grouped = false;
  bool bChanged_alg = true;

  *status = e_idle;

  while (bChanged_alg) {
    // int ExceptLast = exceptlast; // except OneSide
    bChanged_alg =
        m_pConjecture->ApplyAllSimpleAlgebraicRules(sRuleApplied, exceptlast);
    if (bChanged_alg) {
      OutputStep(*m_pConjecture, sRuleApplied, eps_algebraic);
      bChanged_grouped = true;
    }

    if (!bChanged_alg)
      if (CancelMult(*m_pConjecture)) {
        bChanged_alg = true;
        bChanged_grouped = true;
      }

    if (!bChanged_alg) {
      bChanged_alg =
          m_pConjecture->ApplyAllComplexAlgebraicRules(sRuleApplied, exceptlast);
      if (bChanged_alg) {
        OutputStep(*m_pConjecture, sRuleApplied, eps_algebraic);
        bChanged_grouped = true;
      }
    }

    bool bTimeOut = Timeout();
    if (bTimeOut ||
        m_iNumberOfEliminationSteps + m_iNumberOfGeometricSteps +
                m_iNumberOfAlgebraicSteps >
            m_iProofLimit) {

      PrintLaTeX("\\end{displayproof}");
      if (bTimeOut)
        PrintLaTeX("\n\nTimeout; the proving procedure was stopped.");
      else
        PrintLaTeX("\n\nToo many proof steps (more than "
                   "the given proof limit); the proving "
                   "procedure was stopped.");
      PrintLaTeX("\n\n\\proveddisproved{2}\n"); // Failed to prove or disprove

      PrintXML(std::string(m_iProofDepth + 1, '\t'));
      PrintXML("</proof>\n\n");
      if (bTimeOut)
        PrintXML("\n\nTimeout; the proving procedure was stopped.");
      else
        PrintXML("Too many proof steps (more than the "
                 "given proof limit); the proving "
                 "procedure was stopped.");
      PrintXML(std::string(m_iProofDepth + 1, '\t'));
      PrintXML("<status value=\"failed\"></status>\n"); // Failed to prove or
                                                        // disprove
      PrintXML("</main_proof>\n\n");

      if (bTimeOut)
        *status = e_unknown_timeout;
      else
        *status = e_unknown_toomanysteps;

      return false;
    }
  }

  if (bChanged_grouped)
    OutputStep(*m_pConjecture, "algebraic simplifications",
               eps_grouped_algebraic);

  return true;
}

// ----------------------------------------------------------------------------

bool CAreaMethod::Orient(const std::string &sPoint, CGCLCProverExpression &exp) {
  switch (exp.GetType()) {
  case ep_point:
  case ep_number:
  case ep_segment:
    break;

  case ep_equality:
  case ep_sum:
  case ep_mult:
  case ep_ratio:
    if (Orient(sPoint, exp.GetArg(0)) || Orient(sPoint, exp.GetArg(1)))
      return true;
    break;

  case ep_segment_ratio:

    if (exp.GetArgName(0) == sPoint && exp.GetArgName(1) != sPoint) {
      // YA/BC -> -1*AY/BC
      CGCLCProverExpression n1, n2;
      n1 = -1.0;
      n2 = CGCLCProverExpression::sratio(exp.GetArgName(1), exp.GetArgName(0),
                                         exp.GetArgName(2), exp.GetArgName(3));
      exp = (n1 * n2);

      // Lemma 2.1-1
      OutputStep(*m_pConjecture, "Lemma 10", eps_geometric_simplification);
      return true;
    }

    if (exp.GetArgName(2) == sPoint && exp.GetArgName(3) != sPoint) {
      // BC/YA -> -1*BC/AY

      CGCLCProverExpression n1(-1.0);
      CGCLCProverExpression n2 =
          CGCLCProverExpression::sratio(exp.GetArgName(0), exp.GetArgName(1),
                                        exp.GetArgName(3), exp.GetArgName(2));
      exp = (n1 * n2);

      // OutputStep(*m_pConjecture,"Lemma 2.1-1",eps_geometric_simplification);
      OutputStep(*m_pConjecture, "Lemma 10", eps_geometric_simplification);
      return true;
    }

    break;

  case ep_s3:
    if (exp.GetArgName(1) == sPoint && exp.GetArgName(2) != sPoint) {
      // put the elim point in the last position
      // S_AYB -> S_BAY

      std::string s = exp.GetArgName(0);
      exp.SetArgName(0, exp.GetArgName(2));
      exp.SetArgName(2, exp.GetArgName(1));
      exp.SetArgName(1, s);

      // Lemma 2.2-1
      OutputStep(*m_pConjecture, "Lemma 1", eps_geometric_simplification);
      return true;
    }

    if (exp.GetArgName(0) == sPoint && exp.GetArgName(2) != sPoint) {
      // put the elim point in the last position
      // S_YAB -> S_ABY

      std::string s = exp.GetArgName(2);
      exp.SetArgName(2, exp.GetArgName(0));
      exp.SetArgName(0, exp.GetArgName(1));
      exp.SetArgName(1, s);

      // Lemma 2.2-1
      OutputStep(*m_pConjecture, "Lemma 1", eps_geometric_simplification);
      return true;
    }
    return false;

  case ep_p3:
    if (exp.GetArgName(0) == sPoint && exp.GetArgName(2) != sPoint) {
      // put the elim point in the last position
      // P_YAB -> P_BAY

      std::string s = exp.GetArgName(0);
      exp.SetArgName(0, exp.GetArgName(2));
      exp.SetArgName(2, s);

      // Lemma 2.2-1
      OutputStep(*m_pConjecture, "Lemma 18", eps_geometric_simplification);
      return true;
    }
    return false;

  case ep_s4:
    if (exp.GetArgName(0) == sPoint || exp.GetArgName(1) == sPoint ||
        exp.GetArgName(2) == sPoint) {
      // S_YABC -> S_YAB+S_YBC
      // S_AYBC -> S_AYB+S_ABC
      // S_ABYC -> S_ABY+S_AYC
      CGCLCProverExpression n1 = CGCLCProverExpression::s3(
          exp.GetArgName(0), exp.GetArgName(1), exp.GetArgName(2));
      CGCLCProverExpression n2 = CGCLCProverExpression::s3(
          exp.GetArgName(0), exp.GetArgName(2), exp.GetArgName(3));
      exp = (n1 + n2);
      OutputStep(*m_pConjecture, "Definition 4", eps_geometric_simplification);
      return true;
    }
    return false;

  case ep_p4:
    if (exp.GetArgName(0) == sPoint || exp.GetArgName(1) == sPoint ||
        exp.GetArgName(2) == sPoint) {
      // P_ABCD = P_ABD-P_CBD
      CGCLCProverExpression n1 = CGCLCProverExpression::p3(
          exp.GetArgName(0), exp.GetArgName(1), exp.GetArgName(3));
      CGCLCProverExpression n2 = CGCLCProverExpression::p3(
          exp.GetArgName(2), exp.GetArgName(1), exp.GetArgName(3));
      CGCLCProverExpression n3(-1.0);
      exp = (n1 + (n3 * n2));

      OutputStep(*m_pConjecture, "Definition 6", eps_geometric_simplification);
      return true;
    }
    return false;

  default:
    break;
  }

  return false;
}

// ----------------------------------------------------------------------------

eGCLC_conjecture_status
CAreaMethod::ProveCollinear(const std::string &A, const std::string &B, const std::string &C) {
  if (Collinear(A, B, C))
    return e_proved;
  CGCLCProverExpression LemmaLeft = CGCLCProverExpression::s3(A, B, C);
  CGCLCProverExpression LemmaRight = 0.0;
  CGCLCProverExpression Lemma(ep_equality, LemmaLeft, LemmaRight);
  eGCLC_conjecture_status bProved = ProveConjecture(Lemma);
  return bProved;
}

// ----------------------------------------------------------------------------

bool CAreaMethod::Company(const CGCLCProverExpression &exp,
                          CGCLCProverExpression &top_exp) {
  // Rule "Matching quantity
  switch (exp.GetType()) {
  case ep_number:
  case ep_point:
    break;

  case ep_equality:
  case ep_sum:
  case ep_ratio:
  case ep_mult:
    if (Company(exp.GetArg(0), top_exp))
      return true;
    if (Company(exp.GetArg(1), top_exp))
      return true;
    break;

  case ep_segment_ratio:
  case ep_s4:
  case ep_p4:
    break;

  case ep_segment:
    if (ExistsSegmentsimilar(top_exp, exp.GetArgName(0), exp.GetArgName(1)))
      return FindingCompany(top_exp, exp);
    break;

  case ep_s3:
    if (ExistsS3similar(top_exp, exp.GetArgName(0), exp.GetArgName(1),
                        exp.GetArgName(2)))
      return FindingCompany(top_exp, exp);
    break;

  case ep_p3:
    if (ExistsP3similar(top_exp, exp.GetArgName(0), exp.GetArgName(1),
                        exp.GetArgName(2)))
      return FindingCompany(top_exp, exp);
    break;

  default:
    break;
  }

  return false;
}

bool CAreaMethod::FindingCompany(CGCLCProverExpression &exp,
                                 const CGCLCProverExpression &target_exp) {
  bool b1, b2;
  switch (exp.GetType()) {
  case ep_number:
  case ep_point:
    return false;
    break;

  case ep_equality:
  case ep_sum:
  case ep_ratio:
  case ep_mult:
    b1 = FindingCompany(exp.GetArg(0), target_exp);
    b2 = FindingCompany(exp.GetArg(1), target_exp);
    if (b1 || b2)
      return true;
    else
      return false;

  case ep_segment:
    if (target_exp.GetType() != ep_segment)
      return false;
    if (exp.GetArgName(0) == target_exp.GetArgName(1) &&
        exp.GetArgName(1) == target_exp.GetArgName(0) &&
        exp.GetArgName(0) != exp.GetArgName(1)) {
      // AB -> BA
      exp.SetArgName(0, target_exp.GetArgName(0));
      exp.SetArgName(1, target_exp.GetArgName(1));

      // Lemma (AB = BA)
      OutputStep(*m_pConjecture, "equality of segments",
                 eps_geometric_simplification);
      return true;
    }

    return false;

  case ep_s3:
    if (exp.GetType() != target_exp.GetType())
      return false;

    if (exp.GetArgName(0) == target_exp.GetArgName(0) &&
        exp.GetArgName(1) == target_exp.GetArgName(1) &&
        exp.GetArgName(2) == target_exp.GetArgName(2))
      return false;

    if ((exp.GetArgName(0) == target_exp.GetArgName(1) &&
         exp.GetArgName(1) == target_exp.GetArgName(2) &&
         exp.GetArgName(2) == target_exp.GetArgName(0)) ||
        (exp.GetArgName(0) == target_exp.GetArgName(2) &&
         exp.GetArgName(1) == target_exp.GetArgName(0) &&
         exp.GetArgName(2) == target_exp.GetArgName(1))) {
      // S_BCA -> S_ABC
      // S_CAB -> S_ABC
      exp.SetArgName(0, target_exp.GetArgName(0));
      exp.SetArgName(1, target_exp.GetArgName(1));
      exp.SetArgName(2, target_exp.GetArgName(2));

      // OutputStep(*m_pConjecture,"Lemma 2.2-1",eps_geometric_simplification);
      OutputStep(*m_pConjecture, "Lemma 1", eps_geometric_simplification);
      return true;
    }

    if ((exp.GetArgName(0) == target_exp.GetArgName(0) &&
         exp.GetArgName(1) == target_exp.GetArgName(2) &&
         exp.GetArgName(2) == target_exp.GetArgName(1)) ||
        (exp.GetArgName(0) == target_exp.GetArgName(1) &&
         exp.GetArgName(1) == target_exp.GetArgName(0) &&
         exp.GetArgName(2) == target_exp.GetArgName(2)) ||
        (exp.GetArgName(0) == target_exp.GetArgName(2) &&
         exp.GetArgName(1) == target_exp.GetArgName(1) &&
         exp.GetArgName(2) == target_exp.GetArgName(0))) {
      // S_ACB -> -S_ABC
      // S_BAC -> -S_ABC
      // S_CBA -> -S_ABC
      CGCLCProverExpression n1(-1.0);
      CGCLCProverExpression n2(ep_s3, target_exp.GetArgName(0),
                               target_exp.GetArgName(1),
                               target_exp.GetArgName(2));
      exp = (n1 * n2);
      // Lemma 2.2-1
      OutputStep(*m_pConjecture, "Lemma 1", eps_geometric_simplification);
      return true;
    }
    return false;

  case ep_p3:
    if (exp.GetType() != target_exp.GetType())
      return false;

    if (exp.GetArgName(0) == target_exp.GetArgName(0) &&
        exp.GetArgName(1) == target_exp.GetArgName(1) &&
        exp.GetArgName(2) == target_exp.GetArgName(2))
      return false;

    if (exp.GetArgName(0) == target_exp.GetArgName(2) &&
        exp.GetArgName(1) == target_exp.GetArgName(1) &&
        exp.GetArgName(2) == target_exp.GetArgName(0) &&
        exp.GetArgName(0) != exp.GetArgName(2)) {
      // P_CBA -> P_ABC
      exp.SetArgName(0, target_exp.GetArgName(0));
      exp.SetArgName(1, target_exp.GetArgName(1));
      exp.SetArgName(2, target_exp.GetArgName(2));

      OutputStep(*m_pConjecture, "Lemma 18", eps_geometric_simplification);
      return true;
    }

    if (exp.GetArgName(0) == target_exp.GetArgName(1) &&
        exp.GetArgName(1) == target_exp.GetArgName(0) &&
        exp.GetArgName(2) == target_exp.GetArgName(1) &&
        exp.GetArgName(0) == exp.GetArgName(2)) {
      // P_ABA -> P_BAB
      exp.SetArgName(0, target_exp.GetArgName(0));
      exp.SetArgName(1, target_exp.GetArgName(1));
      exp.SetArgName(2, target_exp.GetArgName(0));

      OutputStep(*m_pConjecture, "Lemma 19 ($P_{ABA}=P_{BAB}$)",
                 eps_geometric_simplification);
      return true;
    }

    return false;

  case ep_segment_ratio:
  case ep_s4:
  case ep_p4:
    break;

  default:
    break;
  }

  return false;
}

// ----------------------------------------------------------------------------

bool CAreaMethod::ZeroGeometryQuantity(CGCLCProverExpression &exp) {
  switch (exp.GetType()) {
  case ep_point:
    break;

  case ep_equality:
  case ep_sum:
  case ep_mult:
  case ep_ratio:
    if (ZeroGeometryQuantity(exp.GetArg(0)))
      return true;
    if (ZeroGeometryQuantity(exp.GetArg(1)))
      return true;
    break;

  case ep_segment:
    if (exp.GetArgName(0) == exp.GetArgName(1)) {
      // AA = 0
      exp = 0.0;
      // Lemma
      OutputStep(*m_pConjecture, "by zero segment", eps_geometric_simplification);
      return true;
    }
    break;

  case ep_segment_ratio:
    if (exp.GetArgName(0) == exp.GetArgName(1)) {
      // AA/BC -> 0
      exp = 0.0;
      // OutputStep(*m_pConjecture,"Lemma 2.1",eps_geometric_simplification);
      OutputStep(*m_pConjecture, "Lemma 11", eps_geometric_simplification);
      return true;
    }
    break;

  case ep_s3:
    if ((exp.GetArgName(0) == exp.GetArgName(1) ||
         exp.GetArgName(0) == exp.GetArgName(2) ||
         exp.GetArgName(2) == exp.GetArgName(1))) {
      // S_AAB -> 0
      exp = 0.0;
      // Lemma 2.2-2
      OutputStep(*m_pConjecture, "Lemma 2 (equal)", eps_geometric_simplification);
      return true;
    }
    if (Collinear(exp.GetArgName(0), exp.GetArgName(1), exp.GetArgName(2))) {
      // S_ABC -> 0
      exp = 0.0;
      // Lemma 2.2-2
      OutputStep(*m_pConjecture, "Lemma 2 (collinearity)",
                 eps_geometric_simplification);
      return true;
    }
    break;

  case ep_p3:
    if (exp.GetArgName(0) == exp.GetArgName(1) ||
        exp.GetArgName(1) == exp.GetArgName(2)) {
      // P_AAB -> 0
      // P_BAA -> 0
      exp = 0.0;
      OutputStep(*m_pConjecture, "Lemma 17", eps_geometric_simplification);
      return true;
    }
    break;

  case ep_s4:
  case ep_p4:
    break;

  default:
    break;
  }

  return false;
}

// ----------------------------------------------------------------------------

bool CAreaMethod::ApplyOneGeometricRule(CGCLCProverExpression &exp,
                                        GCLCgeometric_rule rule) {
  switch (rule) {

  //	case er_Orient:
  //		if (Orient())
  //			return true;
  //		break;
  case er_Orient:
    break;

  default:
    break;
  }

  for (unsigned i = 0; i < arity(exp.GetType()); i++)
    if (ApplyOneGeometricRule(exp.GetArg(i), rule))
      return true;

  return false;
}

// ----------------------------------------------------------------------------

bool CAreaMethod::EliminatePoint(GCLCpoint_type ptype, const std::string &sPoint,
                                 std::list<CGCLCProverCommand>::iterator pCommand,
                                 CGCLCProverExpression &exp) {
  bool r;

  switch (exp.GetType()) {
  case ep_point:
  case ep_number:
  case ep_segment:
    return false;

  case ep_equality:
  case ep_sum:
  case ep_mult:
  case ep_ratio:
    if (EliminatePoint(ptype, sPoint, pCommand, exp.GetArg(0)))
      return true;
    if (EliminatePoint(ptype, sPoint, pCommand, exp.GetArg(1)))
      return true;
    return false;

  case ep_segment_ratio:
  case ep_s3:
  case ep_p3:
  case ep_s4:
  case ep_p4:

    r = false;
    if (ptype == e_IntersectionPoint)
      r = EliminateIntersectionPoint(sPoint, pCommand, exp);

    if (ptype == e_PratioPoint)
      r = EliminatePratioPoint(sPoint, pCommand, exp);

    if (ptype == e_TratioPoint)
      r = EliminateTratioPoint(sPoint, pCommand, exp);

    if (ptype == e_FootPoint)
      r = EliminateFootPoint(sPoint, pCommand, exp);

    return r;

  default:
    return false;
  }
}

// ----------------------------------------------------------------------------

bool CAreaMethod::EliminateIntersectionPoint(
    const std::string &sPoint, std::list<CGCLCProverCommand>::iterator pCommand,
    CGCLCProverExpression &exp) {
  switch (exp.GetType()) {
  case ep_segment_ratio:

    if (exp.GetArgName(1) == sPoint && exp.GetArgName(3) == sPoint &&
        ((exp.GetArgName(0) == pCommand->arg[1] &&
          exp.GetArgName(2) == pCommand->arg[2]) ||
         (exp.GetArgName(0) == pCommand->arg[2] &&
          exp.GetArgName(2) == pCommand->arg[1]))) {
      CGCLCProverExpression n1 = CGCLCProverExpression::s3(
          exp.GetArgName(0), pCommand->arg[3], pCommand->arg[4]);
      CGCLCProverExpression n2 = CGCLCProverExpression::s3(
          exp.GetArgName(2), pCommand->arg[3], pCommand->arg[4]);
      exp = (n1 / n2);

      // Lemma 2.4
      std::string sExplanation = "Lemma 8 (point $" + sPoint + "$ eliminated)";

      OutputStep(*m_pConjecture, sExplanation, eps_geometric_elimination);
      return true;
    }

    if (exp.GetArgName(1) == sPoint && exp.GetArgName(3) == sPoint &&
        ((exp.GetArgName(0) == pCommand->arg[3] &&
          exp.GetArgName(2) == pCommand->arg[4]) ||
         (exp.GetArgName(0) == pCommand->arg[4] &&
          exp.GetArgName(2) == pCommand->arg[3]))) {
      CGCLCProverExpression n1 = CGCLCProverExpression::s3(
          exp.GetArgName(0), pCommand->arg[1], pCommand->arg[2]);
      CGCLCProverExpression n2 = CGCLCProverExpression::s3(
          exp.GetArgName(2), pCommand->arg[1], pCommand->arg[2]);
      exp = (n1 / n2);

      std::string sExplanation = "Lemma 8 (point $" + sPoint + "$ eliminated)";
      OutputStep(*m_pConjecture, sExplanation, eps_geometric_elimination);
      return true;
    }

    if (exp.GetArgName(1) == sPoint) {
      // see p13
      // exp == AY/CD
      // should be replaced by S_AUV/S_CUDV or by S_APQ/S_CPDQ
      CGCLCProverExpression n1, n2;

      std::string sCond;
      eGCLC_conjecture_status r =
          ProveCollinear(exp.GetArgName(0), pCommand->arg[1], pCommand->arg[2]);

      // if
      // (Collinear(exp.GetArgName(0),pCommand->arg[1],pCommand->arg[2]))
      if (r == e_proved) {
        // S_APQ/S_CPDQ
        n1 = CGCLCProverExpression::s3(exp.GetArgName(0), pCommand->arg[3],
                                       pCommand->arg[4]);
        n2 = CGCLCProverExpression::s4(exp.GetArgName(2), pCommand->arg[3],
                                       exp.GetArgName(3), pCommand->arg[4]);
        sCond = ", second case --- points $" + exp.GetArgName(0) + "$, $" +
                pCommand->arg[1] + "$, and $" + pCommand->arg[2] +
                "$ are collinear";
      } else if (r == e_disproved) {
        // S_AUV/S_CUDV
        n1 = CGCLCProverExpression::s3(exp.GetArgName(0), pCommand->arg[1],
                                       pCommand->arg[2]);
        n2 = CGCLCProverExpression::s4(exp.GetArgName(2), pCommand->arg[1],
                                       exp.GetArgName(3), pCommand->arg[2]);
        sCond = ", first case --- points $" + exp.GetArgName(0) + "$, $" +
                pCommand->arg[1] + "$, and $" + pCommand->arg[2] +
                "$ are not collinear";
      } else // (r == e_unknown)
      {
        CGCLCProverExpression Left(ep_s3, exp.GetArgName(0), pCommand->arg[1],
                                   pCommand->arg[2]);
        CGCLCProverExpression Right = 0.0;
        // if(!AddNDG(ep_inequality, pLeft, pRight,"(made for Lemma 4.8)"))
        if (!AddNDG(ep_inequality, Left, Right, "(made for Lemma 37)"))
          return false;

        n1 = CGCLCProverExpression::s3(exp.GetArgName(0), pCommand->arg[1],
                                       pCommand->arg[2]);
        n2 = CGCLCProverExpression::s4(exp.GetArgName(2), pCommand->arg[1],
                                       exp.GetArgName(3), pCommand->arg[2]);
        sCond = ", first case --- assuming points $" + exp.GetArgName(0) +
                "$, $" + pCommand->arg[1] + "$, and $" + pCommand->arg[2] +
                "$ are not collinear";
      }
      exp = (n1 / n2);

      // Lemma 4.8
      std::string sExplanation =
          (std::string) "Lemma 37 " + sCond + " (point $" + sPoint + "$ eliminated)";
      OutputStep(*m_pConjecture, sExplanation, eps_geometric_elimination);
      return true;
    }

    if (exp.GetArgName(3) == sPoint) {
      // see p13
      // exp == CD/AY
      // should be replaced by S_CUDV/S_AUV or by S_CPDQ/S_APQ
      CGCLCProverExpression n1, n2;

      // char sCond[COND_LENGHT];
      std::string sCond;

      eGCLC_conjecture_status r =
          ProveCollinear(exp.GetArgName(2), pCommand->arg[1], pCommand->arg[2]);

      if (r == e_proved) {
        n1 = CGCLCProverExpression::s3(exp.GetArgName(2), pCommand->arg[3],
                                       pCommand->arg[4]);
        n2 = CGCLCProverExpression::s4(exp.GetArgName(0), pCommand->arg[3],
                                       exp.GetArgName(1), pCommand->arg[4]);
        sCond = ", second case --- points $" + exp.GetArgName(2) + "$, $" +
                pCommand->arg[1] + "$, and $" + pCommand->arg[2] +
                "$ are collinear";
      } else if (r == e_disproved) {
        n1 = CGCLCProverExpression::s3(exp.GetArgName(2), pCommand->arg[1],
                                       pCommand->arg[2]);
        n2 = CGCLCProverExpression::s4(exp.GetArgName(0), pCommand->arg[1],
                                       exp.GetArgName(1), pCommand->arg[2]);
        sCond = ", first case --- points $" + exp.GetArgName(2) + "$, $" +
                pCommand->arg[1] + "$, and $" + pCommand->arg[2] +
                "$ are not collinear";
      } else // (r == e_unknown)
      {
        CGCLCProverExpression Left(ep_s3, exp.GetArgName(0), pCommand->arg[1],
                                   pCommand->arg[2]);
        CGCLCProverExpression Right = 0.0;
        // if(!AddNDG(ep_inequality, pLeft, pRight,"(made for Lemma 4.8)"))
        if (!AddNDG(ep_inequality, Left, Right, "(made for Lemma 37)"))
          return false;

        n1 = CGCLCProverExpression::s3(exp.GetArgName(2), pCommand->arg[1],
                                       pCommand->arg[2]);
        n2 = CGCLCProverExpression::s4(exp.GetArgName(0), pCommand->arg[1],
                                       exp.GetArgName(1), pCommand->arg[2]);
        sCond = ", first case --- assuming points $" + exp.GetArgName(2) +
                "$, $" + pCommand->arg[1] + "$, and $" + pCommand->arg[2] +
                "$ are not collinear";
      }
      exp = (n2 / n1);

      // Lemma 4.8
      std::string sExplanation = (std::string) "Lemma 37 (reciprocial " + sCond +
                            ") (point $" + sPoint + "$ eliminated)";

      OutputStep(*m_pConjecture, sExplanation, eps_geometric_elimination);
      return true;
    }

    return false;

  case ep_s3:
  case ep_p3:
  case ep_s4:
  case ep_p4:
    if (((exp.GetType() == ep_s3) && (exp.GetArgName(2) == sPoint)) ||
        ((exp.GetType() == ep_s4) && (exp.GetArgName(3) == sPoint)) ||
        ((exp.GetType() == ep_p3) && (exp.GetArgName(2) == sPoint)) ||
        ((exp.GetType() == ep_p4) && (exp.GetArgName(3) == sPoint))) {
      CGCLCProverExpression n_down =
          CGCLCProverExpression::s4(pCommand->arg[1], pCommand->arg[3],
                                    pCommand->arg[2], pCommand->arg[4]);
      CGCLCProverExpression n_up_left1 = CGCLCProverExpression::s3(
          pCommand->arg[1], pCommand->arg[3], pCommand->arg[4]);
      CGCLCProverExpression n_up_left2(exp);

      if ((exp.GetType() == ep_s3) || (exp.GetType() == ep_p3))
        n_up_left2.SetArgName(2, pCommand->arg[2]);
      else
        n_up_left2.SetArgName(3, pCommand->arg[2]);

      CGCLCProverExpression n_up_right1(-1);

      CGCLCProverExpression n_up_right3 = CGCLCProverExpression::s3(
          pCommand->arg[2], pCommand->arg[3], pCommand->arg[4]);

      CGCLCProverExpression n_up_right4 = exp;
      if ((exp.GetType() == ep_s3) || (exp.GetType() == ep_p3))
        n_up_right4.SetArgName(2, pCommand->arg[1]);
      else
        n_up_right4.SetArgName(3, pCommand->arg[1]);

      CGCLCProverExpression n_up = (n_up_left1 * n_up_left2) +
                                   (n_up_right1 * (n_up_right3 * n_up_right4));
      exp = (n_up / n_down);

      // Lemma 4.2
      std::string sExplanation = "Lemma 30 (point $" + sPoint + "$ eliminated)";
      OutputStep(*m_pConjecture, sExplanation, eps_geometric_elimination);
      return true;
    }

    if ((exp.GetType() == ep_p3) && (exp.GetArgName(1) == sPoint)) {
      CGCLCProverExpression r1 =
          CGCLCProverExpression::sratio(pCommand->arg[1], pCommand->arg[0],
                                        pCommand->arg[1], pCommand->arg[2]);
      CGCLCProverExpression r2 =
          CGCLCProverExpression::sratio(pCommand->arg[0], pCommand->arg[2],
                                        pCommand->arg[1], pCommand->arg[2]);
      CGCLCProverExpression r3 =
          CGCLCProverExpression::sratio(pCommand->arg[1], pCommand->arg[0],
                                        pCommand->arg[1], pCommand->arg[2]);
      CGCLCProverExpression r4 =
          CGCLCProverExpression::sratio(pCommand->arg[0], pCommand->arg[2],
                                        pCommand->arg[1], pCommand->arg[2]);
      CGCLCProverExpression p1 = CGCLCProverExpression::p3(
          pCommand->arg[1], pCommand->arg[2], pCommand->arg[1]);

      // CGCLCProverExpression Y(ep_point, pCommand->arg[0]);
      CGCLCProverExpression U(ep_point, pCommand->arg[1]);
      CGCLCProverExpression V(ep_point, pCommand->arg[2]);

      CGCLCProverExpression GU(exp);
      GU.SetArgName(1, U.GetName());

      CGCLCProverExpression GV(exp);
      GV.SetArgName(1, V.GetName());

      CGCLCProverExpression n(-1);
      exp = (((r1 * GV) + (r2 * GU)) + (n * ((r3 * r4) * p1)));

      // Lemma 4.3 (II)
      std::string sExplanation = "Lemma 32 (point $" + sPoint + "$ eliminated)";
      OutputStep(*m_pConjecture, sExplanation, eps_geometric_elimination);
      return true;
    }

    return false;

  default:
    break;
  }

  return false;
}

// ----------------------------------------------------------------------------

bool CAreaMethod::EliminatePratioPoint(
    const std::string &sPoint, std::list<CGCLCProverCommand>::iterator pCommand,
    CGCLCProverExpression &exp) {
  switch (exp.GetType()) {
  case ep_segment_ratio:

    if (exp.GetArgName(1) == sPoint) {
      // see p14
      // exp == AY/CD
      // should be replaced by AR/PQ+r/CD/PQ or by S_APRQ/S_CPDQ
      CGCLCProverExpression n1, n1a, n1b, n2;

      std::string sCond;
      eGCLC_conjecture_status r =
          ProveCollinear(exp.GetArgName(0), pCommand->arg[0], pCommand->arg[1]);

      // if(Collinear(exp.GetArgName(0),pCommand->arg[0],pCommand->arg[1]))
      if (r == e_proved) {
        n1a = CGCLCProverExpression::sratio(exp.GetArgName(0), pCommand->arg[1],
                                            pCommand->arg[2], pCommand->arg[3]);
        double r;
        if (convert(pCommand->arg[4], r))
          n1b = r;
        else
          n1b = CGCLCProverExpression(ep_constant, pCommand->arg[4]);

        n1 = n1a + n1b;
        n2 = CGCLCProverExpression::sratio(exp.GetArgName(2), exp.GetArgName(3),
                                           pCommand->arg[2], pCommand->arg[3]);
        sCond = ", first case --- points $" + exp.GetArgName(0) + "$, $" +
                pCommand->arg[0] + "$, and $" + pCommand->arg[1] +
                "$ are collinear";
      } else if (r == e_disproved) {
        n1 = CGCLCProverExpression::s4(exp.GetArgName(0), pCommand->arg[2],
                                       pCommand->arg[1], pCommand->arg[3]);
        n2 = CGCLCProverExpression::s4(exp.GetArgName(2), pCommand->arg[2],
                                       exp.GetArgName(3), pCommand->arg[3]);
        sCond = ", seconds case --- points $" + exp.GetArgName(0) + "$, $" +
                pCommand->arg[0] + "$, and $" + pCommand->arg[1] +
                "$ are not collinear";
      } else // (r == e_unknown)
      {
        CGCLCProverExpression Left = CGCLCProverExpression::s3(
            exp.GetArgName(0), pCommand->arg[0], pCommand->arg[1]);
        CGCLCProverExpression Right = 0.0;
        //	if(!AddNDG(ep_inequality, pLeft, pRight,"(made for Lemma
        // 4.10)"))
        if (!AddNDG(ep_inequality, Left, Right, "(made for Lemma 39)"))
          return false;

        n1 = CGCLCProverExpression::s4(exp.GetArgName(0), pCommand->arg[2],
                                       pCommand->arg[1], pCommand->arg[3]);
        n2 = CGCLCProverExpression::s4(exp.GetArgName(2), pCommand->arg[2],
                                       exp.GetArgName(3), pCommand->arg[3]);
        sCond = ", seconds case --- assuming points $" + exp.GetArgName(0) +
                "$, $" + pCommand->arg[0] + "$, and $" + pCommand->arg[1] +
                "$ are not collinear";
      }
      exp = (n1 / n2);

      // Lemma 4.10
      std::string sExplanation = "Lemma 39 (point $" + sPoint + "$ eliminated)";
      OutputStep(*m_pConjecture, sExplanation, eps_geometric_elimination);
      return true;
    }

    if (exp.GetArgName(3) == sPoint) {
      // see p13
      // exp == CD/AY
      // similar as above
      CGCLCProverExpression n1, n1a, n1b, n2;

      eGCLC_conjecture_status r =
          ProveCollinear(exp.GetArgName(2), pCommand->arg[0], pCommand->arg[1]);

      // if(Collinear(exp.GetArgName(2),pCommand->arg[0],pCommand->arg[1]))
      if (r == e_proved) {
        n1a = CGCLCProverExpression::sratio(exp.GetArgName(2), pCommand->arg[1],
                                            pCommand->arg[2], pCommand->arg[3]);

        double r;
        if (convert(pCommand->arg[4], r)) {
          n1b = r;
        } else
          n1b = CGCLCProverExpression(ep_constant, pCommand->arg[4]);

        n1 = (n1a + n1b);
        n2 = CGCLCProverExpression::sratio(exp.GetArgName(0), exp.GetArgName(1),
                                           pCommand->arg[2], pCommand->arg[3]);
      } else if (r == e_disproved) {
        n1 = CGCLCProverExpression::s4(exp.GetArgName(2), pCommand->arg[2],
                                       pCommand->arg[1], pCommand->arg[3]);
        n2 = CGCLCProverExpression::s4(exp.GetArgName(0), pCommand->arg[2],
                                       exp.GetArgName(1), pCommand->arg[3]);
      } else // (r == e_unknown)
      {
        CGCLCProverExpression Left = CGCLCProverExpression::s3(
            exp.GetArgName(2), pCommand->arg[0], pCommand->arg[1]);
        CGCLCProverExpression Right = 0.0;
        // if(!AddNDG(ep_inequality, pLeft, pRight,"(made for Lemma 4.10)"))
        if (!AddNDG(ep_inequality, Left, Right, "(made for Lemma 39)"))
          return false;

        n1 = CGCLCProverExpression::s4(exp.GetArgName(2), pCommand->arg[2],
                                       pCommand->arg[1], pCommand->arg[3]);
        n2 = CGCLCProverExpression::s4(exp.GetArgName(0), pCommand->arg[2],
                                       exp.GetArgName(1), pCommand->arg[3]);
      }
      exp = (n2 / n1);

      // Lemma 4.10 (reciprocial)
      std::string sExplanation =
          "Lemma 39 (reciprocial) (point $" + sPoint + "$ eliminated)";
      OutputStep(*m_pConjecture, sExplanation, eps_geometric_elimination);
      return true;
    }

    return false;

  case ep_s3:
  case ep_p3:
  case ep_s4:
  case ep_p4:
    if (((exp.GetType() == ep_s3) && (exp.GetArgName(2) == sPoint)) ||
        ((exp.GetType() == ep_p3) && (exp.GetArgName(2) == sPoint)) ||
        ((exp.GetType() == ep_s4) && (exp.GetArgName(3) == sPoint)) ||
        ((exp.GetType() == ep_p4) && (exp.GetArgName(3) == sPoint))) {

      // CGCLCProverExpression Y(ep_point, pCommand->arg[0]);
      CGCLCProverExpression W(ep_point, pCommand->arg[1]);
      CGCLCProverExpression U(ep_point, pCommand->arg[2]);
      CGCLCProverExpression V(ep_point, pCommand->arg[3]);

      CGCLCProverExpression GW(exp);
      CGCLCProverExpression GV(exp);
      CGCLCProverExpression GU(exp);
      if ((exp.GetType() == ep_s3) || (exp.GetType() == ep_p3))
        GW.SetArgName(2, W.GetName());
      if ((exp.GetType() == ep_s4) || (exp.GetType() == ep_p4))
        GW.SetArgName(3, W.GetName());

      if ((exp.GetType() == ep_s3) || (exp.GetType() == ep_p3))
        GV.SetArgName(2, V.GetName());
      if ((exp.GetType() == ep_s4) || (exp.GetType() == ep_p4))
        GV.SetArgName(3, V.GetName());

      if ((exp.GetType() == ep_s3) || (exp.GetType() == ep_p3))
        GU.SetArgName(2, U.GetName());
      if ((exp.GetType() == ep_s4) || (exp.GetType() == ep_p4))
        GU.SetArgName(3, U.GetName());

      CGCLCProverExpression nR;
      double dR;
      if (convert(pCommand->arg[4], dR))
        nR = dR;
      else
        nR = CGCLCProverExpression(ep_constant, pCommand->arg[4]);

      CGCLCProverExpression n(-1);
      exp = (GW + (nR * (GV + (n * GU))));
      // Lemma 4.1
      std::string sExplanation = "Lemma 29 (point $" + sPoint + "$ eliminated)";
      OutputStep(*m_pConjecture, sExplanation, eps_geometric_elimination);
      return true;
    }

    if ((exp.GetType() == ep_p3) && (exp.GetArgName(1) == sPoint)) {
      CGCLCProverExpression p1 = CGCLCProverExpression::p3(
          exp.GetArgName(0), pCommand->arg[1], exp.GetArgName(2));
      CGCLCProverExpression p2 = CGCLCProverExpression::p3(
          exp.GetArgName(0), pCommand->arg[3], exp.GetArgName(2));
      CGCLCProverExpression p3 = CGCLCProverExpression::p3(
          exp.GetArgName(0), pCommand->arg[2], exp.GetArgName(2));
      CGCLCProverExpression p4 = CGCLCProverExpression::p3(
          pCommand->arg[1], pCommand->arg[2], pCommand->arg[3]);
      CGCLCProverExpression p5 = CGCLCProverExpression::p3(
          pCommand->arg[2], pCommand->arg[3], pCommand->arg[2]);

      CGCLCProverExpression n1;
      double dR;
      if (convert(pCommand->arg[4], dR)) {
        n1 = dR;
      } else
        n1 = CGCLCProverExpression(ep_constant, pCommand->arg[4]);

      CGCLCProverExpression nNegOne(-1);
      CGCLCProverExpression nTwo =
          2; // fix ("2*") added 25.11.2008. This was a bug as
             // in Chou's book
      CGCLCProverExpression n5 = ((p2 + (nNegOne * p3)) + (nTwo * p4));
      CGCLCProverExpression nR(-dR * (1 - dR));
      exp = ((p1 + (n1 * n5)) + (nR * p5));

      // Lemma 4.4
      std::string sExplanation = "Lemma 33 (point $" + sPoint + "$ eliminated)";
      OutputStep(*m_pConjecture, sExplanation, eps_geometric_elimination);
      return true;
    }

    return false;

  default:
    break;
  }

  return false;
}

// ----------------------------------------------------------------------------

bool CAreaMethod::EliminateFootPoint(
    const std::string &sPoint, std::list<CGCLCProverCommand>::iterator pCommand,
    CGCLCProverExpression &exp) {
  switch (exp.GetType()) {
  case ep_segment_ratio:

    if (exp.GetArgName(1) == sPoint) {
      CGCLCProverExpression n1, n2;

      // char sCond[COND_LENGHT];
      std::string sCond;

      eGCLC_conjecture_status r =
          ProveCollinear(exp.GetArgName(0), pCommand->arg[2], pCommand->arg[3]);

      if (r == e_proved) {
        n1 = CGCLCProverExpression::p4(pCommand->arg[1], exp.GetArgName(2),
                                       exp.GetArgName(0), exp.GetArgName(3));
        n2 = CGCLCProverExpression::p3(pCommand->arg[2], pCommand->arg[3],
                                       pCommand->arg[2]);
        sCond = ", first case --- points $" + exp.GetArgName(0) + "$, $" +
                pCommand->arg[2] + "$, and $" + pCommand->arg[3] +
                "$ are collinear";
      } else if (r == e_disproved) {
        n1 = CGCLCProverExpression::s3(exp.GetArgName(0), pCommand->arg[2],
                                       pCommand->arg[3]);
        n2 = CGCLCProverExpression::s4(exp.GetArgName(2), pCommand->arg[2],
                                       exp.GetArgName(3), pCommand->arg[3]);
        sCond = ", seconds case --- points $" + exp.GetArgName(0) + "$, $" +
                pCommand->arg[2] + "$, and $" + pCommand->arg[3] +
                "$ are not collinear";
      } else // (r == e_unknown)
      {
        CGCLCProverExpression Left = CGCLCProverExpression::s3(
            exp.GetArgName(0), pCommand->arg[2], pCommand->arg[3]);
        CGCLCProverExpression Right = 0.0;
        // if(!AddNDG(ep_inequality, pLeft, pRight,"(made for Lemma 4.9)"))
        if (!AddNDG(ep_inequality, Left, Right, "(made for Lemma 38)"))
          return false;

        n1 = CGCLCProverExpression::s3(exp.GetArgName(0), pCommand->arg[2],
                                       pCommand->arg[3]);
        n2 = CGCLCProverExpression::s4(exp.GetArgName(2), pCommand->arg[2],
                                       exp.GetArgName(3), pCommand->arg[3]);
        sCond = ", seconds case --- assuming points $" + exp.GetArgName(0) +
                "$, $" + pCommand->arg[2] + "$, and $" + pCommand->arg[3] +
                "$ are not collinear";
      }

      exp = (n1 / n2);

      // Lemma 4.9
      std::string sExplanation =
          (std::string) "Lemma 38 " + sCond + " (point $" + sPoint + "$ eliminated)";
      OutputStep(*m_pConjecture, sExplanation, eps_geometric_elimination);
      return true;
    }

    return false;

  case ep_s3:
  case ep_p3:
  case ep_s4:
  case ep_p4:

    if (((exp.GetType() == ep_s3) && (exp.GetArgName(2) == sPoint)) ||
        ((exp.GetType() == ep_p3) && (exp.GetArgName(2) == sPoint)) ||
        ((exp.GetType() == ep_s4) && (exp.GetArgName(3) == sPoint)) ||
        ((exp.GetType() == ep_p4) && (exp.GetArgName(3) == sPoint))) {

      CGCLCProverExpression p1 = CGCLCProverExpression::p3(
          pCommand->arg[1], pCommand->arg[2], pCommand->arg[3]);
      CGCLCProverExpression p2 = CGCLCProverExpression::p3(
          pCommand->arg[1], pCommand->arg[3], pCommand->arg[2]);

      // CGCLCProverExpression Y(ep_point, pCommand->arg[0]);
      CGCLCProverExpression U(ep_point, pCommand->arg[2]);
      CGCLCProverExpression V(ep_point, pCommand->arg[3]);

      CGCLCProverExpression GU(exp);
      CGCLCProverExpression GV(exp);
      if ((exp.GetType() == ep_s3) || (exp.GetType() == ep_p3))
        GV.SetArgName(2, V.GetName());
      if ((exp.GetType() == ep_s4) || (exp.GetType() == ep_p4))
        GV.SetArgName(3, V.GetName());

      if ((exp.GetType() == ep_s3) || (exp.GetType() == ep_p3))
        GU.SetArgName(2, U.GetName());
      if ((exp.GetType() == ep_s4) || (exp.GetType() == ep_p4))
        GU.SetArgName(3, U.GetName());

      CGCLCProverExpression p3 = CGCLCProverExpression::p3(
          pCommand->arg[2], pCommand->arg[3], pCommand->arg[2]);

      exp = (((p1 * GV) + (p2 * GU)) / p3);

      // Lemma 4.3
      std::string sExplanation = "Lemma 31 (point $" + sPoint + "$ eliminated)";
      OutputStep(*m_pConjecture, sExplanation, eps_geometric_elimination);
      return true;
    }

    if ((exp.GetType() == ep_p3) && (exp.GetArgName(1) == sPoint)) {
      CGCLCProverExpression r1(ep_segment_ratio, pCommand->arg[2],
                               pCommand->arg[0], pCommand->arg[2],
                               pCommand->arg[3]);
      CGCLCProverExpression r2(ep_segment_ratio, pCommand->arg[0],
                               pCommand->arg[3], pCommand->arg[2],
                               pCommand->arg[3]);
      CGCLCProverExpression r3(ep_segment_ratio, pCommand->arg[2],
                               pCommand->arg[0], pCommand->arg[2],
                               pCommand->arg[3]);
      CGCLCProverExpression r4(ep_segment_ratio, pCommand->arg[0],
                               pCommand->arg[3], pCommand->arg[2],
                               pCommand->arg[3]);
      CGCLCProverExpression p1(ep_p3, pCommand->arg[2], pCommand->arg[3],
                               pCommand->arg[2]);

      CGCLCProverExpression GU(exp);
      CGCLCProverExpression GV(exp);
      // CGCLCProverExpression Y(ep_point, pCommand->arg[0]);
      CGCLCProverExpression U(ep_point, pCommand->arg[2]);
      CGCLCProverExpression V(ep_point, pCommand->arg[3]);

      GV.SetArgName(1, V.GetName());
      GU.SetArgName(1, U.GetName());

      CGCLCProverExpression nNegOne = -1;
      exp = (((r1 * GV) + (r2 * GU)) + (nNegOne * ((r3 * r4) * p1)));

      // Lemma 4.3
      std::string sExplanation = "Lemma 32 (point $" + sPoint + "$ eliminated)";
      OutputStep(*m_pConjecture, sExplanation, eps_geometric_elimination);
      return true;
    }

    return false;

  default:
    break;
  }

  return false;
}

// ----------------------------------------------------------------------------

bool CAreaMethod::EliminateTratioPoint(
    const std::string &sPoint, std::list<CGCLCProverCommand>::iterator pCommand,
    CGCLCProverExpression &exp) {
  switch (exp.GetType()) {
  case ep_segment_ratio:

    if (exp.GetArgName(1) == sPoint) {
      // exp == AY/CD
      CGCLCProverExpression n1, n2;

      // char sCond[COND_LENGHT];
      std::string sCond;
      eGCLC_conjecture_status r =
          ProveCollinear(exp.GetArgName(0), pCommand->arg[0], pCommand->arg[1]);

      if (r == e_proved) {
        CGCLCProverExpression s1(ep_s3, exp.GetArgName(0), pCommand->arg[1],
                                 pCommand->arg[2]);
        CGCLCProverExpression p1(ep_p3, pCommand->arg[1], pCommand->arg[2],
                                 pCommand->arg[1]);

        double dR;
        convert(pCommand->arg[3], dR);

        CGCLCProverExpression up1 = (-dR / 4);
        n1 = (s1 + (up1 * p1));
        n2 = CGCLCProverExpression::s4(exp.GetArgName(2), pCommand->arg[1],
                                       exp.GetArgName(3), pCommand->arg[2]);
        sCond = ", second case --- points $" + exp.GetArgName(0) + "$, $" +
                pCommand->arg[0] + "$, and $" + pCommand->arg[1] +
                "$ are collinear";
      } else if (r == e_disproved) {
        n1 = CGCLCProverExpression::p3(exp.GetArgName(0), pCommand->arg[1],
                                       pCommand->arg[2]);
        n2 = CGCLCProverExpression::p4(exp.GetArgName(2), pCommand->arg[1],
                                       exp.GetArgName(3), pCommand->arg[2]);
        sCond = ", first case --- points $" + exp.GetArgName(0) + "$, $" +
                pCommand->arg[0] + "$, and $" + pCommand->arg[1] +
                "$ are not collinear";
      } else // (r == e_unknown)
      {
        CGCLCProverExpression Left(ep_s3, exp.GetArgName(0), pCommand->arg[1],
                                   pCommand->arg[2]);
        CGCLCProverExpression Right(0.0);
        if (!AddNDG(ep_inequality, Left, Right, "(made for Lemma 40)"))
          return false;

        n1 = CGCLCProverExpression::p3(exp.GetArgName(0), pCommand->arg[1],
                                       pCommand->arg[2]);
        n2 = CGCLCProverExpression::p4(exp.GetArgName(2), pCommand->arg[1],
                                       exp.GetArgName(3), pCommand->arg[2]);
        sCond = ", second case --- assuming points $" + exp.GetArgName(0) +
                "$, $" + pCommand->arg[0] + "$, and $" + pCommand->arg[1] +
                "$ are not collinear";
      }

      exp = (n1 / n2);

      // Lemma 4.11
      std::string sExplanation = "Lemma 40 (point $" + sPoint + "$ eliminated)";
      OutputStep(*m_pConjecture, sExplanation, eps_geometric_elimination);
      return true;
    }

    if (exp.GetArgName(3) == sPoint) {
      // exp == CD/AY
      CGCLCProverExpression n1, n2;
      eGCLC_conjecture_status r =
          ProveCollinear(exp.GetArgName(2), pCommand->arg[0], pCommand->arg[1]);

      if (r == e_proved) {
        CGCLCProverExpression s1(ep_s3, exp.GetArgName(2), pCommand->arg[1],
                                 pCommand->arg[2]);
        CGCLCProverExpression p1(ep_p3, pCommand->arg[1], pCommand->arg[2],
                                 pCommand->arg[1]);

        double dR;
        convert(pCommand->arg[3], dR);

        CGCLCProverExpression up1(-dR / 4);
        n1 = (s1 + (up1 * p1));
        n2 = CGCLCProverExpression::s4(exp.GetArgName(0), pCommand->arg[1],
                                       exp.GetArgName(1), pCommand->arg[2]);
      } else if (r == e_disproved) {
        n1 = CGCLCProverExpression::p3(exp.GetArgName(2), pCommand->arg[1],
                                       pCommand->arg[2]);
        n2 = CGCLCProverExpression::p4(exp.GetArgName(0), pCommand->arg[1],
                                       exp.GetArgName(1), pCommand->arg[2]);
      } else // (r == e_unknown)
      {
        CGCLCProverExpression Left(ep_s3, exp.GetArgName(0), pCommand->arg[1],
                                   pCommand->arg[2]);
        CGCLCProverExpression Right(0.0);
        // if(!AddNDG(ep_inequality, pLeft, pRight,"(made for Lemma 4.11)"))
        if (!AddNDG(ep_inequality, Left, Right, "(made for Lemma 40)"))
          return false;

        n1 = CGCLCProverExpression::p3(exp.GetArgName(2), pCommand->arg[1],
                                       pCommand->arg[2]);
        n2 = CGCLCProverExpression::p4(exp.GetArgName(0), pCommand->arg[1],
                                       exp.GetArgName(1), pCommand->arg[2]);
      }

      exp = (n2 / n1);

      // Lemma 4.11
      std::string sExplanation = "Lemma 40 (point $" + sPoint + "$ eliminated)";
      OutputStep(*m_pConjecture, sExplanation, eps_geometric_elimination);
      return true;
    }
    return false;

  case ep_s4:
    if (exp.GetArgName(3) == sPoint) {
      CGCLCProverExpression n1(ep_s3, exp.GetArgName(0), exp.GetArgName(1),
                               exp.GetArgName(2));
      CGCLCProverExpression n2(ep_s3, exp.GetArgName(0), exp.GetArgName(2),
                               exp.GetArgName(3));
      exp = (n1 + n2);

      Orient(sPoint, exp);
      // preparation step for Lemma 4.5;
      EliminateTratioPoint(sPoint, pCommand, exp);
      return true;
    }
    return false;

  case ep_p4:
    if (exp.GetArgName(3) == sPoint) {
      CGCLCProverExpression n1(ep_p3, exp.GetArgName(0), exp.GetArgName(1),
                               exp.GetArgName(3));
      CGCLCProverExpression n2(ep_p3, exp.GetArgName(2), exp.GetArgName(1),
                               exp.GetArgName(3));
      CGCLCProverExpression n3(-1);
      exp = (n1 + (n3 * n2));
      Orient(sPoint, exp);

      // preparation step for Lemma 4.6
      EliminateTratioPoint(sPoint, pCommand, exp);
      return true;
    }

    return false;

  case ep_p3:
    if (exp.GetArgName(2) == sPoint) {
      CGCLCProverExpression p(ep_p3, exp.GetArgName(0), exp.GetArgName(1),
                              pCommand->arg[1]);
      CGCLCProverExpression s(ep_s4, pCommand->arg[1], exp.GetArgName(0),
                              pCommand->arg[2], exp.GetArgName(1));
      double dR;
      convert(pCommand->arg[3], dR);

      CGCLCProverExpression y(-4 * dR);
      exp = (p + (y * s));

      // Lemma 4.6
      std::string sExplanation = "Lemma 35 (point $" + sPoint + "$ eliminated)";
      OutputStep(*m_pConjecture, sExplanation, eps_geometric_elimination);
      return true;
    }

    if (exp.GetArgName(1) == sPoint) {
      CGCLCProverExpression s1(ep_s3, exp.GetArgName(0), pCommand->arg[1],
                               pCommand->arg[2]);
      CGCLCProverExpression s2(ep_s3, exp.GetArgName(2), pCommand->arg[1],
                               pCommand->arg[2]);

      CGCLCProverExpression p1(ep_p3, exp.GetArgName(0), pCommand->arg[1],
                               exp.GetArgName(2));
      CGCLCProverExpression p2(ep_p3, pCommand->arg[1], pCommand->arg[2],
                               pCommand->arg[1]);

      double dR;
      convert(pCommand->arg[3], dR);

      CGCLCProverExpression n1(-4 * dR);
      CGCLCProverExpression n2(dR * dR);
      exp = ((p1 + (n2 * p2)) + (n1 * (s1 + s2)));

      // Lemma 4.7
      std::string sExplanation = "Lemma 36 (point $" + sPoint + "$ eliminated)";
      OutputStep(*m_pConjecture, sExplanation, eps_geometric_elimination);
      return true;
    }
    return false;

  case ep_s3:
    if ((exp.GetType() == ep_s3) && (exp.GetArgName(2) == sPoint)) {

      CGCLCProverExpression s(ep_s3, exp.GetArgName(0), exp.GetArgName(1),
                              pCommand->arg[1]);
      CGCLCProverExpression p(ep_p4, pCommand->arg[1], exp.GetArgName(0),
                              pCommand->arg[2], exp.GetArgName(1));
      double dR;
      convert(pCommand->arg[3], dR);

      CGCLCProverExpression y(-dR / 4);
      exp = (s + (y * p));

      // Lemma 4.5
      std::string sExplanation = "Lemma 34 (point $" + sPoint + "$ eliminated)";
      OutputStep(*m_pConjecture, sExplanation, eps_geometric_elimination);
      return true;
    }

    return false;

  default:
    break;
  }

  return false;
}

// ----------------------------------------------------------------------------

bool CAreaMethod::S4_to_S3(CGCLCProverExpression &exp) {
  bool s1, s2;
  // S_ABCD = S_ABC + S_ACD
  switch (exp.GetType()) {
  case ep_number:
  case ep_point:
  case ep_segment:
    break;

  case ep_equality:
  case ep_sum:
  case ep_ratio:
  case ep_mult:

    s1 = S4_to_S3(exp.GetArg(0));
    s2 = S4_to_S3(exp.GetArg(1));
    if (s1 || s2)
      return true;
    break;

  case ep_s3:
  case ep_segment_ratio:
  case ep_p3:
  case ep_p4:
    break;

  case ep_s4:
    exp = CGCLCProverExpression::s3(exp.GetArgName(0), exp.GetArgName(1),
                                    exp.GetArgName(2)) +
          CGCLCProverExpression::s3(exp.GetArgName(0), exp.GetArgName(2),
                                    exp.GetArgName(3));
    OutputStep(*m_pConjecture, "Definition 4", eps_geometric_simplification);
    return true;

  default:
    break;
  }

  return false;
}

// ----------------------------------------------------------------------------

bool CAreaMethod::P4_to_P3(CGCLCProverExpression &exp) {
  // P_ABCD = P_ABD - P_CBD
  CGCLCProverExpression nNegOne(-1);

  bool p1, p2;
  switch (exp.GetType()) {
  case ep_number:
  case ep_point:
  case ep_segment:
    break;

  case ep_equality:
  case ep_sum:
  case ep_ratio:
  case ep_mult:
    p1 = P4_to_P3(exp.GetArg(0));
    p2 = P4_to_P3(exp.GetArg(1));
    if (p1 || p2)
      return true;
    break;

  case ep_segment_ratio:
  case ep_s3:
  case ep_s4:
  case ep_p3:
    break;

  case ep_p4:;
    exp = CGCLCProverExpression::p3(exp.GetArgName(0), exp.GetArgName(1),
                                    exp.GetArgName(3)) +
          (nNegOne * CGCLCProverExpression::p3(exp.GetArgName(2),
                                               exp.GetArgName(1),
                                               exp.GetArgName(3)));

    OutputStep(*m_pConjecture, "Definition 6", eps_geometric_simplification);
    return true;

    break;

  default:
    break;
  }

  return false;
}

// ----------------------------------------------------------------------------

bool CAreaMethod::P3_to_segments(CGCLCProverExpression &exp) {
  // P_ABC = AB^2 + CB^2 - AC^2
  bool p1, p2;
  CGCLCProverExpression nNegOne(-1), nTwo(2);
  CGCLCProverExpression n1, n4, n7;
  switch (exp.GetType()) {
  case ep_number:
  case ep_point:
  case ep_segment:
    break;

  case ep_equality:
  case ep_sum:
  case ep_ratio:
  case ep_mult:
    p1 = P3_to_segments(exp.GetArg(0));
    p2 = P3_to_segments(exp.GetArg(1));
    if (p1 || p2)
      return true;
    break;

  case ep_segment_ratio:
  case ep_s3:
  case ep_s4:
  case ep_p4:
    break;

  case ep_p3:
    if (exp.GetArgName(0) == exp.GetArgName(2)) {
      n1 = CGCLCProverExpression::segment(exp.GetArgName(0), exp.GetArgName(1));
      exp = nTwo * (n1 * n1);
      OutputStep(*m_pConjecture, "Lemma 19", eps_geometric_simplification);
      return true;
    }

    n1 = CGCLCProverExpression::segment(exp.GetArgName(0), exp.GetArgName(1));
    n4 = CGCLCProverExpression::segment(exp.GetArgName(2), exp.GetArgName(1));
    n7 = CGCLCProverExpression::segment(exp.GetArgName(0), exp.GetArgName(2));

    exp = (((n1 * n1) + (n4 * n4)) + (nNegOne * (n7 * n7)));
    OutputStep(*m_pConjecture, "Definition 5", eps_geometric_simplification);
    return true;

  default:
    break;
  }

  return false;
}

// ----------------------------------------------------------------------------

bool CAreaMethod::H4points(CGCLCProverExpression &exp) {
  // S_ABC = S_ABD + S_ADC + S_DBC

  std::string A, B, C, D;
  if (exp.GetType() == ep_equality || exp.GetType() == ep_sum ||
      exp.GetType() == ep_ratio || exp.GetType() == ep_mult) {
    if (H4points(exp.GetArg(0)) || H4points(exp.GetArg(1)))
      return true;
  }

  if (exp.GetType() == ep_s3) {
    A = exp.GetArgName(0);
    B = exp.GetArgName(1);
    C = exp.GetArgName(2);
    for (std::list<CGCLCProverCommand>::iterator it = m_ProverCommands.begin();
         it != m_ProverCommands.end(); it++) {
      if ((it->type == p_point) && (it->arg[0] != A) && (it->arg[0] != B) &&
          (it->arg[0] != C)) {
        D = it->arg[0];
        if (ExistsS3similar(*m_pConjecture, A, B,
                            D) && // was ExistsS3 20.07.2008.
            ExistsS3similar(*m_pConjecture, A, D,
                            C) && // was ExistsS3 20.07.2008.
            ExistsS3similar(*m_pConjecture, D, B,
                            C) && // was ExistsS3 20.07.2008.
            A != D &&
            B != D && C != D) {
          CGCLCProverExpression n1(ep_s3, A, B, D);
          CGCLCProverExpression n2(ep_s3, A, D, C);
          CGCLCProverExpression n3(ep_s3, D, B, C);

          CGCLCProverExpression e3(n1 + (n2 + n3));
          CGCLCProverExpression copy_exp(exp);
          FindingCompany(*m_pConjecture, copy_exp);
          m_pConjecture->Replace(copy_exp, e3);

          // Lemma 2.2-3
          std::string sExplanation = "Lemma 4";
          OutputStep(*m_pConjecture, sExplanation, eps_geometric_simplification);
          return true;
        }
      }
    }
  }

  return false;
}

// ----------------------------------------------------------------------------

bool CAreaMethod::PointOccurs(const CGCLCProverExpression &exp,
                              const std::string &A) const {
  switch (exp.GetType()) {
  case ep_number:
    return false;

  case ep_point:
    if (exp.GetName() == A)
      return true;
    else
      return false;
  default:
    for (unsigned i = 0; i < arity(exp.GetType()); i++)
      if (PointOccurs(exp.GetArg(i), A))
        return true;
  }
  return false;
}

// ----------------------------------------------------------------------------

int CAreaMethod::PointOccurences(const std::string &A,
                                 const CGCLCProverExpression &exp) const {
  int s = 0;
  switch (exp.GetType()) {
  case ep_number:
    return 0;

  case ep_point:
    if (exp.GetName() == A)
      return 1;
    else
      return 0;
  default:
    for (unsigned i = 0; i < arity(exp.GetType()); i++)
      s += PointOccurences(A, exp.GetArg(i));
    return s;
  }
  return 0;
}

// ----------------------------------------------------------------------------

bool CAreaMethod::ExistsS3(const CGCLCProverExpression &exp, const std::string &A,
                           const std::string &B, const std::string &C) const {
  if (exp.ExistsAtomicExpression(CGCLCProverExpression::s3(A, B, C)))
    return true;
  return false;
}

// ----------------------------------------------------------------------------

bool CAreaMethod::ExistsS3similar(const CGCLCProverExpression &exp,
                                  const std::string &A, const std::string &B,
                                  const std::string &C) const {
  if (exp.ExistsAtomicExpression(CGCLCProverExpression::s3(A, C, B)))
    return true;
  if (exp.ExistsAtomicExpression(CGCLCProverExpression::s3(B, A, C)))
    return true;
  if (exp.ExistsAtomicExpression(CGCLCProverExpression::s3(B, C, A)))
    return true;
  if (exp.ExistsAtomicExpression(CGCLCProverExpression::s3(C, A, B)))
    return true;
  if (exp.ExistsAtomicExpression(CGCLCProverExpression::s3(C, B, A)))
    return true;
  return false;
}

// ----------------------------------------------------------------------------

bool CAreaMethod::ExistsP3similar(const CGCLCProverExpression &exp,
                                  const std::string &A, const std::string &B,
                                  const std::string &C) const {
  if (A != C)
    if (exp.ExistsAtomicExpression(CGCLCProverExpression::p3(C, B, A)))
      return true;
  if (A == C)
    if (exp.ExistsAtomicExpression(CGCLCProverExpression::p3(B, A, B)))
      return true;
  return false;
}

// ----------------------------------------------------------------------------

bool CAreaMethod::ExistsSegmentsimilar(const CGCLCProverExpression &exp,
                                       const std::string &A, const std::string &B) const {
  if (A != B) {
    if (exp.ExistsAtomicExpression(CGCLCProverExpression::segment(B, A)))
      return true;
  }
  return false;
}

// *************************************************************************************
// *************************************************************************************
// *************************************************************************************

// ----------------------------------------------------------------------------

bool CAreaMethod::GetConstant(const std::string &sConstant, double &r) const {
  for (std::list<CGCLCProverCommand>::const_iterator it = m_ProverCommands.begin();
       it != m_ProverCommands.end(); it++) {
    if (it->type == p_constant) {
      if (it->arg[0] == sConstant) {
        convert(it->arg[1], r);
        return true;
      }
    }

    if (it->type == p_pratio) {
      if (it->arg[4] == sConstant) {
        convert(it->arg[5], r);
        return true;
      }
    }
  }

  r = 0;
  return false;
}

// ----------------------------------------------------------------------------

bool CAreaMethod::ApplyAssumptions(CGCLCProverExpression &exp) {
  switch (exp.GetType()) {
  case ep_number:
  case ep_point:
    return false;

  case ep_s3:
  case ep_segment:
  case ep_segment_ratio:
  case ep_p3:
  case ep_p4:
  case ep_s4:

    for (std::list<CNDGC>::iterator it = m_NDGCs.begin(); it != m_NDGCs.end();
         it++) {
      if ((exp == it->m_Left) && (it->m_type == ep_equality) &&
          (it->m_Right.GetType() == ep_number)) {
        exp = it->m_Right.GetNumber();

        OutputStep(*m_pConjecture, "applying assumption", eps_algebraic);
        return true;
      }
    }
    return false;

  case ep_equality:
  case ep_sum:
  case ep_ratio:
  case ep_mult:
    if (ApplyAssumptions(exp.GetArg(0)))
      return true;
    if (ApplyAssumptions(exp.GetArg(1)))
      return true;
    break;

  default:
    break;
  }

  return false;
}

// ----------------------------------------------------------------------------

bool CAreaMethod::CancelMult(CGCLCProverExpression &exp) {
  CGCLCProverExpression Left;

  if (exp.GetArg(0).GetType() != ep_number)
    Left = exp.GetArg(0);
  else if (exp.GetArg(1).GetType() != ep_number)
    Left = exp.GetArg(1);
  else
    return false;

  CGCLCProverExpression *pLeft = &Left;
  while (pLeft->GetType() == ep_sum)
    pLeft = pLeft->GetArgP(0);
  int iNumOfMultiplicands = pLeft->CountTopLevelOperands(ep_mult);

  if (iNumOfMultiplicands == 1)
    return false;

  std::vector<CGCLCProverExpression *> aMultiplicants;
  aMultiplicants.reserve(iNumOfMultiplicands);

  pLeft->FillTopLevelOperands(ep_mult, aMultiplicants);

  for (CGCLCProverExpression *i : aMultiplicants) {
    if (exp.GetArg(0).AllSummandsHaveFactor(*i) &&
        exp.GetArg(1).AllSummandsHaveFactor(*i)) {
      if (i->GetType() != ep_number) {
        CGCLCProverExpression LemmaLeft(*i);
        CGCLCProverExpression LemmaRight(0.0);
        CGCLCProverExpression Lemma(ep_equality, LemmaLeft, LemmaRight);
        CGCLCProverExpression NDG1(LemmaLeft);
        CGCLCProverExpression NDG2(LemmaRight);

        // for explanation, in case it is not proved
        Lemma.SetType(ep_inequality);
        std::string sCond = Lemma.sPrintLaTeX();
        Lemma.SetType(ep_equality);
        eGCLC_conjecture_status r = ZeroByAssumption(LemmaLeft);

        if (r == e_unknown)
          r = ProveConjecture(Lemma);

        if (r == e_proved) {
          CGCLCProverExpression LHS(*i);
          CGCLCProverExpression zero(0.0);
          m_pConjecture->Replace(LHS, zero);
          OutputStep(*m_pConjecture, "by the above proved lemma.", eps_algebraic);
          return true;
        } else if (r == e_disproved) {
          CGCLCProverExpression Factor(*i);
          if (exp.GetArg(0).CancelationMult(Factor) &&
              exp.GetArg(1).CancelationMult(Factor)) {
            OutputStep(*m_pConjecture, "cancellation rule", eps_algebraic);
            return true;
          }
        } else // (r == e_unknown)
        {
          if (!AddNDG(ep_inequality, NDG1, NDG2, "(cancellation assumption)")) {
            return false;
          }
          CGCLCProverExpression Factor(*i);
          if (exp.GetArg(0).CancelationMult(Factor) &&
              exp.GetArg(1).CancelationMult(Factor)) {
            std::string sCond1 = "cancellation rule (assuming $" + sCond + "$)";
            OutputStep(*m_pConjecture, sCond1, eps_algebraic);
            return true;
          }
        }
      } else // if the common factor is number
      {
        if (i->GetNumber() != 0 && i->GetNumber() != 1) {
          CGCLCProverExpression Factor(*i);
          if (exp.GetArg(0).CancelationMult(Factor) &&
              exp.GetArg(1).CancelationMult(Factor)) {
            OutputStep(*m_pConjecture, "cancellation rule", eps_algebraic);

            return true;
          }
        }
      }
    }
  }

  return false;
}

// ----------------------------------------------------------------------------

eGCLC_conjecture_status
CAreaMethod::ZeroByAssumption(const CGCLCProverExpression &exp) const {
  for (std::list<CNDGC>::const_iterator it = m_NDGCs.begin(); it != m_NDGCs.end();
       it++) {
    bool bFound = false;

    if (exp.GetType() == ep_s3 && (it->m_Left.GetType() == ep_s3) &&
        exp.GetArgName(0) == it->m_Left.GetArgName(0) &&
        exp.GetArgName(1) == it->m_Left.GetArgName(2) &&
        exp.GetArgName(2) == it->m_Left.GetArgName(1) &&
        (it->m_Right.GetType() == ep_number) && (it->m_Right.GetNumber() == 0))
      bFound = true;
    else if (exp.GetType() == ep_s3 && (it->m_Left.GetType() == ep_s3) &&
             exp.GetArgName(0) == it->m_Left.GetArgName(1) &&
             exp.GetArgName(1) == it->m_Left.GetArgName(0) &&
             exp.GetArgName(2) == it->m_Left.GetArgName(2) &&
             (it->m_Right.GetType() == ep_number) &&
             (it->m_Right.GetNumber() == 0))
      bFound = true;
    else if (exp.GetType() == ep_s3 && (it->m_Left.GetType() == ep_s3) &&
             exp.GetArgName(0) == it->m_Left.GetArgName(1) &&
             exp.GetArgName(1) == it->m_Left.GetArgName(2) &&
             exp.GetArgName(2) == it->m_Left.GetArgName(0) &&
             (it->m_Right.GetType() == ep_number) &&
             (it->m_Right.GetNumber() == 0))
      bFound = true;
    else if (exp.GetType() == ep_s3 && (it->m_Left.GetType() == ep_s3) &&
             exp.GetArgName(0) == it->m_Left.GetArgName(2) &&
             exp.GetArgName(1) == it->m_Left.GetArgName(0) &&
             exp.GetArgName(2) == it->m_Left.GetArgName(1) &&
             (it->m_Right.GetType() == ep_number) &&
             (it->m_Right.GetNumber() == 0))
      bFound = true;
    else if (exp.GetType() == ep_s3 && (it->m_Left.GetType() == ep_s3) &&
             exp.GetArgName(0) == it->m_Left.GetArgName(2) &&
             exp.GetArgName(1) == it->m_Left.GetArgName(1) &&
             exp.GetArgName(2) == it->m_Left.GetArgName(0) &&
             (it->m_Right.GetType() == ep_number) &&
             (it->m_Right.GetNumber() == 0))
      bFound = true;
    else if (exp.GetType() == ep_p3 && (it->m_Left.GetType() == ep_segment) &&
             exp.GetArgName(0) == exp.GetArgName(2) &&
             (it->m_Left.GetArgName(0) == exp.GetArgName(0)) &&
             (it->m_Left.GetArgName(1) == exp.GetArgName(1)) &&
             (it->m_Right.GetType() == ep_number) &&
             (it->m_Right.GetNumber() == 0))
      bFound = true;
    else if ((it->m_Left.GetType() == ep_p3) && exp.GetType() == ep_segment &&
             (it->m_Left.GetArgName(0) == it->m_Left.GetArgName(2)) &&
             (it->m_Left.GetArgName(0) == exp.GetArgName(0)) &&
             (it->m_Left.GetArgName(1) == exp.GetArgName(1)) &&
             (it->m_Right.GetType() == ep_number) &&
             (it->m_Right.GetNumber() == 0))
      bFound = true;
    else if (exp == it->m_Left && (it->m_Right.GetType() == ep_number) &&
             (it->m_Right.GetNumber() == 0))
      bFound = true;

    if (bFound) {
      if (it->m_type == ep_equality)
        return e_proved;
      else if (it->m_type == ep_inequality)
        return e_disproved;
    }
  }
  return e_unknown;
}

// ----------------------------------------------------------------------------

double CAreaMethod::EvaluateExpression(const CGCLCProverExpression &exp) const {
  double x1, y1, x2, y2, x3, y3, x4, y4;
  double ab2, cb2, ac2, da2, cd2, r;
  double sign;

  switch (exp.GetType()) {
  case ep_number:
    return exp.GetNumber();

  case ep_point:
    return 0;

  case ep_constant:
    GetConstant(exp.GetName(), r);
    return r;

  case ep_segment:
    GetPointCoordinates(exp.GetArgName(0), x1, y1);
    GetPointCoordinates(exp.GetArgName(1), x2, y2);
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));

  case ep_s3:
    GetPointCoordinates(exp.GetArgName(0), x1, y1);
    GetPointCoordinates(exp.GetArgName(1), x2, y2);
    GetPointCoordinates(exp.GetArgName(2), x3, y3);
    return HeronArea(x1, y1, x2, y2, x3, y3);

  case ep_segment_ratio:
    GetPointCoordinates(exp.GetArgName(0), x1, y1);
    GetPointCoordinates(exp.GetArgName(1), x2, y2);
    GetPointCoordinates(exp.GetArgName(2), x3, y3);
    GetPointCoordinates(exp.GetArgName(3), x4, y4);

    if (fabs(x1 - x2) < EPSILON) {
      if ((y4 - y3) / (y2 - y1) > 0)
        sign = 1;
      else
        sign = -1;
    } else {
      if ((x4 - x3) / (x2 - x1) > 0)
        sign = 1;
      else
        sign = -1;
    }

    return sign * (sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) /
                   sqrt((x3 - x4) * (x3 - x4) + (y3 - y4) * (y3 - y4)));

  case ep_p3:
    GetPointCoordinates(exp.GetArgName(0), x1, y1);
    GetPointCoordinates(exp.GetArgName(1), x2, y2);
    GetPointCoordinates(exp.GetArgName(2), x3, y3);
    ab2 = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
    cb2 = (x3 - x2) * (x3 - x2) + (y3 - y2) * (y3 - y2);
    ac2 = (x1 - x3) * (x1 - x3) + (y1 - y3) * (y1 - y3);

    return ab2 + cb2 - ac2;

  case ep_p4:
    GetPointCoordinates(exp.GetArgName(0), x1, y1);
    GetPointCoordinates(exp.GetArgName(1), x2, y2);
    GetPointCoordinates(exp.GetArgName(2), x3, y3);
    GetPointCoordinates(exp.GetArgName(3), x4, y4);
    ab2 = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
    cb2 = (x3 - x2) * (x3 - x2) + (y3 - y2) * (y3 - y2);
    da2 = (x1 - x4) * (x1 - x4) + (y1 - y4) * (y1 - y4);
    cd2 = (x3 - x4) * (x3 - x4) + (y3 - y4) * (y3 - y4);
    return ab2 + cd2 - cb2 - da2;

  case ep_s4:
    GetPointCoordinates(exp.GetArgName(0), x1, y1);
    GetPointCoordinates(exp.GetArgName(1), x2, y2);
    GetPointCoordinates(exp.GetArgName(2), x3, y3);
    GetPointCoordinates(exp.GetArgName(3), x4, y4);
    // S_ABCD = S_ABC + S_ACD
    return HeronArea(x1, y1, x2, y2, x3, y3) +
           HeronArea(x1, y1, x3, y3, x4, y4);

  case ep_equality:
    return (fabs(EvaluateExpression(exp.GetArg(0)) -
                 EvaluateExpression(exp.GetArg(1))) < EPSILON);

  case ep_sum:
    return (EvaluateExpression(exp.GetArg(0)) +
            EvaluateExpression(exp.GetArg(1)));

  case ep_ratio:
    return (EvaluateExpression(exp.GetArg(0)) /
            EvaluateExpression(exp.GetArg(1)));

  case ep_mult:
    return (EvaluateExpression(exp.GetArg(0)) *
            EvaluateExpression(exp.GetArg(1)));

  default:
    break;
  }

  return 0;
}

// ----------------------------------------------------------------------------

void CAreaMethod::OutputStep(const CGCLCProverExpression &exp,
                             const std::string &explanation, eGCLC_proof_step type) {
  if (type == eps_geometric_elimination)
    m_iNumberOfEliminationSteps++;
  else if (type == eps_geometric_simplification)
    m_iNumberOfGeometricSteps++;
  else if (type == eps_algebraic)
    m_iNumberOfAlgebraicSteps++;

  // LaTeX output
  OutputLaTeXStep(exp, explanation, type);

  if (m_hXMLOutputProof.is_open())
    OutputXMLStep(exp, explanation, type);
}

// ----------------------------------------------------------------------------

void CAreaMethod::OutputLaTeXStep(const CGCLCProverExpression &exp,
                                  const std::string &explanation,
                                  eGCLC_proof_step type) {
  bool bPrint = false;
  // 0 - nothing
  // 1 - elimination steps plus grouped steps
  // 2 - elimination steps plus geometric plus grouped algebraic
  // 3 - plus statements of lemmas
  // 4 - plus elimination steps plus grouped steps in lemmas
  // 5 - plus geometric steps in lemmas
  // 6 - plus algebraic steps at level 0
  // 7 - plus algebraic steps in lemmas

  switch (type) {
  case eps_statement:
    if (m_iProofDepth == 0 || (m_iProofDepth > 0 && m_iProofLevel >= 3))
      bPrint = true;
    break;

  case eps_geometric_elimination:
    if ((m_iProofDepth == 0 && m_iProofLevel > 0) ||
        (m_iProofDepth > 0 && m_iProofLevel >= 4))
      bPrint = true;
    break;

  case eps_grouped_geometric:
    if ((m_iProofDepth == 0 && m_iProofLevel == 1) ||
        (m_iProofDepth > 0 && m_iProofLevel == 4))
      bPrint = true;
    break;

  case eps_geometric_simplification:
    if ((m_iProofDepth == 0 && m_iProofLevel > 1) ||
        (m_iProofDepth > 0 && m_iProofLevel >= 5))
      bPrint = true;
    break;

  case eps_grouped_algebraic:
    if ((m_iProofDepth == 0 && m_iProofLevel > 0 && m_iProofLevel < 6) ||
        (m_iProofDepth > 0 && m_iProofLevel >= 4 && m_iProofLevel < 7))
      bPrint = true;
    break;

  case eps_algebraic:
    if ((m_iProofDepth == 0 && m_iProofLevel >= 6) ||
        (m_iProofDepth > 0 && m_iProofLevel >= 7))
      bPrint = true;
    break;

  default:
    break;
  }

  if (bPrint) {
    double e1 = EvaluateExpression(exp.GetArg(0));
    double e2 = EvaluateExpression(exp.GetArg(1));

    std::string sValue;

    if (fabs((double)(e1 - (int)(e1 + EPSILON))) < EPSILON)
      sValue += i2s((int)(e1 + EPSILON)) + "=";
    else {
      if (e1 == 0.5)
        sValue += "\\frac{1}{2}=";
      else if (e1 == -0.5)
        sValue += "-\\frac{1}{2}=";
      else if (e1 == 0.25)
        sValue += "\\frac{1}{4}=";
      else if (e1 == -0.25)
        sValue += "-\\frac{1}{4}=";
      else if (e1 == 0.75)
        sValue += "\\frac{3}{4}=";
      else if (e1 == -0.75)
        sValue += "-\\frac{3}{4}=";
      else if (e1 == 0.125)
        sValue += "\\frac{1}{8}=";
      else if (e1 == -0.125)
        sValue += "-\\frac{1}{8}=";
      else
        sValue += d2s(e1, 6) + "=";
    }

    if (fabs((double)(e2 - (int)(e2 + EPSILON))) < EPSILON)
      sValue += i2s((int)(e2 + EPSILON));
    else {
      if (e2 == 0.5)
        sValue += "\\frac{1}{2}";
      else if (e2 == -0.5)
        sValue += "-\\frac{1}{2}";
      else if (e2 == 0.25)
        sValue += "\\frac{1}{4}";
      else if (e2 == -0.25)
        sValue += "-\\frac{1}{4}";
      else if (e2 == 0.75)
        sValue += "\\frac{3}{4}";
      else if (e2 == -0.75)
        sValue += "-\\frac{3}{4}";
      else if (e2 == 0.125)
        sValue += "\\frac{1}{8}";
      else if (e2 == -0.125)
        sValue += "-\\frac{1}{8}";
      else
        sValue += d2s(e2, 6);
    }

    // exp.PrettyPrintLaTeX(m_hLaTeXOutputProof, sValue);
    if (exp.GetType() == ep_equality) {
      PrintLaTeX("\\proofstep{");
      PrintLaTeX(exp.GetArg(0).sPrintLaTeX() + "}{=}{" +
                 exp.GetArg(1).sPrintLaTeX() + "}");
      PrintLaTeX("{by " + explanation + "}");
      PrintLaTeX("{" + sValue + "}\n");
    } else
      PrintLaTeX("don't know to print");
  }
}

// ----------------------------------------------------------------------------

void CAreaMethod::OutputXMLStep(const CGCLCProverExpression &exp,
                                const std::string &explanation,
                                eGCLC_proof_step type) {
  bool bPrint = false;
  // 0 - nothing
  // 1 - elimination steps plus grouped steps
  // 2 - elimination steps plus geometric plus grouped algebraic
  // 3 - plus statements of lemmas
  // 4 - plus elimination steps plus grouped steps in lemmas
  // 5 - plus geometric steps in lemmas
  // 6 - plus algebraic steps at level 0
  // 7 - plus algebraic steps in lemmas

  switch (type) {
  case eps_statement:
    if (m_iProofDepth == 0 || (m_iProofDepth > 0 && m_iProofLevel >= 3))
      bPrint = true;
    break;

  case eps_geometric_elimination:
    if ((m_iProofDepth == 0 && m_iProofLevel > 0) ||
        (m_iProofDepth > 0 && m_iProofLevel >= 4))
      bPrint = true;
    break;

  case eps_grouped_geometric:
    if ((m_iProofDepth == 0 && m_iProofLevel == 1) ||
        (m_iProofDepth > 0 && m_iProofLevel == 4))
      bPrint = true;
    break;

  case eps_geometric_simplification:
    if ((m_iProofDepth == 0 && m_iProofLevel > 1) ||
        (m_iProofDepth > 0 && m_iProofLevel >= 5))
      bPrint = true;
    break;

  case eps_grouped_algebraic:
    if ((m_iProofDepth == 0 && m_iProofLevel > 0 && m_iProofLevel < 6) ||
        (m_iProofDepth > 0 && m_iProofLevel >= 4 && m_iProofLevel < 7))
      bPrint = true;
    break;

  case eps_algebraic:
    if ((m_iProofDepth == 0 && m_iProofLevel >= 6) ||
        (m_iProofDepth > 0 && m_iProofLevel >= 7))
      bPrint = true;
    break;

  default:
    break;
  }

  if (bPrint) {
    double e1 = EvaluateExpression(exp.GetArg(0));
    double e2 = EvaluateExpression(exp.GetArg(1));

    // exp.PrettyPrintXML(m_hXMLOutputProof, sValue, 2 * m_iProofDepth + 2);
    PrintXML(std::string(2 * m_iProofDepth + 2, '\t') + "<proof_step>\n");
    exp.PrintXML(m_hXMLOutputProof, 2 * m_iProofDepth + 3);

    PrintXML(make_indent(2 * m_iProofDepth + 3) + "<explanation>" +
             explanation + "</explanation>\n");

    std::string sValue = make_indent(2 * m_iProofDepth + 3) + "<semantics><value>" +
                    d2s(e1, 6) + "</value>"
                                 "<value>" +
                    d2s(e2, 6) + "</value></semantics>\n";
    PrintXML(sValue);

    PrintXML(std::string(2 * m_iProofDepth + 2, '\t') + "</proof_step>\n\n");
  }
}

// ----------------------------------------------------------------------------

void CAreaMethod::PrintMethodSpecificOutput() {
  // LaTeX output
  // PrintLaTeX("\n\\vspace*{2mm} \\hrule \\vspace*{2mm} \n");
  PrintLaTeX("\n");
  PrintLaTeX("\nNumber of elimination proof steps: " +
             i2s(m_iNumberOfEliminationSteps, 6) + "\n");
  PrintLaTeX("\nNumber of geometric proof steps:   " +
             i2s(m_iNumberOfGeometricSteps, 6) + "\n");
  PrintLaTeX("\nNumber of algebraic proof steps:   " +
             i2s(m_iNumberOfAlgebraicSteps, 6) + "\n");
  PrintLaTeX("\nTotal number of proof steps:       " +
             i2s(m_iNumberOfEliminationSteps + m_iNumberOfAlgebraicSteps +
                     m_iNumberOfGeometricSteps,
                 6) +
             "\n");

  // XML output
  PrintXML("\n\t<prover_report><elimination_steps>" +
           i2s(m_iNumberOfEliminationSteps) +
           "</"
           "elimination_steps><geometrical_steps>" +
           i2s(m_iNumberOfGeometricSteps) +
           "</"
           "geometrical_steps><algebraic_steps>" +
           i2s(m_iNumberOfAlgebraicSteps) +
           "</"
           "algebraic_steps><total_number_of_steps>" +
           i2s(m_iNumberOfEliminationSteps + m_iNumberOfAlgebraicSteps +
               m_iNumberOfGeometricSteps) +
           "</"
           "total_number_of_steps><time>" +
           d2s(m_Timer.ElapsedTime()/1000, 3) + "</time></"
                                           "prover_report>");
}

// ----------------------------------------------------------------------------

std::string CAreaMethod::GetMethodSpecificOutput() {
  return "\nThe theorem prover based on the area method used."
         "\nNumber of elimination proof steps: " +
         i2s(m_iNumberOfEliminationSteps, 6) +
         "\nNumber of geometric proof steps:   " +
         i2s(m_iNumberOfGeometricSteps, 6) +
         "\nNumber of algebraic proof steps:   " +
         i2s(m_iNumberOfAlgebraicSteps, 6) +
         "\nTotal number of proof steps:       " +
         i2s(m_iNumberOfEliminationSteps + m_iNumberOfGeometricSteps +
                 m_iNumberOfAlgebraicSteps,
             6);
}

const std::string _arName = "Area";
const std::string &CAreaMethod::GetName() const { return _arName; }
