#include "graph.h"
#include "../Utils/Utils.h"
#include "graph_util.h"
#include <vector>

Graph::Graph() {}

Graph::Graph(const std::string &filename) { fromFile(filename); }

Graph::Graph(Graph &graph) {
  for (int i = 0; i < graph.getNodesNumber(); i++)
    adjacencyList.push_back(graph.adjacencyList[i]);
}

void Graph::deleteGraph() {
  adjacencyList.clear();
}

bool Graph::isEmpty() const { return (getNodesNumber() == 0); }

int Graph::getNodesNumber() const { return (int)adjacencyList.size(); }

int Graph::getHighestNodeNumber() const {
  std::list<GraphNode>::const_iterator itGraphNode = adjacencyList[0].begin();
  int maxNodeNumber = itGraphNode->getNodeNumber();
  for (const std::list<GraphNode> &adjacencies : adjacencyList) {
    itGraphNode = adjacencies.begin();
    if (itGraphNode->getNodeNumber() > maxNodeNumber)
      maxNodeNumber = itGraphNode->getNodeNumber();
  }
  return maxNodeNumber;
}

GraphNode Graph::getGraphNode(int nodeNumber) {
  GraphNode emptyGraphNode;
  if (!containsNode(nodeNumber))
    return emptyGraphNode;
  for (unsigned int i = 0; i < adjacencyList.size(); i++) {
    std::list<GraphNode>::const_iterator itGraphNode = adjacencyList[i].begin();
    if (itGraphNode->getNodeNumber() == nodeNumber)
      return *itGraphNode;
  }
  // function will never come here
  return emptyGraphNode;
}

GraphNode Graph::getNodeAt(int index) {
  if ((unsigned int)index >= adjacencyList.size())
    GraphUtil::Error("index out of bound");
  std::list<GraphNode>::const_iterator it = adjacencyList[index].begin();
  GraphNode returnNode = *it;
  return returnNode;
}

std::vector<GraphNode> Graph::getNodes() {
  std::vector<GraphNode> returnNodes;
  for (unsigned int i = 0; i < adjacencyList.size(); i++) {
    std::list<GraphNode>::const_iterator tempList = adjacencyList[i].begin();
    returnNodes.push_back(*tempList);
  }
  return returnNodes;
}

void Graph::updateNodeLabels() {
  for (unsigned int i = 0; i < adjacencyList.size(); i++) {
    for (std::list<GraphNode>::iterator it = adjacencyList[i].begin();
         it != adjacencyList[i].end(); it++)
      it->updateNodeLabel();
  }
}

bool Graph::addNode(int nodeNumber) {
  if (containsNode(nodeNumber))
    return false;
  GraphNode tempNode(nodeNumber);
  std::list<GraphNode> tempList;
  tempList.push_back(tempNode);
  adjacencyList.push_back(tempList);
  return true;
}

int Graph::getNodesDegree(int nodeNumber) {
  std::vector<int> neighbours = getNeighbours(nodeNumber);
  return (int)neighbours.size();
}

bool Graph::addNode(GraphNode node) {
  if (containsNode(node))
    return false;
  std::list<GraphNode> tempList;
  tempList.push_back(node);
  adjacencyList.push_back(tempList);
  return true;
}

bool Graph::addNode(int nodeNumber, const std::string &nodeLabel) {
  if (containsNode(nodeNumber))
    return false;
  GraphNode tempGraphNode(nodeNumber, nodeLabel);
  addNode(tempGraphNode);
  return true;
}

bool Graph::containsNode(int nodeNumber) {
  std::list<GraphNode>::const_iterator tempIt;
  for (const std::list<GraphNode> &adjacencies : adjacencyList) {
    tempIt = adjacencies.begin();
    if (tempIt->getNodeNumber() == nodeNumber)
      return true;
  }
  return false;
}

bool Graph::containsNode(const GraphNode &node) {
  std::list<GraphNode>::const_iterator tempIt;
  for (const std::list<GraphNode> &adjacencies : adjacencyList) {
    tempIt = adjacencies.begin();
    if (*tempIt == node)
      return true;
  }
  return false;
}

bool Graph::containsEdge(int from, int to) {
  if (!containsNode(from))
    return false;
  int selectedNode = nodeIndex(from);
  for (const GraphNode &node : adjacencyList[selectedNode])
    if (node.getNodeNumber() == to)
      return true;
  return false;
}

int Graph::nodeIndex(int nodeNumber) {
  if (!containsNode(nodeNumber))
    return -1;
  for (unsigned int i = 0; i < adjacencyList.size(); i++) {
    std::list<GraphNode>::const_iterator tempIt = adjacencyList[i].begin();
    if (tempIt->getNodeNumber() == nodeNumber)
      return i;
  }
  return -1;
}

