#include "PolyReader.h"
#include "XPolynomial.h"
#include <cstring>
#include <iostream>
#include <memory>
#include <new>
#include <sstream>
#include <string>

XPolynomial::XPolynomial() { COSTR("xpoly"); }

XPolynomial::~XPolynomial() { DESTR("xpoly"); }

int XPolynomial::_maxt = 0;
int XPolynomial::maxt() { return _maxt; }

//
// Create polynomial of the real number
//
XPolynomial::XPolynomial(REAL x) {
  if (x != 0) {
    XTerm *xt = new XTerm();
    UPolynomialFraction *uf = new UPolynomialFraction(x);
    xt->SetUFraction(uf);
    uf->Dispose();

    this->AddTerm(xt);
    xt->Dispose();
  }

  COSTR("xpoly");
}

//
// Create polynomial of the single variable
//
XPolynomial::XPolynomial(bool free, int index) {
  if (free && index == 0) {
    // empty polynomial, ie zero polynomial
  } else {
    XTerm *xt = new XTerm();
    UPolynomialFraction *uf = new UPolynomialFraction(1);

    if (free) {
      Power *up = new Power(index, 1, VAR_TYPE_U);

      UTerm *ut = (UTerm *)uf->GetNumerator()->GetTerm(0);
      ut->AddPower(up);
      up->Dispose();
    } else {
      Power *xpow = new Power(index, 1, VAR_TYPE_X);

      xt->AddPower(xpow);
      xpow->Dispose();
    }

    xt->SetUFraction(uf);
    uf->Dispose();
    this->AddTerm(xt);
    xt->Dispose();
  }

  COSTR("xpoly");
}

// helper methods to create polynomial conditions

//
// Create polynomial
// (p1 - p2)(p3 - p4)
// p1p3 - p2p3 - p1p4 + p2p4
//
XPolynomial *XPolynomial::CreatePolynomialCondition(bool f1, uint index1,
                                                    bool f2, uint index2,
                                                    bool f3, uint index3,
                                                    bool f4, uint index4) {
  XPolynomial *xp = new XPolynomial();

  XTerm *xt = XTerm::CreatePolynomialConditionTerm(f1, index1, f3, index3);
  if (xt) {
    xp->AddTerm(xt);
    xt->Dispose();
    // PolyReader::PrintPolynomial(xp);
  }

  xt = XTerm::CreatePolynomialConditionTerm(f2, index2, f3, index3);
  if (xt) {
    xt->Inverse();
    xp->AddTerm(xt);
    xt->Dispose();
    // PolyReader::PrintPolynomial(xp);
  }

  xt = XTerm::CreatePolynomialConditionTerm(f1, index1, f4, index4);
  if (xt) {
    xt->Inverse();
    xp->AddTerm(xt);
    xt->Dispose();
    // PolyReader::PrintPolynomial(xp);
  }

  xt = XTerm::CreatePolynomialConditionTerm(f2, index2, f4, index4);
  if (xt) {
    xp->AddTerm(xt);
    xt->Dispose();
    // PolyReader::PrintPolynomial(xp);
  }

  return xp;
}

XPolynomial *XPolynomial::Clone() {
  XPolynomial *xpClone = new XPolynomial();

  _terms->EnumReset();
  while (_terms->EnumMoveNext()) {
    XTerm *xtClone = ((XTerm *)_terms->EnumGetCurrent())->Clone();
    xpClone->AddTerm(xtClone);
    xtClone->Dispose();
  }

  return xpClone;
}

TERM_TYPE XPolynomial::Type() const { return XTERM; }

uint XPolynomial::GetTotalTermCount() {
  uint cnt = 0;

  _terms->EnumReset();
  while (_terms->EnumMoveNext()) {
    XTerm *xt = (XTerm *)_terms->EnumGetCurrent();
    cnt += xt->GetUFraction()->GetNumerator()->GetTermCount();
  }

  if (cnt > (uint)_maxt) {
    _maxt = cnt;
  }

  return cnt;
}

// arithmetic operations

//
// do spol operation with another polynomial
// SPol is important operation in Groebner basis method
//
// xp1 = c1m1 + f1
// xp2 = c2m2 + f2
//
// m = LCD(m1, m2) - least common denominator
// m = s1m1 = s2m2
//
// spol = c2s1f1 - c1s2f2
//
void XPolynomial::SPol(XPolynomial *xp) {
  XTerm *lt1, *lt2, *s1c2, *s2c1, *t;
  uint i;

  if (this->IsZero() || xp->IsZero()) {
    Log::PrintLogF(1, "One of the polynomials is zero!\n\n");
    throw - 1;
  }

  // leading terms
  lt1 = (XTerm *)this->GetTerm(0);
  lt2 = (XTerm *)xp->GetTerm(0);

  // s1 * c2
  s1c2 = lt2->Clone();
  // s1c2->SetUFraction(lt2->GetUFraction()->Clone());
  // safe division
  s1c2->DivideMonoms(lt1);

  // s2 * c1
  s2c1 = lt1->Clone();
  // s2c1->SetUFraction(lt1->GetUFraction()->Clone());
  // safe division
  s2c1->DivideMonoms(lt2);

  // multiply with -1
  s2c1->Inverse();

  std::shared_ptr<TermStorage> tmpTerms = Term::CreateTermStorage();

  // c2 * s1 * f1
  uint size = this->GetTermCount();
  for (i = 1; i < size; i++) {
    t = s1c2->Clone();
    t->Mul(this->GetTerm(i));
    tmpTerms->AddTerm(t);
    t->Dispose();
  }

  // - c1 * s2 * f2
  size = xp->GetTermCount();
  for (i = 1; i < size; i++) {
    t = s2c1->Clone();
    t->Mul(xp->GetTerm(i));
    tmpTerms->AddTerm(t);
    t->Dispose();
  }

  s1c2->Dispose();
  s2c1->Dispose();

  // now replace term storages
  _terms = tmpTerms;
}

