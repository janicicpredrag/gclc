#ifndef __arclayered_h
#define __arclayered_h

#include "drawing.h"
#include "settings.h"

//! Class for arc-layered drawing of any kind of graph
class ArcLayeredDrawing:public Drawing
{
 private:
  std::vector<std::vector <int> > layers; // layers needed for this drawing
  struct Point graphCenter;     // center of graph drawing
  float graphWidth;             // width of graph drawing
  //! Returns boolean value indicating if center and the width of the drawing are provided
  /*! \return boolean value indicating if center and width of the drawing are provided
   */
  bool isCenterAndWidthProvided() const;
  //! Recalculates offset and scaling factors
  /*! Recalculates offset and scaling factors using graph center
   * and graph width. This method is called if second constructor was
   * used to create an object.
   * \param unsigned int widest layer width
   * \param float y coordinate of the top of the drawing
   * \param unsigned int widest arc edge of the top level (if it is greater then 1 used, otherwise don't
   * use it)
   */
  void recalculateOffsetAndScalingFactors(unsigned int, float, unsigned int);
  //! Sets first layer
  /*! \param int number of nodes in first layer (default should be 0)
   * \param map<int, bool>& reference  to map of unexplored nodes
   */
  void setInitialLayer(int, std::map<int, bool>&);
  //! Sets all nodes as unexplored
  /*! \param map<int, bool>& reference to unexplored nodes
   */
  void setAllNodesToUnexplored(std::map<int, bool>&);
  //! Checks if there are still unexplored nodes
  /*! \param map<int, bool> map of unexplored nodes
   */
  bool existUnexploredNode(std::map<int, bool>);
  //! Gets widest layer width (layer with largest number of elements)
  /*! \return unsigned int largest layer width
   */
  unsigned int getWidestLayerWidth() const;
  //! Gets widest arc edge on one layer.
  /*! Gets widest arc edge on one layer. It is used for calculating
   * horizontal distance between two layers:  
   * distance_between_two_layers=getWidestLayerWidth()/2+1, if getWidestLayerWidth()>1
   * distance_between_two_layers=2, otherwise
   * \return unsigned int widest arc edge
   */
  unsigned int getWidestArcEdge(int);
  //! Sets x coordinate for nodes
  /*! Sets x coordinates for nodes in the drawing
   * \param int layer level
   * \param int widest layer width
   */
  void setXCoordinate(int, int);
  //! Sets y coordinate for nodes
  /*! Sets y coordinates for nodes in the drawing
   * \param int layer level
   * \param int current y coordinate
   */
  void setYCoordinate(int, int);
  //! Get all nodes that are descendants from nodes from specified layer
  /*! \param int ancestor layer (we are looking of its descendants)
   * \param map<int, bool>& reference to map of unexplored nodes (it is updated in this function)
   */
  std::vector<int> getDescendants(int, std::map<int, bool>&);
  //! Merges descendants of the layer into one
  /*! \param vector<int>& reference to descendants vector (out argument, vector being merged)
   * \param vector<int> source vector (merging from)
   */
  void mergeDescendants(std::vector<int>&, std::vector<int>, std::map<int, bool>&) const;
  //! Makes layers needed for the drawing
  void makeLayers();
  //! Make connections in GCLC
  /*! Connected nodes with edges
   * \param ofstream& reference to file
   * \param Graph labeled graph
   */
  void makeConnectionsGCLC(std::ofstream&, Graph);
  //! Connects adjacent layers
  /*!\param ofstream& reference to file 
   * \param int layer index
   * \param Graph labeled graph
   */
  void connectAdjLayers(std::ofstream&, int, Graph);
  //! Connects nodes within the layer
  /*!\param ofstream& reference to file 
   * \param int layer index
   * \param Graph labeled graph
   */
  void connectLayer(std::ofstream&, int, Graph);
  //! Computers coordinates
  void computeCoordinates();
 public:
  //! A constructor
  /*!
   * \param Graph graph that should be drawn
   * \param Settings settings object
   * \param string file name
   */
  ArcLayeredDrawing(Graph, Settings, std::string="");
  //! A constructor
  /*! If center of the drawing and width of the drawing parameters are given,
   * then offset and scaling factors from Settings object are overridden (they are
   * calculated based on these two parameters).
   * 
   * \param Graph graph that should be drawn
   * \param Settings settings object
   * \param struct Point center of the drawing
   * \param float width of the drawing
   * \param string file name
   */
  ArcLayeredDrawing(Graph, Settings, struct Point, float, std::string="");
  //! Returns if edge should be drawn as an arc.
  /*! Returns true if nodes are on the same layer, and are consequent on that layer.
   * \param int first nodes node number
   * \param int second nodes node number
   * \return boolean value indicating if the edge should be drawn as an arc
   */
  bool isArc(int, int);
};

#endif
