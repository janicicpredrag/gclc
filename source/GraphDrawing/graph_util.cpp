#include "graph_util.h"
#include <cmath>
#include <cstdlib>
#include <vector>

bool GraphUtil::isElementInVector(std::vector<int> vec, int el) {
  for (unsigned int i = 0; i < vec.size(); i++)
    if (el == vec[i])
      return true;
  return false;
}

bool GraphUtil::areCollinear(float x1, float y1, float x2, float y2, float x3,
                             float y3, float epsilon) {
  return (fabs(x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) < epsilon);
}

struct Point GraphUtil::getDummy1(std::map<int, struct Point> fixedVertices) {
  struct Point dummy1;
  dummy1.x = 0;
  dummy1.y = 0;
  for (std::map<int, struct Point>::const_iterator it = fixedVertices.begin();
       it != fixedVertices.end(); it++) {
    dummy1.x += it->second.x;
    dummy1.y += it->second.y;
  }
  dummy1.x /= (float)fixedVertices.size();
  dummy1.y /= (float)fixedVertices.size();
  return dummy1;
}

struct Point GraphUtil::getDummy2(std::map<int, struct Point> fixedVertices) {
  struct Point dummy2;
  dummy2.x = 0;
  dummy2.y = 0;
  unsigned int count = 1;
  for (std::map<int, struct Point>::const_iterator it = fixedVertices.begin();
       it != fixedVertices.end(); it++) {
    if (count == fixedVertices.size())
      break;
    dummy2.x += it->second.x;
    dummy2.y += it->second.y;
    count++;
  }
  dummy2.x /= (float)(fixedVertices.size() - 1);
  dummy2.y /= (float)(fixedVertices.size() - 1);
  return dummy2;
}

bool GraphUtil::areVectorsEqual(std::vector<int> v1, std::vector<int> v2) {
  unsigned int i;
  // check if v1\in v2
  for (i = 0; i < v1.size(); i++)
    if (!GraphUtil::isElementInVector(v2, v1[i]))
      return false;
  // check if v2\in v1
  for (i = 0; i < v2.size(); i++)
    if (!GraphUtil::isElementInVector(v1, v2[i]))
      return false;
  return true;
}

bool GraphUtil::isVertexFixed(int v, std::map<int, struct Point> fixedVertices) {
  for (std::map<int, struct Point>::const_iterator it = fixedVertices.begin();
       it != fixedVertices.end(); it++) {
    if (it->first == v)
      return true;
  }
  return false;
}

std::vector<int>
GraphUtil::makeFixedVerticesVector(std::map<int, struct Point> fixedVertices) {
  std::vector<int> fixedVerticesVector;
  for (std::map<int, struct Point>::const_iterator it = fixedVertices.begin();
       it != fixedVertices.end(); it++)
    fixedVerticesVector.push_back(it->first);
  return fixedVerticesVector;
}

std::map<int, std::vector<int> >
GraphUtil::getNeighbourMap(Graph graph, std::map<int, struct Point> fixedVertices) {
  std::map<int, std::vector<int> > neighbourMap;
  std::vector<GraphNode> nodes = graph.getNodes();
  for (unsigned int i = 0; i < nodes.size(); i++) {
    int currentNodeNumber = nodes[i].getNodeNumber();
    // if it is not fixed add to map
    if (!GraphUtil::isVertexFixed(currentNodeNumber, fixedVertices)) {
      std::vector<int> neighbours = graph.getNeighbours(currentNodeNumber);
      neighbourMap[currentNodeNumber] = neighbours;
    }
  }
  return neighbourMap;
}

int GraphUtil::getMostSameNeighbours(std::map<int, std::vector<int> > neighbourMap) {
  int max = 1, current = 1;
  for (std::map<int, std::vector<int> >::const_iterator n1 = neighbourMap.begin();
       n1 != neighbourMap.end(); n1++) {
    for (std::map<int, std::vector<int> >::const_iterator n2 = neighbourMap.begin();
         n2 != neighbourMap.end(); n2++) {
      if (n1->first != n2->first) {
        if (GraphUtil::areVectorsEqual(n1->second, n2->second))
          current++;
      }
    }
    if (current > max)
      max = current;
    current = 1;
  }
  return max;
}

void GraphUtil::getBoundingBox(std::map<int, struct Point> fixedVertices,
                               struct Point &leftBottom,
                               struct Point &rightTop) {
  float minX, minY, maxX, maxY;
  std::map<int, struct Point>::const_iterator it = fixedVertices.begin();
  minX = maxX = it->second.x;
  minY = maxY = it->second.y;
  for (it = fixedVertices.begin(); it != fixedVertices.end(); it++) {
    if (it->second.x < minX)
      minX = it->second.x;
    if (it->second.y < minY)
      minY = it->second.y;
    if (it->second.x > maxX)
      maxX = it->second.x;
    if (it->second.y > maxY)
      maxY = it->second.y;
  }
  leftBottom.x = minX;
  leftBottom.y = minY;
  rightTop.x = maxX;
  rightTop.y = maxY;
}

std::map<int, struct Point>
GraphUtil::getDummyVerticesMap(std::map<int, struct Point> fixedVertices,
                               int mostSameNeighbours, Graph graph) {
  std::map<int, struct Point> dummyVerticesMap;
  struct Point lb, rt;
  // get bounding box
  GraphUtil::getBoundingBox(fixedVertices, lb, rt);
  float yCoordinate = (rt.y - lb.y) / 2;
  float xOffset = 0.0;
  int highestNodeNumber = graph.getHighestNodeNumber() + 1;
  // set dummy vertex in the center of bounding box, if there are only most same
  // neighbours
  if (mostSameNeighbours == 2) {
    xOffset = (rt.x - lb.x) / 2;
    dummyVerticesMap[highestNodeNumber].x = lb.x + xOffset;
    dummyVerticesMap[highestNodeNumber].y = yCoordinate;
    return dummyVerticesMap;
  }
  xOffset = (rt.x - lb.x) / (mostSameNeighbours - 2);
  // make makeSameNeighbours-1 dummy vertex
  for (int i = 0; i < mostSameNeighbours - 1; i++) {
    dummyVerticesMap[highestNodeNumber + i].x = lb.x + i * xOffset;
    dummyVerticesMap[highestNodeNumber + i].y = yCoordinate;
  }
  return dummyVerticesMap;
}

