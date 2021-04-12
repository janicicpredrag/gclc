#pragma once

#include "stdinc.h"
#include <fstream>

/*************************************************************
*
* Log.h
*
* Project Logging system. Messages are logged to the standard
* output or any other output file.
*
**************************************************************/

class Log {
  static int _maxLevel;
  static ofstream *_outFileLatex;
  static ofstream *_outFileXML;
  static bool _standardOutput;
  static bool _lockOutputFiles;
  static ofstream *_outFiles[5];

  static bool _sectionStarted;
  static bool _subsectionStarted;
  static bool _paragraphStarted;

#ifdef DBG
  static vector<char *> _objectNames;
  static vector<int> _objectCounts;
  static vector<int> _objectCountsTotal;
#endif

public:
  Log();

  static int StopAfterMaxT;

  static void PrintLogF1(int level, const char *format, const char *text);

  static void PrintLogF(int level, const char *msg, ...);
  static void SetLoggingLevel(int level);
  static bool LogEnabledForLevel(int level);
  static bool LogEnabledLatex();
  static bool LogEnabledXML();

  static void InitOutputFile(char *path);
  static void SetLatexOutputFile(ofstream *outFile);
  static void SetXMLOutputFile(ofstream *outFile);

  static void SetStandardOutput(bool standardOutput);
  static bool GetStandardOutput();

  static void LockOutputFiles(bool lock);

  static void SetOutputFile(ofstream *outFile, int index);

  static double ElapsedTime(double startTime);

  // latex/xml output support
  static void OutputBegins();
  static void OutputSectionBegin(const char *text);
  static void OutputSectionEnd();
  static void OutputSubSectionBegin(const char *text, int notNumber, ...);
  static void OutputSubSectionEnd();
  static void OutputBold(const char *text);
  static void OutputText(const char *text, ...);
  static void OutputTextNoTag(const char *text, ...);
  static void OutputParagraphBegin();
  static void FormatXMLString();
  static void FormatXMLStringHelper(const char *sl, const char *sr);
  static void OutputParagraphEnd();
  static void OutputEnumBegin(const char *enumCommand);
  static void OutputEnumEnd(const char *enumCommand);
  static void OutputEnumItem(const char *text, ...);
  static void OutputEnumItemBegin();
  static void OutputEnumItemEnd();
  static void OutputEnumDescriptionBold(const char *text, ...);
  static void OutputDescriptionItemBegin(const char *text, ...);
  static void OutputDescriptionItemEnd(const char *text, ...);

#ifdef DBG
  static void ObjectCreated(char *x);
  static void ObjectDestroyed(char *x);
  static void ObjectLifeInfo();
  static void CheckMemoryLeaks();
#endif
};

// debug stuff
#ifdef DBG
#ifdef CRT_DBG
#define COSTR(x) ;
#define DESTR(x) ;
#else
#define COSTR(x)                                                               \
  ;                                                                            \
  Log::ObjectCreated(x);
#define DESTR(x)                                                               \
  ;                                                                            \
  Log::ObjectDestroyed(x);
#endif
#else
#define COSTR(x) ;
#define DESTR(x) ;
#endif
