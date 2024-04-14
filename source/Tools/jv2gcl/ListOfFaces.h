// ListOfFaces.h: interface for the CListOfFaces class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(LISTOFFACES_H)
#define LISTOFFACES_H

#include <cstddef>
#include <string>
#include <vector>

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
};


struct point
{
	double x,y;
	unsigned char r,g,b;
	std::string name;
};

class CFace
{
friend class CListOfFaces;

public:
	CFace();
	void AddNewVertex(int uIndex);
	unsigned char GetRColor() { return r; }
	unsigned char GetGColor() { return g; }
	unsigned char GetBColor() { return b; }

	struct vertex* GetFirstVertex();
	struct vertex* GetNextVertex();
	void AttachColors(unsigned char r0,unsigned char g0,unsigned char b0) { r = r0; b=b0; g=g0; }
	std::string name;
private:
	unsigned char r,g,b;

	bool m_bVisible;
	std::vector<vertex> m_vVertices;
	size_t m_iCurrentVertex;
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
	void AddNewVertex(int uIndex);
	CFace* GetFirstFace();
	CFace* GetNextFace();
	int GetGeometry() { return m_iGeometry; }
	void AttachColorsToCurrentFace(unsigned char r,unsigned char g,unsigned char b);
	void SetLastFaceName(const std::string &sName);

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
	void SetLastPointName(std::string &sName);

private:
	int m_iGeometry;
	std::vector<point> m_vPoints;
	size_t m_iCurrentPoint;
};

#endif // !defined(LISTOFFACES_H)
