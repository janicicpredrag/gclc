// jv2gcl.cpp : Defines the entry point for the console application.
//

#include "GCLCInput.h"
#include "FileInput.h"
#include "JavaView.h"


int main(int argc, char* argv[])
{
	int nRetCode = 0;

/*	int iErrorCode, iLine, iPos;
	char *sErrorMessage;*/

	printf("\n");
	printf("JavaView -> GCL Converter  Version 2.00\n");
	printf("Written by Predrag Janicic, Belgrade.\n");
	printf("Copyright (c) 2002,2003. Not for commercial use.\n");
	printf("Made in cooperation with Konrad Polthier's group \n");
	printf("at the Mathematical Institute of TU Berlin.\n\n"); 

	if (argc<3) 
	{ 
		printf("Error in command line. Input/Output files not specified.\n"); 
		return -1;
	}

	FILE* hi = fopen(argv[1], "rt");
	if (hi == NULL)
	{
		printf("File error. Cannot open input file.\n");
		return -1;
	}

	FILE* ho = fopen(argv[2],"w");
	if (ho == NULL) 
	{
		printf("File error. Cannot open output file.\n");
		return -1;
	}

	printf("Input file: %s\n",argv[1]);
	printf("Output file: %s\n",argv[2]);
	printf("\n");

	CFileInput pI{hi};

	CJavaView jv2gcl;

	GReturnValue gRv;
		
	gRv=jv2gcl.Import(&pI, ho);
	
	fclose(hi);	

	if (gRv == rvG_OK)
	{
		printf("File '%s' successfully converted.", argv[1]);
		printf("\n");
		fclose(ho);
	}
	else
	{
		printf("Error while reading line %i", pI.GetCurrentLine());
		switch(gRv)
		{
		case rvG_OutOfMemory: 
			printf(" (Out of memory)\n");
			break;
	
		case rvG_InvalidInput:
			printf(" (Invalid input)\n");
			break;

		case rvG_ErrorInInput:
			printf(" (Error in input)\n");
			break;

		default:
			printf("\n");
			break;
		}

	}

	return 0;
}


