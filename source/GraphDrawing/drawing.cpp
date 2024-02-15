#include "drawing.h"

Drawing::Drawing(std::string _file_name, Graph _graph, Settings _settings) {
  file_name = _file_name;
  graph = _graph;
  settings = _settings;
  if (graph.isUnDirected() && graph.isConnected())
    isValid = true;
  else
    isValid = false;
}

bool Drawing::openFile(std::ofstream &graphFile) {
  // initialize output file
  graphFile.open(file_name.c_str());
  return graphFile.is_open();
}

void Drawing::defineNodesGCLC(std::ofstream &graphFile, Graph labeledGraph) {
  graphFile << "% nodes definition\n";
  for (std::map<int, struct Point>::iterator it = coordinates.begin();
       it != coordinates.end(); it++) {
    GraphNode gn = labeledGraph.getGraphNode(it->first);
    graphFile << "point " << gn.getNodeLabel() << " " << it->second.x << " "
              << it->second.y << std::endl;
    graphFile << "cmark_lt " << gn.getNodeLabel() << std::endl; 
  }
  graphFile << std::endl;
}

void Drawing::drawLineGCLC(std::ofstream &graphFile, int sourceNode,
                           int destinationNode) {
  graphFile << "drawsegment " << sourceNode << " " << destinationNode << std::endl;
}

void Drawing::drawArcGCLC(std::ofstream &graphFile, int sourceNode,
                          int destinationNode) {
  // define new temporary node
  std::string tempNode;
  std::stringstream tempStream;
  tempStream << "temp_" << sourceNode << "_" << destinationNode;
  tempNode = tempStream.str();
  // calculate coordinates for new temp node
  float tempX =
      (coordinates[sourceNode].x - coordinates[destinationNode].x) / 2 +
      coordinates[destinationNode].x;
  float tempY = coordinates[sourceNode].y;
  graphFile << "point " << tempNode << " " << tempX << " " << tempY
            << " % temporary nodes\n";
  if (settings.getDrawingDirection() == DOWN_UP)
    graphFile << "drawarc " << tempNode << " " << destinationNode << " 180\n";
  else if (settings.getDrawingDirection() == UP_DOWN)
    graphFile << "drawarc " << tempNode << " " << destinationNode << " -180\n";
}

void Drawing::writeDrawingToFileGCLC(std::ofstream &graphFile) {
  // create labeled graph for file writing purposes
  Graph labeledGraph(graph);
  labeledGraph.updateNodeLabels();
  defineNodesGCLC(graphFile, labeledGraph);
  makeConnectionsGCLC(graphFile, labeledGraph);
}

void Drawing::scaleAndOffsetCoordinates() {
  for (std::map<int, struct Point>::iterator it = coordinates.begin();
       it != coordinates.end(); it++) {
    it->second.x = it->second.x * settings.getScalingFactor() +
                   settings.getOffsetFactorX();
    it->second.y = it->second.y * settings.getScalingFactor() +
                   settings.getOffsetFactorY();
  }
}

std::string Drawing::getFileName() const { return file_name; }

Graph Drawing::getGraph() { return graph; }

void Drawing::setFileName(std::string _filename) { file_name = _filename; }

void Drawing::setZeroCoordinates() {
  // get all graph nodes
  std::vector<GraphNode> allNodes = graph.getNodes();
  // fill coordinates map and set all values to initial value - zero
  for (unsigned int i = 0; i < allNodes.size(); i++) {
    coordinates[allNodes[i].getNodeNumber()].x = 0;
    coordinates[allNodes[i].getNodeNumber()].y = 0;
  }
}

bool Drawing::draw() {
  // if drawing can't be done
  if (!isValid || file_name == "")
    return false;
  // initialize output file
  std::ofstream graphFile;
  if (!openFile(graphFile))
    return false;

  // write drawing to the file
  writeDrawingToFileGCLC(graphFile);

  // close output file
  graphFile.close();
  return true;
}

std::map<int, struct Point> Drawing::getCoordinates() { return coordinates; }
