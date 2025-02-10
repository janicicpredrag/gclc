// AlgMethod.cpp: implementation of the CAlgMethod class,
// functions related to reducibility problem.
//
//////////////////////////////////////////////////////////////////////

#include "AlgMethod.h"
#include "../AlgebraicMethods/Log.h"
#include "../AlgebraicMethods/PolyReader.h"
#include "../AlgebraicMethods/Reduce.h"

// ----------------------------------------------------------------------------

// is this point used in any other expression than commandSkip
bool CAlgMethod::_UsedPoint(Point *p,
                            std::list<CGCLCProverCommand>::iterator commandSkip) {
  bool checkArg0, checkArg1, checkArg2, checkArg3;

  for (std::list<CGCLCProverCommand>::iterator it = m_ProverCommands.begin();
       it != m_ProverCommands.end(); it++) {
    if (it != commandSkip) {
      checkArg0 = checkArg1 = checkArg2 = checkArg3 = false;
      switch (it->type) {
      case p_point: // point      A
        // skip this because point may be defined anyway
        break;
      // there is a point in command
      // points are capital letter
      case p_line:   // line       a A B
      case p_circle: // circle     k A B
      case med:      // med m A B
        // args 1 and 2
        checkArg1 = checkArg2 = true;
        break;
      case online: // online    A B C or online A p
        if (!it->arg[2].empty()) {
          checkArg0 = checkArg1 = checkArg2 = true;
        } else {
          checkArg0 = true;
        }
        break;
      case midpoint: // midpoint  A B C
      case oncircle: // oncircle  A B C
        // args 0, 1 and 2
        checkArg0 = checkArg1 = checkArg2 = true;
        break;
      case p_bis: // bis       s B A C
        // args 1, 2 and 3
        checkArg1 = checkArg2 = checkArg3 = true;
        break;
      case p_pratio: // tratio D C B A k
        // args 0, 1, 2 and 3
        checkArg0 = checkArg1 = checkArg2 = checkArg3 = true;
        break;
      case p_inter: // intersec  A p q
        // arg 0
        checkArg0 = true;
        break;
      case p_interlc: // intersec2 A B p q
      case p_foot:    // foot      X A p
        // args 0 and 1
        checkArg0 = checkArg1 = true;
        break;
      case perp:     // perp      p A q
      case parallel: // parallel  p A q
        //  arg 1
        checkArg1 = true;
        break;
      default:
        Log::PrintLogF(0, "Unknown or unsupported command %d!\n\n", it->type);
        throw - 1;
      }

      if ((checkArg0 && _FindPoint(it->arg[0]) == p) ||
          (checkArg1 && _FindPoint(it->arg[1]) == p) ||
          (checkArg2 && _FindPoint(it->arg[2]) == p) ||
          (checkArg3 && _FindPoint(it->arg[3]) == p)) {
        return true;
      }
    }
  }

  // check also conjectures!
  return _UsedPointInConjecture(p);
}

// ----------------------------------------------------------------------------

bool CAlgMethod::_UsedPointInConjecture(Point *p) {
  // it's enough to search conjecture strings
  // find a pattern [separator]p[separator]
  // separator may be: {}' ''\t\n
  int ii, size, index, jj;
  for (ii = 0, size = vsConjectures.size(); ii < size; ii++) {
    std::string str = vsConjectures[ii];
    index = 0;
    // searching pattern [%s] in string [%s]\n", p->Name, str);
    while (str[index]) {
      if (str[index] == ' ' || str[index] == '{' || str[index] == '\\' ||
          str[index] == '\t' || str[index] == '\r' || str[index] == '\n') {
        // start searching
        index++;
        jj = 0;
        // compare names
        while (str[index] && p->Name[jj] && str[index] == p->Name[jj]) {
          index++;
          jj++;
        }
        if (str[index] && !(p->Name[jj]) &&
            (str[index] == ' ' || str[index] == '}' || str[index] == '\\' ||
             str[index] == '\t' || str[index] == '\r' || str[index] == '\n')) {
          // found pattern!
          return true;
        }
      } else {
        index++;
      }
    }
  }

  Log::OutputEnumItem("Point $%s$ not used in theorem.", p->Name.c_str());
  return false;
}

// ----------------------------------------------------------------------------

//
// Solve this reducibility case
// p1 != p2
//
void CAlgMethod::_ReducibilityLineCircleIntersection(Point *p1, Point *p2,
                                                     Line *l, Circle *k,
                                                     Circle *k1) {
  // create conditions and then reduce with
  // p1.X != p2.X or p1.Y != p2.Y
  // XPolynomial* xp = NULL;
  vxp polySystem;

  if (l) {
    _AddCondition(_PointOnLineCondition(p1, l->p1, l->p2), false, &polySystem);
    _AddCondition(_PointOnLineCondition(p2, l->p1, l->p2), false, &polySystem);
  }
  if (k) {
    _AddCondition(_EqualSegmentCondition(p2, k->c, k->p, k->c), false,
                  &polySystem);
    _AddCondition(_EqualSegmentCondition(p1, k->c, k->p, k->c), false,
                  &polySystem);
  }
  if (k1) {
    _AddCondition(_EqualSegmentCondition(p2, k1->c, k1->p, k1->c), false,
                  &polySystem);
    _AddCondition(_EqualSegmentCondition(p1, k1->c, k1->p, k1->c), false,
                  &polySystem);
  }

  // call Reduce method for solving reducibility of this type
  // create polynomials for point coordinates
  std::vector<bool> vecPointsFree;
  std::vector<unsigned int> vecPointsIndex;
  vecPointsFree.push_back(p1->X.Free);
  vecPointsIndex.push_back(p1->X.Index);
  vecPointsFree.push_back(p1->Y.Free);
  vecPointsIndex.push_back(p1->Y.Index);
  vecPointsFree.push_back(p2->X.Free);
  vecPointsIndex.push_back(p2->X.Index);
  vecPointsFree.push_back(p2->Y.Free);
  vecPointsIndex.push_back(p2->Y.Index);
  if (k) {
    vecPointsFree.push_back(k->c->X.Free);
    vecPointsIndex.push_back(k->c->X.Index);
    vecPointsFree.push_back(k->c->Y.Free);
    vecPointsIndex.push_back(k->c->Y.Index);
  }
  if (k1) {
    vecPointsFree.push_back(k1->c->X.Free);
    vecPointsIndex.push_back(k1->c->X.Index);
    vecPointsFree.push_back(k1->c->Y.Free);
    vecPointsIndex.push_back(k1->c->Y.Index);
  }
  bool reduced = Reduce::ReduceLineCircleIntersection(
      vecPointsFree, vecPointsIndex, polySystem);

  if (reduced) {
    Log::PrintLogF(
        1, "Reducible case detected, adding NDG condition $%s \\neq %s$\n\n",
        p1->Name.c_str(), p2->Name.c_str());
  }

  // push back polynomials into main system
  for (int ii = 0, size = polySystem.size(); ii < size; ii++) {
    _AddCondition(polySystem[ii], true);
  }
  polySystem.clear();
}
