#ifndef __barycenter_h
#define __barycenter_h
#include "drawing.h"

//! Class barycenter drawing for any kind of graph
class BarycenterDrawing:public Drawing
{
 private:
  map<int, struct Point> fixedVertices; // fixed vertices
  //! Set fixed coordinates
  /*! \param map<int, bool>& reference to x updating variable
   * \param map<int, bool>& reference to y updating variable
   */
  void setFixedCoordinates(map<int, bool>&, map<int, bool>&);
  //! Sets all vertices in state where every vertes needs x and y updating
  /*! \param map<int, bool>& reference to x updating variable
   * \param map<int, bool>& reference to y updating variable
   */
  void resetXYUpdating(map<int, bool>&, map<int, bool>&);
  //! Gets value indicating if all vertices are updated
  /*! If all values are converging then stop working, otherwise keep working.
   * To continue working it is enough that there is at least one value
   * that isn't converging. Another criteria for stop working is that
   * current iteration is greater or equalt to maximum numbeer of iterations.
   * \param map<int, bool> x updating variable
   * \param map<int, bool> y updating variable
   * \param int current iteration
   * \return bool boolean value indicating if there are still some values
   * that needs evaluating
   */
  bool continueWorking(map<int, bool>, map<int, bool>, int);
  //! Starts converging to solution
  /*! Finds all nodes coordinates.
   * \param map<int, bool>& reference to x updating variable
   * \param map<int, bool>& reference to y updating variable
   */
  int convergeToSolution(map<int, bool>&, map<int, bool>&);
  //! Connects nodes in GCLC
  /*!\param ofstream& reference to file
   * \param Graph labeled graph
   */
  void makeConnectionsGCLC(ofstream&, Graph);
  /*! Computes coordinates using this method
   */
  void computeCoordinates();
  //! Adds dummy vertices (if it is needed)
  /*! There are two "bugs" in original barycenter method:
   * 1. when there are nodes of degree 1, which are not fixed. In this case
   * one or two dummy vertices are added
   * 2. there are several vertices which have the same naighbours. In this case
   * dummy vertices are added as well   
   */
  void addDummyVertices();
  //! Fixes one degree vertices bug  
  void fix1DegreeVertices();
  //! Fixes bug with vertices that have same neighbours
  void fixSameNeighbourVertices();
  //! Adds dummy vertices to solve "same naighbour" bug  
  /*! \param map<int, int> dummy map
   * \param map<int, struct Point> dummy vertices map
   */
  void addDummyForSameNeighbour(map<int, int> dummyMap, map<int, struct Point>);
  //! Removes dummy coordinates
  void removeDummyCoordinates();
 public:
  //! A constructor
  /*!
   * \param Graph graph that should be drawn
   * \param Settings settings object
   * \param map<int, struct Point> fixed vertices
   * \param string file name
   */
  BarycenterDrawing(Graph, Settings, map<int, struct Point>, string="");
  //! Returns boolean value indicating if this edge should be drawn as arc.
  /*!
   * \param int first nodes node number
   * \param int second nodes node number
   * \return boolean value indicating if the edge should be drawn as an arc
   */
  bool isArc(int, int);
};

#endif
