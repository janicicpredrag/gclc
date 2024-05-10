#include "Log.h"
#include "stdarg.h"

#include "../Utils/Utils.h"
// #include <stdio.h>

static char __log_msg[1000];

int Log::_maxLevel = 3;
std::ofstream *Log::_outFileLatex = NULL;
std::ofstream *Log::_outFileXML = NULL;
bool Log::_standardOutput = false;
int Log::StopAfterMaxT = -1;
std::ofstream *Log::_outFiles[5];

bool Log::_sectionStarted = false;
bool Log::_subsectionStarted = false;
bool Log::_paragraphStarted = false;

bool Log::LogEnabledForLevel(int level) {
  if (level <= _maxLevel) {
    if (level == 1 && _outFileLatex == NULL) {
      return false;
    }
    if (level == 2 && _outFileXML == NULL) {
      return false;
    }
    return true;
  }
  return false;
}

bool Log::LogEnabledLatex() { return LogEnabledForLevel(1); }

bool Log::LogEnabledXML() { return LogEnabledForLevel(2); }

void Log::SetStandardOutput(bool standardOutput) {
  _standardOutput = standardOutput;
}

void Log::SetOutputFile(std::ofstream *outFile, int index) {
  if (_outFiles[index] && _outFiles[index] != outFile) {
    _outFiles[index]->close();
  }
  _outFiles[index] = outFile;
}

bool Log::GetStandardOutput() { return _standardOutput; }

void Log::PrintLogF(int level, const char *msg, ...) {
  if (level <= _maxLevel) {
    // fetch parameters and print result
    va_list ap;
    va_start(ap, msg);
    char buffer[2048];

    if (_outFileLatex && level == 1) {
      vsprintf(buffer, msg, ap);
      std::string s(buffer);
      Print(*_outFileLatex, s);
    }

    if (_outFileXML && level == 2) {
      vsprintf(buffer, msg, ap);
      std::string s(buffer);
      Print(*_outFileXML, s);
    }

    if (level >= 0 && level < 5 && _outFiles[level]) {
      vsprintf(buffer, msg, ap);
      Print(*_outFiles[level], buffer);
    }

    if (_standardOutput) {
      vfprintf(stdout, msg, ap);
    }

    va_end(ap);
  }
}

void Log::SetLoggingLevel(int level) { _maxLevel = level; }

void Log::InitOutputFile(char *path) {
  if (path) {
    _outFileLatex->open(path);
    if (!_outFileLatex) {
      fprintf(stderr, "Failed to open file %s\n", path);
      throw - 1;
    }
  } else {
    _outFileLatex = NULL;
  }
}

void Log::SetLatexOutputFile(std::ofstream *outFileLatex) {
  if (_outFileLatex && _outFileLatex != outFileLatex) {
    _outFileLatex->close();
  }
  _outFileLatex = outFileLatex;
}

void Log::SetXMLOutputFile(std::ofstream *outFileXML) {
  if (_outFileXML && _outFileXML != outFileXML) {
    _outFileXML->close();
  }
  _outFileXML = outFileXML;
}

#ifdef DBG

vector<char *> Log::_objectNames;
vector<int> Log::_objectCounts;
vector<int> Log::_objectCountsTotal;

void Log::ObjectCreated(char *x) {
  int ii, size = _objectNames.size();
  for (ii = 0; ii < size && strcmp(x, _objectNames[ii]) != 0; ii++)
    ;

  if (ii == size) {
    // create object
    char *s = new char[strlen(x) + 1];
    strcpy(s, x);
    _objectNames.push_back(s);
    _objectCounts.push_back(0);
    _objectCountsTotal.push_back(0);
  }

  _objectCounts[ii] += 1;
  _objectCountsTotal[ii] += 1;
}

void Log::ObjectDestroyed(char *x) {
  int ii, size = _objectNames.size();
  for (ii = 0; ii < size && strcmp(x, _objectNames[ii]) != 0; ii++)
    ;

  if (ii < size) {
    _objectCounts[ii] -= 1;
  }
}

void Log::ObjectLifeInfo() {
  Log::PrintLogF(
      0, "---- ObjectLifeInfo Report -------------------------------\n");
  int ii, size = _objectNames.size();
  for (ii = 0; ii < size; ii++) {
    Log::PrintLogF(0, "Object [%s, %d, %d]\n", _objectNames[ii],
                   _objectCounts[ii], _objectCountsTotal[ii]);
  }
  Log::PrintLogF(
      0, "----------------------------------------------------------\n");
}
void Log::CheckMemoryLeaks() {
  int ii, size = _objectNames.size();
  for (ii = 0; ii < size; ii++) {
    if (_objectCounts[ii] != 0) {
      fprintf(stdout, "Memory Leak: object %s, total = %d, left = %d!!!\n",
              _objectNames[ii], _objectCountsTotal[ii], _objectCounts[ii]);
    }
  }
}

