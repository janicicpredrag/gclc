#include "Log.h"
#include "Wu.h"
#include "PolyReader.h"
#include "Reduce.h"
#include <algorithm>
#include <utility>

const std::string Wu::Description() { return "Wu's method"; }

//
// Prove geometric theorem using Wu's method
//
PROVER_STATUS Wu::Prove(vxp &vxps, XPolynomial *xpConclusion,
                        int conjectureIndex, double timeout) {
  _ProofInitialize(timeout);

  PROVER_STATUS retValue = PROVER_STATUS_INPROGRESS;

  try {
    bool bLinearSystem = _IsLinearSystem(vxps);

    std::vector<int> vars;
    // if (retValue && Wu::_Triangulate(vxps, vars) == false)
    if (retValue == PROVER_STATUS_INPROGRESS &&
        !Reduce::Triangulate(vxps, vars, 1, &_maxt)) {
      retValue = PROVER_STATUS_OTHER;
    }
    if (retValue == PROVER_STATUS_INPROGRESS &&
        Wu::_FinalRemainder(vxps, vars, xpConclusion, conjectureIndex) ==
            false) {
      retValue = PROVER_STATUS_OTHER;
    }
    if (retValue == PROVER_STATUS_INPROGRESS) {
      if (xpConclusion->IsZero()) {
        retValue = PROVER_STATUS_PROVED;
      } else if (bLinearSystem) {
        retValue = PROVER_STATUS_DISPROVED;
      } else {
        retValue = PROVER_STATUS_NONE;
      }
    }
  } catch (int exit_code) {
    if (exit_code == E_TIMEOUT) {
      retValue = PROVER_STATUS_TIMEOUT;
    } else if (exit_code == E_COMPLEX) {
      retValue = PROVER_STATUS_COMPLEX;
    } else {
      retValue = PROVER_STATUS_OTHER;
    }
  }

  _ProofDeInitialize();

  return retValue;
}

