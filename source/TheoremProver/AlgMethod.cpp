// AlgMethodReducible.cpp: implementation of the CAlgMethod class.
//
//////////////////////////////////////////////////////////////////////

#include "AlgMethod.h"
#include "../AlgebraicMethods/PolyReader.h"
#include <algorithm>
#include <assert.h>
#include <memory>
#include <utility>

// ----------------------------------------------------------------------------

CAlgMethod::CAlgMethod() { _Initialize(); }

// ----------------------------------------------------------------------------

CAlgMethod::~CAlgMethod() { CleanUp(); }

void CAlgMethod::_Initialize() {
  _printPoints = true;
  _pointsResolved = false;
  _linesResolved = false;
  lastFreeIndex = 0;
  freePointCoordinatesCount = 0;
  lastDepIndex = 0;
}

// ----------------------------------------------------------------------------

void CAlgMethod::CleanUp() {
  int ii, size;
  for (ii = 0, size = vxps.size(); ii < size; ii++) {
    XPolynomial *xp = vxps[ii];
    xp->Dispose();
  }
  vxps.clear();
  for (ii = 0, size = vxpConjectures.size(); ii < size; ii++) {
    vxpConjectures[ii]->Dispose();
  }
  vxpConjectures.clear();

  // constants
  _constants.clear();
  // points
  for (ii = 0, size = _points.size(); ii < size; ii++) {
    delete _points[ii];
  }
  _points.clear();
  // lines
  _lines.clear();
  // circles
  _circles.clear();
  // conics
  _conics.clear();

  /*	for (ii = 0, size = vpConjectures.size(); ii < size; ii++)
          {
                  //_PrintConjecture(vpConjectures[ii], 2, 0);
                  delete vpConjectures[ii];
                  if (vsConjectures[ii])
                  {
              delete [] vsConjectures[ii]; // added on 10.2015.
                  }
          if (vpOrigConjectures[ii])
          {
              delete vpOrigConjectures[ii];
          }
      }*/
  vpConjectures.clear();
  vsConjectures.clear();
  vpOrigConjectures.clear();

  // prepare for next prover session
  _Initialize();

  // base class cleanup
  CTheoremProver::CleanUp();
}

// ----------------------------------------------------------------------------

void CAlgMethod::_PrintCommand(CGCLCProverCommand &Command) {
  int ii = 0;
  bool outputFirstParam = false;

  Log::OutputParagraphBegin();
  switch (Command.type) {
  case p_point:
    Log::OutputTextNoTag("Point ");
    break;
  case p_line:
    Log::OutputTextNoTag("Line ");
    outputFirstParam = true;
    ii = 1;
    break;
  case p_bis:
    Log::OutputTextNoTag("Bisector ");
    break;
  case p_inter:
    Log::OutputTextNoTag("Intersection of lines, ");
    outputFirstParam = true;
    ii = 1;
    break;
  case p_interlc:
    Log::OutputTextNoTag("Intersection of line and circle, ");
    outputFirstParam = true;
    ii = 1;
    break;
  case p_pratio:
    // translate or towards?
    if (Command.arg[2] != Command.arg[1]) {
      Log::OutputTextNoTag("Translate ");
    } else {
      Log::OutputTextNoTag("Towards ");
    }
    break;
  case p_foot:
    Log::OutputTextNoTag("Foot, ");
    outputFirstParam = true;
    ii = 1;
    break;
  case online:
    Log::OutputTextNoTag("Random point on line, ");
    outputFirstParam = true;
    ii = 1;
    break;
  case midpoint:
    Log::OutputTextNoTag("Midpoint, ");
    outputFirstParam = true;
    ii = 1;
    break;
  case med:
    Log::OutputTextNoTag("Median, ");
    outputFirstParam = true;
    ii = 1;
    break;
  case perp:
    Log::OutputTextNoTag("Perpendicular, ");
    outputFirstParam = true;
    ii = 1;
    break;
  case parallel:
    Log::OutputTextNoTag("Parallel, ");
    outputFirstParam = true;
    ii = 1;
    break;
  case oncircle:
    Log::OutputTextNoTag("Random point on circle, ");
    outputFirstParam = true;
    ii = 1;
    break;
  case p_circle:
    Log::OutputTextNoTag("Circle, ");
    outputFirstParam = true;
    ii = 1;
    break;
  default:
    Log::PrintLogF(0, "_PrintCommand: Unknown type %d\n\n", Command.type);
    throw - 1;
    break;
  }

  if (outputFirstParam) {
#if 0
        Log::OutputTextNoTag("$");
        Log::OutputTextNoTag(Command.arg[0]);
        Log::OutputTextNoTag("$");
        Log::OutputTextNoTag(": ");
#else
    Log::OutputTextNoTag("$%s$: ", Command.arg[0].c_str());
#endif
  }

  while (!Command.arg[ii].empty() && (Command.type != p_point || ii < 1)) {
#if 0
        Log::OutputTextNoTag(" ");
        Log::OutputTextNoTag("$");
        Log::OutputTextNoTag(Command.arg[ii]);
        Log::OutputTextNoTag("$");
#else
    Log::OutputTextNoTag("$%s$ ", Command.arg[ii].c_str());
#endif
    ++ii;
  }

  Log::OutputParagraphEnd();
}

// ----------------------------------------------------------------------------

enum eGCLC_conjecture_status
CAlgMethod::ProveConjecture(const CGCLCProverExpression &Conj) {
  // set or unset latexoutput
  Log::SetLatexOutputFile(&m_hLaTeXOutputProof);

  // set or unset xmloutput
  Log::SetXMLOutputFile(&m_hXMLOutputProof);

  Log::OutputBegins();

  enum eGCLC_conjecture_status retValue = e_unknown;

  Log::OutputSectionBegin("Construction and prover internal state");

  Log::OutputSubSectionBegin("Construction commands:", true);
  Log::OutputEnumBegin("itemize");

  for (std::list<CGCLCProverCommand>::iterator it = m_ProverCommands.begin();
       it != m_ProverCommands.end(); it++) {
    Log::OutputEnumItemBegin();
    _PrintCommand(*it);
    Log::OutputEnumItemEnd();
  }
  Log::OutputEnumEnd("itemize");

  Log::OutputSubSectionBegin("Coordinates assigned to the points:", true);
  Log::OutputEnumBegin("itemize");
  _PrintPoints();
  Log::OutputEnumEnd("itemize");

  Log::OutputSubSectionBegin("Conjecture(s):", true);

  Log::OutputEnumBegin("enumerate");
  // char sCond[COND_LENGHT];
  std::string sCond;

  for (unsigned ii = 0, size = vpConjectures.size(); ii < size; ii++) {
    // Log::OutputEnumItem("");
    if (!vsConjectures[ii].empty()) {
      // Log::OutputEnumItem("\\textbf{Given conjecture, GCLC code}:\n\n%s",
      // vsConjectures[ii]);
      Log::OutputEnumItem("Given conjecture");
      Log::OutputEnumBegin("itemize");
      Log::OutputEnumItemBegin();
      Log::OutputBold("GCLC code:");
      // LaTeX
      Log::PrintLogF(1, "\n\\begin{verbatim}\n%s\n\\end{verbatim}\n\n",
                     vsConjectures[ii].c_str());
      // xml
      Log::PrintLogF(2, "<proof_line>%s</proof_line>",
                     vsConjectures[ii].c_str());
      Log::OutputEnumItemEnd();
      Log::OutputEnumEnd("itemize");
    } else {
      Log::OutputEnumItem("Implicit conjecture, generated by the prover");
    }
    // old output
    _PrintConjecture(vpConjectures[ii], 3, 0);

    // new output
    if (!(vpOrigConjectures[ii] == 0.0)) {
      // Log::OutputEnumItem("\\textbf{Expression:}\n\n$%s$", sCond);
      Log::OutputEnumBegin("itemize");
      Log::OutputEnumItemBegin();
      Log::OutputBold("Expression:");

      if (Log::LogEnabledLatex()) {
        sCond = vpOrigConjectures[ii].sPrintLaTeX();
        Log::PrintLogF(1, "\n\n$%s$", sCond.c_str());
      }

      if (Log::LogEnabledXML()) {
        sCond = vpOrigConjectures[ii].sPrintXML(0);
        Log::PrintLogF(2, "<proof_line>%s</proof_line>", sCond.c_str());
      }
      Log::OutputEnumItemEnd();

      if (Log::LogEnabledLatex()) {
        sCond = vpConjectures[ii].sPrintLaTeX(false);
      }
      // Log::OutputEnumItem("\\textbf{Expression after
      // rationalization}:\n\n$%s$", sCond);
      Log::OutputEnumItemBegin();
      Log::OutputBold("Expression after rationalization:");

      if (Log::LogEnabledLatex()) {
        Log::PrintLogF(1, "\n\n$%s$", sCond.c_str());
      }

      if (Log::LogEnabledXML()) {
        sCond = vpConjectures[ii].sPrintXML(0);
        Log::PrintLogF(2, "<proof_line>%s</proof_line>", sCond.c_str());
      }

      Log::OutputEnumItemEnd();
      Log::OutputEnumEnd("itemize");
    } else {
      // Log::OutputEnumItem("\\textbf{Expression:}\n\n$%s$", sCond);
      Log::OutputEnumBegin("itemize");
      Log::OutputEnumItemBegin();
      Log::OutputBold("Expression:");

      if (Log::LogEnabledLatex()) {
        sCond = vpConjectures[ii].sPrintLaTeX(false);
        Log::PrintLogF(1, "\n\n$%s$", sCond.c_str());
      }

      if (Log::LogEnabledXML()) {
        sCond = vpConjectures[ii].sPrintXML(0);
        Log::PrintLogF(2, "<proof_line>%s</proof_line>", sCond.c_str());
      }

      Log::OutputEnumItemEnd();
      Log::OutputEnumEnd("itemize");
    }
  }
  Log::OutputEnumEnd("enumerate");
  Log::PrintLogF(1, "\\hspace*{4.1mm}\n\n");

  try {
    retValue = _Prove(Conj);
  } catch (...) {
    Log::PrintLogF(0, "Prover failed!\n\n");
    Log::PrintLogF(1, "\\textbf{Prover failed, exception raised!}\n\n");
    Log::PrintLogF(2, "<prove_failed>Exception raised!</prove_failed>\n\n");
    Print(std::cerr, "prover failed, exception raised!");
  }

  CleanUp();
  return retValue;
}

// ----------------------------------------------------------------------------

bool CAlgMethod::AddProverCommand(eGCLC_prover_command type, const std::string &a1,
                                  const std::string &a2, const std::string &a3,
                                  const std::string &a4, const std::string &a5) {
  CGCLCProverCommand Command;
  Command.type = type;
  Command.arg[0] = a1;
  Command.arg[1] = a2;
  Command.arg[2] = a3;
  Command.arg[3] = a4;
  Command.arg[4] = a5;
  m_ProverCommands.push_back(Command);
  return true;
}

// ----------------------------------------------------------------------------

//
// Is point with given name already created
//
bool CAlgMethod::_ExistsPoint(const std::string &name) {
  for (const Point *p : _points) {
    if (p->Name == name)
      return true;
  }
  return false;
}

// ----------------------------------------------------------------------------

void CAlgMethod::_PrintPoints() {
  int ii, size;
  if (_printPoints == false) {
    return;
  }
  Log::PrintLogF(0, "Assigned coordinates: ");
  for (ii = 0, size = _points.size(); ii < size; ii++) {
    _PrintPoint(_points[ii]);
    Log::PrintLogF(0, "%c ", ii < size - 1 ? ',' : '.');
  }
}

// ----------------------------------------------------------------------------