#endif

double Log::ElapsedTime(double startTime) {
#if (defined(_PLATFORM_WIN_) || defined(_PLATFORM_LINUX_))
  long int TimeElapsed, TimeCurrent;
  struct rusage usage;
  getrusage(RUSAGE_SELF, &usage);
  TimeCurrent = (1000000 * usage.ru_utime.tv_sec + usage.ru_utime.tv_usec +
                1000000 * usage.ru_stime.tv_sec + usage.ru_stime.tv_usec)/1000;
  TimeElapsed = TimeCurrent - (long int)startTime;
  return ((double)TimeElapsed);
#else
  time_t t1;
  time(&t1);
  return (double)(t1 - startTime);
#endif
}

void Log::OutputSectionBegin(const char *text) {
  // close previous subsection
  if (_subsectionStarted) {
    OutputSubSectionEnd();
  }

  // close previous section
  if (_sectionStarted) {
    // close previous
    OutputSectionEnd();
  }
  _sectionStarted = true;

  // latex
  Log::PrintLogF(1, "\n\n\n\n\\section{%s}\n\n", text);

  // xml
  Log::PrintLogF(2, "\n\n<proof_section>\n<caption>%s</caption>\n", text);
}

void Log::OutputSectionEnd() {
  if (_sectionStarted) {
    // close xml tag, nothing to do in latex
    Log::PrintLogF(2, "\n</proof_section>\n\n");
    _sectionStarted = false;
  }
}

void Log::OutputBold(const char *text) {
  // latex
  Log::PrintLogF(1, "\\textbf{%s}\n\n", text);

  // xml
  Log::PrintLogF(2, "\n<proof_bold>%s</proof_bold>\n", text);
}

void Log::OutputText(const char *text, ...) {
  va_list ap;
  va_start(ap, text);

  vsprintf(__log_msg, text, ap);
  va_end(ap);

  // latex
  Log::PrintLogF(1, __log_msg);

  // xml
  FormatXMLString();
  Log::PrintLogF(2, "<proof_text>%s</proof_text>", __log_msg);
}

void Log::OutputTextNoTag(const char *text, ...) {
  va_list ap;
  va_start(ap, text);

  vsprintf(__log_msg, text, ap);
  va_end(ap);

  // latex
  Log::PrintLogF(1, __log_msg);

  // xml
  FormatXMLString();
  Log::PrintLogF(2, __log_msg);
}

//
// Make XML string from __log_msg string
//
void Log::FormatXMLString() {
  // rules:
  // $ -> delete
  // \{ -> {
  // \} -> }
  // \ni -> unicode char
  // \emtpyset -> unicode char
  const int rule_pairs = 5;
  const char *rules[] = {"$", "",     "\\{",     "{",          "\\}",
                         "}", "\\ni", "&#8715;", "\\emptyset", "&#8709;"};

  int ii;
  for (ii = 0; ii < rule_pairs; ii++) {
    FormatXMLStringHelper(rules[2 * ii], rules[2 * ii + 1]);
  }
}

//
// Find sl in __log_msg and replace it with sr
//
void Log::FormatXMLStringHelper(const char *sl, const char *sr) {
  // find occurence of sl and replace it with sr
  int pos = 0;       // position in main string
  int size_eq;       // number of characters equal
  int replace_index; // index for doint replacement
  int ii;            // shifting index
  int shift_size;    // shifting size

  while (__log_msg[pos] != 0) {
    // check this position
    size_eq = 0;

    while (__log_msg[pos + size_eq] && sl[size_eq] &&
           __log_msg[pos + size_eq] == sl[size_eq]) {
      size_eq++;
    }
    if (sl[size_eq] == 0) {
      // found, do replacement
      replace_index = 0;

      while (sl[replace_index] && sr[replace_index]) {
        __log_msg[pos + replace_index] = sr[replace_index];
        replace_index++;
      }

      // do we need shifting left or shifting right?

      if (sl[replace_index]) {
        // shift left, how much
        shift_size = 0;
        while (sl[replace_index + shift_size]) {
          shift_size++;
        }

        // shift
        for (ii = pos + replace_index; __log_msg[ii]; ii++) {
          __log_msg[ii] = __log_msg[ii + shift_size];
        }
      } else if (sr[replace_index]) {
        // shift right, how much
        shift_size = 0;
        while (sr[replace_index + shift_size]) {
          shift_size++;
        }

        // find last character
        for (ii = pos + replace_index; __log_msg[ii]; ii++)
          ;

        // shift (make space)
        for (; ii >= pos + replace_index; ii--) {
          __log_msg[ii + shift_size] = __log_msg[ii];
        }

        // fill the space
        for (ii = 0; ii < shift_size; ii++) {
          __log_msg[pos + replace_index + ii] = sr[replace_index + ii];
        }
      }
    }

    pos++;
  }
}