#if 0
//
// Triangulate polynomial system
//
bool Wu::_Triangulate(vxp& vxps, vector<int>& vars)
{
	// if system is already triangular, skip procedure
	if (_IsTriangular(vxps, vars))
	{
		Log::PrintLogF(1, "The system is already triangular.\n\n");
		PolyReader::PrintPolynomials(vxps, 1);
		return true;
	}

	ITimeout::CheckTimeout();

	// create triagonal system of polynomials
	// which means that each new polynomial introduce
	// exactly one new variable

	// find variable with greatest index
	int ii, jj, size = (int)vxps.size();

	int maxIndex = 0;
	for (ii = 0; ii < size; ii++)
	{
		_maxt = std::max(_maxt, vxps[ii]->GetTotalTermCount());
		Power* pow = vxps[ii]->GetTerm(0)->GetPower(0);
		if (pow == NULL)
		{
			Log::PrintLogF(1, "Wu::_Triangulate Error: polynomial %d does not contain independent variable!\n", ii);
			throw -1;
		}
		int maxIndexCand = pow->GetIndex();
		if (maxIndexCand > maxIndex)
		{
			maxIndex = maxIndexCand;
		}
	}

	Log::PrintLogF(1, "The input system is:\n\n");
	PolyReader::PrintPolynomials(vxps, 1);

	// main loop
	// create triangulation system from the end
	for (ii = size - 1; ii >= 0; ii--)
	{
		bool printSystem = true;

		Log::PrintLogF(1, "\\subsection{Triangulation, step %d}\n\n", size - ii);
		Log::PrintLogF(1, "\\begin{description}\n");

		// select new variable
		// find variable which is not present
		// in any previous polynomial
		int var = maxIndex;
		maxIndex --; // prepare for next loop

		int count = 0; // how many polynomials has this variable
		while (count == 0)
		{
			if (var <= 0)
			{
				// case 1 - x_var doesn't appear anywhere
				//  looser assumption, allow gaps in x_i definitions
				Log::PrintLogF(1, "\\item[Error:] Something is wrong, number of variables is less than number of polynomials!\n");
				throw -1;
			}
			Log::PrintLogF(1, "\\item[Choosing variable:]\n Trying the variable with index %d.\n\n", var);

			for (jj = 0; jj <= ii; jj++)
			{
				if (vxps[jj]->LeadTerm(var))
				{
					count ++;
				}
			}

			if (count == 0)
			{
				// this situation happend when some variable is ommited from the system
				// depends on design, this could also be treated as error
				var --;
				// reduce max index also
				maxIndex --;
			}
		}

        Log::PrintLogF(1, "\\item[Variable $x_{%d}$ selected:] The number of polynomials with this variable is %d.\n\n", var, count);

		// case 2: count == 1
		if (count == 1)
		{
			Log::PrintLogF(1, "\\item[Single polynomial with chosen variable:] No reduction needed.\n\n");
			for (jj = 0; jj <= ii; jj++)
			{
				if (vxps[jj]->LeadTerm(var))
				{
					// swap vxps[ii] and vxps[jj]
					if (ii != jj)
					{
						std::swap(vxps[ii], vxps[jj]);
					}
					else
					{
						// system is not changed, no need to print it again
						Log::PrintLogF(1, "\\item The triangular system has not been changed.\n\n");
						printSystem = false;
					}

					break; // break last for loop
				}
			}
		}
		// case 3 or 4
		else if (count > 1)
		{
			// find two minimal degrees of variable
			int min1 = 0, min2 = 0;
			int pos1 = 0, pos2 = 0;
			for (jj = ii; jj >= 0; jj--)
			{
				XTerm* xt = (XTerm*)vxps[jj]->LeadTerm(var);
				if (xt)
				{
					int deg = xt->VariableDeg(var);
					if (min1 == 0)
					{
						min1 = deg;
						pos1 = jj;
					}
					else if (deg < min1)
					{
						min2 = min1;
						pos2 = pos1;
						min1 = deg;
						pos1 = jj;
					}
					else if (min2 == 0 || deg < min2)
					{
						min2 = deg;
						pos2 = jj;
					}
				}
			}

			Log::PrintLogF(1, "\\item[Minimal degrees:] %d polynomials with degree %d and %d polynomials with degree %d.\n\n", pos1, min1, pos2, min2); 

			// check
			if (min1 == 0 || min2 == 0)
			{
				Log::PrintLogF(1, "\\item[Error:] Something is wrong, there should be two polynomials with index %d!\n", var);
				throw -1;
			}

			// case 3 : polynomial with degree 1
			if (min1 == 1)
			{
                Log::PrintLogF(1, "\\item[Polynomial with linear degree:] Removing variable $x_{%d}$ from all other polynomials by reducing them with polynomial $p_{%d}$.\n\n", var, ii);

				// move it to the next pos and reduce all remaining with it
				std::swap(vxps[ii], vxps[pos1]);

				for (jj = 0; jj < ii; jj++)
				{
					if (jj != ii)
					{
						vxps[jj]->PseudoRemainder(vxps[ii], var);
						_maxt = std::max(_maxt, vxps[jj]->GetTotalTermCount());
						if (Log::StopAfterMaxT > 0 && _maxt > (uint)Log::StopAfterMaxT)
						{
							Log::PrintLogF(1, "\\item[Stopping:] maxt = %d has overriched maximum alowed value of %d!\n\n", _maxt, Log::StopAfterMaxT);
							throw -1;
						}
					}
				}
			}
			// case 4 : all polynomials are with degree higher than one
			else
			{
				// reduce two minimal polynomials and repeat the process
                Log::PrintLogF(1, "\\item[No linear degree polynomials:] Reducing polynomial $p_{%d}$ (of degree %d) with $p_{%d}$ (of degree %d).\n\n", pos2, min2, pos1, min1);
				vxps[pos2]->PseudoRemainder(vxps[pos1], var);
				_maxt = std::max(_maxt, vxps[pos2]->GetTotalTermCount());
				if (Log::StopAfterMaxT > 0 && _maxt > (uint)Log::StopAfterMaxT)
				{
					Log::PrintLogF(1, "\\item[Stopping:] maxt = %d has overriched maximum alowed value of %d!\n\n", _maxt, Log::StopAfterMaxT);
					throw -1;
				}

				// continue same iteration
				Log::PrintLogF(1, "\\item[Repeat same iteration:] Two polynomials has been reduced, repeating iteration.\n\n");
                Log::PrintLogF(1, "\\end{description}\n");

				// warning! based on loop direction, iterator should be
				// decreased or increased!
				++ii;
				maxIndex ++;
				continue;
			}
		}

/*
		if (printSystem)
		{
			// remember variable
			Log::PrintLogF(1, "\\item[Variable chosed:] The next variable in triangular system is %d.\n\n", var);
		}
*/

		// remember variable, put it on the first place
		vars.insert(vars.begin(), var);

		Log::PrintLogF(1, "\\end{description}\n");

		if (printSystem)
		{
			Log::PrintLogF(1, "Finished a triangulation step, the current system is:\n\n");
			PolyReader::PrintPolynomials(vxps, 1);
		}

		ITimeout::CheckTimeout();
	}

	// triangulation is done!
	Log::PrintLogF(1, "The triangular system is:\n\n");
	PolyReader::PrintPolynomials(vxps, 1);

	if (vxps.size() != vars.size())
	{
		Log::PrintLogF(1, "Error: Something is wrong, number of variables = %d != %d = number of polynomials!\n", vars.size(), vxps.size());
		throw -1;
	}

	return true;
}