std::map<int, int> GraphUtil::getDummyMap(std::map<int, std::vector<int> > neighbourMap,
                                     Graph graph) {
  std::map<int, int> dummyMap;
  std::map<int, bool> markedMap;
  // create marked map and setup initial values for dummy map (0)
  for (std::map<int, std::vector<int> >::const_iterator it = neighbourMap.begin();
       it != neighbourMap.end(); it++) {
    dummyMap[it->first] = 0;
    markedMap[it->first] = false;
  }
  int counter = 0;
  int highestNodeNumber = graph.getHighestNodeNumber() + 1;
  for (std::map<int, std::vector<int> >::const_iterator it1 = neighbourMap.begin();
       it1 != neighbourMap.end(); it1++) {
    if (!markedMap[it1->first]) {
      markedMap[it1->first] = true;
      for (std::map<int, std::vector<int> >::const_iterator it2 = neighbourMap.begin();
           it2 != neighbourMap.end(); it2++) {
        if (!markedMap[it2->first] && it1->first != it2->first &&
            GraphUtil::areVectorsEqual(it1->second, it2->second)) {
          markedMap[it2->first] = true;
          dummyMap[it2->first] = highestNodeNumber + counter++;
        }
      }
      counter = 0;
    }
  }
  return dummyMap;
}

std::string GraphUtil::printbool(bool argument) {
  return argument ? "TRUE" : "FALSE";
}

std::vector<std::vector<bool>> GraphUtil::allocateMatrix(int size) {
  std::vector<std::vector<bool>> adjacencyMatrix(size);
  for (std::vector<bool> &i : adjacencyMatrix)
    i.resize(size, false);
  return adjacencyMatrix;
}

void GraphUtil::printAdjacencyMatrix(
    const std::vector<std::vector<bool>> &adjacencyMatrix, int size) {
  // print adjacency matrix
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++)
      std::cout << adjacencyMatrix[i][j] << " ";
    std::cout << std::endl;
  }
}

void GraphUtil::printCanonicalOrderStructure(struct CanonicalOrder costruct) {
  std::cout << "Canonical ordering:\n";
  GraphUtil::printVector(costruct.canonicalOrdering);
  std::cout << "outer cycles:\n";
  std::map<int, std::vector<int> >::const_iterator it;
  for (it = costruct.c0Gk.begin(); it != costruct.c0Gk.end(); it++) {
    std::cout << it->first << ":\t";
    GraphUtil::printVector(it->second);
  }
  std::cout << "vk neighbours:\n";
  for (it = costruct.vk_neighbours.begin(); it != costruct.vk_neighbours.end();
       it++) {
    std::cout << it->first << ":\t";
    GraphUtil::printVector(it->second);
  }
}

void GraphUtil::deleteCanonicalOrderStructure(struct CanonicalOrder &costruct) {
  costruct.canonicalOrdering.clear();
  costruct.c0Gk.clear();
  costruct.vk_neighbours.clear();
}

void GraphUtil::printVector(std::vector<int> vec) {
  for (unsigned int i = 0; i < vec.size(); i++)
    std::cout << vec[i] << " ";
  std::cout << std::endl;
}

void GraphUtil::printCoordinates(std::map<int, struct Point> straightLineMap) {
  for (std::map<int, struct Point>::const_iterator it = straightLineMap.begin();
       it != straightLineMap.end(); it++)
    std::cout << it->first << "\t(" << it->second.x << "," << it->second.y << ")\n";
}

void GraphUtil::printSetL(std::map<int, std::vector<int> > L) {
  for (std::map<int, std::vector<int> >::const_iterator it = L.begin(); it != L.end();
       it++) {
    std::cout << it->first << ": ";
    for (unsigned int i = 0; i < it->second.size(); i++)
      std::cout << it->second[i] << " ";
    std::cout << std::endl;
  }
}

void GraphUtil::Error(const std::string &message) {
  std::cout << std::endl << message << std::endl;
  exit(-1);
}

void GraphUtil::writeLayers(std::vector<std::vector<int> > layers) {
  std::cout << "number of layers: " << layers.size() << std::endl;
  for (unsigned int i = 0; i < layers.size(); i++) {
    std::cout << "layer " << i << ":\t";
    for (unsigned int j = 0; j < layers[i].size(); j++)
      std::cout << layers[i][j] << " ";
    std::cout << std::endl;
  }
}

void GraphUtil::writeSettings(Settings settings) {
  std::cout << "drawing type:\t\t\t" << settings.getDrawingType() << '\n'
       << "drawing direction:\t\t" << settings.getDrawingDirection() << '\n'
       << "drawing format:\t\t\t" << settings.getDrawingFormat() << '\n'
       << "scaling factor:\t\t\t" << settings.getScalingFactor() << '\n'
       << "offset factor:\t\t\t" << settings.getOffsetFactorX() << '\n'
       << "offset factor:\t\t\t" << settings.getOffsetFactorY() << '\n'
       << "accuracy:\t\t\t" << settings.getAccuracy() << '\n'
       << "maximum number of iterations:\t" << settings.getMaxIterations()
       << '\n';
}
