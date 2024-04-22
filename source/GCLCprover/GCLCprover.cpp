#include "GCLCprover.h"
#include "string.h"
#include <string>

int main(int argc, char* argv[])
{
	if (argc > 1)
	{
		Dokazi(argc, argv);

#ifdef CRT_DBG
        _CrtDumpMemoryLeaks();
#endif
		return 0;
	}

	//Log::SetStandardOutput(false);
	//Log::StopAfterMaxT = 5000;

    int ii = 0, max = 2;
    while (ii < max)
    {
        // CTheoremProver *pProver = new CWuMethod();
		CTheoremProver *pProver = new CAreaMethod();
        //CTheoremProver *pProver = new CGroebnerMethod();
	    //
	    // Performance measured on AMD Sempron 1.81GH, 1GB Ram
	    // Windows experience index: processor 3.2, ram 3.6
	    //
	    //ProveTheorem(strParallelogram1, pProver);    // pratio not supported
	    //ProveTheorem(strParallelogram2, pProver);    // wu 0.004s, maxt = 3 (0.001s)
	    //ProveTheorem(strParallelogram3, pProver);    // wu 0.013s, maxt = 6 (0.002s)
	    //ProveTheorem(strMedians, pProver);           // wu 0.020s, maxt = 24 (0.003s)
	    //ProveTheorem(strTriangleHeights, pProver);   // wu 0.281s, maxt = 60 (0.018s)
	    //ProveTheorem(strTriangleHeights1, pProver);  // wu 0.022s, maxt = 18 (0.003s)
	    //ProveTheorem(strMidpoint, pProver);          // wu 0.015s, maxt = 8 (0.004s)
	    //ProveTheorem(strSimpson, pProver);           // wu 0.132s, maxt = 60 (0.018s)
	    //ProveTheorem(strButterfly, pProver);         // we 0.491s, maxt = 1832
	    ProveTheorem(strJulien, pProver);              // wu 0.045s, maxt = 13 (0.003s)
	    //ProveTheorem(strMenelaus, pProver);          // wu 0.018s, maxt = 8 (0.001s)
	    //ProveTheorem(strVarignon, pProver);          // wu 4.417s, maxt = 3313 (1.673s)
	    //ProveTheorem(strPappus, pProver);            // wu 0.178s, maxt = 118 (0.025s)
	    //ProveTheorem(strGauss, pProver);             // wu 2.892s, maxt = 661 (0.351s)
	    //ProveTheorem(strPappus1, pProver);           // wu 0.129s, maxt = 12 (0.003s)
	    //ProveTheorem(strThales, pProver);            // wu 0.004s, maxt = 4 (0.001s)
	    //ProveTheorem(strHarmonic, pProver);          // wu 0.136s, maxt = 7 (0.004s)
	    //ProveTheorem(strPascal, pProver);            // wu 1.007s, maxt = 480 (0.100s)
	    //ProveTheorem(strAltitudeDual, pProver);      // wu 0.033s, maxt = 35 (0.005s)
	    //ProveTheorem(strMenelausGen, pProver);       // wu 0.033s , maxt = 30 (0.005s)
	    //ProveTheorem(strE26, pProver);               // failed, mem leak!?
	    //ProveTheorem(strE3, pProver);                // wu 0.238s, maxt = 808
	    //ProveTheorem(strReducible, pProver);         // 
	    //ProveTheorem(strDedCtrl, pProver);             // 
	    //ProveTheorem(strJanThm, pProver);             // 
	    //ProveTheorem(strSimpleAlg, pProver);
	    //ProveTheorem(strSimpleAlg1, pProver);
	    //ProveTheorem(strTrivialIdent, pProver);
	    //ProveTheorem(strLCThm, pProver);
	    Log::PrintLogF(1, "  *** maxt = %d\n\n", XPolynomial::maxt());
	    delete pProver;

        ii ++;
    }

	//pProver->CleanUp();
	//ProveTheorem(strJanThm, pProver);             // 

#ifdef CRT_DBG
    _CrtDumpMemoryLeaks();
#endif

	printf("press any key to continue...\n");
	getchar();
	   
	return 0;
}