//
// this = prem(this, xp, x_index)
//
// Calculate pseudo remainder over variable x_index
// of this polynomial with given xp polynomial
//
// Algorithm: prem(p, q, x)
//
// 1. write polynomials as polynomials over single variable x
//      p = p1 * x^v1 + p2
//      q = q1 * x^v2 + q2
//
// 2.
//      v = min(v1, v2)
//
// 3.
//      prem(p, q, x) = q1 * p / x^(v2 - v) - p1 * q / x^(v1 - v)
//
// The result of pseudo remainder is that degree of
// variable x is reduced
//
// Return value:
//         true : pseudo remainder is calculated
//        false : pseudo remainder could not be calculated
//                because right side is greater than left side
//
void XPolynomial::PseudoRemainder(XPolynomial *xp, int index, bool free,
                                  XPolynomial *xpDivisionResult) {
  int logLevel = 5;
  Log::PrintLogF(logLevel, "Calculating pseudo remainder over variable x%d of "
                           "polynomials p and q:\n",
                 index);
  PolyReader::PrintPolynomial(this, logLevel);
  PolyReader::PrintPolynomial(xp, logLevel);

  bool repeat;
  do {
    repeat = this->_PseudoRemainder(xp, index, free, xpDivisionResult);
  } while (repeat);
}