//
// system is triangular iff each new polynomial introduces
// only one new variable
//
bool Wu::_IsTriangular(vxp& vxps, vector<int>& vars)
{
	// find variable with greatest index
	int ii, jj, kk, size = (int)vxps.size(), size1;

	int maxIndex = 0;
	for (ii = 0; ii < size; ii++)
	{
		int maxIndexCand = vxps[ii]->GetTerm(0)->GetPower(0)->GetIndex();
		if (maxIndexCand > maxIndex)
		{
			maxIndex = maxIndexCand;
		}
	}

	for (ii = 0; ii < size; ii++)
	{
		bool hasNewVar = false;

		// check what variables this polynomial has
		for (jj = 1; jj <= maxIndex; jj++)
		{
			if (vxps[ii]->LeadTerm(jj) != NULL)
			{
				// is this variable exists in previous polynomials
				bool varExists = false;
				for (kk = 0, size1 = (int)vars.size(); kk < size1 && !varExists; kk++)
				{
					varExists = vars[kk] == jj;
				}

				if (!varExists)
				{
					if (hasNewVar)
					{
						// this is second new variable in this polynomial
						// system is clearly not a polynomial
						vars.clear();
						return false;
					}

					// new variable found
					hasNewVar = true;

					// add it to array
					vars.push_back(jj);
				}
			}
		}

		if (!hasNewVar)
		{
			// no new variable in this polynomial
			// again, system not triangular
			vars.clear();
			return false;
		}
	}

	// no triangular rule is broken, system is triangular
	return true;
}
#endif

//
// Calculate final remainder of triangular system
// with polynomial conclusion
//
bool Wu::_FinalRemainder(vxp &vxps, std::vector<int> &vars,
                         XPolynomial *xpConclusion, int conjectureIndex) {
  if (conjectureIndex == 0) {
    Log::OutputSectionBegin("Final remainder");
  }
  // else
  {
    Log::OutputSubSectionBegin("Final remainder for conjecture %d", false,
                               conjectureIndex + 1);
  }
  Log::OutputText("Calculating final remainder of the conclusion:\n");
  PolyReader::PrintPolynomial(xpConclusion, 1, -2);
  Log::OutputText("with respect to the triangular system.\n\n");
  int size = (int)vxps.size();
  Log::OutputEnumBegin("enumerate");
  for (int ii = size - 1; ii >= 0 && !xpConclusion->IsZero(); ii--) {
    Log::OutputEnumItemBegin();
    Log::OutputText("Pseudo remainder with $p_{%d}$ over variable $x_{%d}$:\n",
                    ii, vars[ii]);
    xpConclusion->PseudoRemainder(vxps[ii], vars[ii]);
    PolyReader::PrintPolynomial(xpConclusion, 1, -2);

    _maxt = std::max(_maxt, xpConclusion->GetTotalTermCount());
    if (Log::StopAfterMaxT > 0 && _maxt > (uint)Log::StopAfterMaxT) {
      Log::OutputText("Warning: Stopping wu's alg because maxt = %d has "
                      "exceeded maximum allowed value of %d\n\n",
                      _maxt, Log::StopAfterMaxT);
      throw - 1;
    }

    Log::OutputEnumItemEnd();
    ITimeout::CheckTimeout();
  }
  Log::OutputEnumEnd("enumerate");

  return true;
}

//
// System is linear if every polynomial is linear
// in terms of every variable (i.e. there is no term
// with dependent variable which power is greater than one)
//
bool Wu::_IsLinearSystem(vxp &vxps) {
  bool retValue = true;

  for (int ii = 0, size = vxps.size(); ii < size && retValue; ii++) {
    XPolynomial *xp = vxps[ii];

    // check all terms
    for (int jj = 0, size1 = xp->GetTermCount(); jj < size1 && retValue; jj++) {
      XTerm *xt = (XTerm *)xp->GetTerm(jj);

      // check all powers
      for (int kk = 0, size2 = xt->GetPowerCount();
           kk < size2 && retValue; kk++) {
        retValue = (xt->GetPower(kk)->GetDegree() == 1);
      }
    }
  }

  return retValue;
}