//
// Command Line Prover
//
//#define LOGDOK
//#define GRAPHIC_EPS
void Dokazi(int argc, char* argv[])
{
	CTheoremProver *pProver  = NULL; // prover
	CTheoremProver *pProver1 = NULL; // second prover
	char* ProverName = "Wu"; // or "Grobner" or "All"
	char* GCLCFile = NULL; // input file
	std::string OutputFile; // target output file
	char* LatexFile = NULL; // regular latex output
    char* XmlFile = NULL; // xml file
	char* Graphic = NULL; // diagram file in eps format
	bool geoThms = false;
	char TheoremID[5];

	//Log::SetLoggingLevel(0);

	int ii = 1;
	
	while (ii < argc)
	{
		if (strcmp(argv[ii], "--prover") == 0)
		{
			ProverName = argv[ii+1];
			ii += 2;
			printf("Selected prover %s\n", ProverName);
		}
		else if (strcmp(argv[ii], "--gclcfile") == 0)
		{
			GCLCFile = argv[ii+1];
			ii += 2;
			printf("Proving theorem from file %s\n", GCLCFile);
		}
		else if (strcmp(argv[ii], "--output") == 0)
		{
			OutputFile = argv[ii+1];
			ii += 2;
			printf("Output file is %s\n", OutputFile.c_str());
		}
		else if (strcmp(argv[ii], "--verbose") == 0)
		{
			// log to the standard output also
			Log::SetStandardOutput(true);
			ii += 1;
		}
		else if (strcmp(argv[ii], "--geothms") == 0)
		{
			geoThms = true;
			ii ++;
		}
		else
		{
			printf("Unknown command %s\n", argv[ii]);
			printf("Usage:\n");
			printf("    --prover [Wu|Grobner|All]\n");
			printf("    --gclcfile filename\n");
			printf("    --output fileout\n");
			printf("    --geothms\n");
			printf("    --verbose\n");
			return;
		}
	}

	if (geoThms == false && GCLCFile == NULL)
	{
		printf("parameter --gclcfile isn't provided!\n");
		return;
	}

	// find theorem id, look for pattern thm_XXXX* from the end
	// if theorem id is not specified, skip the file
	TheoremID[0] = TheoremID[4] = 0;
	for (ii = strlen(GCLCFile) - 1; ii >= 7; ii--)
	{
		if (GCLCFile[ii-7] == 't' && GCLCFile[ii-6] == 'h' && GCLCFile[ii-5] == 'm' && GCLCFile[ii-4] == '_')
		{
			TheoremID[0] = GCLCFile[ii - 3];
			TheoremID[1] = GCLCFile[ii - 2];
			TheoremID[2] = GCLCFile[ii - 1];
			TheoremID[3] = GCLCFile[ii - 0];
			if (TheoremID[0] < '0' || TheoremID[0] > '9' ||
				TheoremID[0] < '0' || TheoremID[0] > '9' ||
				TheoremID[0] < '0' || TheoremID[0] > '9' ||
				TheoremID[0] < '0' || TheoremID[0] > '9')
			{
				TheoremID[0] = 0;
			}
			break;
		}
	}
	if (TheoremID[0] == 0)
	{
		printf("theorem id could not be readed from the file name!\n");
		return;
	}

	if (OutputFile == NULL)
	{
		//printf("parameter --output isn't provided!\n");
		//return;
		//printf("parameter --output isn't provided, using default value\n");
		// default value is outputs\file_name.tex
		Graphic = new char[MAX_LINE + 1];

		int skipDir = strlen(GCLCFile);
		int dotPlace = 0;
		while (skipDir > 0 && GCLCFile[skipDir] != '\\' && GCLCFile[skipDir] != '/')
		{
			if (GCLCFile[skipDir] == '.' && dotPlace == 0)
			{
				dotPlace = skipDir;
			}
			skipDir --;
		}
		if (dotPlace > 0)
		{
			GCLCFile[dotPlace] = 0;
		}

		// build files
		OutputFile = std::string{"outputs\\"} + &GCLCFile[skipDir + 1] + ".tex";
#ifdef GRAPHIC_EPS
		sprintf(Graphic, "../graphics/%s.eps", &GCLCFile[skipDir + 1]);
#else
		sprintf(Graphic, "graphics/%s.pic", &GCLCFile[skipDir + 1]);
#endif

		if (dotPlace > 0)
		{
			GCLCFile[dotPlace] = '.';
		}

		printf("Output Latex file = %s\n", OutputFile.c_str());

        // xml file - latex.replace('ltx', 'xml')
        int xs = OutputFile.size();
        XmlFile = new char[xs + 1];
        strcpy(XmlFile, OutputFile.c_str());
        XmlFile[xs] = 0;
        XmlFile[xs - 1] = 'l';
        XmlFile[xs - 2] = 'm';
        XmlFile[xs - 3] = 'x';
		printf("Output XML file = %s\n", XmlFile);
	}

	if (geoThms == false)
	{
		// create latex output file, same as output with suffix '1'
		LatexFile = new char[OutputFile.size() + 10];
		int dotPlace = OutputFile.size();
		while (dotPlace > 0 && OutputFile[dotPlace--] != '.');
		sprintf(LatexFile, "%.*s_short.%s", dotPlace + 1, OutputFile.c_str(),
		        &OutputFile.c_str()[dotPlace + 2]);
	}

	if (geoThms == false && strcmp(ProverName, "Wu") == 0)
	{
		pProver = new CWuMethod();
	}
	else if (geoThms == false && strcmp(ProverName, "Grobner") == 0)
	{
		pProver = new CGroebnerMethod();
	}
	else if (geoThms == false && strcmp(ProverName, "All") == 0)
	{
		pProver = new CWuMethod();
		pProver1 = new CGroebnerMethod();
	}
	else if (geoThms == false)
	{
		printf("--prover parameter is invalid (unknown prover %s)\n", ProverName);
		return;
	}

	if (geoThms == false)
	{
		// prove theorem
		Dokazi2(pProver, pProver1, GCLCFile, OutputFile.c_str(), LatexFile, XmlFile,
		        Graphic, TheoremID);
		delete [] LatexFile;
        delete [] XmlFile;
	}
	else
	{
		// create geothms output
		GeoThmsOutput(GCLCFile, OutputFile.c_str());
	}

    if (Graphic)
    {
        delete [] Graphic;
    }
}

