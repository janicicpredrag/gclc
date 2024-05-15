#ifndef __graph_h
#define __graph_h

#ifdef WIN32
// disable warnings about long names
// #pragma warning( disable : 4786)
#endif

#include "graphnode.h"
#include <fstream>
#include <list>
#include <map>
#include <vector>

//! Graph is class used to represent a graph
class Graph {
private:
  std::vector< std::list<GraphNode> > adjacencyList;
  //! deletes a graph
  void deleteGraph();
  //! function internally used for DFS algorithm
  void DFSSearch(Graph &, std::map<int, bool> &, int);
  //! function used internally for BFS algorithm
  void BFSScan(Graph &, std::map<int, bool> &, std::map<int, int> &, std::list<int> &, int);
  //! returns true if graph is empty
  bool isEmpty() const;

public:
  //! Default constructor
  Graph();
  //! const string & constructor
  /*!\param filename filename to read graph data from
   */
  Graph(const std::string &);
  //! Copy constructor
  Graph(Graph &);
  //! Gets number of nodes
  /*! This function is used to get number of graph nodes
   * \return number of nodes
   */
  int getNodesNumber() const;
  //! Gets highest node number
  /*! \return highest node number
   */
  int getHighestNodeNumber() const;
  //! Gets graph node
  /*! Gets graph node for specified node number
   *\param int node number
   *\return graph node for specified node number
   */
  GraphNode getGraphNode(int);
  //! Gets graph node at specified index
  /*!\param int node index
   * \return GraphNode graph node at specified index in adjacency list
   */
  GraphNode getNodeAt(int);
  //! Gets all graph nodes
  /*!\return vector of all graph nodes
   */
  std::vector<GraphNode> getNodes();
  //! Updates node labels
  /*! Updates node labels in case that they are not allready set. For instance,
   * if node number is 1, then node label will be set to "1" in this
   * function.
   */
  void updateNodeLabels();
  //! Adds new node (int version)
  /*! Default graph node will be created (graph node with provided node
   * number and empty node label).
   *\param int node number of graph node to add
   *\return boolean value indicating if graph node was successfully added
   */
  bool addNode(int);
  //! Adds new node (GraphNode version)
  /*! Specified graph node will be added.
   *\param GraphNode graph node to add
   *\return boolean value indicating if graph node was succesffully added.
   */
  bool addNode(GraphNode);
  //! Adds new node (int, const string& version)
  /*! First, new graph node is created with int and const string& that are
   *provided, and
   * then it's added to the graph.
   *\param int node number
   *\param const string& node label
   *\return boolean value indicating if graph node was successfully added
   */
  bool addNode(int, const std::string &);
  //! Gets nodes degree of specified node
  /*! \param int node number of graph node whose node degree is queried
   */
  int getNodesDegree(int);
  //! Tests if graph node is contained in graph (int version)
  /*!\param int node number to check
   *\return boolean value indicating if graph node is in the graph
   */
  bool containsNode(int);
  //! Tests if graph node is contained in graph (GraphNode version)
  /*!\param GraphNode graph node to check
   *\return boolean value indicating if graph node is in the graph
   */
  bool containsNode(const GraphNode&);
  //! Tests if edge between two graph nodes exists
  /*!\param int originating graph node number
   *\param int destination graph node number
   *\return boolean value indicating if edge between nodes exists
   */
  bool containsEdge(int, int);
  //! Gets node index in adjacency list
  /*! Tries to get index of a node from adjacency list. If node doesn't exist
   * then -1 is returned.
   *\param int node number to look for
   *\return int index value for this node number (-1 if it node isn't in
   *adjacency list)
   */
  int nodeIndex(int);
  //! Adds new edge
  /*! Tries to add new edge. If edge allready exists, this request will be
   *ignored.
   *\param int originating edge node number
   *\param int destination edge node number
   *\return boolean value indicating if new edge is added
   */
  bool addEdge(int, int);
  //! Removes given (directed) edge
  /*! Tries to remove given (directed) edge. If edge doesn't exit, this request
   * will be ignored.
   * \param int source end
   * \param int destination end
   * \return boolean value indicating if (directed) edge was succesfully removed
   */
  bool removeEdge(int, int);
  //! Removes given (undirected) edge
  /*! Tries to remove given (undirected) edge. If edge doesn't exist, or if it
   * isn't
   * undirected, this request will be ignored. It is the same which node comes
   * first
   * (source or destination) since edge should be undirected.
   * \param int first node
   * \param int second node
   * \return boolean value indicating if (undirected) edge was succesfully
   * removed
   */
  bool removeEdge2(int, int);
  //! Removes node (and all edges)
  /*! Tries to remove given node and all edges that lead to it and
   * from it.
   * \param int node number
   * \return boolean velue indicating if node was succesfully removed
   */
  bool removeNode(int);
  //! Gets neighbours of a given node
  /*!\param int node number whose neighbours should be returned
   *\return vector<int> vector of node numbers
   */
  std::vector<int> getNeighbours(int);
  //! DFS algorithm for exploring graph
  /*!\param Graph& DFS tree will be stored in this argument (this is out
   * argument)
   */
  void DFS(Graph &);
  //! BFS algorithm for exploring graph
  /*! \param Graph& BFS tree will be stored in this argument (levels of nodes of
   * BFS tree will be stored as node labels)
   */
  void BFS(Graph &);
  //! Tests if graph is connected graph
  /*!\return boolean value indicating if graph is connected graph
   */
  bool isConnected();
  //! Tests if graph is 2-connected
  /*!\return boolean value indicating if graph is 2-connected
   */
  bool is2Connected();
  //! Gets adjacency matrix
  /*!\param std::vector<std::vector<bool>>& adjacency matrix
   * adjacency matrix (this is out argument). It is very important
   * to proparly allocate memory for adjacency matrix [number_of_nodes x
   * number_of_nodes],
   * since there is no checking from this member function if memory is properly
   * allocated.
   */
  void getAdjacencyMatrix(std::vector<std::vector<bool>> &);
  //! Tests if graph is undirected
  /*!\return boolean value indicating if graph is undirected
   */
  bool isUnDirected();
  //! Makes a undirected graph
  /*! If graph is directed, it is converted to undirected graph. Otherwise,
   * graph
   * remains unchanged.
   */
  void makeUndirected();
  //! Gets outer cycle of the graph
  /*! Returns outer cycle of the (internally triangulated) graph. This can be
   * changed in the future.
   * \param vector<int>& reference to outer cycle vector
   * \return boolean value indicating if graph is internaly triangulated
   */
  bool getOuterCycle(std::vector<int> &);
  //! Computes canonical ordering of the graph
  /*! Calculates canonical ordering of the graph. Graph
   * must be 2-connected and internally triangulated.
   * \param struct CanonicalOrder& reference to CanonicalOrder structure that is
   * going to be
   * filled in this function with canonical ordering. This is out parameter.
   * \return boolean value indicating if canonical ordering has been calculated
   * (every triangulated
   * plane graph has a canonical ordering)
   */
  // bool canonicalOrdering(struct CanonicalOrder&);
  //! Computes coordinates for straightline drawing.
  /*! This function calculates coordinates for straightline drawing. Canonical
   * ordering is calculated within the function.
   * \return map<int struct point> map of coordinate points to be drawn
   */
  // map<int, struct Point> getStraightLineDrawing();
  bool operator==(Graph &);
  bool operator!=(Graph &);
  Graph &operator=(Graph &);
  //! Loads graph from file
  /*!\param const string & filename
   *\return boolean value indicating if graph was loaded properly
   */
  bool fromFile(const std::string &);
  // friend ostream& operator<<(ostream&, Graph&);
};

#endif
