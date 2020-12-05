#include "Groebner.h"
#include "PolyReader.h"

const string Groebner::Description() { return "Buchberger's method"; }

//
// Prove conclusion using Groebner basis method
//
PROVER_STATUS Groebner::Prove(vxp &vxps, XPolynomial *xpConclusion,
                              int /* conjectureIndex*/, double timeout) {
  _ProofInitialize(timeout);

  _bTimeout = false;
  PROVER_STATUS retValue = PROVER_STATUS_INPROGRESS;

  Log::OutputText("Input polynomial system is:\n\n");
  PolyReader::PrintPolynomials(vxps, 1);

  try {
    // calculate Groebner basis
    if (GroebnerBasis2(vxps)) {
      Log::OutputText("Groebner basis succesfully computed.\n\n");

      // reduce conclusion with Groebner basis
      // and check is it zero
      Log::OutputSectionBegin("Reducing Polynomial Conjecture");
      if (FullReduce(vxps, xpConclusion, -1) && xpConclusion->IsZero()) {
        Log::OutputText("Conclusion is reduced to zero.\n\n");
        retValue = PROVER_STATUS_PROVED;
        ;
      } else {
        Log::OutputText(
            "Conclusion is not reduced to zero, the result is:\n\n");
        PolyReader::PrintPolynomial(xpConclusion, 1);
        retValue = PROVER_STATUS_NONE;
      }
      Log::OutputSectionEnd();
    } else {
      retValue = PROVER_STATUS_TIMEOUT;
    }
  } catch (int exit_code) {
    if (exit_code == E_TIMEOUT) {
      retValue = PROVER_STATUS_TIMEOUT;
    }
  }

  Log::PrintLogF(3, "maxt = %d\n\n", _maxt);
  _ProofDeInitialize();

  return retValue;
}

