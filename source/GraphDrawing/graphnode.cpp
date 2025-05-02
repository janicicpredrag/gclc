#include "graphnode.h"

GraphNode::GraphNode(int _nodeNumber, const std::string &tmpChar) {
  std::string tmpString = tmpChar;
  nodeNumber = _nodeNumber;
  nodeLabel = tmpString;
}

int GraphNode::getNodeNumber() const { return nodeNumber; }

void GraphNode::setNodeNumber(int _nodeNumber) { nodeNumber = _nodeNumber; }

std::string GraphNode::getNodeLabel() const { return nodeLabel; }

void GraphNode::setNodeLabel(std::string _nodeLabel) { nodeLabel = _nodeLabel; }

void GraphNode::updateNodeLabel() {
  // set node label if it is not already set
  if (nodeLabel == "") {
    std::stringstream strings;
    strings << nodeNumber;
    nodeLabel = strings.str();
  }
}

bool GraphNode::operator==(const GraphNode &newGraphNode) const {
  return (nodeNumber == newGraphNode.nodeNumber);
}

bool GraphNode::operator!=(const GraphNode &newGraphNode) const {
  return !(*this == newGraphNode);
}

bool GraphNode::operator<(const GraphNode &newGraphNode) const {
  return (nodeNumber < newGraphNode.nodeNumber);
}