void CAlgMethod::_PrintPoint(Point *p) {
  Log::OutputEnumItemBegin();

  // output english print
  Log::OutputTextNoTag("$%s = (", p->Name.c_str());
  if (p->X.Free == false || p->X.Index != 0) {
    Log::OutputTextNoTag("%c_{%d}", p->X.Free ? 'u' : 'x', p->X.Index);
  } else {
    Log::OutputTextNoTag("0");
  }
  Log::OutputTextNoTag(", ");
  if (p->Y.Free == false || p->Y.Index != 0) {
    Log::OutputTextNoTag("%c_{%d}", p->Y.Free ? 'u' : 'x', p->Y.Index);
  } else {
    Log::OutputTextNoTag("0");
  }
  Log::OutputTextNoTag(")$\n\n");

  // serbian print
  Log::PrintLogF(0, "$%s = (", p->Name.c_str());
  if (p->X.Index == 0) {
    Log::PrintLogF(0, "0");
  } else {
    Log::PrintLogF(0, "%c_{%d}", p->X.Free ? 'u' : 'x', p->X.Index);
  }
  Log::PrintLogF(0, ", ");
  if (p->Y.Index == 0) {
    Log::PrintLogF(0, "0");
  } else {
    Log::PrintLogF(0, "%c_{%d}", p->Y.Free ? 'u' : 'x', p->Y.Index);
  }
  Log::PrintLogF(0, ")$");

  Log::OutputEnumItemEnd();
}

void CAlgMethod::_PrintLine(Line &l) {
  Log::OutputEnumItemBegin();

  Log::OutputTextNoTag("$%s \\ni ", l.Name.c_str());

  if (l.Points.size() == 0) {
    Log::OutputTextNoTag("\\emptyset ");
  } else {
    for (int ii = 0, size = l.Points.size(); ii < size; ii++) {
      Log::OutputTextNoTag("%s", l.Points[ii]->Name.c_str());
      if (ii + 1 < size) {
        Log::OutputTextNoTag(", ");
      }
    }
  }
  Log::OutputTextNoTag("$ ");

  if (l.Angle >= 0) {
    if (l.Angle == 0) {
      Log::OutputTextNoTag("; line is horizontal");
      if (l.p1 && l.p2) {
        Log::OutputTextNoTag(" (i.e., $y(%s) = y(%s)$)", l.p1->Name.c_str(),
                             l.p2->Name.c_str());
      }
    } else if (l.Angle == 90) {
      Log::OutputTextNoTag("; line is vertical");
      if (l.p1 && l.p2) {
        Log::OutputTextNoTag(" (i.e., $x(%s) = x(%s))$", l.p1->Name.c_str(),
                             l.p2->Name.c_str());
      }
    } else {
      Log::OutputTextNoTag(
          "; angle between this line and the $x$ axis is $%.0lf^\\circ$",
          l.Angle);
    }
  }
  if (l.implicit) {
    Log::OutputTextNoTag("; line is generated by the prover");
  }
  Log::OutputEnumItemEnd();
}

// ----------------------------------------------------------------------------

void CAlgMethod::_PrintCircle(Circle & /* c */) {}

// ----------------------------------------------------------------------------

void CAlgMethod::_PrintConic(Conic & /* c */) {}

// ----------------------------------------------------------------------------

//
// Extract points from the commands
// and assign free and dependent variables to them
//
bool CAlgMethod::_FindPoints() {
  int freeCount = 0;
  Point *p = NULL;
  if (_pointsResolved)
    return 1;

  for (std::list<CGCLCProverCommand>::iterator it = m_ProverCommands.begin();
       it != m_ProverCommands.end(); it++) {
    switch (it->type) {
    case p_point:
      // free point is defined
      if (_ExistsPoint(it->arg[0]) == false) {
        _AddFreePoint(it->arg[0], false, it->arg[1], it->arg[2]);
        freeCount++;
      }
      break;
    case p_inter:
    case p_foot:
    case midpoint:
    case p_interlc:
      // fully dependant points are defined
      _AddDependantPoint(it->arg[0], &lastDepIndex, false);

      if (it->type == p_interlc) {
        _AddDependantPoint(it->arg[1], &lastDepIndex, false);
      }
      break;
    case online:
    case oncircle:
      // half dependant point is defined
      p = new Point(false, true, it->arg[0]);
      p->Y.Determined = true;
      p->Y.Index = ++lastFreeIndex;
      p->X.Index = ++lastDepIndex;
      _points.push_back(p);
      break;
    case perp:
    case med:
    case parallel:
    case p_line:
    case p_circle:
    case p_bis:
      // no point is defined
      break;
    case p_pratio:
      // dependant point is defined
      _AddDependantPoint(it->arg[0], &lastDepIndex, false);
      break;
    default:
      Log::PrintLogF(0, "_FindPoints: Unknown type %d!\n\n", it->type);
      return 0;
    }
  }
  _pointsResolved = true;
  return 1;
}

// ----------------------------------------------------------------------------

bool CAlgMethod::_AddDependantPoint(const std::string &name, int *pLastDepIndex,
                                    bool addCommand) {
  if (!_ExistsPoint(name)) {
    Point *p = new Point(false, false, name);
    _points.push_back(p);

    p->Y.Determined = true;
    p->Y.Index = ++(*pLastDepIndex);
    p->X.Determined = true;
    p->X.Index = ++(*pLastDepIndex);

    if (addCommand) {
      // add point command as this point is implicitly defined!
      this->AddProverCommand(p_point, name, "-1", "-1");
    }
    return true;
  }
  return false;
}

// ----------------------------------------------------------------------------

bool CAlgMethod::_AddFreePoint(const std::string &name, bool addCommand,
                               const std::string & /* xc */,
                               const std::string & /* yc */) {
  if (!_ExistsPoint(name)) {
    Point *p = new Point(true, true, name);
    _points.push_back(p);

#ifdef GCLC_COORDINATES
    // coordinates
    if (xc) {
      sscanf(xc.c_str(), "%d", &p->X.Coordinate);
    }
    if (yc) {
      sscanf(yc.c_str(), "%d", &p->Y.Coordinate);
    }
#endif

    if (freePointCoordinatesCount == 0) {
      // first point get coordinates (0, 0)
      // second get (u2, u1)
      // third (u4, u3)
      // and so on
      p->Y.Index = 0;
      p->X.Index = 0;
      freePointCoordinatesCount += 2;

      p->Y.Determined = true;
      p->X.Determined = true;
    }
#ifdef GCLC_COORDINATES
    else {
      // check is there point with same coordinate
      for (int ii = 0; ii < _points.size() - 1; ii++) {
        if (_points[ii]->X.Coordinate == p->X.Coordinate) {
          p->X.Index = _points[ii]->X.Index;
          p->X.Determined = true;
        }
        if (_points[ii]->Y.Coordinate == p->Y.Coordinate) {
          p->Y.Index = _points[ii]->Y.Index;
          p->Y.Determined = true;
        }
      }
    }
#else
    else if (freePointCoordinatesCount == 2) {
      // second point get coordinates (u1, 0)
      p->X.Index = ++lastFreeIndex;

      p->Y.Index = 0;
      freePointCoordinatesCount += 2;

      p->Y.Determined = true;
      p->X.Determined = true;
    }
#endif

    // set undetermined positions
    if (!p->X.Determined) {
      p->X.Determined = true;
      p->X.Index = ++lastFreeIndex;
      ++freePointCoordinatesCount;
    }
    if (!p->Y.Determined) {
      p->Y.Determined = true;
      p->Y.Index = ++lastFreeIndex;
      ++freePointCoordinatesCount;
    }

    if (addCommand) {
      // add point command as this point is implicitly defined!
      this->AddProverCommand(p_point, name, "-1", "-1");
    }

    return true;
  }
  return false;
}

// ----------------------------------------------------------------------------

//
// Find constant with given name
//
Constant *CAlgMethod::_FindConstant(const std::string &name) {
  if (name.empty()) {
    return NULL;
  }

  for (std::unique_ptr<Constant> &c : _constants) {
    if (c->Name == name)
      return c.get();
  }

  return NULL;
}

// ----------------------------------------------------------------------------

//
// Find point with given name.
// Returns NULL if point does not exists.
//
Point *CAlgMethod::_FindPoint(const std::string &name) {
  if (name.empty()) {
    return NULL;
  }

  for (Point *p : _points) {
    if (p->Name == name)
      return p;
  }
  return NULL;
}

// ----------------------------------------------------------------------------

//
// Never returns NULL.
//
Point *CAlgMethod::_FindOrAddPoint(const std::string &name) {
  Point *p = _FindPoint(name);

  if (p == NULL) {
    // just add random point
    _AddFreePoint(name, false, NULL, NULL);
    p = _FindPoint(name);
  }

  return p;
}

// ----------------------------------------------------------------------------

//
// Find line with given name
//
Line *CAlgMethod::_FindLine(const std::string &name) {
  for (std::unique_ptr<Line> &line : _lines) {
    if (line->Name == name)
      return line.get();
  }

  return NULL;
}

// ----------------------------------------------------------------------------

//
// Find line with two given points
// FIXME - must improve method, maybe line contains these two
//         points but isn't directly defined with them
//         keep the list of all points on the line!?
//
Line *CAlgMethod::_FindLine(Point *p1, Point *p2) {

  for (std::unique_ptr<Line> &line : _lines) {
    bool containp1 = false;
    bool containp2 = false;
    Line *l = line.get();
    for (int jj = 0, size1 = l->Points.size();
         jj < size1 && (!containp1 || !containp2); jj++) {
      containp1 = containp1 || l->Points[jj] == p1;
      containp2 = containp2 || l->Points[jj] == p2;
    }

    if (containp1 && containp2) {
      return l;
    }
  }

  return NULL;
}

// ----------------------------------------------------------------------------

//
// Find circle with given name
//
Circle *CAlgMethod::_FindCircle(const std::string &name) {

  for (std::unique_ptr<Circle> &circle : _circles) {
    if (circle->Name == name) {
      return circle.get();
    }
  }

  return NULL;
}

// ----------------------------------------------------------------------------

//
// Find conic with given name
//
Conic *CAlgMethod::_FindConic(const std::string &name) {

  for (std::unique_ptr<Conic> &conic : _conics) {
    if (conic->Name == name) {
      return conic.get();
    }
  }

  return NULL;
}

// ----------------------------------------------------------------------------

