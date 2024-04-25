// GCLCApp.cpp : Defines the entry point for the console application.
//

#include "Export/EPSOutput.h"
#include "Export/LaTeXOutput.h"
#include "Export/PSTricksOutput.h"
#include "Export/SVGOutput.h"
#include "Export/TikZOutput.h"
#include "GCLCEngine/GCLC.h"
#include "Input/FileInput.h"
#include "Input/StringInput.h"
#include "Logging/FileLog.h"
#include "Utils/Utils.h"
#include <cstdlib>
#include <cstring>
#include <memory>
#include <fstream>
#include <iostream>

GReturnValue BatchProcess(export_type eOutputType, std::ifstream &hi, std::ofstream &hl,
                          std::ofstream &ho, int iCounter);
bool TakeNextGCLCBlock(CGCLCInput &Input, std::ofstream &ho, std::string &block_text,
                       int &new_lines, int &start_new_lines);

// ----------------------------------------------------------------------------

int main(int argc, char *argv[]) {
  int iErrorCode, iLine, iPos, iCounter, i;
  bool bBatchMode = false;
  std::string sInputFileName, sOutputFileName, sErrorMessage;
  setlocale(LC_ALL, "en_GB");
  srand((unsigned)time(NULL));

  Print(std::cout, "\n");
  Print(std::cout, "GCLC 2024 (GC language (R) -> LaTeX Converter)\n");
  Print(
      std::cout,
      "Copyright (c) 1995-2024 by Predrag Janicic, University of Belgrade.\n");
  Print(std::cout, "Licensed under the Creative Commons licence CC BY-ND.\n");
  Print(std::cout, "\n");

  if (argc < 2) {
    Print(std::cout, "Error in command line. Input file not specified.\n");
    return -1;
  }

  for (i = 1; i < argc; i++)
    if (!strcmp(argv[i], "-b"))
      bBatchMode = true; // support for batch mode (reading a tex file and
                         // inserting figures)

  sInputFileName.assign(argv[1]);
  std::ifstream hi(sInputFileName.c_str());
  if (!hi.is_open()) {
    sInputFileName += bBatchMode ? ".tex" : ".gcl";
    hi.open(sInputFileName.c_str());
    if (!hi.is_open()) {
      Print(std::cout, "File error. Cannot open input file.\n");
      return -1;
    }
  }

  export_type eOutputType = eLaTeXoutput; // default (pic format)
  std::string Ext = ".pic";
  for (i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "-xml")) { // Support for XML output
      eOutputType = eXMLoutput;
      Ext = ".xml";
    }
    if (!strcmp(argv[i], "-svg")) { // Support for XML output
      eOutputType = eSVGoutput;
      Ext = ".svg";
    }
    if (!strcmp(argv[i], "-eps")) { // Support for EPS output
      eOutputType = eEPSoutput;
      Ext = ".eps";
    }
    if (!strcmp(argv[i], "-tikz")) {
      eOutputType = eTikZoutput; // Support for TikZ output
      Ext = ".tkz";
    }
    if (!strcmp(argv[i], "-pst")) { // Support for PSTricks output
      eOutputType = ePSTricksoutput;
      Ext = ".pst";
    }
  }
  std::ofstream ho;
  for (i = 2; i < argc; i++) {
    if (argv[i][0] != '-' && !convert_int(argv[i], iCounter)) {
      sOutputFileName.assign(argv[i]);
      ho.open(sOutputFileName.c_str());
      if (!ho.is_open()) {
        Print(std::cout, "File error. Cannot open output file.\n");
        return -1;
      }
    }
  }
  if (!ho.is_open()) {
    size_t dot_pos = sInputFileName.find_last_of(".");
    sOutputFileName = sInputFileName.substr(0, dot_pos);
    if (bBatchMode)
      sOutputFileName += "-gclc.tex";
    else
      sOutputFileName += Ext;
    ho.open(sOutputFileName.c_str());
    if (!ho.is_open()) {
      Print(std::cout,
            "File error. Cannot open output file (" + sOutputFileName + ").\n");
      return -1;
    }
  }

  if (eOutputType == eLaTeXoutput) {
    int c;
    iCounter = 1;
    for (i = 2; i < argc; i++)
      if (argv[i][0] != '-' && convert_int(argv[i], c))
        iCounter = c;
  }

  std::ofstream hl("gclc.log");
  if (!hl.is_open()) {
    Print(std::cout, "File error. Cannot open log file.\n");
    return -1;
  }

  Print(std::cout, "Input file: " + sInputFileName + "\n");
  Print(std::cout, "Output file: " + sOutputFileName + "\n");
  Print(std::cout, "Log file: gclc.log\n\n");

  if (eOutputType == eLaTeXoutput)
    Print(std::cout, "Starting point number: " + i2s(iCounter) + "\n");

  // support for batch mode
  if (bBatchMode) {
    BatchProcess(eOutputType, hi, hl, ho, iCounter);
    return 0;
  }

  CFileInput Input(hi);
  CFileLog L(hl);

  // Support for the prover
  prover_config ProverConfig;
  ProverConfig.TheoremProvingMethod = tpAreaMethod;
  if (eOutputType == eXMLoutput || eOutputType == eSVGoutput) {
    ProverConfig.bLaTeX = false;
    ProverConfig.bXML = true;
  } else {
    ProverConfig.bLaTeX = true;
    ProverConfig.bXML = false;
  }
  for (i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "-a"))
      ProverConfig.TheoremProvingMethod = tpAreaMethod;
    if (!strcmp(argv[i], "-w"))
      ProverConfig.TheoremProvingMethod = tpWuMethod;
    if (!strcmp(argv[i], "-g"))
      ProverConfig.TheoremProvingMethod = tpGroebnerMethod;
  }
  ProverConfig.bDeductionControl = false;
  if (ProverConfig.TheoremProvingMethod != tpNone) {
    // check whether to apply deduction control
    for (i = 1; i < argc; i++)
      if (!strcmp(argv[i], "-d"))
        ProverConfig.bDeductionControl = true;
  }
  size_t dot_pos = sInputFileName.find_last_of(".");
  ProverConfig.sTheoremFileName = sInputFileName.substr(0, dot_pos);

  CGCLC C(Input, L, ProverConfig, (eOutputType == eXMLoutput), ho);

  eGCLC_conjecture_status prover_output = e_idle;
  double prover_time = 0;
  GReturnValue r = C.Import(prover_output, prover_time);

  if (r == rvG_OK) {
    if (prover_output != e_idle) {
      Print(std::cout, C.GetMethodSpecificOutput());
      Print(std::cout, (std::string) "\n\nTime spent by the prover: " +
                      d2s(prover_time/(double)1000, 3) + " seconds.");

      if (prover_output == e_proved)
        Print(std::cout, "\nThe conjecture successfully proved.");
      else if (prover_output == e_disproved)
        Print(std::cout, "\nThe conjecture disproved.");
      else if (prover_output == e_conjecture_out_of_scope)
        Print(std::cout, "\nThe conjecture out of scope of the prover.");
      else if (prover_output == e_construction_out_of_scope)
        Print(std::cout, "\nThe construction out of scope of the prover.");
      else if (prover_output == e_unknown)
        Print(std::cout, "\nThe conjecture not proved.");
      else if (prover_output == e_unknown_toomanysteps)
        Print(std::cout,
              "\nThe conjecture not proved - maximal number of proof steps "
              "reached.");
      else if (prover_output == e_unknown_timeout)
        Print(std::cout, "\nThe conjecture not proved - timeout.");

      if (prover_output != e_conjecture_out_of_scope) {
        if (ProverConfig.bLaTeX)
          Print(std::cout, "\nThe prover output is written in the file " +
                          ProverConfig.sTheoremFileName + "_proof.tex.\n");
        if (ProverConfig.bXML)
          Print(std::cout, "\nThe prover output is written in the file " +
                          ProverConfig.sTheoremFileName + "_proof.xml.\n");
      }
    }

    std::unique_ptr<CGCLCOutput> pO;
    if (eOutputType == eSVGoutput)
      pO.reset(new CSVGOutput(ho));
    else if (eOutputType == eEPSoutput)
      pO.reset(new CEPSOutput(ho));
    else if (eOutputType == eLaTeXoutput)
      pO.reset(new CLaTeXOutput(ho));
    else if (eOutputType == eTikZoutput)
      pO.reset(new CTikZOutput(ho));
    else if (eOutputType == ePSTricksoutput)
      pO.reset(new CPSTricksOutput(ho));

    if (eOutputType == eXMLoutput) {
      r = rvG_OK; // The input is exported to XML during Import
    } else {
      if (eOutputType == eLaTeXoutput) {
        pO->SetPointCounter(iCounter);
      }
      r = C.Export(*pO);
      iCounter = pO->GetPointCounter();
      pO = nullptr;
    }

    if (r == rvG_OK) {
      Print(std::cout, "\nFile '" + sInputFileName + "' successfully processed.");
      if (eOutputType == eLaTeXoutput)
        Print(std::cout, "\nEnding point number: " + i2s(iCounter) + "\n");
    } else
      Print(std::cout, "\nExport failed.");
    Print(std::cout, "\nTranscript written on gclc.log.\n\n");
  } else { // import failed
    if (C.GetError(iErrorCode, sErrorMessage, iLine, iPos) == rvG_OK) {
      Print(std::cout, "\n" + sErrorMessage);
      Print(std::cout,
            " (Line: " + i2s(iLine) + ", position: " + i2s(iPos) + ")\n\n");
    }
    if (prover_output != e_idle && prover_output != e_unknown &&
        ProverConfig.bDeductionControl) {
      Print(std::cout,
            "\nDeduction check invoked: the property that led to the error "
            "will be tested for validity.\n");
      if (prover_output == e_proved)
        Print(std::cout,
              "\nThe conjecture successfully proved - the critical property "
              "always holds.");
      else if (prover_output == e_disproved)
        Print(std::cout, "\nThe conjecture disproved - the critical property never "
                    "holds.");
      else if (prover_output == e_unknown)
        Print(std::cout,
              "\nThe conjecture not proved - the critical property does not "
              "always hold.");
      else if (prover_output == e_unknown_timeout)
        Print(std::cout, "\nThe conjecture not proved (timeout).");
      else if (prover_output == e_conjecture_out_of_scope)
        Print(std::cout, "\nThe conjecture out of scope of the prover.");
      else if (prover_output == e_construction_out_of_scope)
        Print(std::cout, "\nThe construction out of scope of the prover.");


      if (ProverConfig.bLaTeX)
        Print(std::cout, "\nThe prover output is written in the file "
                    "error-proof.tex.\n\n");
      if (ProverConfig.bXML)
        Print(std::cout, "\nThe prover output is written in the file "
                    "error-proof.xml.\n\n");
    }
  }

  return 0;
}

