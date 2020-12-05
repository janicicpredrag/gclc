//#include <stdinc.h>


#include "../Engine/TheoremProver/TheoremProver.h"
#include "../Engine/TheoremProver/AreaMethod.h"
#include "../Engine/TheoremProver/WuMethod.h"
#include "../Engine/TheoremProver/GroebnerMethod.h"
#include <iostream>


enum GReturnValue
{
	rvG_OK,
	rvG_OutOfMemory,
	rvG_InvalidInput,
	rvG_ErrorInInput,
	rvG_EndOfData,
	rvG_CannotOpenOutputFile,
	rvG_CannotOpenLogFile,
	rvG_NotImplemented,
	rvG_EmptySet,
	rvG_ProverFailed,
	rvG_Error
};

enum eTheoremProvingMethod
{
	tpAreaMethod,
	tpWuMethod,
	tpGroebnerMethod,
	tpNone
};


struct prover_parameters
{
	eTheoremProvingMethod TheoremProvingMethod;
	bool bTheoremProving;
	bool bDeductionControl;
	bool bLaTeX;
	bool bXML;
	double dTime;
	int status;
};
//
// Theorems
//
const char* strParallelogram1   = "Parallelogram Theorem (pratio)";
const char* strParallelogram2   = "Parallelogram Theorem (version 2)";
const char* strParallelogram3   = "Parallelogram Theorem (version 3)";
const char* strMedians          = "Medians Theorem";
const char* strTriangleHeights  = "Triangle Heights Theorem";
const char* strTriangleHeights1 = "Triangle Heights Theorem (version 2)";
const char* strMidpoint         = "Midpoint Theorem";
const char* strSimpson          = "Simpson's Theorem";
const char* strButterfly        = "Butterfly Theorem";
const char* strCeva             = "Ceva's Theorem";
const char* strMenelaus         = "Menelaus' Theorem";
const char* strMenelausGen      = "Menelaus' Generalized Theorem";
const char* strVarignon         = "Varignon's Theorem";
const char* strPappus           = "Pappus' Theorem";
const char* strGauss            = "Gauss' Theorem";
const char* strPappus1          = "Pappus' Theorem (version 2)";
const char* strThales           = "Thales' Theorem";
const char* strHarmonic         = "Harmonic Points Theorem";
const char* strPascal           = "Pascal's Theorem";
const char* strAltitudeDual     = "Altitude-dual Theorem";
const char* strE26              = "Example 26";
const char* strE3               = "Example 3";
const char* strReducible        = "Simple Reducible Case";
const char* strDedCtrl          = "Simple Deduction Control";
const char* strJanThm           = "Breaking example";
const char* strSimpleAlg        = "Simple Algebraic Theorem";
const char* strSimpleAlg1       = "Simple Algebraic False Theorem";
const char* strTrivialIdent     = "Trivial identical";
const char* strLCThm            = "Simple Line/Circle Intersection Theorem";
const char* strJulien           = "Julien's conjecture";