void GeoThmsOutput(char* GCLCFile, char* OutputFile)
{
	char Theorem[MAX_LINE + 1];
	char ThmText[10 * MAX_LINE];
	char ThmNote[5 * MAX_LINE];
	char* ThmFigure[MAX_COMMANDS];
	char* ThmProofCode[MAX_COMMANDS];
	const char* TheoremName = "thmnamee";
	const char* ThmTextName = "thmtexte";
	char Bibtex[MAX_LINE + 1];

    for (int ii = 0; ii < MAX_COMMANDS; ii++)
    {
        ThmFigure[ii] = ThmProofCode[ii] = NULL;
    }

	LoadGCLCFile(GCLCFile, TheoremName, Theorem, 
		ThmTextName, ThmText, 
		ThmNote, ThmFigure, ThmProofCode, Bibtex);

#if 0 // debug
	printf("Theorem = [%s]\n", Theorem);
	printf("ThmText = [%s]\n", ThmText);
	printf("ThmNote = [%s]\n", ThmNote);
	int pos = 0;
	printf("ThmFigure\n");
	while (ThmFigure[pos][0])
	{
		printf("  %s", ThmFigure[pos++]);
	}
	pos = 0;
	printf("ThmProofCode\n");
	while (ThmProofCode[pos][0])
	{
		printf("  %s", ThmProofCode[pos++]);
	}
#endif

	// append output to results file
	FILE* fout = fopen(OutputFile, "a");
	if (fout == NULL)
	{
		printf("Cannot open file %s!\n", OutputFile);
	}

	fprintf(fout, "<result>\n");
	fprintf(fout, "<username>gpreda1</username>\n");
	fprintf(fout, "<theoname>\n");
	fprintf(fout, "%s", Theorem);
	fprintf(fout, "</theoname>\n");
	fprintf(fout, "<description>\n");
	fprintf(fout, "%s", ThmText);
	fprintf(fout, "</description>\n");
	fprintf(fout, "<shortDescription/>\n");
	fprintf(fout, "<observations/>\n");
	fprintf(fout, "<figcode>\n");
	int pos = 0;
	while (ThmFigure[pos])
	{
		fprintf(fout, "%s", ThmFigure[pos++]);
	}	
	fprintf(fout, "</figcode>\n");
	fprintf(fout, "<proofscode>\n");
	pos = 0;
	while (ThmProofCode[pos])
	{
		fprintf(fout, "%s", ThmProofCode[pos++]);
	}
	fprintf(fout, "</proofscode>\n");
    if (Bibtex[0])
    {
        fprintf(fout, "<bibtexentry>%s</bibtexentry>\n", Bibtex);
    }
    else
    {
        fprintf(fout, "<bibtexentry/>\n");
    }
	fprintf(fout, "</result>\n");

	fclose(fout);
}