// ----------------------------------------------------------------------------

GReturnValue BatchProcess(export_type eOutputType, std::ifstream &hi, std::ofstream &hl,
                          std::ofstream &ho, int iCounter) {
  std::unique_ptr<CGCLCOutput> pO;
  std::string block_text;
  CFileInput Input(hi);
  CFileLog L(hl);

  if (eOutputType == eTikZoutput)
    pO.reset(new CTikZOutput(ho));
  else if (eOutputType == ePSTricksoutput)
    pO.reset(new CPSTricksOutput(ho));
  else
    pO.reset(new CLaTeXOutput(ho));

  if (eOutputType == eLaTeXoutput)
    Print(std::cout, "Starting point number: " + i2s(iCounter) + "\n");

  int new_lines = 0;
  int start_new_lines = 0;
  while (TakeNextGCLCBlock(Input, ho, block_text, new_lines, start_new_lines)) {
    Print(std::cout, "\nProcessing GCLC block beginning in line " +
                    i2s(start_new_lines) + "...");
    CStringInput Input(block_text);

    prover_config ProverConfig;
    ProverConfig.TheoremProvingMethod = tpNone;
    std::ofstream XMLoutput;
    CGCLC C(Input, L, ProverConfig, false, XMLoutput);
    eGCLC_conjecture_status prover_output;
    double prover_time;
    GReturnValue r = C.Import(prover_output, prover_time);

    if (r == rvG_OK) {
      pO->SetPointCounter(iCounter);
      r = C.Export(*pO);
      if (r == rvG_OK)
        Print(std::cout, "\nBlock successfully processed.");
      else
        Print(std::cout, "\nExport failed.");

      Print(std::cout, "\nTranscript written on gclc.log.\n\n");
      L.AddText("\n------------------------------------------------ \n");

      iCounter = pO->GetPointCounter();
    } else {
      std::string sErrorMessage;
      int iErrorCode, line, pos;

      if (C.GetError(iErrorCode, sErrorMessage, line, pos) == rvG_OK) {
        Print(std::cout, "\n");
        Print(std::cout, sErrorMessage);
        Print(std::cout, " (Line: " + i2s(start_new_lines + line) + ", position: " +
                        i2s(pos) + ")\n\n");
      }
      return rvG_ErrorInInput;
    }
  }
  if (eOutputType == eLaTeXoutput)
    Print(std::cout, "Ending point number: " + i2s(iCounter) + "\n\n");

  return rvG_OK;
}