int ProveTheorem(const char* theorem, CTheoremProver* pProver)
{
	prover_parameters *ProverStatus;
	char sFile[64] = "output";

	printf("\n");
	printf("GCLC prover Version 2.00\n");
	printf("Written by Predrag Janicic and Goran Predovic, University of Belgrade.\n");
	printf("Copyright (c) 2005-2008. Not for commercial use.\n");
	printf("\n");

	//for (int ii = 0; ii < ProverCount; ii++)
	//for (int ii = 1; ii < 2; ii++)
	//for (int ii = 0; ii < 1; ii++)
	{
		if (strcmp(theorem, strAltitudeDual) == 0)
		{
			// Altitude-dual theorem
			pProver->AddProverCommand(p_point, "A", "0", "0");
			pProver->AddProverCommand(p_point, "B", "0", "10");
			pProver->AddProverCommand(p_point, "C", "20", "20");
			pProver->AddProverCommand(p_point, "O", "35", "55");

			pProver->AddProverCommand(p_line, "bc", "B", "C");
			pProver->AddProverCommand(p_line, "ca", "C", "A");
			pProver->AddProverCommand(p_line, "ab", "A", "B");

			pProver->AddProverCommand(p_line, "oa", "O", "A");
			pProver->AddProverCommand(perp, "od", "O", "oa");
			pProver->AddProverCommand(p_inter, "D", "od", "bc");

			pProver->AddProverCommand(p_line, "ob", "O", "B");
			pProver->AddProverCommand(perp, "oe", "O", "ob");
			pProver->AddProverCommand(p_inter, "E", "oe", "ca");

			pProver->AddProverCommand(p_line, "oc", "O", "C");
			pProver->AddProverCommand(perp, "of", "O", "oc");
			pProver->AddProverCommand(p_inter, "F", "of", "ab");

			pProver->AddProverConjecture("collinear D E F");
		}
		else if (strcmp(theorem, strParallelogram1) == 0)
		{
			// parallelogram example
			// pratio not supported
			pProver->AddProverCommand(p_point,"A","0.00","0.00");
			pProver->AddProverCommand(p_point,"B","10.00","0.00");
			pProver->AddProverCommand(p_point,"C","20.00","20.00");
			pProver->AddProverCommand(p_pratio,"D", "C", "A", "B", "1");

			pProver->AddProverConjecture("parallel A C B D");			
		}
		else if (strcmp(theorem, strParallelogram2) == 0)
		{
			// parallelogram example
			// example works
			pProver->AddProverCommand(p_point, "A", "20", "20");
			pProver->AddProverCommand(p_point, "B", "50", "20");
			pProver->AddProverCommand(p_point, "C", "40", "70");
			pProver->AddProverCommand(p_line, "ab", "A", "B");
			pProver->AddProverCommand(p_line, "bc", "B", "C");

			pProver->AddProverCommand(parallel, "p", "A", "bc");
			pProver->AddProverCommand(parallel, "q", "C", "ab");
			pProver->AddProverCommand(p_inter, "D", "p", "q");

			pProver->AddProverConjecture("equal { sratio A B D C } { 1 }");
		}
		else if (strcmp(theorem, strParallelogram3) == 0)
		{
			// parallelogram example
			// both conjecture works
			pProver->AddProverCommand(p_point, "A", "20", "20");
			pProver->AddProverCommand(p_point, "B", "50", "20");
			pProver->AddProverCommand(p_point, "C", "40", "70");
			pProver->AddProverCommand(p_line, "ab", "A", "B");
			pProver->AddProverCommand(p_line, "bc", "B", "C");

			pProver->AddProverCommand(parallel, "p", "A", "bc");
			pProver->AddProverCommand(parallel, "q", "C", "ab");
			pProver->AddProverCommand(p_inter, "D", "p", "q");

			pProver->AddProverCommand(p_line, "ac", "A", "C");
			pProver->AddProverCommand(p_line, "bd", "B", "D");
			pProver->AddProverCommand(p_inter, "E", "ac", "bd");

			pProver->AddProverConjecture("equal { sratio A E E C } { 1 }");
			pProver->AddProverConjecture("same_length A E E C");
		}
		else if (strcmp(theorem, strMedians) == 0)
		{
			// medians example
			// example works
			pProver->AddProverCommand(p_point, "A", "20", "20");
			pProver->AddProverCommand(p_point, "B", "50", "20");
			pProver->AddProverCommand(p_point, "C", "40", "70");

			pProver->AddProverCommand(med, "mab", "A", "B");
			pProver->AddProverCommand(med, "mac", "A", "C");
			pProver->AddProverCommand(med, "mbc", "B", "C");

			pProver->AddProverCommand(p_inter, "M1", "mab", "mac");
			pProver->AddProverCommand(p_inter, "M2", "mab", "mbc");

			pProver->AddProverConjecture("identical M1 M2");
		}
		else if (strcmp(theorem, strTriangleHeights) == 0)
		{
			// triangle heights example
			// example works
			pProver->AddProverCommand(p_point, "A", "20", "20");
			pProver->AddProverCommand(p_point, "B", "50", "20");
			pProver->AddProverCommand(p_point, "C", "40", "70");

			pProver->AddProverCommand(p_line, "bc", "B", "C");
			pProver->AddProverCommand(p_line, "ab", "A", "B");
			pProver->AddProverCommand(p_line, "ac", "A", "C");

			pProver->AddProverCommand(p_foot, "A1", "A", "bc");
			pProver->AddProverCommand(p_foot, "B1", "B", "ac");
			pProver->AddProverCommand(p_foot, "C1", "C", "ab");

			pProver->AddProverCommand(p_line, "ha", "A", "A1");
			pProver->AddProverCommand(p_line, "hb", "B", "B1");
			pProver->AddProverCommand(p_line, "hc", "C", "C1");

			pProver->AddProverCommand(p_inter, "H1", "ha", "hb");
			pProver->AddProverCommand(p_inter, "H2", "ha", "hc");

			pProver->AddProverConjecture("identical H1 H2");
		}
		else if (strcmp(theorem, strTriangleHeights1) == 0)
		{
			// triangle heights example - short and fast
			// works
			pProver->AddProverCommand(p_point, "A", "20", "20");
			pProver->AddProverCommand(p_point, "B", "50", "20");
			pProver->AddProverCommand(p_point, "C", "40", "70");

			pProver->AddProverCommand(p_line, "bc", "B", "C");
			pProver->AddProverCommand(p_line, "ab", "A", "B");
			pProver->AddProverCommand(p_line, "ac", "A", "C");

			// H1 is intersection of heights ha and hb
			// H2 is intersection of heights ha and hc
			pProver->AddProverCommand(perp, "ha", "A", "bc");
			pProver->AddProverCommand(perp, "hb", "B", "ac");
			pProver->AddProverCommand(perp, "hc", "C", "ab");

			pProver->AddProverCommand(p_inter, "H1", "ha", "hb");
			pProver->AddProverCommand(p_inter, "H2", "ha", "hc");

			pProver->AddProverConjecture("identical H1 H2");
		}
		else if (strcmp(theorem, strMidpoint) == 0)
		{
			// midpoint theorem
			// both examples works
			pProver->AddProverCommand(p_point, "A", "20", "20");
			pProver->AddProverCommand(p_point, "B", "50", "20");
			pProver->AddProverCommand(p_point, "C", "40", "70");

			pProver->AddProverCommand(midpoint, "B1", "B", "C");
			pProver->AddProverCommand(midpoint, "A1", "A", "C");

			pProver->AddProverConjecture("parallel A1 B1 A B");
			// FIXME - sratio here are actualy two conjectures, parallel and ration!
			pProver->AddProverConjecture("equal { sratio A B A1 B1 } { 2 }");
		}
		else if (strcmp(theorem, strSimpson) == 0)
		{
			// simson theorem
			// works
			pProver->AddProverCommand(p_point, "A", "20", "20");
			pProver->AddProverCommand(p_point, "O", "50", "20");

			// new command
			pProver->AddProverCommand(p_circle, "k", "O", "A");

			// new command
			pProver->AddProverCommand(oncircle, "B", "O", "A");
			pProver->AddProverCommand(oncircle, "C", "O", "A");
			pProver->AddProverCommand(oncircle, "D", "O", "A");

			pProver->AddProverCommand(p_line, "ab", "A", "B");
			pProver->AddProverCommand(p_line, "bc", "B", "C");
			pProver->AddProverCommand(p_line, "ac", "A", "C");

			pProver->AddProverCommand(p_foot, "E", "D", "bc");
			pProver->AddProverCommand(p_foot, "F", "D", "ac");
			pProver->AddProverCommand(p_foot, "G", "D", "ab");

			pProver->AddProverConjecture("collinear E F G");
		}
		else if (strcmp(theorem, strButterfly) == 0)
		{
			// butterfly theorem -- works
			pProver->AddProverCommand(p_point, "P1", "20", "20");
			pProver->AddProverCommand(p_point, "O", "50", "20");

			pProver->AddProverCommand(p_circle, "k", "O", "P1");

			pProver->AddProverCommand(oncircle, "P2", "O", "P1");
			pProver->AddProverCommand(oncircle, "P3", "O", "P1");
			pProver->AddProverCommand(oncircle, "P4", "O", "P1");

			pProver->AddProverCommand(p_line, "p1p3", "P1", "P3");
			pProver->AddProverCommand(p_line, "p2p4", "P2", "P4");
			pProver->AddProverCommand(p_line, "p1p4", "P1", "P4");
			pProver->AddProverCommand(p_line, "p2p3", "P2", "P3");

			pProver->AddProverCommand(p_inter, "M", "p1p3", "p2p4");
			pProver->AddProverCommand(p_line, "om", "O", "M");

			pProver->AddProverCommand(perp, "l", "M", "om");

			pProver->AddProverCommand(p_inter, "Y", "l", "p2p3");
			pProver->AddProverCommand(p_inter, "X", "l", "p1p4");

			// no need to check same length as segments are on the same line!
			//pProver->AddProverConjecture("same_length M Y M X");
			pProver->AddProverConjecture("equal { sratio X M M Y } { 1 }");
		}
		else if (strcmp(theorem, strCeva) == 0)
		{
			// Ceva'a Theorem
			// first conjecture works, second last too long
			pProver->AddProverCommand(p_point, "A", "20", "20");
			pProver->AddProverCommand(p_point, "B", "50", "20");
			pProver->AddProverCommand(p_point, "C", "40", "70");

			pProver->AddProverCommand(p_point, "O", "30", "30");

			pProver->AddProverCommand(p_line, "ao", "A", "O");
			pProver->AddProverCommand(p_line, "bo", "B", "O");
			pProver->AddProverCommand(p_line, "co", "C", "O");

			pProver->AddProverCommand(p_line, "ab", "A", "B");
			pProver->AddProverCommand(p_line, "bc", "B", "C");
			pProver->AddProverCommand(p_line, "ac", "A", "C");

			pProver->AddProverCommand(p_inter, "D", "ao", "bc");
			pProver->AddProverCommand(p_inter, "E", "bo", "ac");
			pProver->AddProverCommand(p_inter, "F", "co", "ab");

			pProver->AddProverConjecture("equal { mult { mult { sratio A F F B } { sratio B D D C } } { sratio C E E A } } 1");
			//pProver->AddProverConjecture("equal { mult { mult { segment A F } { segment B D } } { segment C E } } { mult { mult { segment F B } { segment D C } } { segment E A } }");
		}
		else if (strcmp(theorem, strMenelaus) == 0)
		{
			// Menelaus's Theorem
			// conjecture proved
			pProver->AddProverCommand(p_point, "A", "20", "20");
			pProver->AddProverCommand(p_point, "B", "50", "20");
			pProver->AddProverCommand(p_point, "C", "40", "70");
			pProver->AddProverCommand(p_line, "ab", "A", "B");
			pProver->AddProverCommand(p_line, "bc", "B", "C");
			pProver->AddProverCommand(p_line, "ac", "A", "C");

			pProver->AddProverCommand(online, "D", "B", "C");
			pProver->AddProverCommand(online, "E", "A", "C");

			pProver->AddProverCommand(p_line, "de", "D", "E");
			pProver->AddProverCommand(p_inter, "F", "ab", "de");

			pProver->AddProverConjecture("equal { mult { mult { sratio A F F B } { sratio B D D C } } { sratio C E E A } } -1");
		}
		else if (strcmp(theorem, strMenelausGen) == 0)
		{
			// Menelaus's Generalized Theorem
			pProver->AddProverCommand(p_point, "A", "20", "20");
			pProver->AddProverCommand(p_point, "B", "50", "20");
			pProver->AddProverCommand(p_point, "C", "40", "70");
			pProver->AddProverCommand(p_point, "D", "37", "84");

			pProver->AddProverCommand(p_line, "ab", "A", "B");
			pProver->AddProverCommand(p_line, "bc", "B", "C");
			pProver->AddProverCommand(p_line, "da", "D", "A");
			pProver->AddProverCommand(p_line, "cd", "C", "D");

			pProver->AddProverCommand(online, "A1", "A", "B");
			pProver->AddProverCommand(online, "B1", "B", "C");

			pProver->AddProverCommand(p_line, "a1b1", "A1", "B1");

			pProver->AddProverCommand(p_inter, "C1", "a1b1", "cd");
			pProver->AddProverCommand(p_inter, "D1", "a1b1", "da");

			pProver->AddProverConjecture("equal { mult { mult { mult { sratio A A1 A1 B } { sratio B B1 B1 C } } { sratio C C1 C1 D } } { sratio D D1 D1 A } } 1");
		}
		else if (strcmp(theorem, strVarignon) == 0)
		{
			// Varignon's Theorem
			// both conjectures proved
			// second one last long (42s debug, 3.3s release)
			// this is good example where it is worth to have support 
			// to define several conjectures at once
			pProver->AddProverCommand(p_point, "A1", "20", "20");
			pProver->AddProverCommand(p_point, "A2", "50", "20");
			pProver->AddProverCommand(p_point, "A3", "40", "70");
			pProver->AddProverCommand(p_point, "A4", "30", "60");

			pProver->AddProverCommand(midpoint, "M1", "A1", "A2");
			pProver->AddProverCommand(midpoint, "M2", "A2", "A3");
			pProver->AddProverCommand(midpoint, "M3", "A3", "A4");
			pProver->AddProverCommand(midpoint, "M4", "A4", "A1");

			pProver->AddProverConjecture("parallel M1 M2 M3 M4");
			pProver->AddProverConjecture("same_length M1 M2 M3 M4");

		}
		else if (strcmp(theorem, strPappus) == 0)
		{
			// Pappus theorem
			// not proved, result is not zero
			pProver->AddProverCommand(p_point, "A", "20", "20");
			pProver->AddProverCommand(p_point, "B", "20", "40");

			pProver->AddProverCommand(p_point, "A1", "30", "30");
			pProver->AddProverCommand(p_point, "B1", "40", "45");

			pProver->AddProverCommand(p_line, "a", "A", "B");
			pProver->AddProverCommand(online, "C", "A", "B");

			pProver->AddProverCommand(p_line, "a1", "A1", "B1");
			pProver->AddProverCommand(online, "C1", "A1", "B1");

			pProver->AddProverCommand(p_line, "a1b", "A1", "B");
			pProver->AddProverCommand(p_line, "ab1", "A", "B1");
			pProver->AddProverCommand(p_line, "ac1", "A", "C1");
			pProver->AddProverCommand(p_line, "a1c", "A1", "C");
			pProver->AddProverCommand(p_line, "b1c", "B1", "C");
			pProver->AddProverCommand(p_line, "bc1", "B", "C1");

			pProver->AddProverCommand(p_inter, "P", "a1b", "ab1");
			pProver->AddProverCommand(p_inter, "Q", "a1c", "ac1");
			pProver->AddProverCommand(p_inter, "S", "bc1", "b1c");

			pProver->AddProverConjecture("collinear P Q S");
		}
		else if (strcmp(theorem, strGauss) == 0)
		{
			// Gauss's theorem
			// last too long, but Area Method last less than second!?
			// correction, this was the second line: 
			// pProver->AddProverCommand(p_point, "B", "90", "20");
			// because second point wasn't on X axis, number of variables
			// was greater by one and that has cause much more computations!!!
			// this could be some kind of bug!
			pProver->AddProverCommand(p_point, "A", "50", "10");
			pProver->AddProverCommand(p_point, "B", "90", "10");
			pProver->AddProverCommand(p_point, "C", "75", "40");
			pProver->AddProverCommand(p_point, "D", "55", "25");

			pProver->AddProverCommand(p_line, "a12", "B", "C");
			pProver->AddProverCommand(p_line, "a03", "A", "D");
			pProver->AddProverCommand(p_line, "a01", "A", "B");
			pProver->AddProverCommand(p_line, "a23", "C", "D");

			pProver->AddProverCommand(p_inter, "X", "a12", "a03");
			pProver->AddProverCommand(p_inter, "Y", "a01", "a23");

			pProver->AddProverCommand(midpoint, "P", "B", "D");
			pProver->AddProverCommand(midpoint, "Q", "A", "C");
			pProver->AddProverCommand(midpoint, "R", "X", "Y");

			pProver->AddProverConjecture("collinear P Q R");
			//pProver->AddProverConjecture("equal { signed_area3 M_1 M_2 M_3 } { 0 }");
		}
		else if (strcmp(theorem, strPappus1) == 0)
		{
			// Pappus theorem, second version
			// proved
			pProver->AddProverCommand(p_point, "A", "50", "10");
			pProver->AddProverCommand(p_point, "B", "90", "10");
			pProver->AddProverCommand(p_point, "A1", "35", "40");
			pProver->AddProverCommand(p_point, "A2", "45", "43");
        
			pProver->AddProverCommand(p_line, "ab", "A", "B");
			pProver->AddProverCommand(online, "C", "A", "B");

			pProver->AddProverCommand(p_line, "a12", "A1", "A2");
			pProver->AddProverCommand(p_line, "a1b", "A1", "B");

			pProver->AddProverCommand(parallel, "ab1", "A", "a1b");
			pProver->AddProverCommand(p_inter, "B1", "a12", "ab1");

			pProver->AddProverCommand(p_line, "cb1", "C", "B1");
			pProver->AddProverCommand(parallel, "bc1", "B", "cb1");
			pProver->AddProverCommand(p_inter, "C1", "a12", "bc1");

			pProver->AddProverConjecture("parallel A1 C A C1");
		}
		else if (strcmp(theorem, strThales) == 0)
		{
			// Thales theorem
			// proved
			pProver->AddProverCommand(p_point, "O", "40", "30");
			pProver->AddProverCommand(p_point, "A", "80", "30");
			pProver->AddProverCommand(p_point, "C", "60", "50");
        
			pProver->AddProverCommand(p_line, "oa", "O", "A");
			pProver->AddProverCommand(online, "B", "O", "A");

			pProver->AddProverCommand(p_line, "a", "A", "C");
			pProver->AddProverCommand(p_line, "c", "O", "C");

			pProver->AddProverCommand(parallel, "b", "B", "a");
			pProver->AddProverCommand(p_inter, "D", "c", "b");

			pProver->AddProverConjecture("equal { sratio O A O B } { sratio O C O D  }");
		}
		else if (strcmp(theorem, strHarmonic) == 0)
		{
			// Harmonic points theorem
			// proved
			pProver->AddProverCommand(p_point, "A", "20", "20");
			pProver->AddProverCommand(p_point, "B", "50", "20");
			pProver->AddProverCommand(p_point, "C", "60", "50");
			pProver->AddProverCommand(p_point, "D", "70", "30");

			pProver->AddProverCommand(p_line, "ab", "A", "B");
			pProver->AddProverCommand(p_line, "cd", "C", "D");
			pProver->AddProverCommand(p_inter, "L", "ab", "cd");

			pProver->AddProverCommand(p_line, "ad", "A", "D");
			pProver->AddProverCommand(p_line, "bc", "B", "C");
			pProver->AddProverCommand(p_inter, "K", "ad", "bc");

			pProver->AddProverCommand(p_line, "bd", "B", "D");
			pProver->AddProverCommand(p_line, "kl", "K", "L");
			pProver->AddProverCommand(p_inter, "F", "bd", "kl");

			pProver->AddProverCommand(p_line, "ac", "A", "C");
			pProver->AddProverCommand(p_inter, "G", "ac", "kl");

			pProver->AddProverConjecture("harmonic L K F G");
		}
		else if (strcmp(theorem, strPascal) == 0)
		{
			// Pascal's theorem
			pProver->AddProverCommand(p_point, "A", "20", "20");
			pProver->AddProverCommand(p_point, "O", "50", "20");

			pProver->AddProverCommand(p_circle, "k", "O", "A");
			pProver->AddProverCommand(oncircle, "B", "O", "A");
			pProver->AddProverCommand(oncircle, "C", "O", "A");
			pProver->AddProverCommand(oncircle, "D", "O", "A");
			pProver->AddProverCommand(oncircle, "F", "O", "A");
			pProver->AddProverCommand(oncircle, "E", "O", "A");

			pProver->AddProverCommand(p_line, "ab", "A", "B");
			pProver->AddProverCommand(p_line, "df", "D", "F");
			pProver->AddProverCommand(p_line, "bc", "B", "C");
			pProver->AddProverCommand(p_line, "fe", "F", "E");
			pProver->AddProverCommand(p_line, "cd", "C", "D");
			pProver->AddProverCommand(p_line, "ea", "E", "A");

			pProver->AddProverCommand(p_inter, "P", "ab", "df");
			pProver->AddProverCommand(p_inter, "Q", "bc", "fe");
			pProver->AddProverCommand(p_inter, "S", "cd", "ea");

			pProver->AddProverConjecture("collinear P Q S");
		}
		else if (strcmp(theorem, strE26) == 0)
		{
			// example 26 from the book
			pProver->AddProverCommand(p_point, "A", "20", "20");
			pProver->AddProverCommand(p_point, "B", "50", "20");
			pProver->AddProverCommand(p_point, "C", "60", "50");

			pProver->AddProverCommand(p_point, "O", "23", "27");
			pProver->AddProverCommand(p_point, "U", "31", "42");

			pProver->AddProverCommand(p_line, "ao", "A", "O");
			pProver->AddProverCommand(p_line, "bo", "B", "O");
			pProver->AddProverCommand(p_line, "co", "C", "O");

			pProver->AddProverCommand(p_line, "ab", "A", "B");
			pProver->AddProverCommand(p_line, "bc", "B", "C");
			pProver->AddProverCommand(p_line, "ca", "C", "A");

			pProver->AddProverCommand(p_inter, "P", "ao", "bc");
			pProver->AddProverCommand(p_inter, "Q", "bo", "ca");
			pProver->AddProverCommand(p_inter, "R", "co", "ab");

			pProver->AddProverCommand(p_line, "pu", "P", "U");
			pProver->AddProverCommand(p_line, "qu", "Q", "U");
			pProver->AddProverCommand(p_line, "ru", "R", "U");

			pProver->AddProverCommand(p_line, "qr", "Q", "R");
			pProver->AddProverCommand(p_line, "rp", "R", "P");
			pProver->AddProverCommand(p_line, "pq", "P", "Q");

			pProver->AddProverCommand(p_inter, "X", "pu", "qr");
			pProver->AddProverCommand(p_inter, "Y", "qu", "rp");
			pProver->AddProverCommand(p_inter, "Z", "ru", "pq");

			pProver->AddProverCommand(p_line, "ax", "A", "X");
			pProver->AddProverCommand(p_line, "by", "B", "Y");
			pProver->AddProverCommand(p_inter, "M", "ax", "by");

			pProver->AddProverConjecture("collinear M C Z");
		}
		else if (strcmp(theorem, strE3) == 0)
		{
			// example 3 from the book
			pProver->AddProverCommand(p_point, "A", "20", "20");
			pProver->AddProverCommand(p_point, "B", "50", "20");
			pProver->AddProverCommand(p_line, "ab", "A", "B");
			pProver->AddProverCommand(online, "C", "A", "B");

			pProver->AddProverCommand(p_point, "D", "30", "40");
			pProver->AddProverCommand(p_point, "E", "70", "43");

			pProver->AddProverCommand(p_line, "ae", "A", "E");
			pProver->AddProverCommand(p_line, "cd", "C", "D");
			pProver->AddProverCommand(p_line, "ad", "A", "D");
			pProver->AddProverCommand(p_line, "be", "B", "E");
			pProver->AddProverCommand(p_line, "bd", "B", "D");
			pProver->AddProverCommand(p_line, "ec", "E", "C");
			pProver->AddProverCommand(p_line, "de", "D", "E");

			pProver->AddProverCommand(p_inter, "J", "ae", "cd");
			pProver->AddProverCommand(p_inter, "G", "ad", "be");
			pProver->AddProverCommand(p_inter, "K", "bd", "ec");
			pProver->AddProverCommand(p_inter, "I", "ae", "bd");
			pProver->AddProverCommand(p_inter, "L", "cd", "be");
			pProver->AddProverCommand(p_inter, "H", "ec", "ad");
			pProver->AddProverCommand(p_inter, "F", "de", "ab");

			pProver->AddProverCommand(p_line, "jh", "J", "H");
			pProver->AddProverCommand(p_line, "ig", "I", "G");
			pProver->AddProverCommand(p_inter, "O", "jh", "ig");

			pProver->AddProverConjecture("collinear O E D");
			pProver->AddProverConjecture("collinear O L K");
		}
		else if (strcmp(theorem, strReducible) == 0)
		{
			// simple reducible case
			pProver->AddProverCommand(p_point, "A", "20", "20");
			pProver->AddProverCommand(p_point, "O", "50", "20");

			pProver->AddProverCommand(p_circle, "k", "O", "A");

			pProver->AddProverCommand(p_line, "ao", "A", "O");
			pProver->AddProverCommand(p_interlc, "B", "X", "k", "ao");

			pProver->AddProverCommand(oncircle, "C", "O", "A");

			pProver->AddProverConjecture("perpendicular A C B C");
		}
		else if (strcmp(theorem, strDedCtrl) == 0)
		{
			// simple deduction control case
			// prover should dedect inconsistency in construction
			pProver->AddProverCommand(p_point, "A", "20", "20");
			pProver->AddProverCommand(p_point, "B", "50", "20");
			pProver->AddProverCommand(p_point, "C", "40", "70");

			pProver->AddProverCommand(p_line, "p", "A", "B");
			pProver->AddProverCommand(parallel, "q", "C", "p");
			//pProver->AddProverCommand(online, "X", "q");

			// impossible intersection
			pProver->AddProverCommand(p_inter, "D", "p", "q");

			pProver->AddProverConjecture("collinear A B D");
		}
		else if (strcmp(theorem, strJanThm) == 0)
		{
			// prover broke on this example
			pProver->AddProverCommand(p_point, "A", "40", "10");
			pProver->AddProverCommand(p_point, "B", "40", "20");
			//pProver->AddProverCommand(p_line, "ab", "A", "B");
			pProver->AddProverCommand(online, "C", "A", "B");

			pProver->AddProverCommand(p_point, "A1", "25", "40");
			pProver->AddProverCommand(p_point, "B1", "45", "45");
			//pProver->AddProverCommand(p_line, "a1b1", "A1", "B1");
			pProver->AddProverCommand(online, "C1", "A1", "B1");

			pProver->AddProverCommand(p_line, "ab1", "A", "B1");
			pProver->AddProverCommand(p_line, "ac1", "A", "C1");
			pProver->AddProverCommand(p_line, "ba1", "B", "A1");
			pProver->AddProverCommand(p_line, "bc1", "B", "C1");
			pProver->AddProverCommand(p_line, "ca1", "C", "A1");
			pProver->AddProverCommand(p_line, "cb1", "C", "B1");

			pProver->AddProverCommand(p_inter, "P", "ab1", "ba1");
			pProver->AddProverCommand(p_inter, "Q", "ac1", "ca1");
			pProver->AddProverCommand(p_inter, "S", "cb1", "bc1");

			pProver->AddProverConjecture("collinear P Q S");
		}
		else if (strcmp(theorem, strSimpleAlg) == 0)
		{
			pProver->AddProverConjecture("equal 	{ mult { sum { a } { b } } { sum { c } { d } } } 	{ mult { sum { d } { c } } { sum { b } { a } } } })");
		}
		else if (strcmp(theorem, strSimpleAlg1) == 0)
		{
			pProver->AddProverConjecture("equal 	{ mult { sum { a } { b } } { sum { c } { d } } } 	{ mult { sum { d } { c } } { sum { b } { b } } } })");
		}
		else if (strcmp(theorem, strTrivialIdent) == 0)
		{
			pProver->AddProverCommand(p_point, "A", "40", "10");
			pProver->AddProverConjecture("identical A A");
		}
		else if (strcmp(theorem, strLCThm) == 0)
		{
			// line/circle intersection simple theorem
			pProver->AddProverCommand(p_point, "A", "40", "50");
			pProver->AddProverCommand(p_point, "B", "80", "50");

			pProver->AddProverCommand(p_circle, "a", "A", "B");
			pProver->AddProverCommand(p_line, "AB", "A", "B");

			//pProver->AddProverCommand(p_point, "C", "50", "70");
			pProver->AddProverCommand(p_interlc, "X1", "X2", "a", "AB");

			pProver->AddProverConjecture("collinear X1 B X2");
			//pProver->AddProverConjecture("identical A A");
		}
		else if (strcmp(theorem, strJulien) == 0)
		{
			// Ceva'a Theorem
			// first conjecture works, second last too long
			pProver->AddProverCommand(p_point, "A", "20", "20");
			pProver->AddProverCommand(p_point, "B", "50", "20");
			pProver->AddProverCommand(p_point, "C", "40", "70");

			pProver->AddProverCommand(midpoint, "M", "B", "C");
			pProver->AddProverCommand(p_tratio,"H", "A", "C", "-1");
			pProver->AddProverCommand(p_tratio,"F", "A", "B", "1");
			
			pProver->AddProverConjecture("equal { pythagoras_difference3 H F H } { mult { pythagoras_difference3 A M A } { 4 } }");
			//pProver->AddProverConjecture("equal { mult { mult { segment A F } { segment B D } } { segment C E } } { mult { mult { segment F B } { segment D C } } { segment E A } }");
		}

		else
		{
			printf("Unknown theorem!\n");
			return 0;
		}

		ProverStatus = new prover_parameters;
		ProverStatus->bTheoremProving = true;
		ProverStatus->bLaTeX = true; 
		ProverStatus->bXML   = false;
		ProverStatus->bDeductionControl=false;		

		if ((ProverStatus!=NULL)&&(ProverStatus->bTheoremProving))
		{
			if(pProver->ValidConjecture())
			{
				ProverStatus->status = 0;
				char sLaTeXFile[1024],sXMLFile[1024];

				FILE *hProofLaTeXOutput, *hProofXMLOutput;
				if (ProverStatus->bLaTeX)
				{
					sprintf(sLaTeXFile,sFile);
					sprintf(sLaTeXFile+strlen(sFile),"_proof1.tex");

					hProofLaTeXOutput = fopen(sLaTeXFile,"wt");  
					if (hProofLaTeXOutput == NULL)
					{
						pProver->CleanUp();
						return rvG_CannotOpenOutputFile;
					}
				}
				else
					hProofLaTeXOutput = NULL;

				if (ProverStatus->bXML)
				{
					sprintf(sXMLFile,sFile);
					sprintf(sXMLFile+strlen(sFile),"_proof.xml");
					hProofXMLOutput = fopen(sXMLFile,"wt");  
					if (hProofXMLOutput == NULL)
					{
						pProver->CleanUp();
						return rvG_CannotOpenOutputFile;
					}
				}
				else
					hProofXMLOutput = NULL;

				double Time;

				bool r;

				//Log::PrintLogF(1, "Proving theorem [%s]...\n\n", theorem);

				try
				{
					eGCLC_conjecture_status e;
					r = pProver->Prove(hProofLaTeXOutput, hProofXMLOutput, &Time, theorem, &e);
					ProverStatus->dTime = Time;

					if (r)
					{
						ProverStatus->status = 1;
						printf("\nThe conjecture successfully proved.");
						printf("\n\nProblem is True\n");
					}
					else
					{
						ProverStatus->status = 0;
						printf("\nThe conjecture not proved.");
					}
				}
				catch (int hr)
				{
					printf("Exception %d happened!\n", hr);
				}

				if(hProofLaTeXOutput!=NULL)
				{
					printf("\nThe prover output is written in the file %s.\n",sLaTeXFile);
					fclose(hProofLaTeXOutput);
				}

				if(hProofXMLOutput!=NULL)
				{
					printf("\nThe prover output is written in the file %s.\n",sXMLFile);
					fclose(hProofXMLOutput);
				}
			}
			else
				ProverStatus->status = -1;
		}

		delete ProverStatus;
    }

	return 0;
}