//
// Extract lines from the commands
// and resolve them
// Line is resolved if and only if two points
// are found on it
//
// Find circles and resolve it
// circle is resolved with center and one point
//
bool CAlgMethod::_FindLinesCircles() {
  Point *p = NULL, *q = NULL;
  Line *l = NULL, *l1 = NULL, *l2 = NULL;
  Circle *k = NULL;
  bool anglePerp;

  for (std::list<CGCLCProverCommand>::iterator it = m_ProverCommands.begin();
       it != m_ProverCommands.end() && !_linesResolved; it++) {
    l = NULL;
    p = NULL;

    switch (it->type) {
    case p_point:
    case oncircle:
      // no line or point on line in command
      break;
    case med:
      l = _FindLine(it->arg[0]);
      if (l == NULL) {
        std::unique_ptr<Line> line{new Line(it->arg[0])};
        l = line.get();
        _lines.push_back(std::move(line));
      }
      break;
    case midpoint:
      // midpoint M A B
      // M is on line (A,B)
      l = _FindLine(_FindPoint(it->arg[1]), _FindPoint(it->arg[2]));
      if (l == NULL) {
        l = _CreateLine(_FindPoint(it->arg[1]), _FindPoint(it->arg[2]));
      }
      l->AddPoint(_FindPoint(it->arg[0]));
      break;
    case online:
      // 'online C A B' or 'online C a'
      // point C on line (A,B)
      if (!it->arg[2].empty()) {
        // online C A B
        l = _FindLine(_FindPoint(it->arg[1]), _FindPoint(it->arg[2]));
      } else {
        // online C a
        l = _FindLine(it->arg[1]);
      }
      if (l == NULL) {
        // this is first version for sure
        l = _CreateLine(_FindPoint(it->arg[1]), _FindPoint(it->arg[2]));
      }
      l->AddPoint(_FindPoint(it->arg[0]));
      break;
    case p_foot:
      // foot P A q
      // point arg0 on line arg2
      l = _FindLine(it->arg[2]);
      l->AddPoint(_FindPoint(it->arg[0]));

      // new line with points P and A is define
      l = _FindLine(_FindPoint(it->arg[0]), _FindPoint(it->arg[1]));
      if (l == NULL) {
        l = _CreateLine(_FindPoint(it->arg[0]), _FindPoint(it->arg[1]));
      }
      break;
    case perp:
    case p_line:
    case p_bis:
      // probably new line is defined
      l = _FindLine(it->arg[0]);
      if (l == NULL) {
        std::unique_ptr<Line> line{new Line(it->arg[0])};
        l = line.get();
        _lines.push_back(std::move(line));
      }

      if (it->type != p_bis) {
        p = _FindPoint(it->arg[1]);
        l->AddPoint(p);
      }

      if (it->type == p_line || it->type == p_bis) {
        // second point belongs to line for line and bis commands
        p = _FindPoint(it->arg[2]);
        l->AddPoint(p);
      }

      break;
    case p_inter:
      if (it->arg[4].empty()) {
        // two lines in definition
        p = _FindPoint(it->arg[0]);

        l = _FindLine(it->arg[1]);
        if (l == NULL) {
          std::unique_ptr<Line> line{new Line(it->arg[1])};
          l = line.get();
          _lines.push_back(std::move(line));
        }
        l->AddPoint(p);

        l = _FindLine(it->arg[2]);
        if (l == NULL) {
          std::unique_ptr<Line> line{new Line(it->arg[2])};
          l = line.get();
          _lines.push_back(std::move(line));
        }
        l->AddPoint(p);
      } else {
        // two implicit lines in definition
        // TODO
      }
      break;
    case p_interlc:
      // line circle intersection, or circle circle intersection
      // therefore, there are three forms
      // p1 p2 k l
      // p1 p2 l k
      // p1 p2 k1 k2
      p = _FindPoint(it->arg[0]);

      // line must exists, but it is either third or fourth argument
      l = _FindLine(it->arg[3]);
      if (l == NULL) {
        l = _FindLine(it->arg[2]);
      }
      if (l) {
        l->AddPoint(p);

        // second point
        p = _FindPoint(it->arg[1]);
        l->AddPoint(p);
      }
      break;
    case parallel:
      // two lines in definition
      p = _FindPoint(it->arg[1]);

      l = _FindLine(it->arg[0]);
      if (l == NULL) {
        std::unique_ptr<Line> line{new Line(it->arg[0])};
        l = line.get();
        _lines.push_back(std::move(line));
      }
      l->AddPoint(p);

      l = _FindLine(it->arg[2]);
      if (l == NULL) {
        std::unique_ptr<Line> line{new Line(it->arg[2])};
        l = line.get();
        _lines.push_back(std::move(line));
      }
      break;
    case p_circle:
      // circle in definition
      p = _FindPoint(it->arg[1]);
      q = _FindPoint(it->arg[2]);

      k = new Circle(it->arg[0], p, q);
      _circles.emplace_back(k);
      break;
    case p_pratio:
      // p_pratio P A B C r
      // point P belongs to line (A,C) if and only if A == B
      if (it->arg[1] == it->arg[2]) {
        l = _FindLine(_FindPoint(it->arg[1]), _FindPoint(it->arg[3]));
        if (l == NULL) {
          l = _CreateLine(_FindPoint(it->arg[1]), _FindPoint(it->arg[3]));
        }
        l->AddPoint(_FindPoint(it->arg[0]));
      }
      break;
    default:
      Log::PrintLogF(0, "_FindLinesCircles: Unknown type %d!\n\n", it->type);
      return 0;
      // throw -1;
    }
  }

  // find all missing lines if they exists
  _FindParallelLines();

  // resolve line angles
  bool lineAngleResolved = true;
  while (lineAngleResolved) {
    lineAngleResolved = false;

    for (std::list<CGCLCProverCommand>::iterator it = m_ProverCommands.begin();
         it != m_ProverCommands.end() && !_linesResolved; it++) {
      switch (it->type) {
      case p_line:
      case online:

        if (it->type == p_line) {
          // p_line p A B
          // first chance resolve
          // if both coordinates are same
          l = _FindLine(it->arg[0]);
        } else if (it->type == online && !it->arg[2].empty()) {
          // online A B C
          l = _FindLine(_FindPoint(it->arg[1]), _FindPoint(it->arg[2]));
        } else if (it->type == online) {
          // online A c
          l = _FindLine(it->arg[1]);
        }

        if (l->Angle < 0 && l->p1 && l->p2) {
          if (l->p1->X.Free == l->p2->X.Free &&
              l->p1->X.Index == l->p2->X.Index) {
            l->Angle = 90;
            lineAngleResolved = true;
          }
          if (l->p1->Y.Free == l->p2->Y.Free &&
              l->p1->Y.Index == l->p2->Y.Index) {
            l->Angle = 0;
            lineAngleResolved = true;
          }
        }
        break;
      case p_foot:
      case med:
      case perp:
      case parallel:
        anglePerp = true;
        if (it->type == p_foot) {
          // p_foot M A p
          // find line that contains two points M and A, q(M, A)
          // if p or q is resolved, resolve other one
          l = _FindLine(_FindPoint(it->arg[0]), _FindPoint(it->arg[1]));
          if (l == NULL) {
            // no line defined with those two points
            continue;
          }
          l1 = _FindLine(it->arg[2]);
        } else if (it->type == med) {
          // med m A B
          // find line containing points A and B
          l = _FindLine(_FindPoint(it->arg[1]), _FindPoint(it->arg[2]));
          l1 = _FindLine(it->arg[0]);
        } else if (it->type == perp) {
          // perp p X q
          // resolve p with q or vice versa if possible
          l = _FindLine(it->arg[0]);
          l1 = _FindLine(it->arg[2]);
        } else if (it->type == parallel) {
          // parallel p X q
          // lines p and q have same angle
          l = _FindLine(it->arg[0]);
          l1 = _FindLine(it->arg[2]);

          anglePerp = false;
        }

        if (l && l1 && l->Angle >= 0 && l1->Angle < 0) {
          l1->Angle = anglePerp ? 90 - l->Angle : l->Angle;
          lineAngleResolved = true;
        }
        if (l && l1 && l->Angle < 0 && l1->Angle >= 0) {
          l->Angle = anglePerp ? 90 - l1->Angle : l1->Angle;
          lineAngleResolved = true;
        }
        break;
      case p_bis:
        // well, this is the most complex line interaction
        // if both line angles are resolved, then bisector line
        // may also be resolved
        l = _FindLine(it->arg[0]);
        l1 = _FindLine(_FindPoint(it->arg[2]), _FindPoint(it->arg[1]));
        l2 = _FindLine(_FindPoint(it->arg[2]), _FindPoint(it->arg[3]));

        if (l && l1 && l2 && l1->Angle >= 0 && l2->Angle >= 0) {
          l->Angle = 90 - (l1->Angle + l2->Angle) / 2;
        }
        break;
      default:
        // no lines interaction
        break;
      }
    }
  }

#if 1
  // fix points on lines parallel with X or Y axis
  // example:
  // A(0, 0), B(0, u1), C online AB
  // then C should be set to (0, u2)
  // this code must be tested better!
  for (std::unique_ptr<Line> &l : _lines) {
    if (l->Angle == 0 || l->Angle == 90) {
      // check are all points on line has equal Y coordinates
      // first find minimal coordinate
      bool minFree = false;
      int minIndex = -1;

      for (Point *p : l->Points) {
        HalfPoint *hp = l->Angle == 0 ? &p->Y : &p->X;

        if (hp->Free && !minFree) {
          minFree = true;
          minIndex = -1;
        }
        if (minFree == hp->Free && (minIndex < 0 || hp->Index < minIndex)) {
          minIndex = hp->Index;
        }
      }

      // set minimal index to all points on line
      for (Point *p : l->Points) {
        HalfPoint *hp = l->Angle == 0 ? &p->Y : &p->X;
        HalfPoint *hp1 = l->Angle == 0 ? &p->X : &p->Y;

        if (hp->Free != minFree || hp->Index != minIndex) {
          // special case - online point with dependant variable
          // which is not dependant
          //					if (hp->Free != hp1->Free &&
          // minFree
          //==
          // true)
          if (hp->Free != hp1->Free && minFree && !hp1->Free) {
            hp1->Free = true;
            hp1->Index = ++lastFreeIndex;
          }

          hp->Free = minFree;
          hp->Index = minIndex;
        }
      }
    }
  }
#endif

  // print objects
  Log::OutputEnumBegin("itemize");
  for (std::unique_ptr<Line> &l : _lines) {
    _PrintLine(*l);
  }
  for (std::unique_ptr<Circle> &c : _circles) {
    _PrintCircle(*c);
  }
  for (std::unique_ptr<Conic> &c : _conics) {
    _PrintConic(*c);
  }
  Log::OutputEnumEnd("itemize");

  _linesResolved = true;
  return 1;
}

// ----------------------------------------------------------------------------

//
// Create line for Points with same coordinates
// These lines are important because of diffx and diffy
// operations.
//
void CAlgMethod::_FindParallelLines() {
  _FindParallelLines(true);
  _FindParallelLines(false);
}

// ----------------------------------------------------------------------------

void CAlgMethod::_FindParallelLines(bool xAxis) {
  int ii, size, jj, kk;
  Point *p1, *p2, *p3;
  Line *l;

  for (ii = 0, size = _points.size(); ii < size; ii++) {
    p1 = _points[ii];
    for (jj = ii + 1; jj < size; jj++) {
      p2 = _points[jj];
      if (_HalfPointsEquals(p1, p2, xAxis)) {
        l = _FindLine(p1, p2);

        // if line doesn't exists, create one
        if (l == NULL) {
          l = _CreateLine(p1, p2);
        }

        // resolve line angle
        l->Angle = xAxis ? 90 : 0;

        // find aditional points and add them to the line
        for (kk = jj + 1; kk < size; kk++) {
          p3 = _points[kk];
          if (_HalfPointsEquals(p1, p3, xAxis)) {
            l->AddPoint(p3);
          }
        }
      }
    }
  }
}

// ----------------------------------------------------------------------------

bool CAlgMethod::_HalfPointsEquals(Point *p1, Point *p2, bool xAxis) {
  if (xAxis) {
    return p1->X.Free == p2->X.Free && p1->X.Index == p2->X.Index;
  } else {
    return p1->Y.Free == p2->Y.Free && p1->Y.Index == p2->Y.Index;
  }
}

// ----------------------------------------------------------------------------

//
// Create line with two given points
//
Line *CAlgMethod::_CreateLine(Point *p1, Point *p2) {
  std::string lName = p1->Name + p2->Name;
  std::unique_ptr<Line> line{new Line(lName)};
  Line *l = line.get();
  _lines.push_back(std::move(line));
  l->AddPoint(p1);
  l->AddPoint(p2);
  l->implicit = true;

  return l;
}

// ----------------------------------------------------------------------------

//
// Create circle where first point is center and
// second point is on the circle
//
Circle *CAlgMethod::_CreateCircle(Point *p1, Point *p2) {
  std::string lName = p1->Name + p2->Name;
  Circle *k = new Circle(lName, p1, p2);
  _circles.emplace_back(k);

  return k;
}

// ----------------------------------------------------------------------------

bool CAlgMethod::_OnTheSameLine(Point *p, Point *q, Point *r) {
  if (p && q && r) {
    Line *l1 = _FindLine(p, q);
    Line *l2 = _FindLine(p, r);
    return (l1 == l2);
  }
  return false;
}

// ----------------------------------------------------------------------------