// ----------------------------------------------------------------------------

bool TakeNextGCLCBlock(CGCLCInput &Input, std::ofstream &ho, std::string &block_text,
                       int &new_lines, int &start_new_lines) {
  unsigned char c;
  unsigned int i;
  const std::string sStartBlockKeyword = "\\begin{gclc}";
  const std::string sEndBlockKeyword = "\\end{gclc}";
  std::string tmp_text;

  bool bFoundKeyword = false;
  while (!Input.EndOfInputData() && !bFoundKeyword) {
    Input.ReadChar(c);
    if (c == '\n')
      new_lines++;

    i = 0;
    tmp_text.clear();
    while (c == sStartBlockKeyword[i]) {
      tmp_text += c;
      Input.ReadChar(c);
      if (c == '\n')
        new_lines++;
    }

    if (i == sStartBlockKeyword.size()) {
      bFoundKeyword = 1;
      start_new_lines = new_lines;
    } else {
      if (i != 0)
        Print(ho, tmp_text.substr(0, i));
      Print(ho, c);
    }
  }

  if (!bFoundKeyword)
    return false;

  bFoundKeyword = false;
  while (!Input.EndOfInputData() && !bFoundKeyword) {
    Input.ReadChar(c);
    if (c == '\n')
      new_lines++;

    i = 0;
    tmp_text.clear();
    while (c == sEndBlockKeyword[i]) {
      tmp_text += c;
      Input.ReadChar(c);
      if (c == '\n')
        new_lines++;
    }

    if (i == sEndBlockKeyword.size()) {
      bFoundKeyword = 1;
      return true;
    } else {
      if (i != 0)
        block_text += tmp_text.substr(0, i);
      block_text += c;
    }
  }

  return false;
}

// ----------------------------------------------------------------------------