//
// Helper method for prem() function
// Do one reduction
//
bool XPolynomial::_PseudoRemainder(XPolynomial *xp, int index, bool free,
                                   XPolynomial *xpDivisionResult) {
  int logLevel = 5;

  // leading monoms over variable index
  XTerm *lm1 = (XTerm *)this->LeadTerm(index, free);
  XTerm *lm2 = (XTerm *)xp->LeadTerm(index, free);

  if (lm1) {
    Log::PrintLogF(logLevel, "\\hspace{20pt} power of variable x%d is %d, "
                             "maxt1 = %d, maxt2 = %d\n\n",
                   index, lm1->GetPowerByIndex(index)->GetDegree(),
                   this->GetTotalTermCount(), xp->GetTotalTermCount());
    if (Log::StopAfterMaxT > 0 &&
        this->GetTotalTermCount() > (uint)Log::StopAfterMaxT) {
      Log::PrintLogF(1, "stopping wu alg because maxt = %d has overriched "
                        "maximum alowed value of %d\n\n",
                     this->GetTotalTermCount(), Log::StopAfterMaxT);
      throw - 1;
    }
  }

  if (lm1 == NULL) {
    Log::PrintLogF(logLevel, "PseudoRemainder() left side degree is zero!\n");
    return false;
  }

  // variable degrees
  int varDeg1 = lm1->VariableDeg(index, free);
  int varDeg2 = lm2 == NULL ? 0 : lm2->VariableDeg(index, free);

  if (varDeg1 < varDeg2) {
    Log::PrintLogF(
        logLevel,
        "PseudoRemainder() left side degree is less than right side degree!\n");
    return false;
  }

  // divide both polynomials (lm(p,x) and lm(q,x)) with smaller degree
  int deg = varDeg1 < varDeg2 ? varDeg1 : varDeg2;

  // lm(p, x) is polynomial subset of p where
  // each term has varDeg1 degree of variable with index index

  // create lm(p, x)
  XPolynomial *p = new XPolynomial();
  for (int ii = 0, size = this->GetTermCount(); ii < size; ii++) {
    XTerm *xt = (XTerm *)this->GetTerm(ii);

    // add only terms where degree of variable index match it highest degree
    if (xt->VariableDeg(index, free) == varDeg1) {
      XTerm *xtClone = NULL;

      // iterate sub terms if free variable is in question
      if (free) {
        // THIS WORKS ONLY FOR LINEAR CASES!!!
        Polynomial *pp = xt->GetUFraction()->GetNumerator();
        xtClone = xt->ClonePowers();
        for (int jj = 0 /*, size1 = pp->GetTermCount()*/; jj < size; jj++) {
          Term *tt = pp->GetTerm(jj);
          if (tt->VariableDeg(index, free) == varDeg1) {
            Term *ttc = tt->Clone();
            xtClone->GetUFraction()->GetNumerator()->AddTerm(ttc);
            ttc->Dispose();
          }
        }
      } else {
        // remove degree
        xtClone = xt->Clone();
        xtClone->ChangePowerDegree(index, -deg);
      }

      // add it to the polynomial
      p->AddTerm(xtClone);
      xtClone->Dispose();
    }
  }

  // create lm(q, x)
  // it is zero if lm2 is null
  XPolynomial *q = new XPolynomial();
  if (lm2 != NULL) {
    for (int ii = 0, size = xp->GetTermCount(); ii < size; ii++) {
      XTerm *xt = (XTerm *)xp->GetTerm(ii);

      // add only terms where degree of variable index match it highest degree
      if (xt->VariableDeg(index, free) == varDeg2) {
        // remove degree
        XTerm *xtClone = xt->Clone();
        xtClone->ChangePowerDegree(index, -deg);

        // add it to the polynomial
        q->AddTerm(xtClone);
        xtClone->Dispose();
      }
    }
  } else {
    XTerm *unit = new XTerm();
    UPolynomialFraction *unitFraction = new UPolynomialFraction(1);
    unit->SetUFraction(unitFraction);
    unitFraction->Dispose();
    q->AddTerm(unit);
    unit->Dispose();
  }

  Log::PrintLogF(logLevel,
                 "\\hspace{20pt}              maxtP = %d, maxtQ = %d\n\n",
                 p->GetTotalTermCount(), q->GetTotalTermCount());

  Log::PrintLogF(logLevel, "lm1(p, x%d) = \n", index);
  PolyReader::PrintPolynomial(p, logLevel);
  Log::PrintLogF(logLevel, "lm1(q, x%d) = \n", index);
  PolyReader::PrintPolynomial(q, logLevel);

  XPolynomial *xpClone = xp->Clone();
  xpClone->Mul(p);

  Log::PrintLogF(logLevel, "p1 * lm(p0) = \n");
  PolyReader::PrintPolynomial(xpClone, logLevel);

  this->Mul(q);

  Log::PrintLogF(logLevel, "p0 * lm(p1) = \n");
  PolyReader::PrintPolynomial(this, logLevel);

  this->Subtract(xpClone);

  if (xpDivisionResult) {
    xpDivisionResult->Add(p);
  }
  xpClone->Dispose();
  p->Dispose();
  q->Dispose();

  Log::PrintLogF(logLevel, "prem(p, q, x%d) = \n", index);
  PolyReader::PrintPolynomial(this, logLevel);

  Log::PrintLogF(logLevel, "\\hspace{20pt}              new maxt = %d\n\n",
                 this->GetTotalTermCount());

  return true;
}

//
// Simple reduction of the fractions associated
// with polynomial terms
//
void XPolynomial::SimpleReduction() {
  uint size = this->GetTermCount();
  for (uint k = 0; k < size; k++) {
    XTerm *xt = (XTerm *)this->GetTerm(k);
    xt->GetUFraction()->SimpleReduction();
  }
}

//
// latex output
//
char *XPolynomial::PrintLatex() const {
  std::ostringstream ss;

  uint size = this->GetTermCount();
  for (uint ii = 0; ii < size; ii++) {
    if (ii > 0) {
      // Log::PrintLogF(0, " + ");
      ss << '+';
    }
    this->GetTerm(ii)->PrintLatex(ss);
  }
  if (size == 0) {
    ss << '0';
  }

  const std::string str = ss.str();
  char *res = new char[str.size() + 1];
  memcpy(res, str.c_str(), str.size() + 1);

  // +- => -
  // -+ => -
  // N1x => Nx
  // N1u => Nu
  // where N is non-number char
  _ResReplace(res, '+', '-', '-', false);
  _ResReplace(res, '-', '+', '-', false);

#if 1
  // not safe for short latex output!
  // u_1x_2
  _ResReplace(res, '1', 'x', 'x', true);
  _ResReplace(res, '1', 'u', 'u', true);
#endif

  return res;
}

void XPolynomial::PrintLatex(std::ostream & /* sb */) const {}

//
// Improve latex output by removing unit constants
// and merging +- combinations of operators
//
void XPolynomial::_ResReplace(char *res, char l1, char l2, char r1,
                              bool nnCond) const {
  bool replaced = true;
  while (replaced) {
    replaced = false;

    // search pattern
    int ii = 0;
    while (replaced == false && res[ii]) {
      if (res[ii] == l1 && res[ii + 1] == l2) {
        // check non-number condition
        if (!(nnCond && ii > 0 &&
              ((res[ii - 1] <= '9' && res[ii - 1] >= '0') || res[ii] == '.'))) {
          // pattern is found!
          replaced = true;

          // shift left
          res[ii] = r1;
          for (int jj = ii + 1; res[jj]; jj++) {
            res[jj] = res[jj + 1];
          }
        }
      }

      ii++;
    }
  }
}