void Log::OutputParagraphBegin() {
  if (_paragraphStarted) {
    // close previous
    OutputParagraphEnd();
  }
  _paragraphStarted = true;

  // latex
  Log::PrintLogF(1, "\n\n");

  // xml
  Log::PrintLogF(2, "\n<proof_line>\n");
}

void Log::OutputParagraphEnd() {
  // latex
  Log::PrintLogF(1, "\n");

  if (_paragraphStarted) {
    // close xml tag, nothing to do in latex
    Log::PrintLogF(2, "\n</proof_line>\n\n");
    _paragraphStarted = false;
  }
}

void Log::OutputEnumBegin(const char *enumCommand) {
  // latex
  Log::PrintLogF(1, "\\begin{%s}\n\n", enumCommand);

  // xml
  Log::PrintLogF(2, "\n<proof_enum_%s>\n", enumCommand);
}

void Log::OutputEnumEnd(const char *enumCommand) {
  // latex
  Log::PrintLogF(1, "\\end{%s}\n\n", enumCommand);

  // xml
  Log::PrintLogF(2, "\n</proof_enum_%s>\n", enumCommand);
}

void Log::OutputEnumItem(const char *text, ...) {
  va_list ap;
  va_start(ap, text);

  vsprintf(__log_msg, text, ap);
  va_end(ap);

  // latex
  Log::PrintLogF(1, "\\item %s\n\n", __log_msg);

  // xml
  FormatXMLString();
  Log::PrintLogF(2, "\n<proof_enum_item>%s</proof_enum_item>\n", __log_msg);
}

void Log::OutputEnumItemBegin() {
  Log::PrintLogF(1, "\\item ");
  Log::PrintLogF(2, "\n<proof_enum_item>");
}

void Log::OutputEnumItemEnd() { Log::PrintLogF(2, "</proof_enum_item>"); }

void Log::OutputBegins() {
  _sectionStarted = false;
  _subsectionStarted = false;
  _paragraphStarted = false;
}

void Log::OutputSubSectionBegin(const char *text, int notNumber, ...) {
  if (_subsectionStarted) {
    // close previous
    OutputSubSectionEnd();
  }
  _subsectionStarted = true;

  va_list ap;
  va_start(ap, notNumber);

  vsprintf(__log_msg, text, ap);
  va_end(ap);

  // latex
  if (notNumber) {
    Log::PrintLogF(1, "\n\n\n\n\\subsection*{%s}\n\n", __log_msg);
  } else {
    Log::PrintLogF(1, "\n\n\n\n\\subsection{%s}\n\n", __log_msg);
  }

  // xml
  Log::PrintLogF(2, "\n\n<proof_subsection>\n<caption>%s</caption>\n",
                 __log_msg);
}

void Log::OutputSubSectionEnd() {
  if (_subsectionStarted) {
    // close xml tag, nothing to do in latex
    Log::PrintLogF(2, "\n</proof_subsection>\n\n");
    _subsectionStarted = false;
  }
}

void Log::OutputEnumDescriptionBold(const char *text, ...) {
  va_list ap;
  va_start(ap, text);

  vsprintf(__log_msg, text, ap);
  va_end(ap);

  // latex
  Log::PrintLogF(1, "[%s] ", __log_msg);

  // xml
  FormatXMLString();
  Log::PrintLogF(2, "\n<proof_bold>%s</proof_bold>\n", __log_msg);
}

void Log::OutputDescriptionItemBegin(const char *text, ...) {
  va_list ap;
  va_start(ap, text);

  vsprintf(__log_msg, text, ap);
  va_end(ap);

  // latex
  Log::PrintLogF(1, "\\item [%s] ", __log_msg);

  // xml
  FormatXMLString();
  Log::PrintLogF(2,
                 "\n<proof_enum_item><proof_text_bold>%s</proof_text_bold>\n",
                 __log_msg);
}

void Log::OutputDescriptionItemEnd(const char *text, ...) {
  va_list ap;
  va_start(ap, text);

  vsprintf(__log_msg, text, ap);
  va_end(ap);

  // latex
  Log::PrintLogF(1, " %s", __log_msg);

  // xml
  FormatXMLString();
  Log::PrintLogF(2, "\n %s</proof_enum_item>\n", __log_msg);
}