//
// Create polynomial form of the conditions
//
void CAlgMethod::_ExtractConditionPolynomials() {
  Line *l1, *l2;
  Circle *k, *k1;
  Point *p1, *p2, *p3, *p4;
  bool b1, b2;

  for (std::list<CGCLCProverCommand>::iterator it = m_ProverCommands.begin();
       it != m_ProverCommands.end(); it++) {
    // Log::OutputEnumItem("");
    Log::OutputEnumItemBegin();
    _PrintCommand(*it);

    if (it->type != p_point && it->type != p_circle) {
      Log::OutputEnumBegin("itemize");
    }
    switch (it->type) {
    case p_point:
    case p_circle:
      // no condition
      Log::OutputText("\n\nno condition");
      break;
    case p_line:
      // possible point on line condition
      l1 = _FindLine(it->arg[0]);
      if (l1 == NULL) {
        Log::PrintLogF(0, "Line in line command does not exists!");
        throw - 1;
      }
      _AddCondition(
          _PointOnLineCondition(_FindPoint(it->arg[1]), l1->p1, l1->p2), false);
      _AddCondition(
          _PointOnLineCondition(_FindPoint(it->arg[2]), l1->p1, l1->p2), false);
      break;
    case p_bis:
      // bis s B A C
      _AddCondition(_EqualTangensCondition(
          _FindLine(it->arg[0]), _FindPoint(it->arg[1]), _FindPoint(it->arg[2]),
          _FindPoint(it->arg[3])));
      break;
    case p_inter:
      if (it->arg[4].empty()) {
        // possible point on line condition
        l1 = _FindLine(it->arg[1]);
        if (l1 == NULL) {
          Log::PrintLogF(0, "Line in inter command does not exists!");
          throw - 1;
        }
        _AddCondition(
            _PointOnLineCondition(_FindPoint(it->arg[0]), l1->p1, l1->p2),
            false);
        l1 = _FindLine(it->arg[2]);
        if (l1 == NULL) {
          Log::PrintLogF(0, "Line in inter command does not exists!");
          throw - 1;
        }
        _AddCondition(
            _PointOnLineCondition(_FindPoint(it->arg[0]), l1->p1, l1->p2),
            false);
      } else {
        // two triples of collinear points
        _AddCondition(_PointOnLineCondition(_FindPoint(it->arg[0]),
                                            _FindPoint(it->arg[1]),
                                            _FindPoint(it->arg[2])),
                      false);
        _AddCondition(_PointOnLineCondition(_FindPoint(it->arg[0]),
                                            _FindPoint(it->arg[3]),
                                            _FindPoint(it->arg[4])),
                      false);
      }
      break;
    case p_interlc:
      // point on line and point on circle conditionss
      // three forms - line/circle, circle/line, circle/circle
      l1 = _FindLine(it->arg[3]);
      if (l1 == NULL) {
        l1 = _FindLine(it->arg[2]);
      }
      k = _FindCircle(it->arg[2]);
      k1 = _FindCircle(it->arg[3]);

      // don't add conditions for points which are not used throughout theorem
      // this command is often used to get only one intersection of line and
      // circle
      // and the other one is omitted
      p1 = _FindPoint(it->arg[0]);
      p2 = _FindPoint(it->arg[1]);
      b1 = _UsedPoint(p1, it);
      b2 = _UsedPoint(p2, it);

#ifdef REDUCIBLE_LINE_CIRCLE
      // solve this kind of reducibility case
      if (b1 && b2) {
        _ReducibilityLineCircleIntersection(p1, p2, l1, k, k1);
      } else
#endif
      {
        if (b1) {
          if (l1) {
            _AddCondition(_PointOnLineCondition(p1, l1->p1, l1->p2), false);
          }
          if (k) {
            _AddCondition(_EqualSegmentCondition(p1, k->c, k->p, k->c), true);
          }
          if (k1) {
            _AddCondition(_EqualSegmentCondition(p1, k1->c, k1->p, k1->c),
                          true);
          }
        }

        if (b2) {
          if (l1) {
            _AddCondition(_PointOnLineCondition(p2, l1->p1, l1->p2), false);
          }
          if (k) {
            _AddCondition(_EqualSegmentCondition(p2, k->c, k->p, k->c), true);
          }
          if (k1) {
            _AddCondition(_EqualSegmentCondition(p2, k1->c, k1->p, k1->c),
                          true);
          }
        }
      }
      break;
    case p_foot:
      // two conditions, point on line and perpendicular lines
      l1 = _FindLine(it->arg[2]);
      if (l1 == NULL) {
        Log::PrintLogF(0, "Line in foot command does not exists!");
        throw - 1;
      }
      _AddCondition(
          _PointOnLineCondition(_FindPoint(it->arg[0]), l1->p1, l1->p2), false);
      _AddCondition(_PerpendicularCondition(_FindPoint(it->arg[0]),
                                            _FindPoint(it->arg[1]), l1->p1,
                                            l1->p2),
                    false);
      break;
    case online:
      // point on line condition
      if (!it->arg[2].empty()) {
        l1 = _FindLine(_FindPoint(it->arg[1]), _FindPoint(it->arg[2]));
      } else {
        l1 = _FindLine(it->arg[1]);
      }

      if (l1 && l1->p1 && l1->p2) {
        _AddCondition(
            _PointOnLineCondition(_FindPoint(it->arg[0]), l1->p1, l1->p2),
            false);
      }
      break;
    case midpoint:
        _AddCondition(_MidpointCondition(_FindPoint(it->arg[0]),
                                            _FindPoint(it->arg[1]),
                                            _FindPoint(it->arg[2]), true), // for x
                      false);
        _AddCondition(_MidpointCondition(_FindPoint(it->arg[0]),
                                         _FindPoint(it->arg[1]),
                                         _FindPoint(it->arg[2]), false), // for y
                      false);
/*
 *    Replaced (07.03.2024):
      // two conditions
      // point on line and equal segments
      _AddCondition(_PointOnLineCondition(_FindPoint(it->arg[0]),
                                          _FindPoint(it->arg[1]),
                                          _FindPoint(it->arg[2])),
                    false);
      _AddCondition(_EqualSegmentCondition(
                        _FindPoint(it->arg[0]), _FindPoint(it->arg[1]),
                        _FindPoint(it->arg[0]), _FindPoint(it->arg[2])),
                    true);*/
      break;
    case p_pratio:
      // pratio D C B A k
      // meaning: CD = k * BA
      // two conditions:
      // 1) CD parallel BA
      // 2) sratio CD BA  = k, or AD parallel BC if k = 1 and B != A
      //                       and {C, B, A} not on the same line!
      _AddCondition(
          _ParallelCondition(_FindPoint(it->arg[1]), _FindPoint(it->arg[0]),
                             _FindPoint(it->arg[2]), _FindPoint(it->arg[3])));
      if (it->arg[4] == "1" && it->arg[2] == it->arg[3] &&
          _OnTheSameLine(_FindPoint(it->arg[1]), _FindPoint(it->arg[2]),
                         _FindPoint(it->arg[3])) == false) {
        _AddCondition(
            _ParallelCondition(_FindPoint(it->arg[3]), _FindPoint(it->arg[0]),
                               _FindPoint(it->arg[2]), _FindPoint(it->arg[1])));
      } else {
        p1 = _FindPoint(it->arg[0]);
        p2 = _FindPoint(it->arg[1]);
        p3 = _FindPoint(it->arg[2]);
        p4 = _FindPoint(it->arg[3]);
        l1 = _FindLine(p2, p3);
        _AddCondition(_DiffRatioCondition(p2, p1, p3, p4, l1, it->arg[4]),
                      false);
      }
      break;
    /*
                    case p_tratio:
                            // towards P A B r
                            // point on line condition
                            //    P belongs to line (A,B)
                            // diffx or diffy ratio condition
                            //    PA.diffx / PB.diffx = r (or diffy if line is
       parallel with Y axis)
                            p1 = _FindPoint(pc->arg[0]);
                            p2 = _FindPoint(pc->arg[1]);
                            p3 = _FindPoint(pc->arg[2]);
                            _AddCondition(_PointOnLineCondition(p1, p2, p3),
       false);
                            _AddCondition(_DiffRatioCondition(p2, p1, p2, p3,
       _FindLine(p2, p3), pc->arg[3]), false);
                            break;
    */
    case med:
      // two conditions
      // perpendicular lines and equal segments
      // first find points of medians
      l1 = _FindLine(it->arg[0]);
      if (l1 == NULL) {
        Log::PrintLogF(0, "Line in med command does not exists!");
        throw - 1;
      }
      //_AddCondition(_PerpendicularCondition(l1->p1, l1->p2,
      //_FindPoint(pc->arg[1]), _FindPoint(pc->arg[2])));
      _AddCondition(_EqualSegmentCondition(l1->p1, _FindPoint(it->arg[1]),
                                           l1->p1, _FindPoint(it->arg[2])),
                    true);
      _AddCondition(_EqualSegmentCondition(l1->p2, _FindPoint(it->arg[1]),
                                           l1->p2, _FindPoint(it->arg[2])),
                    true);
      break;
    case perp:
      // perpendicular lines condition
      // fist find points on lines
      l1 = _FindLine(it->arg[0]);
      l2 = _FindLine(it->arg[2]);
      if (l1 == NULL || l2 == NULL) {
        Log::PrintLogF(0, "Line in perp command does not exists!");
        throw - 1;
      }
      _AddCondition(_PerpendicularCondition(l1->p1, l1->p2, l2->p1, l2->p2),
                    false);
      break;
    case parallel:
      // parallel lines condition
      // fist find points on lines
      l1 = _FindLine(it->arg[0]);
      l2 = _FindLine(it->arg[2]);
      if (l1 == NULL || l2 == NULL) {
        Log::PrintLogF(0, "Line in parallel command does not exists!");
        throw - 1;
      }
      _AddCondition(_ParallelCondition(l1->p1, l1->p2, l2->p1, l2->p2), false);
      break;
    case oncircle:
      // equal distance condition
      _AddCondition(_EqualSegmentCondition(
                        _FindPoint(it->arg[0]), _FindPoint(it->arg[1]),
                        _FindPoint(it->arg[2]), _FindPoint(it->arg[1])),
                    true);
      break;
    default:
      Log::PrintLogF(0, "_ExtractConditionPolynomials: Unknown or unsupported "
                        "command %d!\n\n",
                     it->type);
      throw - 1;
    }
    if (it->type != p_point && it->type != p_circle) {
      Log::OutputEnumEnd("itemize");
    }

    Log::OutputEnumItemEnd();
  }
}

// ----------------------------------------------------------------------------

//
// Add new condition to the list of conditions
// Don't add zero conditions (which are alwaus true)
// If parameter check is true, then check is there same
// condition as the newly added (don't add twice the same condition)
//
void CAlgMethod::_AddCondition(XPolynomial *xp, bool check, vxp *polySystem) {
  bool add = true;
  int ii, size;
  XPolynomial *xp1 = NULL;

  if (xp == NULL || xp->IsZero()) {
    add = false;
  } else if (check) {
    // is there same condition
    for (ii = 0, size = vxps.size(); add && ii < size; ii++) {
      xp1 = polySystem ? polySystem->at(ii)->Clone() : vxps[ii]->Clone();
      xp1->Subtract(xp);

      add = add && !xp1->IsZero();

      if (!add) {
        Log::PrintLogF(1, "Rejected, there is already same condition!\n\n");
      }

      xp1->Dispose();

      if (add) {
        // check reverse sign!
        xp1 = polySystem ? polySystem->at(ii)->Clone() : vxps[ii]->Clone();
        xp1->Add(xp);

        add = add && !xp1->IsZero();

        if (!add) {
          Log::PrintLogF(1, "Rejected, there is already same condition!\n\n");
        }

        xp1->Dispose();
      }
    }
  }

  // PolyReader::PrintPolynomial(xp);
  if (add) {
    if (polySystem) {
      polySystem->push_back(xp);
    } else {
      vxps.push_back(xp);
    }
  } else if (xp) {
    xp->Dispose();
  }
}

// ----------------------------------------------------------------------------

// conditions