bool Graph::addEdge(int from, int to) {
  if (!containsNode(from))
    return false;
  if (!containsNode(to))
    return false;
  if (containsEdge(from, to))
    return false;

  // get ending point GraphNode
  GraphNode endPointEdge = getGraphNode(to);

  for (std::list<GraphNode> &adjacencies : adjacencyList) {
    std::list<GraphNode>::const_iterator itGraphNode = adjacencies.begin();
    if (itGraphNode->getNodeNumber() == from)
      adjacencies.push_back(endPointEdge);
  }
  return true;
}

bool Graph::removeEdge(int from, int to) {
  if (!containsNode(from) || !containsNode(to) || !containsEdge(from, to))
    return false;
  // get node index
  int node_index = nodeIndex(from);
  adjacencyList[node_index].remove(to);

  return true;
}

bool Graph::removeEdge2(int from, int to) {
  if (!containsNode(from) || !containsNode(to) || !containsEdge(from, to) ||
      !containsEdge(to, from))
    return false;
  // remove first edge
  int node_index = nodeIndex(from);
  adjacencyList[node_index].remove(to);
  node_index = nodeIndex(to);
  adjacencyList[node_index].remove(from);
  return true;
}

bool Graph::removeNode(int removingNode) {
  if (!containsNode(removingNode))
    return false;
  // get all neighbours
  std::vector<int> neighbours = getNeighbours(removingNode);
  // go through every neighbour and remove edges to removingNode
  unsigned int i;
  for (i = 0; i < neighbours.size(); i++)
    removeEdge(neighbours[i], removingNode);
  // remove removingNode
  unsigned int node_index = nodeIndex(removingNode);
  std::vector< std::list<GraphNode> >::iterator tempIt = adjacencyList.begin();
  for (i = 0; i < node_index; i++)
    tempIt++;
  adjacencyList.erase(tempIt);

  return true;
}

std::vector<int> Graph::getNeighbours(int _nodeNumber) {
  int currentIndex = nodeIndex(_nodeNumber);
  std::vector<int> neighbours;
  std::list<GraphNode>::const_iterator from_second =
      adjacencyList[currentIndex].begin();
  if (adjacencyList[currentIndex].size() > 1)
    from_second++;
  else
    return neighbours;
  for (std::list<GraphNode>::const_iterator it = from_second;
       it != adjacencyList[currentIndex].end(); it++)
    neighbours.push_back(it->getNodeNumber());
  return neighbours;
}

void Graph::DFS(Graph &dfsTree) {
  // set dfstree to empty tree
  dfsTree.deleteGraph();
  if (isEmpty())
    return;
  std::map<int, bool> vertex_map;
  // get all nodes and mark them as new
  std::vector<GraphNode> nodes = getNodes();
  for (unsigned int i = 0; i < nodes.size(); i++)
    vertex_map[nodes[i].getNodeNumber()] = true;

  // for every vertex marked as new do DFSSearch()
  for (const std::pair<int, bool> it : vertex_map)
    if (it.second) {
      dfsTree.addNode(it.first);
      DFSSearch(dfsTree, vertex_map, it.first);
    }
}

void Graph::DFSSearch(Graph &dfsTree, std::map<int, bool> &vertex_map,
                      int _nodeNumber) {
  // mark vertex as old
  vertex_map[_nodeNumber] = false;
  // get neighbours for currentNode
  std::vector<int> neighbours = getNeighbours(_nodeNumber);
  // for every neighbour in adjacencylist
  for (unsigned int i = 0; i < neighbours.size(); i++) {
    // if vertex is marked as new
    if (vertex_map[neighbours[i]]) {
      dfsTree.addNode(neighbours[i]);
      dfsTree.addEdge(_nodeNumber, neighbours[i]);
      DFSSearch(dfsTree, vertex_map, neighbours[i]);
    }
  }
}

