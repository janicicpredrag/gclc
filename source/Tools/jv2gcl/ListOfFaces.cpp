// ListOfFaces.cpp: implementation of the CListOfFaces class.
//
//////////////////////////////////////////////////////////////////////

#include "ListOfFaces.h"
#include <string>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CListOfFaces::CListOfFaces()
{
	m_iCurrentFace = 0;
}

void CListOfFaces::DeleteAll()
{
	m_iCurrentFace = 0;
	m_vFaces.clear();
}

void CListOfFaces::SetGeometryIndex(unsigned int uIndex)
{
	m_iGeometry = uIndex;
}

void CListOfFaces::AddNewFace()
{
	CFace pFace;

	pFace.r = 0;
	pFace.g = 0;
	pFace.b = 0;

	m_vFaces.push_back(pFace);
}

void CListOfFaces::AddNewVertex(int uIndex)
{
	m_vFaces.back().AddNewVertex(uIndex);
}

void CListOfFaces::AttachColorsToCurrentFace(unsigned char r,unsigned char g,unsigned char b)
{
	m_vFaces[m_iCurrentFace].AttachColors(r, g, b);
}

CFace* CListOfFaces::GetFirstFace()
{
	m_iCurrentFace = 0;
	return m_vFaces.empty() ? nullptr : &m_vFaces.front();
}

CFace* CListOfFaces::GetNextFace()
{
	if (m_iCurrentFace + 1 < m_vFaces.size())
	{
		++m_iCurrentFace;
		return &m_vFaces[m_iCurrentFace];
	}
	else
		return NULL;
}

void CListOfFaces::SetLastFaceName(const std::string &sName)
{
	m_vFaces.back().name = sName;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CFace::CFace()
{
	m_iCurrentVertex = 0;
}

void CFace::AddNewVertex(int uIndex)
{
	vertex vVertex = {.index = uIndex};
	m_vVertices.push_back(vVertex);
}



struct vertex* CFace::GetFirstVertex()
{
	m_iCurrentVertex = 0;
	return m_vVertices.empty() ? nullptr : &m_vVertices.front();
}


struct vertex* CFace::GetNextVertex()
{
	if (m_iCurrentVertex + 1 < m_vVertices.size())
	{
		++m_iCurrentVertex;
		return &m_vVertices[m_iCurrentVertex];
	}
	else
		return NULL;
}



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CListOfPoints::CListOfPoints()
{
	m_iCurrentPoint = 0;
}

void CListOfPoints::DeleteAll() {
	m_iCurrentPoint = 0;
	m_vPoints.clear();
}

void CListOfPoints::SetGeometryIndex(unsigned int uIndex)
{
	m_iGeometry = uIndex;
}

bool CListOfPoints::AddNewPoint(double x,double y,unsigned char r,unsigned char g,unsigned char b)
{
	struct point pPoint;

	pPoint.x = x;
	pPoint.y = y;
	pPoint.r = r;
	pPoint.g = g;
	pPoint.b = b;

	m_vPoints.push_back(pPoint);
	return true;
}

void CListOfPoints::AttachColorsToCurrentPoint(unsigned char r,unsigned char g,unsigned char b)
{
	m_vPoints[m_iCurrentPoint].r = r;
	m_vPoints[m_iCurrentPoint].g = g;
	m_vPoints[m_iCurrentPoint].b = b;
}

struct point* CListOfPoints::GetFirstPoint()
{
	m_iCurrentPoint = 0;
	return m_vPoints.empty() ? NULL : &m_vPoints.front();
}

struct point* CListOfPoints::GetNextPoint()
{
	if (m_iCurrentPoint + 1 < m_vPoints.size())
	{
		++m_iCurrentPoint;
		return &m_vPoints[m_iCurrentPoint];
	}
	else
		return NULL;
}

void CListOfPoints::GetCurrentPoint(double *x,double *y) 
{ 
	*x = m_vPoints[m_iCurrentPoint].x;
	*y = m_vPoints[m_iCurrentPoint].y;
}

void CListOfPoints::SetLastPointName(const std::string &sName)
{
	m_pLastPoint->name = sName;
}

