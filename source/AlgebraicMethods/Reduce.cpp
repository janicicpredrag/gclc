#include "Reduce.h"
#include "PolyReader.h"

// points are in following order:
// 0,1 : first point on circle
// 2,3 : second point on circle
// 4,5 : center of the circle 
bool Reduce::ReduceLineCircleIntersection(
        std::vector<bool>& vecPointsFree,
        std::vector<uint>& vecPointsIndex,
        vxp& polySystem)
{
    return false;
#if 0
    if (polySystem.size() < 3)
    {
        return false;
    }
#endif

    XPolynomial *xp11, *xp12, *xp21, *xp22;
    xp11 = new XPolynomial(vecPointsFree[0], vecPointsIndex[0]);
    xp12 = new XPolynomial(vecPointsFree[1], vecPointsIndex[1]);
    xp21 = new XPolynomial(vecPointsFree[2], vecPointsIndex[2]);
    xp22 = new XPolynomial(vecPointsFree[3], vecPointsIndex[3]);

    bool bRet = true;

    // entry info
    int level = 1;
    Log::PrintLogF(level, "Reduce following polynomial system:\n\n");
    PolyReader::PrintPolynomials(polySystem, level);
    Log::PrintLogF(level, "First point:\n\n");
    PolyReader::PrintPolynomial(xp11, level);
    PolyReader::PrintPolynomial(xp12, level);
    Log::PrintLogF(level, "Second point:\n\n");
    PolyReader::PrintPolynomial(xp21, level);
    PolyReader::PrintPolynomial(xp22, level);

#if 1
    Log::PrintLogF(level, "Reducing second with first in x6:\n\n");
    polySystem[1]->PseudoRemainder(polySystem[0], 6, false);
    PolyReader::PrintPolynomial(polySystem[1], level);

    polySystem[2]->Subtract(polySystem[1]);
    Log::PrintLogF(level, "Subtracting last two pols:\n\n");
    PolyReader::PrintPolynomial(polySystem[2], level);

    xp11->Subtract(xp21);
    Log::PrintLogF(level, "Divide third with x7 - x2:\n\n");
    XPolynomial* xd1 = new XPolynomial();
    polySystem[2]->PseudoRemainder(xp11, 7, false, xd1);
    PolyReader::PrintPolynomial(polySystem[2], level);
    Log::PrintLogF(level, "Division remainder:\n\n");
    PolyReader::PrintPolynomial(polySystem[2], level);
    polySystem[2] = xd1;
    Log::PrintLogF(level, "Division result:\n\n");
    PolyReader::PrintPolynomial(polySystem[2], level);
    throw -1;
    return false;
#endif

    // test new approach
    // first point (x4, x3)
    // second point (x6, x5)
    //
    // goal: create polynomial which has factor (x6 - x4)
    //       then reduce such polynomial
    //
    // execution plan:
    //       e1) target polynomial is that with quadratic of x6
    //       e2) subtract from it pol with quadratic x4
    //       e3) we have two linear pols with x3 and x5
    //       e4) use them to eliminate x3 and x5 from target pol
    //       e5) target pol should have factor x6-x4
    //       e6) divide it with x6-x4
    //       
    // issues:
    //       i1) what if x6 = 0, x6 = u?
    //       i2) what if x4 = 0, x4 = u?
    //       i3) what if there is only one linear pol?

    // implementation of new approach
    // preconditions:
    // 1) there are three or four pols
    // 2) last two pols are quadratic in terms of points coordinates

    int size = polySystem.size();
	int ii;

/*
    // (e1), (e2)
    polySystem[size - 1]->Subtract(polySystem[size - 2]);
    Log::PrintLogF(level, "Subtracting last two pols:\n\n");
    PolyReader::PrintPolynomial(polySystem[size - 1], level);
*/

    // pols 0, size - 3 are linear in x11 and x21
    Log::PrintLogF(level, "x12 = :\n\n");
    PolyReader::PrintPolynomial(xp12, level);
    Log::PrintLogF(level, "x22 = :\n\n");
    PolyReader::PrintPolynomial(xp22, level);

    for (ii = 0; ii <= size - 3; ii++)
    {
        Log::PrintLogF(level, "linear in x12 or x22:\n\n");
        PolyReader::PrintPolynomial(polySystem[ii], level);
    }

#if 1
    // (e4) eliminate x12 and x22 from last two pols
    // that's done with pseudo reduction, but what if
    // x is u - try to reduce with it anyway
    // x is zero - nothing to do
    for (ii = 0; ii <= size - 3; ii++)
    {
        XPolynomial *x = polySystem[ii];
        Log::PrintLogF(level, "Reducing with pol %d\n\n", ii);
        PolyReader::PrintPolynomial(x, level);

        if (xp12->IsZero() == false && x->LeadTerm(vecPointsIndex[1], vecPointsFree[1]))
        {
            Log::PrintLogF(level, "Reducing last pol with x12 (second point) index %d:\n\n", vecPointsIndex[1]);
            polySystem[size - 1]->PseudoRemainder(x, vecPointsIndex[1], vecPointsFree[1]);
            PolyReader::PrintPolynomial(polySystem[size - 1], level);

            Log::PrintLogF(level, "Reducing next to last pol with x12 (second point) index %d:\n\n", vecPointsIndex[1]);
            polySystem[size - 2]->PseudoRemainder(x, vecPointsIndex[1], vecPointsFree[1]);
            PolyReader::PrintPolynomial(polySystem[size - 2], level);
        }
        if (xp22->IsZero() == false && x->LeadTerm(vecPointsIndex[3], vecPointsFree[3]))
        {
            Log::PrintLogF(level, "Reducing last pol with x2 (fourth point) index %d:\n\n", vecPointsIndex[3]);
            polySystem[size - 1]->PseudoRemainder(x, vecPointsIndex[3], vecPointsFree[3]);
            PolyReader::PrintPolynomial(polySystem[size - 1], level);

            Log::PrintLogF(level, "Reducing next to last pol with x2 (fourth point) index %d:\n\n", vecPointsIndex[3]);
            polySystem[size - 2]->PseudoRemainder(x, vecPointsIndex[3], vecPointsFree[3]);
            PolyReader::PrintPolynomial(polySystem[size - 2], level);
        }
    }
    // (e1), (e2)
    polySystem[size - 1]->Subtract(polySystem[size - 2]);
    Log::PrintLogF(level, "Subtracting last two pols:\n\n");
    PolyReader::PrintPolynomial(polySystem[size - 1], level);
#else
    Log::PrintLogF(level, "Reducing first with second in x2:\n\n");
    polySystem[0]->PseudoRemainder(polySystem[1], 2, false);
    PolyReader::PrintPolynomial(polySystem[0], level);

    Log::PrintLogF(level, "Reducing third with second in x2:\n\n");
    polySystem[2]->PseudoRemainder(polySystem[1], 2, false);
    PolyReader::PrintPolynomial(polySystem[2], level);

    Log::PrintLogF(level, "Reducing third with first in x5:\n\n");
    polySystem[2]->PseudoRemainder(polySystem[0], 5, false);
    PolyReader::PrintPolynomial(polySystem[2], level);
#endif

    // (e5) last pol should have factor (x11 - x21)
    // (e6) divide with (x11 - x21)
    Log::PrintLogF(level, "dividing last pol with (x11 - x21) (first - third point):\n\n");
    XPolynomial* xf = (XPolynomial*)xp11->Clone();
    xf->Subtract(xp21);
    Log::PrintLogF(level, "x11 - x21 = \n\n");
    PolyReader::PrintPolynomial(xf, level);

    // division result
    XPolynomial *xd = new XPolynomial();
    polySystem[size - 1]->PseudoRemainder(xf, vecPointsIndex[0], vecPointsFree[0], xd);

    Log::PrintLogF(level, "division result is = \n\n");
    PolyReader::PrintPolynomial(xd, level);

    if (polySystem[size - 1]->IsZero())
    {
        Log::PrintLogF(level, "division was successful, (x11-x21) is real factor\n\n");

        // replace two pols
        polySystem[size - 1]->Dispose();
        polySystem[size - 1] = xd;

        bRet = true;
    }
    else
    {
        Log::PrintLogF(level, "division failed, (x11-x21) is not a factor!\n\n");
        bRet = false;
    }

    // exit info
    Log::PrintLogF(level, "After reduction:\n\n");
    PolyReader::PrintPolynomials(polySystem, 1);

    xp11->Dispose();
    xp12->Dispose();
    xp21->Dispose();
    xp22->Dispose();

    // clean exit
    //throw -1;

    return bRet;
}