//
// Point p is on line (q1, q2)
// Create condition if and only if q1 != p and q2 != p
//
// P = (a1 - b1)(b2 - c2) - (a2 - b2)(b1 - c1)
// P = (p.x - q1.x)(q1.y - q2.y) - (p.y - q1.y)(q1.x - q2.x) = 0
//
XPolynomial *CAlgMethod::_PointOnLineCondition(Point *p, Point *q1, Point *q2) {
  XPolynomial *xp = NULL;

  Log::OutputEnumItemBegin();
  if (p && q1 && q2) {
    Log::OutputParagraphBegin();
    Log::OutputTextNoTag("point $%s$ is on the line ($%s$, $%s$)",
                         p->Name.c_str(), q1->Name.c_str(), q2->Name.c_str());
    Log::OutputParagraphEnd();

    if (q1 != p && q2 != p) {
      xp = _HelperCondition(&p->X, &q1->X, &q1->Y, &q2->Y, &p->Y, &q1->Y,
                            &q1->X, &q2->X);
    }
  } else {
    Log::OutputText("Line or points not used.");
  }

  if (xp == NULL || xp->IsZero()) {
    Log::OutputText("\n\nno condition");
  }
  Log::OutputEnumItemEnd();

  return xp;
}

// ----------------------------------------------------------------------------

//
// Point p is midpoint of (q1, q2)
// P = (q1+q2)/2 i.e.
// 2P = q1+q2
XPolynomial *CAlgMethod::_MidpointCondition(Point *p, Point *q1, Point *q2, bool bX) {
    if (!p || !q1 || !q2) {
      Log::OutputText("Points not used.");
    }

    Log::OutputEnumItemBegin();
    Log::OutputParagraphBegin();
    Log::OutputTextNoTag("point $%s$ is the midpoint of ($%s$, $%s$) (%s)",
                          p->Name.c_str(), q1->Name.c_str(), q2->Name.c_str(),
                         (bX ? "$x$" : "$y$"));
    Log::OutputParagraphEnd();

    // xp = p - q1/2 - q2/2;
    XPolynomial *xp = new XPolynomial(bX ? p->X.Free : p->Y.Free, bX ? p->X.Index : p->Y.Index);
    XPolynomial *x1 = new XPolynomial(bX ? q1->X.Free : q1->Y.Free, bX ? q1->X.Index : q1->Y.Index);
    XPolynomial *x2 = new XPolynomial(bX ? q2->X.Free : q2->Y.Free, bX ? q2->X.Index : q2->Y.Index);
    XPolynomial *h = new XPolynomial(2);

    xp->Mul(h);
    xp->Subtract(x1);
    xp->Subtract(x2);
    h->Dispose();
    x1->Dispose();
    x2->Dispose();

    if (xp == NULL || xp->IsZero()) {
        Log::OutputText("\n\nno condition");
    }
    PolyReader::PrintPolynomial(xp, 1);
    Log::OutputEnumItemEnd();

    return xp;
}

// ----------------------------------------------------------------------------

//
// l is bisector of angle BAC
// Assumption is that we know another point on l that is
// in the interior of the angle BAC
//
// Condition: tan(l, ac) - tan(ab, l) = 0
// tan(x, y, z) = tannum(x, y, z)/tanden(x, y, z)
// We must do rationalization because tangens is rational expression
//
XPolynomial *CAlgMethod::_EqualTangensCondition(Line *l, Point *b, Point *a,
                                                Point *c) {
  Point *p = l->p2;

  if (p == NULL) {
    Log::OutputEnumItem(
        "Bisector $%s$ contains only one point, no condition generated",
        l->Name.c_str());
    return NULL;
  }

  Log::OutputEnumItem(
      "Equal angles condition, line $%s$ bisecting $\\angle %s%s%s$ ",
      l->Name.c_str(), b->Name.c_str(), a->Name.c_str(), c->Name.c_str());
  return _EqualTangensCondition(p, a, c, b, a, p, true);
}

// ----------------------------------------------------------------------------

XPolynomial *CAlgMethod::_EqualTangensCondition(Point *b1, Point *a1, Point *c1,
                                                Point *b2, Point *a2, Point *c2,
                                                bool print) {
  XPolynomial *xp = NULL, *xp1 = NULL, *xp2 = NULL;

  xp = _TangensNum(b1, a1, c1);
  xp1 = _TangensDen(b2, a2, c2);
  xp->Mul(xp1);
  xp1->Dispose();

  xp1 = _TangensDen(b1, a1, c1);
  xp2 = _TangensNum(b2, a2, c2);
  xp1->Mul(xp2);
  xp2->Dispose();

  xp->Subtract(xp1);
  xp1->Dispose();

  if (print && xp) {
    PolyReader::PrintPolynomial(xp, 1, -1);
  }

  return xp;
}

// ----------------------------------------------------------------------------

//
// P = (b2 - a2)(c1 - a1) - (c2 - a2)(b1 - a1)
//
XPolynomial *CAlgMethod::_TangensNum(Point *b, Point *a, Point *c) {
  // (b2 - a2)(c1 - a1) - (c2 - a2)(b1 - a1)
  if (!a || !b || !c) {
    return NULL;
  }
  return _HelperCondition(&b->Y, &a->Y, &c->X, &a->X, &c->Y, &a->Y, &b->X,
                          &a->X, false);
}

// ----------------------------------------------------------------------------

//
// P = (b1 - a1)(c1 - a1) + (b2 - a2)(c2 - a2)
//
XPolynomial *CAlgMethod::_TangensDen(Point *b, Point *a, Point *c) {
  // (b1 - a1)(c1 - a1) - (a2 - b2)(c2 - a2)
  if (!a || !b || !c) {
    return NULL;
  }
  return _HelperCondition(&b->X, &a->X, &c->X, &a->X, &a->Y, &b->Y, &c->Y,
                          &a->Y, false);
}

// ----------------------------------------------------------------------------

//
// Ratio of segments condition
// P = (p1.X - p2.X) - r * (p3.X - p4.X), or
// P = (p1.Y - p2.Y) - r * (p3.Y - p4.Y)
// Line is there to check are points on the line parallel to X or Y axis
//
XPolynomial *CAlgMethod::_DiffRatioCondition(Point *p1, Point *p2, Point *p3,
                                             Point *p4, Line *l,
                                             const std::string &sr) {
  XPolynomial *xp = NULL;

  // get real number
  REAL r = 0;
  sscanf(sr.c_str(), "%lf", &r);

  //
  // what's default - x axis or y axis?
  //

  // bool diffy = true;
  bool diffy = false;

  // if (l && l->Angle == 0)
  if (l && l->Angle == 90) {
    // diffy = false;
    diffy = true;
  }
  if (diffy) {
    xp = _DiffRatioCondition(&p1->Y, &p2->Y, &p3->Y, &p4->Y, r);
  } else {
    xp = _DiffRatioCondition(&p1->X, &p2->X, &p3->X, &p4->X, r);
  }

  Log::OutputEnumItem("Ratio of projections of the segments ($%s$, $%s$) and "
                      "($%s$, $%s$) on axis $%c$ equal to %.3lf",
                      p1->Name.c_str(), p2->Name.c_str(), p3->Name.c_str(),
                      p4->Name.c_str(), diffy ? 'y' : 'x', r);
  if (xp) {
    PolyReader::PrintPolynomial(xp, 1);
  }

  return xp;
}

// ----------------------------------------------------------------------------

//
// See main _DiffRatioCondition function
//
XPolynomial *CAlgMethod::_DiffRatioCondition(HalfPoint *h1, HalfPoint *h2,
                                             HalfPoint *h3, HalfPoint *h4,
                                             REAL r) {
  XPolynomial *xp = _DiffPoints(h1, h2);
  XPolynomial *xp1 = _DiffPoints(h3, h4);
  xp1->Mul(r);
  xp->Subtract(xp1);
  xp1->Dispose();

  return xp;
}

// ----------------------------------------------------------------------------

//
// Simple diff between two points
//
XPolynomial *CAlgMethod::_DiffPoints(HalfPoint *h1, HalfPoint *h2) {
  XPolynomial *xp = new XPolynomial(h1->Free, h1->Index);
  XPolynomial *xp1 = new XPolynomial(h2->Free, h2->Index);
  xp->Subtract(xp1);
  xp1->Dispose();

  return xp;
}

// ----------------------------------------------------------------------------

//
// p == q
// P = (p.x - q.x)(p.x - q.x) + (p.y - q.y)(p.y - q.y)
// P = (p.x - q.x)(p.x - q.x) - (q.y - p.y)(p.y - q.y)
//
XPolynomial *CAlgMethod::_IdenticalCondition(Point *p, Point *q) {
  if (p && q) {
    Log::OutputEnumItemBegin();
    Log::OutputText("point $%s$ identical to point $%s$", p->Name.c_str(),
                    q->Name.c_str());
    if (p != q) {
      Log::OutputEnumItemEnd(); // 25.04.2008
      return _HelperCondition(&p->X, &q->X, &p->X, &q->X, &q->Y, &p->Y, &p->Y,
                              &q->Y);
    } else {
      Log::OutputText("\n\nno condition");
    }
    Log::OutputEnumItemEnd();
  }

  return NULL;
}

// ----------------------------------------------------------------------------

XPolynomial *CAlgMethod::_AlgSumZero3Expression(XPolynomial *xc1,
                                                XPolynomial *xc2,
                                                XPolynomial *xc3) {
  XPolynomial *xp, *xp1;
  XPolynomial *x1, *x2, *x3;

  x1 = _AddSegmentLengthConditionVariable(xc1);
  x2 = _AddSegmentLengthConditionVariable(xc2);
  x3 = _AddSegmentLengthConditionVariable(xc3);

  // conjecture is P described above
  xp = _SumOfTerms(x1, x2, x3, 1, -1, -1);
  xp1 = _SumOfTerms(x1, x2, x3, 1, -1, 1);
  xp->Mul(xp1);
  xp1->Dispose();
  xp1 = _SumOfTerms(x1, x2, x3, 1, 1, -1);
  xp->Mul(xp1);
  xp1->Dispose();
  xp1 = _SumOfTerms(x1, x2, x3, 1, 1, 1);
  xp->Mul(xp1);
  xp1->Dispose();

  x1->Dispose();
  x2->Dispose();
  x3->Dispose();

  return xp;
}

// ----------------------------------------------------------------------------

//
// Algebraic sum of three segments is zero
// x^2 = |ab|
// y^2 = |cd|
// z^2 = |ef|
// P = (x-y-z)(x-y+z)(x+y-z)(x+y+z)
//
XPolynomial *CAlgMethod::_AlgSumZero3Expression(Point *a, Point *b, Point *c,
                                                Point *d, Point *e, Point *f) {
  // assign three new dependant variables and add them to the conditions list
  XPolynomial *xp, *xp1, *xp2, *xp3;

  xp1 = _HelperCondition(&a->X, &b->X, &a->X, &b->X, &a->Y, &b->Y, &b->Y, &a->Y,
                         false);
  xp2 = _HelperCondition(&c->X, &d->X, &c->X, &d->X, &c->Y, &d->Y, &d->Y, &c->Y,
                         false);
  xp3 = _HelperCondition(&e->X, &f->X, &e->X, &f->X, &e->Y, &f->Y, &f->Y, &e->Y,
                         false);

  xp = _AlgSumZero3Expression(xp1, xp2, xp3);

  xp1->Dispose();
  xp2->Dispose();
  xp3->Dispose();

  return xp;
}

// ----------------------------------------------------------------------------

XPolynomial *CAlgMethod::_AddSegmentLengthConditionVariable(XPolynomial *xp1) {
  XPolynomial *xp, *x1;

  // x^2 - |ab|
  // x^2
  xp = new XPolynomial(false, ++lastDepIndex);
  x1 = xp->Clone();
  xp->Mul(x1);
  // -|ab|
  xp->Subtract(xp1);

  Log::OutputParagraphBegin();
  Log::OutputText("Introducing new variable $x_{%d}^2 = p$, where $p$ is:",
                  lastDepIndex);
  Log::OutputParagraphEnd();

  PolyReader::PrintPolynomial(xp1, 1);
  // PolyReader::PrintPolynomial(xp, 1);
  _AddCondition(xp, false);

  return x1;
}