void Graph::BFS(Graph &bfsTree) {
  // set bfsTree to empty tree
  bfsTree.deleteGraph();
  if (isEmpty())
    return;
  // BFS can be applied only to connected graph
  if (!isConnected())
    return;
  std::list<int> nodeQueue;
  // map of not reached nodes (at the start all are initialized to true)
  std::map<int, bool> notReached;
  std::map<int, int> mapLevel;
  int i;
  for (i = 0; i < getNodesNumber(); i++) {
    std::list<GraphNode>::const_iterator tempIt = adjacencyList[i].begin();
    notReached[tempIt->getNodeNumber()] = true;
    mapLevel[tempIt->getNodeNumber()] = 0;
  }
  // add first node in the queue and mark it as reached
  std::list<GraphNode>::const_iterator firstNodeIt = adjacencyList[0].begin();
  nodeQueue.push_back(firstNodeIt->getNodeNumber());
  notReached[firstNodeIt->getNodeNumber()] = false;
  mapLevel[firstNodeIt->getNodeNumber()] = 1;
  // add first node to BFSTree
  bfsTree.addNode(firstNodeIt->getNodeNumber());

  // main while loop
  while (!nodeQueue.empty()) {
    std::list<int>::const_iterator headIt = nodeQueue.begin();
    int tempValue = *headIt;
    nodeQueue.pop_front();
    BFSScan(bfsTree, notReached, mapLevel, nodeQueue, tempValue);
  }

  // update levels to nodes
  for (i = 0; i < getNodesNumber(); i++) {
    // update neighbours
    for (GraphNode &node : bfsTree.adjacencyList[i])
      node.setNodeLabel(i2s(mapLevel[node.getNodeNumber()]));
  }
}

void Graph::BFSScan(Graph &bfsTree, std::map<int, bool> &notReached,
                    std::map<int, int> &mapLevel, std::list<int> &nodeQueue, int head) {
  std::vector<int> neighbours = getNeighbours(head);
  // for every neighbour
  for (unsigned int i = 0; i < neighbours.size(); i++) {
    if (notReached[neighbours[i]]) {
      bfsTree.addNode(neighbours[i]);
      bfsTree.addEdge(head, neighbours[i]);
      mapLevel[neighbours[i]] = mapLevel[head] + 1;
      nodeQueue.push_back(neighbours[i]);
      notReached[neighbours[i]] = false;
    }
  }
}

bool Graph::isConnected() {
  if (isEmpty())
    return false;
  Graph dfsTree;
  std::map<int, bool> vertex_map;
  // get all nodes and mark them as new
  std::vector<GraphNode> nodes = getNodes();
  for (unsigned int i = 0; i < nodes.size(); i++)
    vertex_map[nodes[i].getNodeNumber()] = true;

  // temp map iterator
  std::map<int, bool>::const_iterator tempIt = vertex_map.begin();
  dfsTree.addNode(tempIt->first);
  DFSSearch(dfsTree, vertex_map, tempIt->first);

  // if all nodes can be explored from the first one, graph is connected
  if (dfsTree.getNodesNumber() == getNodesNumber())
    return true;
  else
    return false;
}

bool Graph::is2Connected() {
  // get all neighbours
  std::vector<GraphNode> allNodes = getNodes();
  // go through every node and check if graph without that node is connected
  for (unsigned int i = 0; i < allNodes.size(); i++) {
    Graph workingGraph = *this;
    workingGraph.removeNode(allNodes[i].getNodeNumber());
    if (!workingGraph.isConnected())
      return false;
  }
  return true;
}

void Graph::getAdjacencyMatrix(
    std::vector<std::vector<bool>> &adjacencyMatrix) {
  unsigned int numberOfNodes = getNodesNumber();
  // set every element of adjacency matrix to false
  unsigned int i, j;
  for (i = 0; i < numberOfNodes; i++)
    for (j = 0; j < numberOfNodes; j++)
      adjacencyMatrix[i][j] = false;
  // create a map for indexing
  std::map<int, int> matrixIndex;
  std::vector<GraphNode> nodes = getNodes();
  for (i = 0; i < nodes.size(); i++)
    matrixIndex[nodes[i].getNodeNumber()] = i;

  // for every node
  for (i = 0; i < adjacencyList.size(); i++) {
    std::list<GraphNode>::const_iterator nodeIt = adjacencyList[i].begin();
    int currentNode = nodeIt->getNodeNumber();
    std::vector<int> nodeNeighbours = getNeighbours(currentNode);
    // go through every neighbour and update adjacencyMatrix
    for (j = 0; j < nodeNeighbours.size(); j++)
      adjacencyMatrix[matrixIndex[currentNode]]
                     [matrixIndex[nodeNeighbours[j]]] = true;
  }
}

bool Graph::isUnDirected() {
  // allocate memmory for adjacency matrix
  std::vector<std::vector<bool>> adjacencyMatrix =
    GraphUtil::allocateMatrix(getNodesNumber());
  int i;
  // get adjacency matrix
  getAdjacencyMatrix(adjacencyMatrix);

  // test if graph is undirected
  bool graphIsUndirected = true;
  for (i = 0; i < getNodesNumber() - 1; i++)
    for (int j = i + 1; j < getNodesNumber(); j++)
      if (adjacencyMatrix[i][j] != adjacencyMatrix[j][i])
        graphIsUndirected = false;

  return graphIsUndirected;
}

