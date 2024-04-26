#include "Term.h"
#include "TermStorage.h"
#include "TermStorageAvl.h"
#include "TermStorageVector.h"
#include "XTerm.h"
#include <memory>

Term::Term() {
#if 0
	static int cnt = 0;

	if (cnt == 12237)
	{
		int stop = 1;
		MyTerm = this;
	}
	cnt++;

	//Log::PrintLogF(0, "C:%d:%x\n", cnt++, this);
#endif

  COSTR("term");
}

Term::~Term() {
#if 0
	static int cnt = 0;
	//Log::PrintLogF(0, "D:%d:%x\n", cnt++, this);
#endif
  for (int ii = 0, size = (int)_powers.size(); ii < size; ii++) {
    _powers[ii]->Dispose();
  }

  DESTR("term");
}

//
// Compare two terms
// Return values:
//   -1 : this < other
//    0 : this = other
//    1 : this > other
//
int Term::Compare(Term * /* other */) const { return -1; }

//
// Merge two equal terms
//
void Term::Merge(Term * /* other */) {}

TermKeyType Term::Key() { return this; }

std::shared_ptr<TermStorage> Term::CreateTermStorage() {
#if TERM_STORAGE_CLASS_VECTOR
  return std::make_shared<TermStorageVector>();
#elif TERM_STORAGE_CLASS_AVL_TREE
  return std::make_shared<TermStorageAvlTree>();
#endif
}

//
// Merge powers from other term.
// Several usages:
// 1. To add powers (multiplication)
// 2. To remove powers (division)
// 3. To remove powers (integer division, gcd)
//
void Term::MergePowers(Term *t, bool add) {
  uint cnt1 = (uint)_powers.size();
  uint cnt2 = (uint)t->_powers.size();

  uint ii = 0, jj = 0;

  int op = add ? 1 : -1;

  while (ii < cnt1 && jj < cnt2) {
    Power *w1 = _powers[ii];
    Power *w2 = t->_powers[jj];

    if (w1->GetIndex() == w2->GetIndex()) {
      // case 1, w1 == w2
      // add/subtract degree
      int degree = w1->GetDegree() + op * w2->GetDegree();
      // degree = max(0, degree);
      if (degree < 0)
        degree = 0;

      if (degree == 0) {
        // remove power
        w1->Dispose();
        _powers.erase(_powers.begin() + ii);

        cnt1--;
        jj++;
      } else {
        w1->SetDegree(degree);

        ii++;
        jj++;
      }
    } else if (w1->GetIndex() < w2->GetIndex()) {
      // should not happen if add == false
      // correction - could happen, it means safe division!
      if (!add) {
        jj++;
        continue;
        // Log::PrintLogF(0, "Attempt to do rational division of powers!\n");
        // throw -1;
      }

      // case 2, w1 < w2
      // insert power and increase powers count
      _powers.insert(_powers.begin() + ii, w2->Clone());
      cnt1++;

      ii++;
      jj++;
    } else {
      // case 3, w1 > w2
      // move on with first iterator
      ii++;
    }
  }

  // ii < cnt2, nothing to do
  while (jj < cnt2) {
    // should not happen if add == false
    // correction - could happen, it means safe division!
    if (!add) {
      jj++;
      continue;
      // Log::PrintLogF(0, "Attempt to do rational division of powers!\n");
      // throw -1;
    }

    // add remaining powers
    Power *w2 = t->_powers[jj];
    _powers.push_back(w2->Clone());
    jj++;
  }
}

//
// Is this term divisible with other term
// Check powers
//
bool Term::Divisible(Term *t) const {
  uint i1 = 0, i2 = 0;
  uint s1 = (uint)_powers.size(), s2 = (uint)t->_powers.size();

  // constant is divisable only with constant
  bool divisible = (s1 > 0) || (s2 == 0);

  while (i1 < s1 && i2 < s2 && divisible) {
    // check current degree in t
    uint index2 = t->_powers[i2]->GetIndex();

    // must match it in this term
    while (i1 < s1 && _powers[i1]->GetIndex() > index2) {
      i1++;
    }

    if (i1 >= s1 || _powers[i1]->GetIndex() != index2 ||
        _powers[i1]->GetDegree() < t->_powers[i2]->GetDegree()) {
      divisible = false;
    }
    i2++;
  }

  return divisible;
}

uint Term::GetPowerCount() const { return (uint)_powers.size(); }

Power *Term::GetPower(uint index) const { return _powers[index]; }

//
// degree of variable with given index
//
int Term::VariableDeg(int index, bool free) const {
  if (free == true && this->Type() == XTERM) {
    XTerm *xt = (XTerm *)this;
    Term *t = xt->GetUFraction()->GetNumerator()->LeadTerm(index, free);
    if (t) {
      return t->VariableDeg(index, free);
    } else {
      return 0;
    }
  }
  Power *pow = this->GetPowerByIndex(index);
  if (pow != NULL) {
    return pow->GetDegree();
  }
  return 0;
}

//
// Find and return power with given variable index
//
Power *Term::GetPowerByIndex(int index) const {
  int vecIndex = this->GetIndexOfIndex(index);
  return vecIndex >= 0 ? this->GetPower(vecIndex) : NULL;
}

//
// Return index in array of power with given index
// -1 if power doesn't exists
//
int Term::GetIndexOfIndex(int index) const {
  // search power using binary search
  int l = 0, r = this->GetPowerCount() - 1, m;

  while (l <= r) {
    m = (l + r) / 2;
    Power *pow = this->GetPower(m);
    if (pow->GetIndex() == (unsigned)index) {
      return m;
    } else if ((int)pow->GetIndex() < index) {
      // array is reversaly sorted
      r = m - 1;
    } else {
      l = m + 1;
    }
  }

  return -1;
}

//
// Change degree of a power with given index
// power doesn't have to exists
//
void Term::ChangePowerDegree(int index, int change) {
  if (change != 0) {
    int powIndex = this->GetIndexOfIndex(index);
    if (powIndex >= 0) {
      Power *pow = this->GetPower(powIndex);
      pow->SetDegree(pow->GetDegree() + change);
      if (pow->GetDegree() == 0) {
        this->RemovePower(powIndex);
      }
    }
  }
}

//
// Remove power at index
//
void Term::RemovePower(uint index) {
  _powers[index]->Dispose();
  _powers.erase(_powers.begin() + index);
}

//
// this = GCD(this, t)
//
void Term::GCDWith(const Term *t) {
  uint j = 0;
  uint k, size1, size2;

  size1 = this->GetPowerCount();
  size2 = t->GetPowerCount();

  for (k = 0; k < size1; k++) {
    uint index1 = this->GetPower(k)->GetIndex();

    while (j < size2 && index1 < t->GetPower(j)->GetIndex()) {
      j++;
    }

    if (j < size2 && index1 == t->GetPower(j)->GetIndex()) {
      uint d_k = this->GetPower(k)->GetDegree();
      uint d_j = t->GetPower(j)->GetDegree();
      uint d = (d_k < d_j ? d_k : d_j);

      this->GetPower(k)->SetDegree(d);
      if (this->GetPower(k)->GetDegree() == 0) {
        this->RemovePower(k);
        k--;
        size1--;
      }
    } else {
      this->RemovePower(k);
      k--;
      size1--;
    }
  }
}