// ----------------------------------------------------------------------------

XPolynomial *CAlgMethod::_SumOfTerms(XPolynomial *x1, XPolynomial *x2,
                                     XPolynomial *x3, int z1, int z2, int z3) {
  XPolynomial *xp = new XPolynomial();

  z1 > 0 ? xp->Add(x1) : xp->Subtract(x1);
  z2 > 0 ? xp->Add(x2) : xp->Subtract(x2);
  z3 > 0 ? xp->Add(x3) : xp->Subtract(x3);

  return xp;
}

// ----------------------------------------------------------------------------

//
// Line (p1, p2) is perpendicular with line (q1, q2)
//
// P = (a1 - b1)(c1 - d1) + (a2 - b2)(c2 - d2)
// P = (p1.x - p2.x)(q1.x - q2.x) + (p1.y - p2.y)(q1.y - q2.y)
// P = (p1.x - p2.x)(q1.x - q2.x) - (p2.y - p1.y)(q1.y - q2.y)
//
XPolynomial *CAlgMethod::_PerpendicularCondition(Point *p1, Point *p2,
                                                 Point *q1, Point *q2) {
  if (p1 && p2 && q1 && q2) {
    Log::OutputEnumItem(
        "Line ($%s$, $%s$) perpendicular with line ($%s$, $%s$)",
        p1->Name.c_str(), p2->Name.c_str(), q1->Name.c_str(), q2->Name.c_str());
    return _HelperCondition(&p1->X, &p2->X, &q1->X, &q2->X, &p2->Y, &p1->Y,
                            &q1->Y, &q2->Y);
  }
  return NULL;
}

// ----------------------------------------------------------------------------

//
// Segment [p1, p2] has equal lenght as segment [q1, q2]
//
// P = (p1.x - p2.x)^2 + (p1.y - p2.y)^2 - (q1.x - q2.x)^2 - (q1.y - q2.y)^2
// P = P1 + P2
// P1 = (p1.x - p2.x)(p1.x - p2.x) - (q1.x - q2.x)(q1.x - q2.x)
// P2 = (p1.y - p2.y)(p1.y - p2.y) - (q1.y - q2.y)(q1.y - q2.y)
//
XPolynomial *CAlgMethod::_EqualSegmentCondition(Point *p1, Point *p2, Point *q1,
                                                Point *q2) {
  if (p1 && p2 && q1 && q2) {
    Log::OutputEnumItem(
        "\n Segment [$%s$, $%s$] equal size as segment [$%s$, $%s$]",
        p1->Name.c_str(), p2->Name.c_str(), q1->Name.c_str(), q2->Name.c_str());

    XPolynomial *xp1 = _HelperCondition(&p1->X, &p2->X, &p1->X, &p2->X, &q1->X,
                                        &q2->X, &q1->X, &q2->X, false);
    XPolynomial *xp2 = _HelperCondition(&p1->Y, &p2->Y, &p1->Y, &p2->Y, &q1->Y,
                                        &q2->Y, &q1->Y, &q2->Y, false);
    xp1->Add(xp2);
    xp2->Dispose();

    PolyReader::PrintPolynomial(xp1, 1);

    return xp1;
  }
  return NULL;
}

// ----------------------------------------------------------------------------

//
// Point p is on conic h with parameters H1, H2, H3, H4, H5
//
// (x2, x1) on Conic(u1, u2, u3, u4, u5)
// P = x2^2 + (u1x1 + u3)x2 + u2x1^2 + u4x1 + u5 = 0
//
XPolynomial *CAlgMethod::_PointOnConicCondition(Conic *h, Point *p) {
  XPolynomial *x2 = new XPolynomial(p->X.Free, p->X.Index);
  XPolynomial *x1 = new XPolynomial(p->Y.Free, p->Y.Index);
  XPolynomial *u1 = new XPolynomial(h->H1.Free, h->H1.Index);
  XPolynomial *u2 = new XPolynomial(h->H2.Free, h->H2.Index);
  XPolynomial *u3 = new XPolynomial(h->H3.Free, h->H3.Index);
  XPolynomial *u4 = new XPolynomial(h->H4.Free, h->H4.Index);
  XPolynomial *u5 = new XPolynomial(h->H5.Free, h->H5.Index);

  // x2
  XPolynomial *xp = x2->Clone();
  // x2^2
  xp->Mul(x1);

  // x1
  XPolynomial *xp1 = x1->Clone();
  // u1x1
  xp1->Mul(u1);
  // u1x1 + u3
  xp1->Add(u3);
  // (u1x1 + u3)x2
  xp1->Mul(x2);

  // x2^2 + (u1x1 + u3)x2
  xp->Add(xp1);
  xp1->Dispose();

  // u2x1^2
  xp1 = x1->Clone();
  xp1->Mul(x1);
  xp1->Mul(u2);

  // x2^2 + (u1x1 + u3)x2 + u2x1^2
  xp->Add(xp1);
  xp1->Dispose();

  // x2^2 + (u1x1 + u3)x2 + u2x1^2 + u4x1 + u5
  xp1 = x1->Clone();
  xp1->Mul(u4);
  xp->Add(xp1);
  xp1->Dispose();
  xp->Add(u5);

  x1->Dispose();
  x2->Dispose();
  u1->Dispose();
  u2->Dispose();
  u3->Dispose();
  u4->Dispose();
  u5->Dispose();

  return xp;
}

// ----------------------------------------------------------------------------

//
// Create polynomial for the segment [p, q]
//
// P = (p.x - q.x)^2 + (p.y - q.y)^2
// P = (p.x - q.x)(p.x - q.x) + (p.y - q.y)(p.y - q.y)
// P = (p.x - q.x)(p.x - q.x) - (q.y - p.y)(p.y - q.y)
//
XPolynomial *CAlgMethod::_SegmentSize(Point *p, Point *q) {
  if (p == NULL || q == NULL) {
    return NULL;
  }
  return _HelperCondition(&p->X, &q->X, &p->X, &q->X, &q->Y, &p->Y, &p->Y,
                          &q->Y, false);
}

// ----------------------------------------------------------------------------

//
// Line (p1, p2) is parallel with line (q1, q2)
//
// P = (a1 - b1)(c2 - d2) - (a2 - b2)(c1 - d1)
// P = (p1.x - p2.x)(q1.y - q2.y) - (p1.y - p2.y)(q1.x - q2.x)
//
XPolynomial *CAlgMethod::_ParallelCondition(Point *p1, Point *p2, Point *q1,
                                            Point *q2) {
  if (p1 && p2 && q1 && q2) {
    Log::OutputEnumItem("Line ($%s$, $%s$) parallel with line ($%s$, $%s$)",
                        p1->Name.c_str(), p2->Name.c_str(), q1->Name.c_str(),
                        q2->Name.c_str());
    return _HelperCondition(&p1->X, &p2->X, &q1->Y, &q2->Y, &p1->Y, &p2->Y,
                            &q1->X, &q2->X);
  }
  return NULL;
}

// ----------------------------------------------------------------------------

//
// Create condition
// P = (h1 - h2)(h3 - h4) - (h5 - h6)(h7 - h8)
// P = (h1 - h2)(h3 - h4) + (h6 - h5)(h7 - h8)
//
XPolynomial *CAlgMethod::_HelperCondition(HalfPoint *h1, HalfPoint *h2,
                                          HalfPoint *h3, HalfPoint *h4,
                                          HalfPoint *h5, HalfPoint *h6,
                                          HalfPoint *h7, HalfPoint *h8,
                                          bool print) {
  if (h1 == NULL || h2 == NULL || h3 == NULL || h4 == NULL || h4 == NULL ||
      h5 == NULL || h6 == NULL || h7 == NULL) {
    Log::PrintLogF(0, "_HelperCondition: one of the halfpoints is NULL!\n\n");
    throw - 1;
  }

  // (h1 - h2)(h3 - h4)
  XPolynomial *xp = _HalfHelperCondition(h1, h2, h3, h4);

  // (h6 - h5)(h7 - h8)
  XPolynomial *xp1 = _HalfHelperCondition(h6, h5, h7, h8);
  xp->Add(xp1);
  xp1->Dispose();

  if (print && xp) {
    if (xp->IsZero()) {
      Log::OutputText(" --- true by the construction\n\n");
    } else {
      PolyReader::PrintPolynomial(xp, 1);
    }
  }
  return xp;
}

// ----------------------------------------------------------------------------

//
// P = (h1 - h2)(h3 - h4)
//
XPolynomial *CAlgMethod::_HalfHelperCondition(HalfPoint *h1, HalfPoint *h2,
                                              HalfPoint *h3, HalfPoint *h4) {
  if (h1 == NULL || h2 == NULL || h3 == NULL || h4 == NULL) {
    return NULL;
  }

  return XPolynomial::CreatePolynomialCondition(h1->Free, h1->Index, h2->Free,
                                                h2->Index, h3->Free, h3->Index,
                                                h4->Free, h4->Index);
}

// ----------------------------------------------------------------------------

void CAlgMethod::_ExtractConjecturePolynomial() {
  for (int ii = 0, size = vpConjectures.size(); ii < size; ii++) {
    XPolynomial *xpConjecture =
        _ExtractPolynomialExpression(&vpConjectures[ii]);
    if (xpConjecture &&
        !xpConjecture->IsZero()) // zero polynomial is always true!
    {
      vxpConjectures.push_back(xpConjecture);
    } else if (xpConjecture) {
      xpConjecture->Dispose();
    }
  }
}

// ----------------------------------------------------------------------------