void Graph::makeUndirected() {
  // if graph is undirected return
  if (isUnDirected())
    return;

  // allocate memory for adjacency matrix
  std::vector<std::vector<bool>> adjacencyMatrix =
    GraphUtil::allocateMatrix(getNodesNumber());
  int i;
  // get adjacency matrix
  getAdjacencyMatrix(adjacencyMatrix);

  // make undirected graph
  for (i = 0; i < getNodesNumber() - 1; i++) {
    for (int j = i + 1; j < getNodesNumber(); j++) {
      if (adjacencyMatrix[i][j] && !adjacencyMatrix[j][i]) {
        GraphNode fromNode = getNodeAt(j);
        GraphNode toNode = getNodeAt(i);
        addEdge(fromNode.getNodeNumber(), toNode.getNodeNumber());
      }
      if (adjacencyMatrix[j][i] && !adjacencyMatrix[i][j]) {
        GraphNode fromNode = getNodeAt(i);
        GraphNode toNode = getNodeAt(j);
        addEdge(fromNode.getNodeNumber(), toNode.getNodeNumber());
      }
    }
  }
}

bool Graph::getOuterCycle(std::vector<int> &outerCycle) {
  int startingNode;       // starting node
  int outerNode;          // current outer node
  std::vector<int> neighbours; // neighbours of the current outer node
  std::vector<int>::const_iterator it;
  std::list<GraphNode>::const_iterator tempIt =
      adjacencyList[0].begin(); // temp iterator to get the first node

  // empty outerCycle
  if (!outerCycle.empty())
    outerCycle.clear();

  // if graph is empty return false
  if (isEmpty())
    return false;

  outerNode = tempIt->getNodeNumber();
  startingNode = outerNode;

  // push first outerNode to the returning vector
  outerCycle.push_back(outerNode);
  // get neighbours to get the second outerNode
  neighbours = getNeighbours(outerNode);
  it = neighbours.end();
  outerNode = *(--it);
  outerCycle.push_back(outerNode);

  // get neighbours to get the third outerNode
  neighbours = getNeighbours(outerNode);
  it = neighbours.end();
  outerNode = *(--it);
  outerCycle.push_back(outerNode);

  // get neighbours to get the fourth outerNode (to check if graph is really
  // triangulated)
  neighbours = getNeighbours(outerNode);
  it = neighbours.end();
  outerNode = *(--it);
  if (outerNode != startingNode)
    return false;

  return true;
}

bool Graph::operator==(Graph &newGraph) {
  if (adjacencyList == newGraph.adjacencyList)
    return true;
  else
    return false;
}

bool Graph::operator!=(Graph &newGraph) {
  if (*this == newGraph)
    return false;
  else
    return true;
}

Graph &Graph::operator=(Graph &newGraph) {
  if (*this == newGraph)
    return *this;
  deleteGraph();
  adjacencyList = newGraph.adjacencyList;
  return *this;
}

bool Graph::fromFile(const std::string &filename) {
  std::ifstream graphFile(filename.c_str());
  if (!graphFile.is_open())
    return false;
  // delete existing graph
  deleteGraph();
  // get file info
  std::string file_info;
  graphFile >> file_info;
  // nodes are represented only with node numbers
  if (file_info == "numbers") {
    int node = -1;
    // get all nodes
    while (!graphFile.eof()) {
      while (node != 0) {
        graphFile >> node;
        if (node != 0)
          addNode(node);
      }
      break;
    }
    // read edges
    while (!graphFile.eof()) {
      node = -1;
      graphFile >> node;
      if (graphFile.eof())
        break;
      int currentNode = node;
      // add edges
      while (node != 0) {
        graphFile >> node;
        if (node != 0)
          addEdge(currentNode, node);
      }
    }
  } else if (file_info == "full")
    return false;
  else
    return false;

  graphFile.close();
  return true;
}

// ostream& operator<<(ostream& ostr, Graph& graph)
//{
//  for (int i=0; i<graph.getNodesNumber(); i++)
//    {
//      std::list<GraphNode>::const_iterator itGraphNode;
//      for (itGraphNode=graph.adjacencyList[i].begin();
//      itGraphNode!=graph.adjacencyList[i].end(); itGraphNode++)
//	ostr<<*itGraphNode<<" ";
//      ostr<<endl;
//    }
//  return ostr;
//}