const int MAX_LINE            = 101;
const int MAX_CONJECTURE_LINE = 1001;
const int MAX_COMMANDS        = 1001;
int LoadGCLCFile(char* GCLCFile, const char* TheoremName, char* Theorem, 
				 const char* ThmTextName, char* ThmText, char* ThmNote, 
				 char* ThmFigure[MAX_COMMANDS],
				 char* ThmProofCode[MAX_COMMANDS],
                 char* Bibtex);
void Dokazi(int argc, char* argv[]);
void Dokazi2(CTheoremProver* pProver, CTheoremProver* pProver1,
			 char* GCLCFile, char* OutputFile, char* LatexFile, char* XmlFile, char* Graphic, char TheoremID[5]);
void Dokazi3(CTheoremProver* pProver, 
             FILE* latexOut, FILE* xmlOut,
             FILE* log, 
			 char* Theorem, bool printPoints, char* ThmName);
int GetString(char* buffer, char* command, int pos);
void GetArgs(char args[6][MAX_LINE + 1], int count, char* buffer, int pos);
void GetArgNumAndCommand(char* command, int* pArgNum, eGCLC_prover_command* peCommand, bool* pConjecture, bool* pDrawing);
void GeoThmsOutput(char* GCLCFile, char* OutputFile);
bool RemoveApostrophe(char* buffer);
