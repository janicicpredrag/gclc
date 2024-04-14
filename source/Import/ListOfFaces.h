// ListOfFaces.h: interface for the CListOfFaces class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(LISTOFFACES_H)
#define LISTOFFACES_H

#include <cstddef>
#include <string>
#include <vector>

struct vertex {
  int index;
};

struct point {
  double x, y;
  unsigned char r, g, b;
  std::string name;
};

class CFace {
  friend class CListOfFaces;

public:
  CFace();
  void AddNewVertex(int uIndex);
  unsigned char GetRColor() { return r; }
  unsigned char GetGColor() { return g; }
  unsigned char GetBColor() { return b; }

  struct vertex *GetFirstVertex();
  struct vertex *GetNextVertex();
  void AttachColors(unsigned char r0, unsigned char g0, unsigned char b0) {
    r = r0;
    b = b0;
    g = g0;
  }
  std::string name;

private:
  unsigned char r, g, b;

  bool m_bVisible;
  std::vector<vertex> m_vVertices;
  size_t m_iCurrentVertex;
};

class CListOfFaces {
public:
  CListOfFaces();
  void DeleteAll();
  void SetGeometryIndex(unsigned int uIndex);
  void AddNewFace();
  void AddNewVertex(int uIndex);
  CFace *GetFirstFace();
  CFace *GetNextFace();
  int GetGeometry() { return m_iGeometry; }
  void AttachColorsToCurrentFace(unsigned char r, unsigned char g,
                                 unsigned char b);
  void SetLastFaceName(const std::string &sName);

private:
  int m_iGeometry;
  std::vector<CFace> m_vFaces;
  size_t m_iCurrentFace;
};

class CListOfPoints {
public:
  CListOfPoints();
  void DeleteAll();
  bool AddNewPoint(double x, double y, unsigned char r, unsigned char g,
                   unsigned char b);
  struct point *GetFirstPoint();
  struct point *GetNextPoint();
  void SetGeometryIndex(unsigned int uIndex);
  int GetGeometry() { return m_iGeometry; }
  void AttachColorsToCurrentPoint(unsigned char r, unsigned char g,
                                  unsigned char b);
  void GetCurrentPoint(double &x, double &y);
  void SetLastPointName(const std::string &sName);

private:
  int m_iGeometry;
  std::vector<point> m_vPoints;
  size_t m_iCurrentPoint;
};

#endif // !defined(LISTOFFACES_H)
