#ifndef __graph_util_h
#define __graph_util_h

#include "graph.h"
#include "settings.h"
#include <vector>

/*! \brief This structure is used for representing points for drawing.
 *
 * With this structure coordinate points are represented.
 */
struct Point {
  //! x coordinate
  float x;
  //! y coordinate
  float y;
};

/*! \class GraphUtil
 * \brief GraphUtil is a class that's used for all sorts of helper functions
 * needed for drawing methods and working with graphs.
 *
 * This class consits only of static member functions, that are used as helper
 * functions for drawing methods and
 * other graph related stuff.
 */
class GraphUtil {
public:
  //! Computes neighbours of a vertex v_k (w_p, ..., w_q)
  /*! Computes outer cycle for Gk-1, using previous outer cycle.
   * C_0(G_(k-1))=w_1, ..., w_t, where w_1=v_1 and w_t=v_2.
   * \param Graph graph Gk
   * \param vector<int> C0Gk, outer cycle for Gk (clockwise)
   * \param int vk, removing vertex
   * \return vector<int> C_0(G_(k-1)) - outer cycle of G_(k-1)
   */
  // static vector<int> findOuterCycle(Graph, vector<int>, int);
  //! Finds neighbours of v_k
  /*! This function is used in finding canonical ordering. It finds
   * neighbours of v_k (w_p, ..., w_q) for graph G_k. They should be
   * ordered clockwise, and they should be consecutive.
   * \param Graph graph G_k
   * \param int vertex v_k
   * \param int vertex v_1
   * \param int vertex v_2
   * \return vector<int> vector of neighbours of v_k
   */
  // static vector<int> findvkNeighbours(Graph, int, int, int);
  //! Gets neighbours of v_k, which have mark(w_i)==false
  /*! This function is used in finding canonical ordering. It returns all
   * neighbours of vk, which have mark(w_i) set to false. This function acts as
   * a filter for getting all unmarked neighbours.
   * \param vector<int> all neighbours of v_k
   * \param map<int, bool> mark map
   * \return vector<int> vector of unmarked neighbours
   */
  // static vector<int> getUnmarkedNeighbours(vector<int>, map<int, bool>);
  //! Updates chords for vertex wi and its neighbours
  /*! For each vertex wi, p<i<q, variable chords is updated for wi and all
   * of its neighbours.
   * \param Graph G_k graph
   * \param vector<int> unmarked neighbours of vk
   * \param map<int, int>& chords variable (this is out parameter)
   * \param map<int, bool> out variable
   */
  // static void updateChords(Graph, vector<int>, map<int, int>&, map<int,
  // bool>);
  //! Checks if vertex chord should be increased
  /*! \param vector<int> vector of wpq neighbours
   * \param int z neighbour
   * \param map<int, bool> out variable
   * \param int index i
   */
  // static bool increaseVertexChords(vector<int>, int, map<int, bool>, int);
  //! Checks if neighbour chord should be increased
  /*! \param vector<int> vector of wpq neighbours
   * \param int z neighbour
   * \param map<int, bool> out variable
   * \param int index i
   */
  // static bool increaseNeighbourChords(vector<int>, int, map<int, bool>, int);
  //! Checks if element is in vertex
  /*! \param vector<int> vector to check in
   * \param int element to look for
   * \return boolean value indicating if element is in vector
   */
  static bool isElementInVector(std::vector<int>, int);
  /*! \brief Returns any vertex x such that mark(x)=false, out(x)=true,
   * chords(x)!=0 and
   * x!=v1, x!=v2
   * \param Graph G_k graph in k-th iteration
   * \param vector<int> C_0(G_k), outer cycle of G_k
   * \param map<int, int> chords map
   * \param map<int, bool> out map
   * \param map<int, bool> mark map
   * \param int vertex v1
   * \param int vertex v2
   * \return vertex x (-1 if there is an error, which means that graph is wrong)
   */
  // static int canonical_choose_vertex(Graph, vector<int>, map<int, int>,
  // map<int, bool>, map<int, bool>, int, int);
  //! Sets initial values for coordinates and L sets
  /*! Sets: P(v1)=(0,0), P(v2)=(2,0), P(v3)=(1,1) and
   *        L(v1)=v1   , L(v2)=v2   , L(v3)=v3
   * \param struct CanonicalOrder CanonicalOrder structure
   * \param map<int, struct Point> reference to map of points
   * \param map<int, vector<int> > reference to L sets
   */
  // static void setInitialStraightLine(struct CanonicalOrder, map<int, struct
  // Point>&, map<int, vector<int> >&);
  //! Processes w_(p+1), ..., w_(q-1)
  /*! For each v in U(L(wi)), i=p+1, ..., q-1, do x(v)++
   * \param map<int, struct Point> refernce to map of coordinate points for the
   * drawing
   * \param vector<int> neighbours of vk
   * \param map<int, vector<int> > map of sets L
   */
  // static void processwpq(map<int, struct Point>& pointMap, vector<int> wpq,
  // map<int, vector<int> > L);
  //! Process w_q, ..., w_t
  /*! For each v in U(L(wi)), i=q, ..., t do x(v)+=2
   * \param map<int, struct Point> reference to map of coordinate points for the
   * drawing
   * \param vector<int> outerCycle of G
   * \param map<int, vector<int> > map of L sets
   * \param int q, update should start from here
   */
  // static void processwqt(map<int, struct Point>& pointMap, vector<int> w1t,
  // map<int, vector<int> > L, int q);
  //! Calculates P(vk)
  /*! P(vk)=nu(wp, wq)=(1/2*(x1-y1+x2+y2), 1/2*(-x1+y1+x2+y2))
   * \param map<int, struct Point> reference to map of coordinate points for the
   * drawing
   * \param int wp
   * \param int wq
   * \param int k index (index of new vertex)
   */
  // static void computeNewVertex(map<int, struct Point>& pointMap, int wp, int
  // wq, int k);
  //! Calculates L(vk)
  /*! L(vk)=vkU{U L(wi)}, i=p+1, ..., q-1
   * \param map<int, vector<int> > reference to map of L sets
   * \param vector<int> neighbours of vk - wp, ..., wq
   * \param int vk vertex (vertex whose L set is being calculated)
   */
  // static void computeNewL(map<int, vector<int> >& L, vector<int> wpq, int
  // vk);
  //! Checks if first argument is less then the second in canonical ordering
  /*! If first argument comes before the second then first argument is
   * considered
   * to be less then second.
   * \param struct CanonicalOrder canonical order structure
   * \param int first argument
   * \param int second argument
   * \return boolean value indicating if first argument is less then second (in
   * canonical ordering)
   */
  // static bool canonicalOrderLess(struct CanonicalOrder, int , int);
  //! Checks if three points are colinear
  /*!
   * \param float x1
   * \param float y1
   * \param float x2
   * \param float y2
   * \param float x3
   * \param float y3
   * \param float accuracy
   * \return boolean value indicating if three points are colinear
   */
  static bool areCollinear(float, float, float, float, float, float, float);
  //! Gets dummy1 vertex
  /*! Dummy1 vertex is vertex that is barycenter of all fixed vertices
   * \param map<int, struct Point> fixed vertices
   * \return struct Point dummy vertex
   */
  static struct Point getDummy1(std::map<int, struct Point>);
  //! Gets dummy2 vertex
  /*! Dummy1 vertex is vertex that is barycenter of all vertices except for
   * the last.
   * \param map<int, struct Point> fixed vertices
   * \return struct Point dummy vertex
   */
  static struct Point getDummy2(std::map<int, struct Point>);
  //! Checks if two vectors are equal
  /*!\param vector<int> first vector
   * \param vector<int> second vector
   * \return boolean value indicating if vectors are equal
   */
  static bool areVectorsEqual(std::vector<int>, std::vector<int>);
  //! Cheks if vertex is fixed vertex
  /*! \param int vertex
   * \param map<int, struct Point> map of fixed vertices
   */
  static bool isVertexFixed(int, std::map<int, struct Point>);
  //! Gets vector of fixed vertices
  /*! Gets vector of fixed vertices based on the map of fixed vertices.
   * \param map<int, struct Point> map of fixed vertices
   * \return vector<int> vector of fixed vertices
   */
  static std::vector<int> makeFixedVerticesVector(std::map<int, struct Point>);
  //! Gets map of neighbours for every free vertex
  /*! \param Graph graph
   * \param map<int, struct Point> fixed vertices
   * \return map<int, vector<int> > map of neighbours of free vertices
   */
  static std::map<int, std::vector<int> > getNeighbourMap(Graph, std::map<int, struct Point>);
  //! Gets highest number of vertices that have same neighbours
  /*! \param map<int, vector<int> > neighbourMap
   * \return int highest number of vertices with same neighbours
   */
  static int getMostSameNeighbours(std::map<int, std::vector<int> >);
  //! Gets bouding box
  /*! Gets bouding box of fixed vertices
   * \param map<int, struct Point> fixed vertices
   * \param struct Point& left bottom (this is out argument)
   * \param struct Point& right top (this is out argument)
   */
  static void getBoundingBox(std::map<int, struct Point> fixedVertices,
                             struct Point &, struct Point &);
  //! Gets dummy vertices
  /*! Gets dummy vertices. Dummy vertices is map of new vertices with their
   * coordinates.
   * \param map<int, struct Point> fixed vertices
   * \param int highest number of vertices that have the same neighbours
   * \param Graph graph
   * \return map<int, struct Point> map of dummy vertices
   */
  static std::map<int, struct Point> getDummyVerticesMap(std::map<int, struct Point>, int,
                                                    Graph);
  //! Gets dummy map
  /*! Gets dummy map. Dummy map is a map of free vertices, and dummy vertices
   * that
   * should get connected with them. In case, that a vertex doesn't need dummy
   * vertex
   * value is 0.
   * \param map<int, vector<int> > neighbour map
   * \param Graph graph
   * \return map<int, int> dummy map
   */
  static std::map<int, int> getDummyMap(std::map<int, std::vector<int> >, Graph);
  //! Prints "TRUE" or "FALSE" depending on the argument
  /*! This functions is used to ease debugging process. It prints TRUE or FALSE
   * depending on the argument.
   * \param bool boolean value that should be printed out
   * \return string "TRUE" or "FALSE"
   */
  static std::string printbool(bool);
  //! Formats and prints all nodes of the given graph
  /*!\param Graph& graph that should be printed
   */
  // static void printNodes(Graph&);
  //! Allocates memory for adjacency matrix
  /*! Memory for adjacency matrix should be allocated before entering member
   * function
   * that returns adjacency matrix. Memory should be allocated for matrix of the
   * size
   * [n x n], where n is number of nodes of the given graph.
   * \param int size (n) of the adjacency matrix
   * \return bool** address of the adjacency matrix
   */
  static std::vector<std::vector<bool>> allocateMatrix(int);
  //! Prints adjacency matrix
  /*! \param bool** adjacency matrix
   * \param int size of the adjacency matrix
   */
  static void printAdjacencyMatrix(bool **, int);
  //! Prints all data memebers from CanonicalOrder structure
  /*! \param struct CanonicalOrder CanonicalOrder structure
   */
  static void printCanonicalOrderStructure(struct CanonicalOrder);
  //! Deletes CanonicalOrder structure
  /*! \param struct CanonicalOrder& reference to CanonicalOrder structure that
   * should be deleted
   */
  static void deleteCanonicalOrderStructure(struct CanonicalOrder &);
  //! Prints vector of ints
  /*!
   * \param vector<int> vector to be printed out
   */
  static void printVector(std::vector<int>);
  //! Prints map of coordinates of the drawing
  /*! \param map<int, struct Point> straightline map
   */
  static void printCoordinates(std::map<int, struct Point>);
  //! Prints elements of set L
  /*! Prints all of the elements for every set L
   * \param map<int, vector<int> > vector of sets L
   */
  static void printSetL(std::map<int, std::vector<int> >);
  //! Prints error message and stops execution of the program
  /*! \param string error message to be printed out
   */
  static void Error(const std::string &message);
  //! Prints layers used for Arc-Layered drawing
  /*! \param vector<vector<int> > layers to be printed out
   */
  static void writeLayers(std::vector<std::vector<int> >);
  //! Prints all settings values
  /*! \param Settings settings object
   */
  static void writeSettings(Settings);
};

/*! \brief Structure for working with canonical order.
 *
 * This structure is filled with elements needed for working with canonical
 * ordering. All of the
 * members of this structure are used while calculating straightline drawing.
 */
struct CanonicalOrder {
  //! Canonical ordering
  std::vector<int> canonicalOrdering;
  //! Map of outer cycles of Gk
  std::map<int, std::vector<int> > c0Gk;
  //! Map of neighbours of vertex vk
  std::map<int, std::vector<int> > vk_neighbours;
};

#endif
