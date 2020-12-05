#include "arclayereddrawing.h"
#include <cmath>

ArcLayeredDrawing::ArcLayeredDrawing(Graph _graph, Settings _settings,
                                     string _filename)
    : Drawing(_filename, _graph, _settings) {
  // set graph center and graph width valued to invalid values
  graphCenter.x = -1.0;
  graphCenter.y = -1.0;
  graphWidth = -1.0;

  // compute coordinates
  if (isValid)
    computeCoordinates();
}

ArcLayeredDrawing::ArcLayeredDrawing(Graph _graph, Settings _settings,
                                     struct Point _graphCenter,
                                     float _graphWidth, string _filename)
    : Drawing(_filename, _graph, _settings) {
  graphCenter = _graphCenter;
  graphWidth = _graphWidth;

  // compute coordinates
  if (isValid)
    computeCoordinates();
}

bool ArcLayeredDrawing::isCenterAndWidthProvided() const {
  if (graphCenter.x != -1.0 && graphCenter.y != -1.0 && graphWidth != -1.0)
    return true;
  else
    return false;
}

void ArcLayeredDrawing::recalculateOffsetAndScalingFactors(
    unsigned int widestLayerWidth, float topYCoordinate,
    unsigned int widestArc) {
  float scalingFactor, offsetFactorX, offsetFactorY;
  offsetFactorX = graphCenter.x - graphWidth / 2;
  scalingFactor = graphWidth / ((widestLayerWidth - 1) * 2);
  if (widestArc <= 1)
    topYCoordinate -= 2;
  else
    topYCoordinate -= widestArc / 2;
  offsetFactorY = graphCenter.y - (float)(topYCoordinate * scalingFactor / 2);
  settings.setOffsetFactorX(offsetFactorX);
  settings.setOffsetFactorY(offsetFactorY);
  settings.setScalingFactor(scalingFactor);
}

void ArcLayeredDrawing::setInitialLayer(int n,
                                        map<int, bool> &unExploredNodes) {
  if (n > graph.getNodesNumber())
    GraphUtil::Error("index out of bound");
  vector<int> firstLayer;
  for (int i = 0; i < n; i++) {
    GraphNode gn = graph.getNodeAt(i);
    firstLayer.push_back(gn.getNodeNumber());
    unExploredNodes[gn.getNodeNumber()] = true;
  }
  layers.push_back(firstLayer);
}

void ArcLayeredDrawing::setAllNodesToUnexplored(
    map<int, bool> &unExploredNodes) {
  vector<GraphNode> allNodes = graph.getNodes();
  for (unsigned int i = 0; i < allNodes.size(); i++)
    unExploredNodes[allNodes[i].getNodeNumber()] = false;
}

bool ArcLayeredDrawing::existUnexploredNode(map<int, bool> unExploredNodes) {
  for (map<int, bool>::const_iterator it = unExploredNodes.begin();
       it != unExploredNodes.end(); it++)
    if (!it->second)
      return true;
  return false;
}

unsigned int ArcLayeredDrawing::getWidestLayerWidth() const {
  unsigned int widestLayerWidth = 0;
  for (unsigned int i = 0; i < layers.size(); i++)
    if (layers[i].size() > widestLayerWidth)
      widestLayerWidth = layers[i].size();
  return widestLayerWidth;
}

void ArcLayeredDrawing::setXCoordinate(int layerLevel, int widestLayerWidth) {
  int starting_position = widestLayerWidth - layers[layerLevel].size();
  for (unsigned int i = 0; i < layers[layerLevel].size(); i++)
    coordinates[layers[layerLevel][i]].x = starting_position + 2 * i;
}

void ArcLayeredDrawing::setYCoordinate(int layerLevel, int currentYCoordinate) {
  for (unsigned int i = 0; i < layers[layerLevel].size(); i++)
    coordinates[layers[layerLevel][i]].y = currentYCoordinate;
}

unsigned int ArcLayeredDrawing::getWidestArcEdge(int layerLevel) {
  unsigned int widestArcEdge = 0;
  for (unsigned int i = 0; i < layers[layerLevel].size() - 1; i++)
    for (unsigned int j = i + 1; j < layers[layerLevel].size(); j++)
      if (graph.containsEdge(layers[layerLevel][i], layers[layerLevel][j]))
        if (j - i > widestArcEdge)
          widestArcEdge = j - i;
  return widestArcEdge;
}

vector<int> ArcLayeredDrawing::getDescendants(int i,
                                              map<int, bool> &unExploredNodes) {
  vector<int> descendants;
  for (unsigned int j = 0; j < layers[i].size(); j++) {
    vector<int> neighbours = graph.getNeighbours(layers[i][j]);
    mergeDescendants(descendants, neighbours, unExploredNodes);
  }
  return descendants;
}