//
// Compute Groebner basis of a given
// XPolynomial system
// Groebner basis is computed in-place,
// result basis is in the same container vector
//
// Return value is true if basis is successfuly computed
// false otherwise
//
bool Groebner::GroebnerBasis(vxp &vxps) {
  Log::PrintLogF(4, "Groebner::GroebnerBasis\n\n");

  uint k, i;
  XPolynomial *xp;

  // reduce all polynomials on the beginning
  // should make things faster
  Log::OutputSectionBegin("Groebner Basis Calculation");

#if 1
  Log::PrintLogF(1, "\\subsection{Initial Reduction}\n\n");
  Log::OutputText("Reducing all polynomial hypotheses with respect to the "
                  "remaining set of hypotheses.\n\n");
  if (Groebner::ReduceAll(vxps) == false) {
    return false;
  }
  Log::OutputText("Polynomial system after reduction:\n\n");
  PolyReader::PrintPolynomials(vxps, 1);
#endif

  // do simple reduction of polynomials
  Log::OutputText("Performing simple reduction of the polynomials.\n\n");
  for (k = 0; k < vxps.size(); k++) {
    vxps[k]->SimpleReduction();
  }
  Log::OutputText("Polynomial system after simple reduction:\n\n");
  PolyReader::PrintPolynomials(vxps, 1);

  // k is the current size of Grobner basis
  // in each iteration introduce new Polynomial
  // create S-polynomial of that polynomial and
  // current base and reduce base
  k = 1;
  bool stopProcessing = false;
  Log::PrintLogF(1, "\\subsection{Creating Groebner basis}\n\n");
  Log::OutputText("Starting from empty set, in each step new hypothesis is "
                  "added into Groebner basis. ");
  Log::OutputText("That hypothesis is reduced with respect to all other "
                  "polynomials from the set,");
  Log::OutputText("and all non-empty results are added into Groebner set.\n\n");
  Log::PrintLogF(1, "\\begin{description}\n\n");
  while (stopProcessing == false && k < vxps.size()) {
    Log::PrintLogF(1, "\\item[Step %d]\n\n", k);

    Log::PrintLogF(1, "Adding new hypothesis to the Groebner set.");
    Log::PrintLogF(1, "Current basis contains %d polynomial%c and ", k,
                   k > 1 ? 's' : ' ');
    Log::PrintLogF(1, "there are %d more polynomials to process.\n\n",
                   vxps.size() - k);

    Log::PrintLogF(1, "Groebner basis is:\n\n");
    PolyReader::PrintPolynomials(vxps, 1, k + 1);

    Log::PrintLogF(
        1, "Reducing polynomial $p_{%d}$ with polynomials $p_i, i < %d$.\n\n",
        k, k);

#if 0
        // does it have gcd = 1 with any polynomial?
        bool gcdCond = false;
		for (i = 0; i < k && gcdCond == false; i++)
        {
			XTerm* m1 = (XTerm*)vxps[i]->GetTerm(0);
			XTerm* m2 = (XTerm*)vxps[k]->GetTerm(0);

            if (m1 && m2)
            {
                m1 = (XTerm*)m1->Clone();
                m1->GCDWith(m2);
                gcdCond = (m1->GetPowerCount() == 0);
                m1->Dispose();

                if (gcdCond)
                {
                    Log::PrintLogF(1, "Polynomial will be reduced to zero because $GCD(lm(p_{%d}), lm(p_{%d})) = 1$.", i, k);
                    Log::PrintLogF(1, "No need to do any further reduction.\n\n");
                }
            }
        }
#endif

    for (i = 1; stopProcessing == false && i < k; i++) {
      Log::PrintLogF(
          1, "Reducing polynomial $p_{%d}$ with polynomial $p_{%d}$\n\n", k, i);
      //
      // add S-pol(i, k) to the array
      // special - if GCD(lm(p1), lm(p2)) = 1, then spol(p1, p2)
      // could be fully reduced
      // in other words, if GCG(lm(i), lm(k)) = 1, then we can faster this
      // process
      // - just don't do anything
      //
      // This improvement will faster program by 2-3 times!!!
      // new comment: program run much much faster, great improvement!
      //
      XTerm *m1 = (XTerm *)vxps[i]->GetTerm(0);
      XTerm *m2 = (XTerm *)vxps[k]->GetTerm(0);
      if (m1 == NULL || m2 == NULL) {
        // should not happen
        Log::PrintLogF(1, "Error: leading terms are zeros!\n\n");
        continue;
      }

      m1 = (XTerm *)m1->Clone();
      m1->GCDWith(m2);
      if (m1->GetPowerCount() == 0) {
        Log::PrintLogF(1, "Polynomial will be reduced to zero because "
                          "$GCD(p_{%d}, p_{%d}) = 1$.\n\n",
                       i, k);
        m1->Dispose();
        continue;
      }
      m1->Dispose();

      xp = (XPolynomial *)vxps[i]->Clone();
      xp->SPol(vxps[k]);
      _maxt = MAX2(_maxt, xp->GetTotalTermCount());

      if (Groebner::FullReduce(vxps, xp, -1) == false) {
        xp->Dispose();
        stopProcessing = true;
      }
      Log::PrintLogF(4, "   After FullReduce:\n\n");
      // PolyReader::PrintPolynomials(vxps, 1);

      if (stopProcessing == false) {
        if (!(xp->IsZero())) {
          Log::PrintLogF(1,
                         "Reduction is not zero, adding new S-polynomial:\n\n");
          PolyReader::PrintPolynomial(xp, 1);
          vxps.push_back(xp);
          if (Groebner::ReduceAll(vxps) == false) {
            stopProcessing = true;
          }
        } else {
          xp->Dispose();
          Log::PrintLogF(
              1, "Reduced to zero, polynomial not added to the set.\n\n");
        }
      }
    }

    k++;

    if (ITimeout::CheckTimeoutSafe() == false) {
      _bTimeout = true;
      stopProcessing = true;
    }
  }
  Log::PrintLogF(1, "\\end{description}\n\n");

  if (stopProcessing) {
    Log::OutputText("Groebner basis calculatation stopped.\n\n");
    return false;
  }

  // do simple reduction of polynoms
  for (k = 0; k < vxps.size(); k++) {
    if (vxps[k]->IsZero()) {
      // should not happen
      Log::PrintLogF(3, "ZERO ???");
      throw - 1;
    }

    vxps[k]->SimpleReduction();
  }

  // goodbye message
  Log::OutputText("Groebner basis is:\n\n");
  PolyReader::PrintPolynomials(vxps, 1);
  Log::OutputSectionEnd();

  return true;
}