int LoadGCLCFile(char* GCLCFile, const char* TheoremName, char* Theorem, 
				 const char* ThmTextName, char* ThmText, char* ThmNote, 
				 char* ThmFigure[MAX_COMMANDS],
				 char* ThmProofCode[MAX_COMMANDS],
                 char* Bibtex)
{
	FILE *gclc;

	printf("Processing gclc source %s\n", GCLCFile);

	Theorem[0] = 0;
    Bibtex[0] = 0;

	gclc = fopen(GCLCFile, "r");
	if (gclc == NULL)
	{
		printf("Failed to open file %s for reading\n", GCLCFile);
		return -1;
	}

	// process gclc file
	char buffer[MAX_CONJECTURE_LINE + 1];
	char command[MAX_LINE + 1];
	
	ThmText[0] = 0;
	ThmNote[0] = 0;
	int pos = 0;
	bool skipNextCommand = false;
	bool proofCommandOnly = false;
	bool figureCommandOnly = false;

	int figurePos = 0;
	int proofCodePos = 0;

	try
	{
		while (fgets(buffer, MAX_CONJECTURE_LINE, gclc))
		{
			bool commentCommand = false;
			bool hasApostrophe = RemoveApostrophe(buffer);
			pos = 0;
			proofCommandOnly = false;

			//printf("Buffer = [%s]\n", buffer);
			if (buffer[0] == '%')
			{
				// proof commands are commented in figure code
				proofCommandOnly = true;

				// comment line
				// it may be special command
				pos = GetString(buffer, command, 1);
				if (command[0])
				{
					if (strcmp(command, "skip") == 0)
					{
						// commands that are skipped are only valid in figure code
						skipNextCommand = true;
						figureCommandOnly = true;
					}
					else if (strcmp(command, TheoremName) == 0)
					{
						strcpy(Theorem, &buffer[pos]);
                        // is this example from chou's book?
                        char* s = strstr(Theorem, "from \\cite{chou}");
                        if (s)
                        {
                            Theorem[(int)s - (int)Theorem] = 0;
                            strcat(Bibtex, "chou88");
                        }
					}
					else if (strcmp(command, ThmTextName) == 0)
					{
						strcat(ThmText, &buffer[pos]);
					}
					else if (strcmp(command, "thmnote") == 0)
					{
						strcat(ThmNote, &buffer[pos]);
					}
					else if (strcmp(command, "command") == 0)
					{
						commentCommand = true;
					}
					else if (strcmp(command, "prove") == 0)
					{
						// add prove command through the comments because
						// prove will slow down gclc processor
                        ThmProofCode[proofCodePos] = new char[strlen(&buffer[pos+1]) + 10];
						sprintf(ThmProofCode[proofCodePos++], "prove %s", &buffer[pos+1]);
					}
				}

				if (commentCommand == false)
				{
					pos = 0;
					continue;
				}
			}

			// get first string, unless we are in the comment and have command
			// otherwise, then continue parsing command line
			pos = GetString(buffer, command, pos);

			if (command[0])
			{
				if (skipNextCommand)
				{
					// skip this command, it is drawing related
					skipNextCommand = false;
					//continue; // don't continue in this mode
				}

				// what command is in question?
				int argNum = 0;
				eGCLC_prover_command eCommand;
				bool conjecture = false;
                bool drawing = false;

				GetArgNumAndCommand(command, &argNum, &eCommand, &conjecture, &drawing);
                if (drawing)
                {
                    figureCommandOnly = true;
                }

				if (argNum > 0 || drawing)
				{
					if (! figureCommandOnly)
					{
                        ThmProofCode[proofCodePos] = new char[strlen(&buffer[pos]) + 20];
						sprintf(ThmProofCode[proofCodePos++], "%s %s", command, &buffer[pos]);
					}
					figureCommandOnly = false;

					if (! proofCommandOnly && ! hasApostrophe)
					{
                        ThmFigure[figurePos] = new char[strlen(&buffer[pos+1]) + 20];
						sprintf(ThmFigure[figurePos++], "%s %s", command, &buffer[pos]);
					}
				}
				else if (conjecture)
				{
					// extract conjecture as a whole string
                    ThmProofCode[proofCodePos] = new char[strlen(&buffer[pos]) + 1];
					strcpy(ThmProofCode[proofCodePos++], &buffer[pos]);
				}
				else
				{
					// unknown command
				}
			}
		}
	}
	catch (...)
	{
		printf("Exception while processing file!\n");
	}

	if (Theorem[0] == 0)
	{
		sprintf(Theorem, "%s", GCLCFile);
	}

	return 0;
}

