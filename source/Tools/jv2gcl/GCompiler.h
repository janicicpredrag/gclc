// GCompiler.h: interface for the GCompiler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(GCOMPILER_H)
#define GCOMPILER_H

class CGCLCInput;
class CGCLCOutput;
class CGCLCLog;

class CListOfPrimitives;
class CListOfFixedPoints;

#define EPS 0.0001
#define PI 3.1415925

struct FixedPoint 
{
	char *sName;
	int iStartLine;
	int iStartPosition;
	int iEndLine;
	int iEndPosition;
	int iStartGlobalPosition;
	int iEndGlobalPosition;
	double x,y;
//	FixedPoint *pNext;
};



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
	rvG_Error
};



class CGCompiler  
{
public:
	CGCompiler();
	virtual ~CGCompiler();
	void CleanUp();

	virtual GReturnValue Import(CGCLCInput *pInput, CGCLCLog *pLog)=0;
	virtual GReturnValue GetError(int *piErrorCode, char** sErrMessage, int *line, int*pos)=0;
	GReturnValue Export(CGCLCOutput *pOutput);

	virtual GReturnValue GetValue(char* sVarName, char *sValue)=0;

	GReturnValue GetFirstPoint(FixedPoint **ppFP);
	GReturnValue GetNextPoint(FixedPoint **ppFP);

protected:

	GReturnValue SetPixel(double x1,double y1);
	GReturnValue DrawSegment(double x1,double y1,double x2,double y2,bool sensitive);
	GReturnValue DrawLineSensitive(double x1,double y1,double x2,double y2);
	GReturnValue EmptyCircle(double x1,double y1,double x2,double y2);
	GReturnValue SetThickness(double t);
	GReturnValue PrintText(double x, double y, char* position, char* text);
	GReturnValue SetDim(double x,double y);
	GReturnValue SetColor(double r,double g,double b);
	GReturnValue SetBackgroundColor(double r,double g,double b);

	GReturnValue CGCompiler::AddPoint(double x,double y,char* sName, 
		int iStartLine, int iStartPosition, int iEndLine, int iEndPosition, 
		int iStartGlobalPos, int iEndGlobalPos);

private:
	CGCLCInput *m_pInput;
	CListOfPrimitives *m_pPrim;
        std::map<string,FixedPoint>* m_pPoints;
//        CListOfFixedPoints *m_pPoints;
};

#endif // !defined(GCOMPILER_H)

