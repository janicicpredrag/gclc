// GCLCApp.cpp : Defines the entry point for the wasm application.
//

#include "Export/EPSOutput.h"
#include "Export/LaTeXOutput.h"
#include "Export/PSTricksOutput.h"
#include "Export/SVGOutput.h"
#include "Export/TikZOutput.h"
#include "GCLCEngine/GCLC.h"
#include "GenericEngine/GCompiler.h"
#include "Input/StringInput.h"
#include "Logging/DummyLog.h"
#include "Logging/FileLog.h"
#include "Utils/Common.h"
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <emscripten/emscripten.h>
#include <memory>
#include <sstream>
#include <string>

#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN
#endif

EXTERN EMSCRIPTEN_KEEPALIVE void render(char *input, char *fileName,
                                        char **outputPtr, int outputType,
                                        int proofType, int deductionControl,
                                        char **logPtr) {

  std::ostringstream outputStream, logStream, latexProofStream, xmlProofStream;
  std::string inputStr(input);
  std::string fileNameStr(fileName);
  std::ofstream a;
  CStringInput CStringInput(inputStr);
  CFileLog L(logStream);

  prover_config ProverConfig;
  ProverConfig.sTheoremFileName = fileNameStr;
  ProverConfig.TheoremProvingMethod = tpNone;
  if (proofType == 1)
    ProverConfig.TheoremProvingMethod = tpAreaMethod;
  else if (proofType == 2)
    ProverConfig.TheoremProvingMethod = tpWuMethod;
  else if (proofType == 3)
    ProverConfig.TheoremProvingMethod = tpGroebnerMethod;

  ProverConfig.bDeductionControl = false;
  if (deductionControl != 0)
    ProverConfig.bDeductionControl = true;

  ProverConfig.bLaTeX = false;
  if (proofType != 0)
    ProverConfig.bLaTeX = true;

  ProverConfig.bXML = false;
  if (proofType != 0)
    ProverConfig.bXML = true;

  CGCLC C(CStringInput, L, ProverConfig, false, a);

  eGCLC_conjecture_status prover_output = e_idle;
  double prover_time = 0;
  GReturnValue r = C.Import(prover_output, prover_time);

  if (r == rvG_OK) {
    std::unique_ptr<CGCLCOutput> pO;
    if (outputType == eSVGoutput)
      pO.reset(CSVGOutput(outputStream));
    else if (outputType == eEPSoutput)
      pO.reset(new CEPSOutput(outputStream));
    else if (outputType == eLaTeXoutput)
      pO.reset(new CLaTeXOutput(outputStream));
    else if (outputType == eTikZoutput)
      pO.reset(new CTikZOutput(outputStream));
    else if (outputType == ePSTricksoutput)
      pO.reset(new CPSTricksOutput(outputStream));

    r = C.Export(*pO);
    pO->GetPointCounter();

    if (r != rvG_OK) {
      return;
    }
  }

  std::string outputStr = outputStream.str();
  size_t length = outputStr.length();
  char *outputBuffer = (char *)std::malloc(length + 1);
  if (outputBuffer != NULL) {
    outputStr.copy(outputBuffer, length);
    outputBuffer[length] = 0x00;
    *outputPtr = outputBuffer;
  }

  std::string logStr = logStream.str();
  length = logStr.length();
  char *logBuffer = (char *)std::malloc(length + 1);
  if (logBuffer != NULL) {
    logStr.copy(logBuffer, length);
    logBuffer[length] = 0x00;
    *logPtr = logBuffer;
  }
}

EXTERN EMSCRIPTEN_KEEPALIVE void fastRender(char *input, char **outputPtr) {
  std::ostringstream outputStream;
  std::ofstream a;
  std::string inputStr(input);
  CStringInput CStringInput(inputStr);
  CDummyLog Log;

  prover_config ProverConfig;
  ProverConfig.sTheoremFileName = "temp";
  ProverConfig.TheoremProvingMethod = tpNone;
  ProverConfig.bDeductionControl = false;
  ProverConfig.bLaTeX = false;
  ProverConfig.bXML = false;

  CGCLC C(CStringInput, Log, ProverConfig, false, a);

  eGCLC_conjecture_status prover_output = e_idle;
  double prover_time = 0;
  GReturnValue r = C.Import(prover_output, prover_time);

  if (r == rvG_OK) {
    CSVGOutput pO{outputStream};

    r = C.Export(pO);
    pO.GetPointCounter();

    if (r != rvG_OK) {
      return;
    }
  }

  std::string outputStr = outputStream.str();
  size_t length = outputStr.length();
  char *outputBuffer = (char *)std::malloc(length + 1);
  if (outputBuffer != NULL) {
    outputStr.copy(outputBuffer, length);
    outputBuffer[length] = 0x00;
    *outputPtr = outputBuffer;
  }
}