void Dokazi2(CTheoremProver* pProver, CTheoremProver* pProver1,
			 char* GCLCFile, char* OutputFile, char* LatexFile, char* XmlFile, char* Graphic,
			 char TheoremID[5])
{
	FILE *gclc, *out;
	char Theorem[MAX_LINE + 1];

	FILE* log = fopen("log", "a");
	fprintf(log, "Processing file %s, output at %s\n", GCLCFile, OutputFile);

	Theorem[0] = 0;
	gclc = fopen(GCLCFile, "r");
	if (gclc == NULL)
	{
		printf("Failed to open file %s for reading\n", GCLCFile);
		return;
	}

	out = fopen(OutputFile, "w");
	if (out == NULL)
	{
		printf("Failed to open file %s for writing\n", OutputFile);
		return;
	}

	FILE* latex = fopen(LatexFile, "w");
	if (latex == NULL)
	{
		printf("Failed to open file %s for writing\n", LatexFile);
		return;
	}

	Log::SetOutputFile(latex, 0);

	FILE* xml = fopen(XmlFile, "w");
	if (xml == NULL)
	{
		printf("Failed to open file %s for writing\n", XmlFile);
		return;
	}

	// process gclc file
	char buffer[MAX_CONJECTURE_LINE + 1];
	char command[MAX_LINE + 1];
	char ThmText[5 * MAX_LINE];
	ThmText[0] = 0;
	char ThmNote[5 * MAX_LINE];
	ThmNote[0] = 0;
	char args[6][MAX_LINE + 1];
	int pos = 0;
	bool skipNextCommand = false;
	char ThmName[MAX_LINE + 1];
	ThmName[0] = 0;

#if 0
	// header
	// theorem diagram
	if (Graphic != NULL)
	{
		Log::PrintLogF(0, "\\addtocounter{slika}{1}\n");
#ifdef GRAPHIC_EPS
		Log::PrintLogF(0, "\\includegraphics{%s}\n\n", Graphic);
#else
		Log::PrintLogF(0, "\\input{%s}\n\n", Graphic);
#endif
		Log::PrintLogF(0, "\\begin{center}Slika \\arabic{slika}\\end{center}\n");
	}
#endif

	// theorem text
	Log::PrintLogF(0, "\\begin{primer}\n");
    Log::PrintLogF(0, "\\label{primer_%s}\n", TheoremID);

	try
	{
		while (fgets(buffer, MAX_CONJECTURE_LINE, gclc))
		{
			bool commentCommand = false;
			RemoveApostrophe(buffer);
			pos = 0;

			//printf("Buffer = [%s]\n", buffer);
			if (buffer[0] == '%')
			{
				// comment line
				// it may be special command
				pos = GetString(buffer, command, 1);
				if (command[0])
				{
					if (strcmp(command, "skip") == 0)
					{
						skipNextCommand = true;
					}
					else if (strcmp(command, "thmname") == 0)
					{
						//Log::PrintLogF(0, "%s (slika \\arabic{slika}).", &buffer[pos]);
						Log::PrintLogF(0, "%s (slika \\ref{slika_pr%s}).", &buffer[pos], TheoremID);
						strcat(ThmName, &buffer[pos]);
					}
					else if (strcmp(command, "thmtext") == 0)
					{
						strcat(ThmText, &buffer[pos]);
					}
					else if (strcmp(command, "thmnote") == 0)
					{
						strcat(ThmNote, &buffer[pos]);
					}
					else if (strcmp(command, "command") == 0)
					{
						commentCommand = true;
					}
					else if (strcmp(command, "prove") == 0)
					{
						// add prove command through the comments because
						// prove will slow down gclc processor
						pProver->AddProverConjecture(&buffer[pos + 1]);
						if (pProver1)
						{
							pProver1->AddProverConjecture(&buffer[pos + 1]);
						}
					}
				}

				if (commentCommand == false)
				{
					pos = 0;
					continue;
				}
			}

			// get first string, unless we are in the comment and have command
			// otherwise, then continue parsing command line
			pos = GetString(buffer, command, pos);

			if (command[0])
			{
#ifdef LOGDOK
				printf("   command [%s] found!\n", command);
#endif
				if (skipNextCommand)
				{
					// skip this command, it is drawing related
					skipNextCommand = false;
					continue;
				}

				// what command is in question?
				int argNum = 0;
				eGCLC_prover_command eCommand;
				bool conjecture = false;
                bool drawing = false;

				GetArgNumAndCommand(command, &argNum, &eCommand, &conjecture, &drawing);

				if (argNum > 0)
				{
					int ia = 0;
					GetArgs(args, argNum, buffer, pos);

                    if (eCommand == p_pratio)
                    {
                        // towards P A B r
                        // sent to prover as [p_pratio P A A B r]
                        if (strcmp(command, "towards") == 0)
                        {
                            strcpy(args[4], args[3]);
                            strcpy(args[3], args[2]);
                            strcpy(args[2], args[1]);
                        }
                        // translate D B A C
                        // sent to prover as [p_pratio D C B A 1]
                        else if (strcmp(command, "translate") == 0)
                        {
                            strcpy(args[4], "1");
                            strcpy(args[5], args[1]);
                            strcpy(args[1], args[3]);
                            strcpy(args[3], args[2]);
                            strcpy(args[2], args[5]);
                            args[5][0] = 0;
                        }

                        argNum = 5;
                    }

#ifdef LOGDOK
					// print command
					printf("%s", command);
					for (ia = 0; ia < argNum; ia ++)
					{
						printf(" %s", args[ia]);
					}
					printf("\n");
#endif

					// add command to the prover
					pProver->AddProverCommand(eCommand, 
						args[0], 
						argNum > 1 && args[1][0] ? args[1] : NULL, 
						argNum > 2 && args[2][0] ? args[2] : NULL, 
						argNum > 3 && args[3][0] ? args[3] : NULL, 
						argNum > 4 && args[4][0] ? args[4] : NULL);
					if (pProver1)
					{
						pProver1->AddProverCommand(eCommand, 
							args[0], 
							argNum > 1 && args[1][0] ? args[1] : NULL, 
							argNum > 2 && args[2][0] ? args[2] : NULL, 
							argNum > 3 && args[3][0] ? args[3] : NULL, 
                            argNum > 4 && args[4][0] ? args[4] : NULL);
					}
				}
				else if (conjecture)
				{
					// extract conjecture as a whole string
					pProver->AddProverConjecture(&buffer[pos]);
					if (pProver1)
					{
						pProver1->AddProverConjecture(&buffer[pos]);
					}
				}
				else
				{
#ifdef LOGDOK
					printf("   Cannot process command %s\n", command);
#endif
				}
			}
		}
	}
	catch (...)
	{
		fprintf(log, "Exception while processing file!\n");
	}

	if (Theorem[0] == 0)
	{
		sprintf(Theorem, "%s", GCLCFile);
	}

    // where to end primer?
	//Log::PrintLogF(0, "\\end{primer}\n\n");
	Log::PrintLogF(0, "%s\n\n", ThmText);
    Log::PrintLogF(0, "\\end{primer}\n\n");

	if (ThmNote[0])
	{
		Log::PrintLogF(0, "{\\em %s}\n\n", ThmNote);
	}
#if 1
	// theorem diagram
	if (Graphic != NULL)
	{
		Log::PrintLogF(0, "\\begin{figure}\n");
#ifdef GRAPHIC_EPS
		Log::PrintLogF(0, "\\includegraphics{%s}\n\n", Graphic);
#else
		Log::PrintLogF(0, "\\input{%s}\n\n", Graphic);
#endif
		Log::PrintLogF(0, "\\caption{%s}\n", ThmName ? ThmName : "primer \\arabic{primer}");
		Log::PrintLogF(0, "\\label{slika_pr%s}\n", TheoremID);
		Log::PrintLogF(0, "\\end{figure}\n");
	}
#endif


	// now we can prove conjecture
	Log::PrintLogF(0, "\\begin{itemize}\n");
	Dokazi3(pProver, out, xml, log, Theorem, true, ThmName);
	if (pProver1)
	{
		Dokazi3(pProver1, out, xml, log, Theorem, false, ThmName);
	}
	Log::PrintLogF(0, "\\end{itemize}\n");

	// dispose provers
	delete pProver;
	if (pProver1)
	{
		delete pProver1;
	}

#ifdef DBG
	// memory leaks
	//Log::ObjectLifeInfo();
	Log::CheckMemoryLeaks();
#endif

	fclose(gclc);
	fclose(out);
	fclose(log);
}

