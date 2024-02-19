#ifndef __graphNode_
#define __graphNode_
#include <iostream>
#include <sstream>
#include <string>


//! GraphNode is class used to represent one graph node.
/*! Graph is consisted of nodes, and this class is used to represent a
 * graph node.
 */
class GraphNode {
private:
  int nodeNumber;
  std::string nodeLabel;
  //  list<GraphNode>::const_iterator it;
public:
  /*!\brief A constructor
   *
   * This is constructor with two arguments.
   * \param _nodeNumber represents value of node number (default value is 0)
   * \param *tmpChar represents value of node label (default value "")
   */
  GraphNode(int _nodeNumber = 0, const std::string &tmpChar = "");
  /*!\brief get number of the graph node
   * \return number of the graph node
   */
  int getNodeNumber() const;
  /*!\brief sets number of the graph node
   *
   * \param _nodeNumber set node number to this value
   */
  void setNodeNumber(int _nodeNumber);
  /*!\brief gets label of the graph node
   *
   *\return label of the graph node
   */
  std::string getNodeLabel() const;
  /*!\brief sets label of the graph node
   *
   *\param _nodeLabel set node label to this value
   */
  void setNodeLabel(std::string _nodeLabel);
  //! Updates node label acording to node number
  /*! Updates node label if it isn't allready set. New node label will be
   * analogue to node number.
   */
  void updateNodeLabel();
  bool operator==(const GraphNode &) const;
  bool operator!=(const GraphNode &) const;
  GraphNode &operator=(const GraphNode &);
  bool operator<(const GraphNode &) const;
  // friend ostream& operator<<(ostream& ostr, const GraphNode& gn);
  // friend istream& operator>>(istream& istr, GraphNode& gn);
};

#endif