XPolynomial *
CAlgMethod::_ExtractPolynomialExpression(CGCLCProverExpression *e) {
  XPolynomial *xp = NULL, *xp1 = NULL, *xp2 = NULL, *xp3 = NULL;
  Constant *c = NULL;
  Point *p1, *p2, *p3, *p4;

  switch (e->GetType()) {
  case ep_constant:
    // extract constant as a new free variable
    c = _FindConstant(e->GetName());
    if (c == NULL) {
      // create constant
      c = new Constant(e->GetName(), ++lastFreeIndex);
      _constants.emplace_back(c);
    }
    return new XPolynomial(true, c->Index);
    break;
  case ep_inequality:
  case ep_unknown:
  case ep_point: // supported, but not on this place!
    Log::PrintLogF(0, "Not supported expression type %d!\n\n", e->GetType());
    throw - 1;
  case ep_number:
    // create real number
    xp = new XPolynomial(e->GetNumber());
    break;
  case ep_equality:
    // E1 - E2
    xp = this->_ExtractPolynomialExpression(e->GetArgP(0));
    xp1 = this->_ExtractPolynomialExpression(e->GetArgP(1));
    if (xp == NULL || xp1 == NULL) {
      Log::PrintLogF(0, "Expression is NULL!\n");
      throw - 1;
    }
    xp->Subtract(xp1);
    xp1->Dispose();
    break;
  case ep_mult:
    // E1 * E2
    xp = this->_ExtractPolynomialExpression(e->GetArgP(0));
    xp1 = this->_ExtractPolynomialExpression(e->GetArgP(1));
    if (xp == NULL || xp1 == NULL) {
      Log::PrintLogF(0, "Expression is NULL!\n");
      throw - 1;
    }
    xp->Mul(xp1);
    xp1->Dispose();
    break;
  case ep_ratio:
    // E1 / E2
    // this is tricky one
    Log::PrintLogF(0, "Not supported expression type %d!\n\n", e->GetType());
    throw - 1;
    break;
  case ep_segment:
    // |P1 P2|
    xp = this->_SegmentSize(_FindPoint(e->GetArgName(0)),
                            _FindPoint(e->GetArgName(1)));
    break;
  case ep_segment_ratio:
    // sratio P1 P2 P3 P4 = |P1 P2| / |P3 P4|
    // tricky, also contains division!
    Log::PrintLogF(0, "Not supported expression type %d!\n\n", e->GetType());
    throw - 1;
    break;
  case ep_s3:
    // P(P1 P2 P3)
    p1 = _FindPoint(e->GetArgName(0));
    p2 = _FindPoint(e->GetArgName(1));
    p3 = _FindPoint(e->GetArgName(2));

    xp = this->_HelperCondition(&p1->X, &p2->X, &p2->Y, &p3->Y, &p1->Y, &p2->Y,
                                &p2->X, &p3->X, false);
    xp->Mul(0.5);
    break;
  case ep_s4:
    // P(P1 P2 P3 P4)
    p1 = _FindPoint(e->GetArgName(0));
    p2 = _FindPoint(e->GetArgName(1));
    p3 = _FindPoint(e->GetArgName(2));
    p4 = _FindPoint(e->GetArgName(3));

    xp = this->_HelperCondition(&p1->X, &p2->X, &p2->Y, &p3->Y, &p1->Y, &p2->Y,
                                &p2->X, &p3->X, false);
    xp1 = this->_HelperCondition(&p1->X, &p3->X, &p3->Y, &p4->Y, &p1->Y, &p3->Y,
                                 &p3->X, &p4->X, false);
    xp->Add(xp1);
    xp1->Dispose();
    xp->Mul(0.5);
    break;
  case ep_p3:
    // pythagoras difference PD(P1 P2 P3)
    // P1P2^2 + P2P3^2 - P3P1^2
    p1 = _FindPoint(e->GetArgName(0));
    p2 = _FindPoint(e->GetArgName(1));
    p3 = _FindPoint(e->GetArgName(2));

    xp = this->_SegmentSize(p1, p2);
    xp1 = this->_SegmentSize(p2, p3);
    xp->Add(xp1);
    xp1->Dispose();
    xp1 = this->_SegmentSize(p3, p1);
    xp->Subtract(xp1);
    xp1->Dispose();
    break;
  case ep_p4:
    // PD(P1 P2 P3 P4)
    Log::PrintLogF(0, "Not implemented yet %d!\n\n", e->GetType());
    throw - 1;
    break;
  case ep_identical:
    // P1 = P2
    // |P1 P2|
    xp = _IdenticalCondition(_FindOrAddPoint(e->GetArgName(0)),
                             _FindOrAddPoint(e->GetArgName(1)));
    break;
  case ep_collinear:
    // P1 on P2P3
    xp = _PointOnLineCondition(_FindPoint(e->GetArgName(0)),
                               _FindPoint(e->GetArgName(1)),
                               _FindPoint(e->GetArgName(2)));
    break;
  case ep_perpendicular:
    // P1P2 perpendicular with P3P4
    xp = _PerpendicularCondition(
        _FindPoint(e->GetArgName(0)), _FindPoint(e->GetArgName(1)),
        _FindPoint(e->GetArgName(2)), _FindPoint(e->GetArgName(3)));
    break;
  case ep_parallel:
    // P1P2 parallel with P3P4
    xp = _ParallelCondition(
        _FindPoint(e->GetArgName(0)), _FindPoint(e->GetArgName(1)),
        _FindPoint(e->GetArgName(2)), _FindPoint(e->GetArgName(3)));
    break;
  case ep_midpoint:
  case ep_same_length:
    // |P1P2| - |P1P3|
    xp = _EqualSegmentCondition(
        _FindPoint(e->GetArgName(0)), _FindPoint(e->GetArgName(1)),
        _FindPoint(e->GetArgName(2)), _FindPoint(e->GetArgName(3)));
    break;
  case ep_harmonic:
    // Harmonic(P1, P2, P3, P4)
    Log::PrintLogF(0, "Not implemented yet %d!\n\n", e->GetType());
    throw - 1;
    break;
  case ep_diffx:
    // A.x - B.x
    xp = _DiffPoints(&_FindPoint(e->GetArgName(0))->X,
                     &_FindPoint(e->GetArgName(1))->X);
    break;
  case ep_diffy:
    // A.y - B.y
    xp = _DiffPoints(&_FindPoint(e->GetArgName(0))->Y,
                     &_FindPoint(e->GetArgName(1))->Y);
    break;
  case ep_sum:
    // E1 + E2
    xp = this->_ExtractPolynomialExpression(e->GetArgP(0));
    xp1 = this->_ExtractPolynomialExpression(e->GetArgP(1));
    if (xp == NULL || xp1 == NULL) {
      Log::PrintLogF(0, "Expression is NULL!\n");
      throw - 1;
    }
    xp->Add(xp1);
    xp1->Dispose();
    break;
  case ep_algsumzero3:
    /* xp =_AlgSumZero3Expression(_FindPoint(e->GetArgName(0)),
       _FindPoint(e->GetArgName(1)), _FindPoint(e->GetArgName(2)),
       _FindPoint(e->GetArgName(3)), _FindPoint(e->GetArg(4)->GetName()),
       _FindPoint(e->GetArg(5)->GetName()));
    */
    xp1 = this->_ExtractPolynomialExpression(e->GetArgP(0));
    xp2 = this->_ExtractPolynomialExpression(e->GetArgP(1));
    xp3 = this->_ExtractPolynomialExpression(e->GetArgP(2));
    if (xp1 == NULL || xp2 == NULL || xp3 == NULL) {
      Log::PrintLogF(1, "ep_algsumzero3: one of subexpressions is NULL!\n");
      throw - 1;
    }
    xp = _AlgSumZero3Expression(xp1, xp2, xp3);
    xp1->Dispose();
    xp2->Dispose();
    xp3->Dispose();
    break;
  case ep_angle:
    Log::PrintLogF(1, "Angle command should be rationalized!\n");
    throw - 1;
    break;
  case ep_tangens_num:
    // numerator of tangens rational expression
    xp = _TangensNum(_FindPoint(e->GetArgName(0)), _FindPoint(e->GetArgName(1)),
                     _FindPoint(e->GetArgName(2)));
    break;
  case ep_tangens_den:
    // denominator of tangens rational expression
    xp = _TangensDen(_FindPoint(e->GetArgName(0)), _FindPoint(e->GetArgName(1)),
                     _FindPoint(e->GetArgName(2)));
    break;
  default:
    Log::PrintLogF(0, "unknown expression type %d!\n\n", e->GetType());
    throw - 1;
  }

  if (xp == NULL) {
    Log::PrintLogF(0, "Expression type %d is not yet implemented!\n\n",
                   e->GetType());
  }
  return xp;
}

// ----------------------------------------------------------------------------

enum eGCLC_conjecture_status
CAlgMethod::_Prove(const CGCLCProverExpression & /* pConj */) {
  eGCLC_conjecture_status eRet = e_unknown;

  if (!this->ValidConjecture()) {
    Log::PrintLogF(
        1, "Please provide valid conjecture before calling Prove method!\n\n");
    return eRet;
  }

  // extract points
  // Log::OutputSectionBegin("Resolving constructed points");
  // Log::PrintLogF(1, "\\hspace*{4.1mm}\n");
  _FindPoints();

  // extract and resolve lines
  Log::OutputSectionBegin("Resolving constructed lines");
  // Log::OutputText("Finding points that belongs to line, is angle horizontal
  // or vertical in given configuration.");
  // Log::PrintLogF(1, "\\hspace*{4.1mm}\n");
  _FindLinesCircles();

  // now, go through all commands and generate polynomials based on them
  // example:
  // p_inter, A, p, q
  // may generate two polynomials
  //   first A on p
  //   second A on q
  // how many polynomials will actually be generated depends how
  // lines p and q are resolved. If A is used to resolve line p,
  // then "A on p" doesn't generate polynomial, otherwise it does.
  // Same goes for second condition.
  Log::OutputSectionBegin("Creating polynomials from hypotheses");
  Log::OutputEnumBegin("itemize");
  _ExtractConditionPolynomials();
  Log::OutputEnumEnd("itemize");

  // conjecture polynomial
  Log::OutputSectionBegin("Creating polynomial from the conjecture");
  Log::OutputEnumBegin("itemize");
  Log::OutputEnumItem("Processing given conjecture(s).");
  _ExtractConjecturePolynomial();
  Log::OutputEnumEnd("itemize");
  int ii = 0, size = vxpConjectures.size();
  if (size > 0) {
    Log::OutputEnumBegin("description");
    for (; ii < size; ii++) {
      Log::OutputEnumItemBegin();
      Log::OutputEnumDescriptionBold("Conjecture %d:", ii + 1);
      PolyReader::PrintPolynomial(vxpConjectures[ii], 1);
      Log::OutputEnumItemEnd();
    }
    Log::OutputEnumEnd("description");
  } else {
    Log::OutputBold("Conjecture always holds.");
  }
  Log::OutputSectionBegin("Invoking the theorem prover");

  // now call internal prover on a given polynomial system
  Log::OutputText("The used proving method is %s.\n\n",
                  _prover->Description().c_str());

  eRet = e_proved;
  _maxt = 0;

  for (ii = 0, size = vxpConjectures.size(); ii < size && eRet == e_proved;
       ii++) {
    // when prover is called second time, input system is
    // already triangular!

    double timeout = m_iProverTimeout < 0 ?
                      -1 :
                      std::max(0.0, (double)m_iProverTimeout - m_Timer.ElapsedTime());

    // proof status
    PROVER_STATUS ps = _prover->Prove(vxps, vxpConjectures[ii], ii, timeout);

    if (ps == PROVER_STATUS_PROVED) {
      if (size > 1) {
        Log::PrintLogF(3, "Conjecture %d is True...\n\n", ii + 1);
      }
    } else if (ps == PROVER_STATUS_TIMEOUT) {
      eRet = e_unknown_timeout;
    } else if (ps == PROVER_STATUS_NONE) {
      eRet = e_unknown;
    } else if (ps == PROVER_STATUS_DISPROVED) {
      eRet = e_disproved;
    } else if (ps == PROVER_STATUS_COMPLEX) {
      eRet = e_unknown_toomanysteps;
    } else {
      eRet = e_unknown;
    }

    if (_prover->GetMaxTerms() > _maxt) {
      _maxt = _prover->GetMaxTerms();
    }
  }
  return eRet;
}

// ----------------------------------------------------------------------------

void CAlgMethod::PrintProofFooter(eGCLC_conjecture_status eRet) {
  double sec = m_Timer.ElapsedTime()/(double)1000;

  // is this needed?
  Log::OutputSectionBegin("Prover report");
  Log::OutputSectionEnd();

  Log::OutputEnumBegin("description");

  Log::OutputDescriptionItemBegin("Status:");

  if (eRet == e_proved) {
    Log::OutputDescriptionItemEnd("The conjecture has been proved.\n\n");
  } else if (eRet == e_unknown) {
    Log::OutputDescriptionItemEnd(
        "The conjecture has not been proved nor disproved.\n\n");
  } else if (eRet == e_disproved) {
    Log::OutputDescriptionItemEnd("The conjecture has been disproved.\n\n");
  } else if (eRet == e_unknown_timeout) {
    Log::OutputDescriptionItemEnd("Program has reach its timeout!\n\n");
  } else {
    Log::OutputDescriptionItemEnd(
        "The conjecture has not been proved. Returned status is unknown.\n\n");
  }

  Log::OutputDescriptionItemBegin("Space Complexity:");
  Log::OutputDescriptionItemEnd("The biggest polynomial obtained during proof "
                                "process contained %u terms.\n\n",
                                _maxt);

  Log::OutputDescriptionItemBegin("Time Complexity:");
  Log::OutputDescriptionItemEnd("Time spent by the prover: %.3f seconds. ",
                                sec);

  PrintNDGConditions(true);
  // PrintMethodSpecificOutput();

  Log::OutputEnumEnd("description");

  // close outputs
  Log::PrintLogF(1, "\\end{document}\n");

  // xml footer
  // Log::PrintLogF(2, "\n\t<time>%.3f</time>", sec);
  Log::PrintLogF(2, "\n\n</main_proof>\n\n");
}