//
// Perform reduction while it is possible
// to reduce any polynomial from the system with remaining
// polynomials
//
bool Groebner::ReduceAll(vxp &vxps) {
  vector<XPolynomial *>::iterator it = vxps.begin();
  uint k = 0;

  Log::PrintLogF(5, "REDUCE ALL\n\n");

  int maxIter = -1;

  // reduce current polynomial
  while (it != vxps.end() && maxIter != 0) {
    maxIter--;

    XPolynomial *xp = (*it);
    if (xp->IsZero()) {
      // should not happen!
      Log::PrintLogF(4, "ZERO !!!");
      throw - 1;
      /*
                        xp->Dispose();
                        vxps.erase(it);
                        it = vxps.begin();
                        continue;
      */
    }

    Log::PrintLogF(4, "      reducing polynomial %d\n\n", k);

    // try to reduce
    if (Groebner::FullReduce(vxps, xp, k)) {
      // check did polynomial vanished
      if (xp->IsZero()) {
        Log::PrintLogF(4, "      erasing polynomial %d\n\n", k);
        xp->Dispose();
        vxps.erase(it);
      }

      // start from the beginning because system has changed
      it = vxps.begin();
      k = 0;
    } else {
      if (xp->IsZero()) {
        // should not happen!
        Log::PrintLogF(3, "ZERO !!!!!!!!!!!!!!!");
        throw - 1;
      }

      it++;
      k++;
    }

    if (ITimeout::CheckTimeoutSafe() == false) {
      _bTimeout = true;
      return false;
    }
  }

  return true;
}

//
// Reduce polynomial xp with the current groebner basis system
// Parameter exclude is the index of polynomial xp in system vxps
// or <0 if polynomial xp is not from the system
//     -1 if it is conjecture
//     -2 if it is not conjecture
//
bool Groebner::FullReduce(vxp &vxps, XPolynomial *xp, int exclude) {
  uint k = 0;
  int kk;
  int candidate, high_score;
  bool ret = false, not_end = true;

  while (not_end) {
    not_end = false;
    candidate = high_score = -1;

    // find highest candidate for reduction
    int size;
    for (k = 0, size = (int)vxps.size(); k < (uint)size; k++) {
      if ((kk = k) != exclude) {
        int score = Groebner::CanReduce(xp, vxps[k]);
        if (score == -2) {
          return false;
        }
        if ((score >= 0) && (high_score == -1 || score < high_score)) {
          high_score = score;
          candidate = k;
        }
      }
    }

    not_end = (candidate >= 0);

    if (not_end) {
      ret = true;
      if (exclude == -1) {
        Log::OutputText("Reducing with polynomial $p_{%d}$, the result is:\n\n",
                        candidate);
      }
      if (Groebner::Reduce(xp, vxps[candidate]) == false) {
        return false;
      }
      if (exclude == -1) {
        PolyReader::PrintPolynomial(xp, 1);
      }
    }

    if (ITimeout::CheckTimeoutSafe() == false) {
      _bTimeout = true;
      return false;
    }
  }

  return ret;
}

//
// May xp1 be reduced with xp2?
// If yes, return highest index of term in xp1 which may be reduced
//
int Groebner::CanReduce(XPolynomial *xp1, XPolynomial *xp2) {
  uint i;
  XTerm *lmp;

  if (xp2->GetTermCount() == 0) {
    // should not happen
    throw - 1;
    // return -1;
  }

  // lm(xp1) - leading monom
  lmp = (XTerm *)xp2->GetTerm(0);

  // find monom in xp1 which is divisible with lm(p)
  for (i = 0;
       i < xp1->GetTermCount() && xp1->GetTerm(i)->Divisible(lmp) == false; i++)
    ;

  if (i >= xp1->GetTermCount()) {
    // cannot be reduced
    return -1;
  }

  // lowest index of reducable term
  return i;
}

