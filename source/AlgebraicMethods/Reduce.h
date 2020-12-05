#pragma once

#include "XPolynomial.h"

/*************************************************************
*
* Reduce.h
*
* Algorithms and methods for solving
* reducible cases
*
**************************************************************/

class Reduce
{
private:
    static XPolynomial* _FindIndex(uint index, 
        XPolynomial* xp11, 
        XPolynomial* xp12, 
        XPolynomial* xp21, 
        XPolynomial* xp22);
    static bool _IsTriangular(vxp& vxps, vector<int>& vars, int level);

public:
    static bool ReduceLineCircleIntersection(
        vector<bool>& vecPointsFree,
        vector<uint>& vecPointsIndex,
        vxp& polySystem);
    static bool Triangulate(vxp& vxps, vector<int>& vars, int level, uint* pMaxt);
};
