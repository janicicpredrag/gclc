#if !defined(COMMON_H)
#define COMMON_H

#define EPS 0.00001
#define PI 3.1415925
#define PT_PER_MM 2.845
#define MM_PER_PT 0.35149
#define FLOAT_PRECISION 6

enum GReturnValue {
  rvG_OK,
  rvG_OutOfMemory,
  rvG_InvalidInput,
  rvG_ErrorInInput,
  rvG_EndOfData,
  rvG_CannotOpenOutputFile,
  rvG_CannotOpenLogFile,
  rvG_NotImplemented,
  rvG_EmptySet,
  rvG_ProverFailed,
  rvG_Error
};

enum export_type {
  eLaTeXoutput,
  eTikZoutput,
  ePSTricksoutput,
  eXMLoutput,
  eSVGoutput,
  eEPSoutput,
  eGenericLaTeXoutput
};

#endif // !defined(COMMON_H)