void Dokazi3(CTheoremProver* pProver, 
             FILE* latexOut, FILE* xmlOut, FILE* log, 
			 char* Theorem, bool printPoints, char* ThmName)
{
	eGCLC_conjecture_status e = e_unknown;
	int timeout = 37000;

	if (printPoints == false)
	{
		if (strcmp(pProver->GetName(), "Wu's") == 0 ||
			strcmp(pProver->GetName(), "Groebner bases") == 0)
		{
			((CAlgMethod*)pProver)->SetPrintPoints(printPoints);
		}
	}
	else
	{
		Log::PrintLogF(0, "\\item\n");
	}

	if (pProver->ValidConjecture())
	{
		double time;
		bool proved;
		try
		{
			pProver->AddProverTimeout(timeout);
			proved = pProver->Prove(latexOut, xmlOut, &time, ThmName, &e);
		}
		catch (...)
		{
			proved = false;
			fprintf(log, "Exception happened in prove method!\n");
		}

		fprintf(log, "Theorem %s running time %.3lfs\n", Theorem, time);
		switch (e)
		{
		case e_proved:
			fprintf(log, "%s ", "PROVED");
			printf("    %s ", "PROVED");
			break;
		case e_disproved:
			fprintf(log, "%s ", "DISPROVED");
			printf("    %s ", "DISPROVED");
			break;
		case e_unknown_timeout:
			fprintf(log, "%s ", "TIMEOUT");
			printf("    %s ", "TIMEOUT");
			break;
		case e_unknown:
		default:
			fprintf(log, "%s ", "NOT PROVED !!!");
			printf("    %s ", "NOT PROVED !!!");
			break;
		}
		fprintf(log, "[%s]\n\n", pProver->GetName());
		printf("[%s]\n", pProver->GetName());

		// method name
		char strMethodName[30] = "Vuovom";
		if (pProver->GetName()[0] == 'G')
		{
			strcpy(strMethodName, "Buhbergerovom");
		}

		Log::PrintLogF(0, "\\item\n");
		Log::PrintLogF(0, "\n\n%s %s metodom.\n\n", 
			proved ? 
			"Teorema je dokazana " : "Teorema nije dokazana ",
			strMethodName);
		Log::PrintLogF(0, "{\\em ");
		if (e == e_proved)
		{
			Log::PrintLogF(0, "Kompleksnost: %.3lfs, %d terma.\n\n", time, XPolynomial::maxt());
		}
		else if (e == e_unknown_timeout)
		{
			Log::PrintLogF(0, "Izvr\\sh avanje prekinuto posle %.3lfs.\n\n", time);
		}
		else if (e == e_unknown)
		{
			Log::PrintLogF(0, "Krajnje svo\\dj enje nije rezultiralo nula polinomom!\n\n");
		}
		else if (e == e_disproved)
		{
			Log::PrintLogF(0, "Teorema je opovrgnuta!\n\n");
		}
		Log::PrintLogF(0, "}\n");
	}
	else
	{
		fprintf(stdout, "Conjecture NOT VALID!!!\n");
	}
}