XPolynomial* Reduce::_FindIndex(uint index, 
                                XPolynomial* xp11, 
                                XPolynomial* xp12, 
                                XPolynomial* xp21, 
                                XPolynomial* xp22)
{
    if (xp11->GetTerm(0)->GetPower(0)->GetIndex() == index)
    {
        return xp11;
    }
    if (xp12->GetTerm(0)->GetPower(0)->GetIndex() == index)
    {
        return xp12;
    }
    if (xp21->GetTerm(0)->GetPower(0)->GetIndex() == index)
    {
        return xp21;
    }
    if (xp22->GetTerm(0)->GetPower(0)->GetIndex() == index)
    {
        return xp22;
    }
    return NULL;
}

//
// Triangulate polynomial system
//
bool Reduce::Triangulate(vxp& vxps, std::vector<int>& vars, int level, uint* pMaxt)
{
	// if system is already triangular, skip procedure
	if (_IsTriangular(vxps, vars, level))
	{
        if (level == 1)
        {
            Log::OutputText("The system is already triangular.\n\n");
        }
		PolyReader::PrintPolynomials(vxps, level);
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
		(*pMaxt) = MAX2((*pMaxt), vxps[ii]->GetTotalTermCount());
		Power* pow = vxps[ii]->GetTerm(0)->GetPower(0);
		if (pow == NULL)
		{
            if (level == 1)
            {
                Log::OutputText("Wu::_Triangulate Error: polynomial %d does not contain independent variable!\n", ii);
            }
			throw -1;
		}
		int maxIndexCand = pow->GetIndex();
		if (maxIndexCand > maxIndex)
		{
			maxIndex = maxIndexCand;
		}
	}

    if (level == 1)
    {
        Log::OutputText("The input system is:\n\n");
    }
	PolyReader::PrintPolynomials(vxps, level);

	// main loop
	// create triangulation system from the end
	for (ii = size - 1; ii >= 0; ii--)
	{
		bool printSystem = true;

        if (level == 1)
        {
            Log::OutputSubSectionBegin("Triangulation, step %d", false, size - ii);
            Log::OutputEnumBegin("description");
        }

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
                if (level == 1)
                {
                    Log::OutputDescriptionItemBegin("Error:");
                    Log::OutputDescriptionItemEnd("Something is wrong, number of variables is less than number of polynomials!\n");
                }
				throw -1;
			}
            if (level == 1)
            {
                Log::OutputDescriptionItemBegin("Choosing variable:");
                Log::OutputDescriptionItemEnd("Trying the variable with index %d.\n\n", var);
            }

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

        if (level == 1)
        {
            Log::OutputDescriptionItemBegin("Variable $x_{%d}$ selected:", var);
            Log::OutputDescriptionItemEnd("The number of polynomials with this variable is %d.\n\n", count);
        }

		// case 2: count == 1
		if (count == 1)
		{
            if (level == 1)
            {
                Log::OutputDescriptionItemBegin("Single polynomial with chosen variable:");
                Log::OutputDescriptionItemEnd("No reduction needed.\n\n");
            }
			for (jj = 0; jj <= ii; jj++)
			{
				if (vxps[jj]->LeadTerm(var))
				{
					// swap vxps[ii] and vxps[jj]
					if (ii != jj)
					{
						XPolynomial* xp = vxps[ii];
						vxps[ii] = vxps[jj];
						vxps[jj] = xp;
					}
					else
					{
						// system is not changed, no need to print it again
                        if (level == 1)
                        {
                            Log::OutputEnumItem("The triangular system has not been changed.\n\n");
                        }
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

            if (level == 1)
            {
                Log::OutputDescriptionItemBegin("Minimal degrees:");
                Log::OutputDescriptionItemEnd("%d polynomials with degree %d and %d polynomials with degree %d.\n\n", pos1, min1, pos2, min2); 
            }

			// check
			if (min1 == 0 || min2 == 0)
			{
                if (level == 1)
                {
                    Log::OutputDescriptionItemBegin("Error:");
                    Log::OutputDescriptionItemEnd("Something is wrong, there should be two polynomials with index %d!\n", var);
                }
				throw -1;
			}

			// case 3 : polynomial with degree 1
			if (min1 == 1)
			{
                if (level == 1)
                {
                    Log::OutputDescriptionItemBegin("Polynomial with linear degree:");
                    Log::OutputDescriptionItemEnd("Removing variable $x_{%d}$ from all other polynomials by reducing them with polynomial $p_{%d}$.\n\n", var, ii);
                }

				// move it to the next pos and reduce all remaining with it
				XPolynomial* xp = vxps[ii];
				vxps[ii] = vxps[pos1];
				vxps[pos1] = xp;

				for (jj = 0; jj < ii; jj++)
				{
					if (jj != ii)
					{
						vxps[jj]->PseudoRemainder(vxps[ii], var);
						(*pMaxt) = MAX2((*pMaxt), vxps[jj]->GetTotalTermCount());
						if (Log::StopAfterMaxT > 0 && (*pMaxt) > (uint)Log::StopAfterMaxT)
						{
                            if (level == 1)
                            {
                                Log::OutputDescriptionItemBegin("Stopping:");
                                Log::OutputDescriptionItemEnd("maxt = %d has overriched maximum alowed value of %d!\n\n", (*pMaxt), Log::StopAfterMaxT);
                            }
							throw -1;
						}
					}
				}
			}
			// case 4 : all polynomials are with degree higher than one
			else
			{
				// reduce two minimal polynomials and repeat the process
                if (level == 1)
                {
                        Log::OutputDescriptionItemBegin("No linear degree polynomials:");
                        Log::OutputDescriptionItemEnd("Reducing polynomial $p_{%d}$ (of degree %d) with $p_{%d}$ (of degree %d).\n\n", pos2, min2, pos1, min1);
               }
				vxps[pos2]->PseudoRemainder(vxps[pos1], var);
				(*pMaxt) = MAX2((*pMaxt), vxps[pos2]->GetTotalTermCount());
				if (Log::StopAfterMaxT > 0 && (*pMaxt) > (uint)Log::StopAfterMaxT)
				{
                    if (level == 1)
                    {
                        Log::OutputDescriptionItemBegin("Stopping:");
                        Log::OutputDescriptionItemEnd("maxt = %d has overriched maximum alowed value of %d!\n\n", (*pMaxt), Log::StopAfterMaxT);
                    }
					throw -1;
				}

				// continue same iteration
                if (level == 1)
                {
                    Log::OutputDescriptionItemBegin("Repeat same iteration:");
                    Log::OutputDescriptionItemEnd("Two polynomials has been reduced, repeating iteration.\n\n");
                    Log::OutputEnumEnd("description");
                }

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

        if (level == 1)
        {
            Log::OutputEnumEnd("description");
        }

		if (printSystem)
		{
            if (level == 1)
            {
                Log::OutputText("Finished a triangulation step, the current system is:\n\n");
            }
			PolyReader::PrintPolynomials(vxps, level);
		}

		ITimeout::CheckTimeout();
	}

	// triangulation is done!
    if (level == 1)
    {
        Log::OutputText("The triangular system is:\n\n");
    }
	PolyReader::PrintPolynomials(vxps, level);

	if (vxps.size() != vars.size())
	{
        if (level == 1)
        {
            Log::OutputText("Error: Something is wrong, number of variables = %d != %d = number of polynomials!\n", vars.size(), vxps.size());
        }
		throw -1;
	}

	return true;
}

//
// system is triangular iff each new polynomial introduces
// only one new variable
//
bool Reduce::_IsTriangular(vxp& vxps, std::vector<int>& vars, int /* level */)
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
				for (kk = 0, size1 = (int)vars.size(); kk < size1 && varExists == false; kk++)
				{
					varExists = vars[kk] == jj;
				}

				if (varExists == false)
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

		if (hasNewVar == false)
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