void ArcLayeredDrawing::mergeDescendants(
    vector<int> &descendants, vector<int> neighbours,
    map<int, bool> &unExploredNodes) const {
  for (unsigned int i = 0; i < neighbours.size(); i++)
    if (!GraphUtil::isElementInVector(descendants, neighbours[i]) &&
        !unExploredNodes[neighbours[i]]) {
      descendants.push_back(neighbours[i]);
      unExploredNodes[neighbours[i]] = true;
    }
}

void ArcLayeredDrawing::makeLayers() {
  // set unexplored nodes
  map<int, bool> unExploredNodes;
  setAllNodesToUnexplored(unExploredNodes);
  // instead of 1, number should be checked in settings object
  setInitialLayer(1, unExploredNodes);
  // set all layers
  int i = 0;
  while (existUnexploredNode(unExploredNodes)) {
    vector<int> descendants = getDescendants(i, unExploredNodes);
    layers.push_back(descendants);
    i++;
  }
}

void ArcLayeredDrawing::makeConnectionsGCLC(ofstream &graphFile,
                                            Graph labeledGraph) {
  for (unsigned int i = 0; i < layers.size() - 1; i++) {
    connectAdjLayers(graphFile, i, labeledGraph);
    connectLayer(graphFile, i, labeledGraph);
  }
  connectLayer(graphFile, layers.size() - 1, labeledGraph);
}

void ArcLayeredDrawing::connectAdjLayers(ofstream &graphFile, int layerIndex,
                                         Graph labeledGraph) {
  for (unsigned int i = 0; i < layers[layerIndex].size(); i++) {
    for (unsigned int j = 0; j < layers[layerIndex + 1].size(); j++) {
      if (labeledGraph.containsEdge(layers[layerIndex][i],
                                    layers[layerIndex + 1][j])) {
        GraphNode gn1 = labeledGraph.getGraphNode(layers[layerIndex][i]);
        GraphNode gn2 = labeledGraph.getGraphNode(layers[layerIndex + 1][j]);
        drawLineGCLC(graphFile, gn1.getNodeNumber(), gn2.getNodeNumber());
      }
    }
  }
}

void ArcLayeredDrawing::connectLayer(ofstream &graphFile, int layerIndex,
                                     Graph labeledGraph) {
  for (unsigned int i = 0; i < layers[layerIndex].size() - 1; i++) {
    for (unsigned int j = i + 1; j < layers[layerIndex].size(); j++) {
      if (labeledGraph.containsEdge(layers[layerIndex][i],
                                    layers[layerIndex][j])) {
        if (i + 1 == j) // draw line
          drawLineGCLC(graphFile, layers[layerIndex][i], layers[layerIndex][j]);
        else // draw arc
          drawArcGCLC(graphFile, layers[layerIndex][i], layers[layerIndex][j]);
      }
    }
  }
}

void ArcLayeredDrawing::computeCoordinates() {
  // create layers
  makeLayers();

  unsigned int widestLayerWidth =
      getWidestLayerWidth();         // get widest layer width
  unsigned int horizontalOffset = 0; // offset between layers (initially zero)

  // set all coordinates initially to zero
  setZeroCoordinates();
  // set up coordinates for each layer
  int currentYCoordinate = 0;
  for (unsigned int layerLevel = 0; layerLevel < layers.size(); layerLevel++) {
    horizontalOffset = getWidestArcEdge(layerLevel);
    setXCoordinate(layerLevel, widestLayerWidth);
    setYCoordinate(layerLevel, currentYCoordinate);
    if (horizontalOffset > 1)
      currentYCoordinate += horizontalOffset + 1;
    else
      currentYCoordinate += 2;
  }
  // recalculate offset and scaling factors
  if (isCenterAndWidthProvided())
    recalculateOffsetAndScalingFactors(
        widestLayerWidth, (float)currentYCoordinate, horizontalOffset);
  // scale and offset coordinates
  scaleAndOffsetCoordinates();
}

bool ArcLayeredDrawing::isArc(int u, int v) {
  if (u == v)
    return false;
  if (!graph.containsNode(u) || !graph.containsNode(v))
    return false;
  if (!graph.containsEdge(u, v))
    return false;
  if (coordinates[u].y != coordinates[v].y)
    return false;
  if (fabs(coordinates[u].x - coordinates[v].x) ==
      2 * settings.getScalingFactor())
    return false;
  return true;
}
