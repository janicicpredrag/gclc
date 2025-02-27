#include "Log.h"
#include "PolyReader.h"
#include <algorithm>
#include <memory>
#include <string>

void PolyReader::_Print(char *stream, int s, int e) {
  for (int ii = s; ii <= e; ii++) {
    Log::PrintLogF(0, "%c", stream[ii]);
  }
  Log::PrintLogF(0, "\n");
}

void PolyReader::_Error(const char *msg) {
  Log::PrintLogF(0, msg);
  throw - 1;
}

void PolyReader::_Assert(bool assert, const char *msg) {
  if (!assert) {
    _Error(msg);
  }
}

void PolyReader::PrintPolynomials(std::vector<XPolynomial *> &vpols, int level,
                                  int size) {
  Log::PrintLogF(1, "\\begin{eqnarray*}\n");
  Log::PrintLogF(2, "<polynomial_system>\n");

  unsigned count = size > 0 ? std::min((size_t)size, vpols.size()) : vpols.size();
  for (unsigned i = 0; i < count; i++) {
    PrintPolynomial(vpols[i], level, i, true);
  }

  Log::PrintLogF(1, "\\end{eqnarray*}\n");
  Log::PrintLogF(2, "</polynomial_system>\n");
}

void PolyReader::PrintPolynomial(XPolynomial *xp, int level, int index,
                                 bool tabular) {
  if (!Log::LogEnabledForLevel(level)) {
    return;
  }

  // don't print too big polynomials
  if (xp->GetTotalTermCount() > 255) {
    // too big
    if (tabular)
      Log::PrintLogF(1, " & & \\mbox\{");
    Log::OutputText(" Polynomial too big for output (number of terms is %d)\n",
                    xp->GetTermCount());
    if (tabular)
      Log::PrintLogF(1, "} \\\\ \n");
    Log::PrintLogF(2, "\n\n");
    return;
  }

  std::string ltx = xp->PrintLatex();
  static int cnt = 0;

  // separate chunks
  const int chunk = 122;
  const int chunkDelta = 20;
  int curChunk = chunk;
  const int size = ltx.size();

  // failed to print too large polynomials
  if (size > 1000) {
    if (!tabular) {
      Log::OutputText("\nPolynomial too big for output (text size is %d "
                      "characters, number of terms is %d)",
                      size, xp->GetTermCount());
      Log::PrintLogF(1, "\\\\ \n\n");
    } else {
      if (level == 1) {
        Log::PrintLogF(2, "<polynomial>p_{%d} = ...</polynomial>\n", index);
      }
      if (tabular && index > 0) {
        Log::PrintLogF(1, "p_{%d} &=& ", index);
      }
      Log::PrintLogF(1, "\\ldots \\\\ \n ");
      Log::PrintLogF(2, "\n\n");
    }
    return;
  }

  if (level == 1) {
    Log::PrintLogF(2, "<polynomial>\n");
  }

  int pos = 0, posb = 0;

  bool first = true;
  while (pos < size) {
    posb = pos;
    pos += curChunk;
    if (pos > size) {
      pos = size;
    }

    // must break on a correct place
    // combination +x, -x, +(, -(
    while (pos > 1 /* added on 10.2015 */ && pos < size && pos > posb &&
           ((ltx[pos - 1] != 'x' && ltx[pos - 1] != '(') ||
            (ltx[pos - 2] != '+' && ltx[pos - 2] != '-'))) {
      pos--;
    }
    if (pos > 1 /* added on 10.2015 */ && pos < size) {
      pos--;
    }

    if (pos <= posb) {
      // could stop on + or - sign if and only if
      // there is equal number of characters '{' and '}' after
      pos += curChunk;
      if (pos > size) {
        pos = size;
      }

      bool found = false;
      while (!found) {
        if (pos == posb) {
          found = true;
        } else {
          if (ltx[pos - 1] == '+' || ltx[pos - 1] == '-') {
            // check '{' and '}' after
            int diff = 0;
            for (int bi = pos; bi < size; bi++) {
              if (ltx[bi] == '{') {
                diff++;
              } else if (ltx[bi] == '}') {
                diff--;
              }
            }

            found = (diff == 0);
          }
        }
        if (!found) {
          pos--;
        }
      }
    }

    if (pos <= posb) {
      // impossible to break with current chunk
      // try with greater chunk
      curChunk += chunkDelta;
      pos = posb;
      continue;
    } else {
      curChunk = chunk;
    }

    if (!tabular) {
      Log::PrintLogF(level, "$$\n");
    }
    if (first) {
      first = false;
      ++cnt;
      if (index == -2) {
        // conclusion
        Log::PrintLogF(level, "g %c=%c ", tabular ? '&' : ' ',
                       tabular ? '&' : ' ');
        if (level == 1) {
          Log::PrintLogF(2, "g = ");
        }
      } else {
        Log::PrintLogF(level, "p_{%d} %c=%c ", index >= 0 ? index : cnt,
                       tabular ? '&' : ' ', tabular ? '&' : ' ');
        if (level == 1) {
          Log::PrintLogF(2, "p_{%d} = ", index >= 0 ? index : cnt);
        }
      }
    } else if (tabular) {
      Log::PrintLogF(level, "& & ");
    }

    char ct = ltx[pos];
    ltx[pos] = 0;

    Log::PrintLogF(level, "%s", &ltx.data()[posb]);
    if (level == 1) {
      Log::PrintLogF(2, "%s", &ltx.data()[posb]);
    }
    if (tabular) {
      Log::PrintLogF(level, "\\\\");
    }
    Log::PrintLogF(level, "\n");
    ltx[pos] = ct;

    if (!tabular) {
      Log::PrintLogF(level, "$$\n");
    }
  }
  if (level == 1) {
    Log::PrintLogF(2, "</polynomial>\n");
  }
}
