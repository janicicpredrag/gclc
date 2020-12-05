#include "GCLC.h"

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_conjecture() {
  GCLCError iRv;
  string text;

  if ((iRv = take_text(text)) != rvGCLCOK)
    return iRv;

  if (ProvingTheorem()) {
    if (SetProverConjecture(text) != rvG_OK)
      return rvGCLCInvalidConjecture;

    //-------- Support for input exported to XML form ---------
    if (m_bXMLOutput && m_hXMLOutput.good()) {
      ChangeCurrentXMLGroup(eXMLconjecture);
      Print(m_hXMLOutput, "\t\t<prove proof_level=\"" + i2s(GetProofLevel()) +
                              "\" proof_limit=\"" + i2s(GetProofLimit()) +
                              "\">\n");

      GetConjecture().PrintXML(m_hXMLOutput, 3);
      Print(m_hXMLOutput, "\t\t</prove>\n");

      m_CurrentXMLgroup = eXMLconjecture;
    }
    //-------- End of support for XML form ---------
  }

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_proof_level() {
  GCLCError iRv;
  double num;

  if ((iRv = ReadNumber(num)) != rvGCLCOK)
    return iRv;
  int iProofLevel = (int)num;
  if (iProofLevel < 0)
    iProofLevel = 0;
  if (iProofLevel > 7)
    iProofLevel = 7;
  SetProofLevel(iProofLevel);
  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_proof_limit() {
  GCLCError iRv;
  double num;

  if ((iRv = ReadNumber(num)) != rvGCLCOK)
    return iRv;
  int iProofLimit = (int)num;
  if (iProofLimit < 0)
    iProofLimit = 0;
  SetProofLimit(iProofLimit);
  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_prover_timeout() {
  GCLCError iRv;
  double num;

  if ((iRv = ReadNumber(num)) != rvGCLCOK)
    return iRv;
  int iProverTimeout = (num < 0) ? -1 /* no time limit */ : (int)(num * 1000);
  SetProverTimeout(iProverTimeout);
  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_prover_theorem_name() {
  GCLCError iRv;
  string s;
  if ((iRv = take_text(s)) != rvGCLCOK)
    return iRv;
  m_ProverConfig.m_sTheoremName = s;
  return rvGCLCOK;
}

// ----------------------------------------------------------------------------
