// ListOfFaces.h: interface for the CListOfFaces class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(LISTOFFACES_H)
#define LISTOFFACES_H


struct javaviewface
{
	int geometry;
	int v1,v2,v3,v4;
	unsigned char r,g,b;
	bool visible;
};


struct vertex
{
	int index;
	struct vertex* pnext;
};


struct point
{
	double x,y;
	unsigned char r,g,b;
	char *name;
	struct point* pnext;
};



class CFace
{
friend class CListOfFaces;

public:
	CFace();
	virtual ~CFace();
	bool AddNewVertex(unsigned int uIndex);
	unsigned char GetRColor() { return r; };
	unsigned char GetGColor() { return g; }
	unsigned char GetBColor() { return b; }

	struct vertex* GetFirstVertex();
	struct vertex* GetNextVertex();
	void AttachColors(unsigned char r0,unsigned char g0,unsigned char b0) { r = r0; b=b0; g=g0; }
	char *name;
private:
	unsigned char r,g,b;

	bool m_bVisible;
	struct vertex *m_pFirstVertex, *m_pCurrentVertex, *m_pLastVertex;
	CFace* m_pNextFace;
};



class CListOfFaces  
{
public:
	CListOfFaces();
	virtual ~CListOfFaces();
	void DeleteAll();
	void SetGeometryIndex(unsigned int uIndex);
	bool AddNewFace();
	bool AddNewVertex(unsigned int uIndex);
	CFace* GetFirstFace();
	CFace* GetNextFace();
	int GetGeometry() { return m_iGeometry; }
	void AttachColorsToCurrentFace(unsigned char r,unsigned char g,unsigned char b);
	void SetLastFaceName(char* sName);

private:
	int m_iGeometry;
	CFace *m_pFirstFace, *m_pCurrentFace, *m_pLastFace;
};




class CListOfPoints
{
public:
	CListOfPoints();
	virtual ~CListOfPoints();
	void DeleteAll();
	bool AddNewPoint(double x,double y,unsigned char r,unsigned char g,unsigned char b);
	struct point* GetFirstPoint();
	struct point* GetNextPoint();
	void SetGeometryIndex(unsigned int uIndex);
	int GetGeometry() { return m_iGeometry; }
	void AttachColorsToCurrentPoint(unsigned char r,unsigned char g,unsigned char b);
	void GetCurrentPoint(double *x,double *y);
	void SetLastPointName(char* sName);

private:
	int m_iGeometry;
	struct point *m_pFirstPoint, *m_pCurrentPoint, *m_pLastPoint;
};




#endif // !defined(LISTOFFACES_H)