int GetString(char* buffer, char* command, int pos)
{
	int pos1 = 0;
	command[0] = 0;

	while (buffer[pos] != 0 && (buffer[pos] == ' ' || buffer[pos] == '\t'))
	{
		pos ++;
	}

	while (buffer[pos] != 0 && buffer[pos] != ' ' && buffer[pos] != '\t' 
		&& buffer[pos] != '\r' && buffer[pos] != '\n')
	{
		command[pos1++] = buffer[pos++];
	}
	command[pos1] = 0;

	return pos;
}

void GetArgs(char args[6][MAX_LINE + 1], int count, char* buffer, int pos)
{
	int ii;
	for (ii = 0; ii < count; ii++)
	{
		pos = GetString(buffer, args[ii], pos);
	}
}

void GetArgNumAndCommand(char* command, int* pArgNum, eGCLC_prover_command* peCommand, bool* pConjecture, bool* pDrawing)
{
	// three arguments commands
	if (strcmp(command, "point") == 0)
	{
		// point A 40.0 40.0
		*pArgNum = 3;
		*peCommand = p_point;
	}
	else if (strcmp(command, "line") == 0)
	{
		// line ab A B
		*pArgNum = 3;
		*peCommand = p_line;
	}
	else if (strcmp(command, "intersec") == 0)
	{
		// intersec X p q
		// or
		// intersec X A B C D
		// just in case, put 5 parameters, if there are only three
		// last two parameters will be NULL
		*pArgNum = 5;
		*peCommand = p_inter;
	}
	else if (strcmp(command, "intersec2") == 0)
	{
		// intersec2 X Y k p
		*pArgNum = 4;
		*peCommand = p_interlc;
	}
	else if (strcmp(command, "midpoint") == 0)
	{
		// midpoint M A B
		*pArgNum = 3;
		*peCommand = midpoint;
	}
	else if (strcmp(command, "parallel") == 0)
	{
		// parallel b B a
		*pArgNum = 3;
		*peCommand = parallel;
	}
	else if (strcmp(command, "perp") == 0)
	{
		// perp a Y b
		*pArgNum = 3;
		*peCommand = perp;
	}
	else if (strcmp(command, "online") == 0)
	{
		// online B O A or online A p
		*pArgNum = 3;
		*peCommand = online;
	}
	else if (strcmp(command, "oncircle") == 0)
	{
		// oncircle B O A
		*pArgNum = 3;
		*peCommand = oncircle;
	}
	else if (strcmp(command, "med") == 0)
	{
		// med a A B
		*pArgNum = 3;
		*peCommand = med;
	}
	else if (strcmp(command, "bis") == 0)
	{
		// bis s A B C
		*pArgNum = 4;
		*peCommand = p_bis;
	}
	else if (strcmp(command, "foot") == 0)
	{
		// foot Y A a
		*pArgNum = 3;
		*peCommand = p_foot;
	}
	else if (strcmp(command, "midpoint") == 0)
	{
		// midpoint M A B
		*pArgNum = 3;
		*peCommand = midpoint;
	}
	else if (strcmp(command, "circle") == 0)
	{
		// circle k O A
		*pArgNum = 3;
		*peCommand = p_circle;
	}
	else if (strcmp(command, "towards") == 0)
	{
		// towards P A B r
		*pArgNum = 4;
		*peCommand = p_pratio;
        // sent to prover as [p_pratio P A A B r]
	}
	else if (strcmp(command, "translate") == 0)
	{
		// translate D B A C
		*pArgNum = 4;
		*peCommand = p_pratio;
        // sent to prover as [p_pratio D C B A 1]
	}
	else if (strcmp(command, "prove") == 0)
	{
		// conjecture
		*pConjecture = true;
	}
    else if (strstr(command, "cmark") != NULL)
    {
        *pDrawing = true;
    }
    else if (strstr(command, "draw") != NULL)
    {
        *pDrawing = true;
    }
    else if (strstr(command, "print") != NULL)
    {
        *pDrawing = true;
    }
    else if (strcmp(command, "dim") == 0)
    {
        *pDrawing = true;
    }
}

//
// replace apostrophe with blank character
//
bool RemoveApostrophe(char* buffer)
{
	int pp = 0;
    bool hasApostrophe = false;
	while (buffer[pp] != 0)
	{
		if (buffer[pp] == '\'')
		{
            hasApostrophe = true;
			buffer[pp] = ' ';
		}
		pp++;
	}

    return hasApostrophe;
}