//
// Reduce xp1 with xp2
//
#if 0
bool Groebner::Reduce(XPolynomial* xp1, XPolynomial* xp2)
{
  int i, j;
  XTerm *lmp, *jm, *m1;
  UPolynomialFraction *ct1 = NULL, *ct2 = NULL;
  int status = 0;

  uint s1 = xp1->GetTermCount(), s2 = xp2->GetTermCount();

  if (s2 == 0)
  {
	  // should not happen!
	  throw -1;
	  //return false;
  }

  // lm(xp2) - leading monom
  lmp = (XTerm*)xp2->GetTerm(0);

  // find monom in xp1 which is divisible with lm(xp1)
  for (i = 0; (uint)i < s1 && xp1->GetTerm(i)->Divisible(lmp) == false; i++);

  if (i == xp1->GetTermCount())
  {
	  // reduction not possible
	  return false;
  }

  // divide i-th monom from xp1 with lm(xp2)
  jm = (XTerm*)xp1->GetTerm(i)->Clone();
  jm->Divide(lmp);
  jm->Inverse();

  // exclude i-th monom
  xp1->RemoveTerm(xp1->GetTerm(i));

  // add all remaining monoms from p (except leading)
  // multiplied with jm
  for (j = 1; (uint)j < xp2->GetTermCount() && status == 0; j++)
  {
	  m1 = (XTerm*)xp2->GetTerm(j);
	  m1 = (XTerm*)m1->Clone();
	  status = m1->Mul(jm);
	  if (status == 0)
	  {
		  status = xp1->AddTerm(m1);
	  }
	  m1->Dispose();
  }
  
  jm->Dispose();

  _maxt = MAX2(_maxt, xp1->GetTotalTermCount());

  if (ITimeout::CheckTimeoutSafe() == false)
  {
	  _bTimeout = true;
	  return false;
  }
  return true;
}
#else
// reduce without division, only multiplication
// p1 = c1f1 + q, where c1f1 doesn't have to be leading term
// p2 = c2f2 + r, where c2f2 is leading term and f1 = f2 * g
//
// p1 = c1f1g + q
// replace p1 with:
//   c2p1 - c1gp2 = c2c1f1g + c2q - c2c1gf2 - c1gr =
//                = c2q - c1gr
bool Groebner::Reduce(XPolynomial *xp1, XPolynomial *xp2) {
  XTerm *c1f1 = NULL, *c2f2 = NULL, *g = NULL;
  // UPolynomialFraction *c1 = NULL, *c2 = NULL;
  int ii;

  // int status = 0;

  // lm(xp2) - leading monom
  c2f2 = (XTerm *)xp2->GetTerm(0);

  // find monom in xp1 which is divisible with lm(xp1)
  uint s1 = xp1->GetTermCount();
  for (ii = 0; (uint)ii < s1 && c1f1 == NULL; ii++) {
    c1f1 = (XTerm *)xp1->GetTerm(ii);
    if (c1f1->Divisible(c2f2) == false) {
      c1f1 = NULL;
    }
  }

  if (c1f1 == NULL) {
    // reduction not possible
    return false;
  }

  // divide f1/f2
  g = (XTerm *)c1f1->Clone();
  g->Divide(c2f2);

  // multiply first with c2 (must create c2 first)
  XPolynomial *pc2 = new XPolynomial();
  XTerm *tc2 = new XTerm();
  UPolynomialFraction *c2Clone = c2f2->GetUFraction()->Clone();
  tc2->SetUFraction(c2Clone);
  c2Clone->Dispose();
  pc2->AddTerm(tc2);
  tc2->Dispose();

  // multiply second with c1g (create c1 and g)
  XPolynomial *pc1g = new XPolynomial();
  XTerm *tc1g = (XTerm *)g->Clone();
  UPolynomialFraction *c1Clone = c1f1->GetUFraction()->Clone();
  tc1g->SetUFraction(c1Clone);
  c1Clone->Dispose();
  pc1g->AddTerm(tc1g);
  tc1g->Dispose();
  XPolynomial *xp2Clone = (XPolynomial *)xp2->Clone();
  xp2Clone->Mul(pc1g);
  pc1g->Dispose();

  xp1->Mul(pc2);
  pc2->Dispose();

  // subtract
  xp1->Subtract(xp2Clone);
  xp2Clone->Dispose();
  g->Dispose();

  _maxt = MAX2(_maxt, xp1->GetTotalTermCount());

  if (ITimeout::CheckTimeoutSafe() == false) {
    _bTimeout = true;
    return false;
  }
  return true;
}
#endif