// ----------------------------------------------------------------------------

//
// Return true if points are collinear by the construction
// Function may return false even if points are collinear
// but in that case that is the theorem
//
bool CAlgMethod::_CollinearPoints(Point *a, Point *b, Point *c) {
  bool retValue = false;
  Line *l = _FindLine(a, b);

  if (l) {
    // does this line contains point c
    for (int ii = 0, size = l->Points.size(); ii < size && !retValue; ii++) {
      retValue = l->Points[ii] == c;
    }
  }

  return retValue;
}

// ----------------------------------------------------------------------------

CGCLCProverExpression *CAlgMethod::_CloneExpression(CGCLCProverExpression *e) {
  CGCLCProverExpression *eClone = new CGCLCProverExpression(*e);
  return eClone;
}

// ----------------------------------------------------------------------------

bool CAlgMethod::_RationalizeConjecture(CGCLCProverExpression *conjecture) {
  CGCLCProverExpression exprToRationalize;
  CGCLCProverExpression eNum, eDen;
  CGCLCProverExpression *eA, *eB, *eC, *eD;
  GCLCexpression_type diffType;
  Line *l;
  bool foundRational = false;
  bool rationalized = false;
  Point *A, *B, *C, *D;

  do {
    // find if exists sub-expression not rationalized yet
    foundRational = _FindRationalExpression(*conjecture, exprToRationalize);

    /*
    conjecture->PrettyPrint();
    cout << "----------" << endl;

    exprToRationalize.PrettyPrint();
    cout << "----------" << endl;
*/
    if (foundRational) {
      // rationalize it
      // find form A/B, then replace it with A and propagate B up
      // A = eNum
      // B = eDen

      rationalized = true;

      switch (exprToRationalize.GetType()) {
      case ep_ratio:
        eNum = exprToRationalize.GetArg(0);
        eDen = exprToRationalize.GetArg(1);
        break;

      case ep_segment_ratio:
        // add new conjecture that two lines are parallel
        // quick check - if points are collinear don't create new conjecture
        eA = exprToRationalize.GetArgP(0);
        eB = exprToRationalize.GetArgP(1);
        eC = exprToRationalize.GetArgP(2);
        eD = exprToRationalize.GetArgP(3);
        A = _FindPoint(eA->GetName());
        B = _FindPoint(eB->GetName());
        C = _FindPoint(eC->GetName());
        D = _FindPoint(eD->GetName());
        if (A == NULL || B == NULL || C == NULL || D == NULL) {
          Print(std::cout, "ep_segment_ratio: some of the points is NULL!\n");
          throw - 1;
        }

        if (!_CollinearPoints(A, B, C) || !_CollinearPoints(A, B, D)) {
          // create ep_parallel conjecture
          vpConjectures.push_back(
              CGCLCProverExpression(ep_parallel, eA->GetName(), eB->GetName(),
                                    eC->GetName(), eD->GetName()));
          vsConjectures.push_back("" /*NULL*/);
          vpOrigConjectures.push_back(0.0);
        } else {
          Log::PrintLogF(
              1, "Points are on the same line, sratio pre-condition is ok\n\n");
        }

        diffType = ep_diffy;
        // numerator: A.x - B.x (or A.y - B.y if parallel with Y axis)
        l = _FindLine(_FindPoint(eA->GetName()), _FindPoint(eB->GetName()));
        if (l && l->Angle == 0) {
          diffType = ep_diffx;
        }
        // try other line also
        l = _FindLine(_FindPoint(eC->GetName()), _FindPoint(eD->GetName()));
        if (l && l->Angle == 0) {
          diffType = ep_diffx;
        }
        eNum = CGCLCProverExpression(diffType, eA->GetName(), eB->GetName());
        // denominator: C.x - D.x (or C.y - D.y if parallel with Y axis)
        eDen = CGCLCProverExpression(diffType, eC->GetName(), eD->GetName());

        // !!! FIXME
        // this is not yet correct
        // sometimes it is not simple to determine is line parallel
        // to X or Y axis. For instance, line connecting medians of
        // triangle is parallel to oposite segment.
        break;

      case ep_angle:
        // tangens(b,a,c) = tannum(b,a,c)/tanden(b,a,c)
        eA = exprToRationalize.GetArgP(0);
        eB = exprToRationalize.GetArgP(1);
        eC = exprToRationalize.GetArgP(2);
        // tannum(b,a,c)
        eNum = CGCLCProverExpression(ep_tangens_num, eA->GetName(),
                                     eB->GetName(), eC->GetName());
        // tanden(b,a,c)
        eDen = CGCLCProverExpression(ep_tangens_den, eA->GetName(),
                                     eB->GetName(), eC->GetName());
        break;

      default:
        Log::PrintLogF(0, "Unsupported ration expression type!\n");
        throw - 1;
      }

      // replace it with eNum
      CGCLCProverExpression *parent;
      int index;
      bool b = exprToRationalize.GetParentIndex(*conjecture, parent, index);
      assert(b);
      parent->SetArg(index, eNum);

      // propagate eDen
      _PropagateRationalExpression(conjecture, parent, &eDen, index);
    }
  } while (foundRational);

  return rationalized;
}

// ----------------------------------------------------------------------------

//
// propagate e1 starting from e and above
//
void CAlgMethod::_PropagateRationalExpression(CGCLCProverExpression *conjecture,
                                              CGCLCProverExpression *e,
                                              CGCLCProverExpression *eDen,
                                              int index) {
  CGCLCProverExpression *er = NULL;
  // bool e1reffed = false;

  switch (e->GetType()) {
  case ep_equality:
  // equality A B | e1
  //
  // index = 1
  // equality {mult A e1} B
  //
  // index = 0
  // equality A {mult B e1}
  case ep_sum:
  // similar as equality, but must propagate further
  case ep_mult:
    // only propagate futher
    if (e->GetType() != ep_mult) {
      // create helper expression
      er = new CGCLCProverExpression(ep_mult, e->GetArg(1 - index), *eDen);
      // replace, replaced expression is linked and no need to delete it
      e->SetArg(1 - index, er);
    }

    if (e->GetType() != ep_equality) {
      CGCLCProverExpression *parent;
      int index;
      e->GetParentIndex(*conjecture, parent, index);
      _PropagateRationalExpression(conjecture, parent, eDen, index);
    }
    break;
  case ep_ratio:
    // should not happen
    Log::PrintLogF(0, "_PropagateRationalExpression, unrationalized ratio "
                      "found above rational!\n");
    throw - 1;
  default:
    // should not happen
    Log::PrintLogF(
        0,
        "Failed in _PropagateRationalExpression, wrong expression type %d!\n",
        e->GetType());
    throw - 1;
  }
}

// ----------------------------------------------------------------------------

bool CAlgMethod::_FindRationalExpression(const CGCLCProverExpression &e,
                                         CGCLCProverExpression &ratExp) {
  switch (e.GetType()) {
  case ep_ratio:
  case ep_segment_ratio:
  case ep_angle:
    ratExp = e;
    return true;

  default:
    for (unsigned ii = 0; ii < arity(e.GetType()); ii++) {
      // find rational expression recursively
      if (_FindRationalExpression(e.GetArg(ii), ratExp))
        return true;
    }
  }

  return false;
}

// ----------------------------------------------------------------------------

void CAlgMethod::_PrintConjecture(const CGCLCProverExpression &e, int level,
                                  int space) {
  unsigned ii;
  for (ii = 0; ii < (unsigned)space; ii++)
    Log::PrintLogF(level, "  ");
  if (e.GetType() == ep_point)
    Log::PrintLogF(level, "$%s$", e.GetName().c_str());
  else
    Log::PrintLogF(level, "%s", e.GetName().c_str());
  if (e.GetType() == ep_number)
    Log::PrintLogF(level, "  [%lf]", e.GetNumber());
  Log::PrintLogF(level, "\n");
  // recursively print children
  for (ii = 0; ii < arity(e.GetType()); ii++)
    _PrintConjecture(e.GetArg(ii), level, space + 1);
}

// ----------------------------------------------------------------------------

bool CAlgMethod::SetProverConjecture(const std::string &strConjecture) {
  int nInputPos = 0;
  m_bValidConjecture = false;

  // resolve points and lines!
  // need to do it here because there may be implicit point needed by
  // expression!
  _FindPoints();
  if (!_FindLinesCircles())
    return false;

  if (!GetExpression(strConjecture, nInputPos, m_InitialConjecture)) {
    Log::PrintLogF(1, "Failed to Get Expression from input string!\n");
    return false;
  }
  m_InitialConjecture.ToGeometricQuantities();

  CGCLCProverExpression orig_conjecture = m_InitialConjecture;

  switch (m_InitialConjecture.GetType()) {
  case ep_equality:
  case ep_identical:
  case ep_collinear:
  case ep_perpendicular:
  case ep_parallel:
  case ep_midpoint:
  case ep_same_length:
  case ep_harmonic:
  case ep_algsumzero3:
  case ep_angle:
    // m_bValidConjecture = (conjecture != NULL);
    m_bValidConjecture = true;
    break;
  default:
    Log::PrintLogF(0, "Wrong conjecture type %d!\n\n", m_InitialConjecture.GetType());
    break;
  }

  // harmonic conjecture in a form of a sratio
  if (m_InitialConjecture.GetType() == ep_harmonic) {
    // remember original conjecture
    vpOrigConjectures.push_back(m_InitialConjecture);
    std::string A = m_InitialConjecture.GetArgName(0);
    std::string B = m_InitialConjecture.GetArgName(1);
    std::string C = m_InitialConjecture.GetArgName(2);
    std::string D = m_InitialConjecture.GetArgName(3);
    m_InitialConjecture = CGCLCProverExpression(
        ep_equality, CGCLCProverExpression::sratio(A, C, C, B),
        CGCLCProverExpression::sratio(D, A, D, B));

    _RationalizeConjecture(&m_InitialConjecture);
    vpConjectures.push_back(m_InitialConjecture);
    vsConjectures.push_back(strConjecture);
  } else {

    // Rationalize conjecture
    // ie remove ratio, sratio and similar commands
    // at the same time, add more conjectures if needed
    // (eg sratio implicitly implies parallel lines)
    bool rationalized = false;
    rationalized = _RationalizeConjecture(&m_InitialConjecture);
    // add it to the system
    vpConjectures.push_back(m_InitialConjecture);
    vsConjectures.push_back(strConjecture);

    if (rationalized)
      vpOrigConjectures.push_back(orig_conjecture);
    else
      vpOrigConjectures.push_back(0.0);
  }

  m_pConjecture = &m_InitialConjecture;
  return m_bValidConjecture;
}

// ----------------------------------------------------------------------------

void CAlgMethod::SetPrintPoints(bool printPoints) {
  _printPoints = printPoints;
}

// ----------------------------------------------------------------------------

void CAlgMethod::PrintMethodSpecificOutput() {
  Log::PrintLogF(1, "The biggest polynomial obtained during proof process "
                    "contained %u terms.\n\n",
                 _maxt);
  Log::PrintLogF(2, "\n\t<prover_report><maximal_polynomial_terms>%d</"
                    "maximal_polynomial_terms></prover_report>",
                 _maxt);
}

// ----------------------------------------------------------------------------

std::string CAlgMethod::GetMethodSpecificOutput() {
  return "\nThe theorem prover based on the " + GetName() +
         " method used.\nThe largest "
         "polynomial obtained during the proof process contains " +
         i2s(_maxt) + " terms.\n";
}

// ----------------------------------------------------------------------------

void CAlgMethod::PrintProofTitleXML(const std::string &theoremName) {
  // XML output
  std::string name = "GCLC Prover Output for conjecture " +
                     (!theoremName.empty() ? theoremName
                                           : (std::string) "[not named] (" +
                                                 GetName() + " method used)");
  Print(m_hXMLOutputProof, "\n<proof_title>" + name + +"</proof_title>\n\n");
}

// ----------------------------------------------------------------------------