// second chance to do it right
bool Groebner::GroebnerBasis2(vxp &vxps) {
  // S0 = S
  // repeat this process:
  // 1) Form all S-polynomials of all pairs from Si
  // 2) Reduce each of them modulo Si and add any non-zero result
  //    in order to form Si+1
  // 3) Stop when Si+1 = Si

  vxp pairs;
  int step = 0;
  unsigned startAt = 0;
  char sstext[50];

  do {
    step++;

    sprintf(sstext, "Iteration %d", step);
    Log::OutputSubSectionBegin(sstext, false);

    Log::OutputText("Current set is $S_{%d} = $", step);

    PolyReader::PrintPolynomials(vxps, 1);

    // form all S-polynomials of all pairs
    Log::OutputEnumBegin("enumerate");
    pairs.clear();
    uint size = vxps.size();
    for (uint ii = 0; ii < size - 1; ii++) {
      for (uint jj = MAX2(ii + 1, startAt); jj < size; jj++) {
        Log::OutputEnumItem(
            "Creating S-polynomial from the pair $(p_{%d}, p_{%d}$).", ii, jj);
        // don't process this pair if gcd cond
        XPolynomial *xp1 = vxps[ii];
        XPolynomial *xp2 = vxps[jj];
        if (GCDCondition(xp1, xp2)) {
          Log::OutputText("Skipping pair $p_{%d}$ and $p_{%d}$ because gcd of "
                          "their leading monoms is zero.",
                          ii, jj);
          continue;
        }
        XPolynomial *xp = (XPolynomial *)xp1->Clone();
        xp->SPol(xp2);
        // Reduce(xp, xp2);

        Log::OutputText("Forming S-pol of $p_{%d}$ and $p_{%d}$:", ii, jj);
        PolyReader::PrintPolynomial(xp, 1);

        if (ITimeout::CheckTimeoutSafe() == false) {
          xp->Dispose();
          _bTimeout = true;
          Log::OutputEnumEnd("enumerate");
          return false;
        }

        // do reduction
        FullReduce(vxps, xp, -2);

        if (ITimeout::CheckTimeoutSafe() == false) {
          xp->Dispose();
          Log::OutputEnumEnd("enumerate");
          return false;
        }

        if (xp && xp->IsZero() == false) {
          // should be add
          pairs.push_back(xp);
          xp->AddRef();
          Log::OutputText("S-pol added.");
        } else {
          Log::OutputText("Reduced to zero.");
        }

        if (xp) {
          xp->Dispose();
        }
      }
    }

    Log::OutputEnumEnd("enumerate");
    startAt = vxps.size();
    // add all pairs
    for (uint kk = 0, size1 = pairs.size(); kk < size1; kk++) {
      vxps.push_back(pairs[kk]);
    }
  } while (pairs.size() > 0);

  Log::OutputSubSectionBegin("Groebner Basis", false);
  Log::OutputText("Groebner basis has %d polynomials:", vxps.size());
  PolyReader::PrintPolynomials(vxps, 1);

  return true;
}

bool Groebner::GCDCondition(XPolynomial *xp1, XPolynomial *xp2) {
  bool cond = false;
  XTerm *m1 = (XTerm *)xp1->GetTerm(0);
  XTerm *m2 = (XTerm *)xp2->GetTerm(0);

  if (m1 && m2) {
    m1 = (XTerm *)m1->Clone();
    m1->GCDWith(m2);
    cond = (m1->GetPowerCount() == 0);
    m1->Dispose();
  }

  return cond;
}
