#include "barycenterdrawing.h"
#include <cmath>

BarycenterDrawing::BarycenterDrawing(Graph _graph, Settings _settings, std::map<int, struct Point> _fixedVertices, std::string _filename):
  Drawing(_filename, _graph, _settings)
{
  fixedVertices=_fixedVertices;
  if (fixedVertices.size()<3)
    GraphUtil::Error("Number of fixed vertices must be greater or equal to 3");

  // store original data
  Graph originalGraph=graph;
  std::map<int, struct Point> originalFixedVertices=fixedVertices;
  
  // add dummy vertices (if they are needed)
  addDummyVertices();
  if (isValid)
    computeCoordinates();

  // restore original data
  if (graph!=originalGraph)
    {
      graph=originalGraph;
      fixedVertices=originalFixedVertices;
      removeDummyCoordinates();
    }
}

void BarycenterDrawing::setFixedCoordinates(std::map<int, bool>& needsXUpdating, std::map<int, bool>& needsYUpdating)
{
  for (std::map<int, struct Point>::const_iterator it=fixedVertices.begin(); it!=fixedVertices.end(); it++)
    {
      coordinates[it->first].x=it->second.x;
      coordinates[it->first].y=it->second.y;
      needsXUpdating[it->first]=false;
      needsYUpdating[it->first]=false;
    }
}

void BarycenterDrawing::resetXYUpdating(std::map<int, bool>& needsXUpdating, std::map<int, bool>& needsYUpdating)
{
  for (std::map<int, struct Point>::const_iterator it=coordinates.begin(); it!=coordinates.end(); it++)
    {
      needsXUpdating[it->first]=true;
      needsYUpdating[it->first]=true;
    }
}

bool BarycenterDrawing::continueWorking(std::map<int, bool> needsXUpdating, std::map<int, bool> needsYUpdating, int currentIteration)
{
  if (currentIteration>=settings.getMaxIterations())
    return false;
  std::map<int, bool>::const_iterator it;
  for (it=needsXUpdating.begin(); it!=needsXUpdating.end(); it++)
    if (it->second)
      return true;
  for (it=needsYUpdating.begin(); it!=needsYUpdating.end(); it++)
    if (it->second)
      return true;
  return false;
}

int BarycenterDrawing::convergeToSolution(std::map<int, bool>& needsXUpdating, std::map<int, bool>& needsYUpdating)
{
  int currentIteration=0;
  do
    {
      for (std::map<int, struct Point>::iterator it=coordinates.begin(); it!=coordinates.end(); it++)
	{
	  int nodesDegree=graph.getNodesDegree(it->first);
	  std::vector<int> neighbours=graph.getNeighbours(it->first);
	  if (needsXUpdating[it->first]) // do calculations for new x coordinate
	    {
	      float newX=0.0;
	      for (unsigned int i=0; i<neighbours.size(); i++)
		newX+=coordinates[neighbours[i]].x;
	      newX/=nodesDegree;
	      if (fabsf(newX-it->second.x)<settings.getAccuracy())
		needsXUpdating[it->first]=false;
	      it->second.x=newX;
	    }
	  if (needsYUpdating[it->first]) // do calculations for new y coordinate
	    {
	      float newY=0.0;
	      for (unsigned int i=0; i<neighbours.size(); i++)
		newY+=coordinates[neighbours[i]].y;
	      newY/=nodesDegree;
	      if (fabsf(newY-it->second.y)<settings.getAccuracy())
		needsYUpdating[it->first]=false;
	      it->second.y=newY;
	    }
	}
      currentIteration++;    
    }
  while(continueWorking(needsXUpdating, needsYUpdating, currentIteration));
  return currentIteration;
}

void BarycenterDrawing::makeConnectionsGCLC(std::ofstream& graphFile, Graph labeledGraph)
{
  std::vector<GraphNode> allNodes=labeledGraph.getNodes();
  for (unsigned int i=0; i<allNodes.size()-1; i++)    
    for (unsigned int j=i+1; j<allNodes.size(); j++)	
      if (labeledGraph.containsEdge(allNodes[i].getNodeNumber(), allNodes[j].getNodeNumber()))
	drawLineGCLC(graphFile, allNodes[i].getNodeNumber(), allNodes[j].getNodeNumber());	    
}

bool BarycenterDrawing::isArc(int u, int v)
{
  if (!graph.containsEdge(u,v))
    return false;
  std::vector<GraphNode> nodes=graph.getNodes();  
  // test if u and v are collinear with any node
  for (unsigned int i=0; i<nodes.size(); i++)
    {
      int nodeNumber=nodes[i].getNodeNumber();
      if (nodeNumber!=u && nodeNumber!=v)
	{
	  // is node collinear with u and v and between them
	  if ((GraphUtil::areCollinear(coordinates[u].x, coordinates[u].y, 
				       coordinates[v].x, coordinates[v].y,
				       coordinates[nodeNumber].x, coordinates[nodeNumber].y, 
				       (float)settings.getAccuracy()))
	      &&
	      (
	       //(coordinates[u].x<coordinates[nodeNumber].x || coordinates[v].x<coordinates[nodeNumber].x)
	       //&&
	       //(coordinates[nodeNumber].x<coordinates[u].x || coordinates[nodeNumber].x<coordinates[v].x)
	       (coordinates[u].x<coordinates[nodeNumber].x && coordinates[nodeNumber].x<coordinates[v].x)
	       ||
	       (coordinates[u].y<coordinates[nodeNumber].y && coordinates[nodeNumber].y<coordinates[v].y)
		   ||
   	       (coordinates[v].x<coordinates[nodeNumber].x && coordinates[nodeNumber].x<coordinates[u].x)
	       ||
	       (coordinates[v].y<coordinates[nodeNumber].y && coordinates[nodeNumber].y<coordinates[u].y)
		   )
		 )
	    return true;
	}
    }
  return false;
}

void BarycenterDrawing::computeCoordinates()
{
  // calculate coordinates
  setZeroCoordinates();                               // set all coordinates initially to 0
  std::map<int, bool> needsXUpdating, needsYUpdating;      // indicating if further calculations are needed
  resetXYUpdating(needsXUpdating, needsYUpdating);    // initially everybody needs updating
  setFixedCoordinates(needsXUpdating, needsYUpdating);// set fixed coordinates
  convergeToSolution(needsXUpdating, needsYUpdating); // start converging to solution 
  scaleAndOffsetCoordinates();                        // scale and offset coordinates 
}

void BarycenterDrawing::addDummyVertices()
{
  // fixes bug with 1 degree vertices
  fix1DegreeVertices();
  // fixes bugs with vertices that have same neighbours
  fixSameNeighbourVertices();
}

void BarycenterDrawing::fix1DegreeVertices()
{
  int dummy1NodeNumber=graph.getHighestNodeNumber()+1;
  int dummy2NodeNumber=dummy1NodeNumber+1;
  struct Point dummy1=GraphUtil::getDummy1(fixedVertices);
  struct Point dummy2=GraphUtil::getDummy2(fixedVertices);
  std::vector<int> fixedVerticesVector=GraphUtil::makeFixedVerticesVector(fixedVertices);
  bool dummy1Added=false, dummy2Added=false;
  std::vector<GraphNode> nodes=graph.getNodes();
  // for every node
  for (unsigned int i=0; i<nodes.size(); i++)
    {
      int v=nodes[i].getNodeNumber(); // current vertex
      std::vector<int> neighbours=graph.getNeighbours(v);
      if (graph.getNodesDegree(v)==1 && !GraphUtil::isVertexFixed(v, fixedVertices)) // this is one degree node which is not fixed
	{
	  // get neighbours of neighbour
	  std::vector<int> neighbourNeighbours=graph.getNeighbours(neighbours[0]);
	  std::vector<int> compareValue=fixedVerticesVector;
	  compareValue.push_back(v);
	  if (GraphUtil::areVectorsEqual(compareValue, neighbourNeighbours)) // add dummy2
	    {
	      // add dummy2 - all neighbours are fixed vertices plus vertex v, which means
	      // that dummy2 has to be added
	      if (!dummy2Added)
		{
		  dummy2Added=true;
		  graph.addNode(dummy2NodeNumber);
		  fixedVertices[dummy2NodeNumber]=dummy2;
		}
	      graph.addEdge(v, dummy2NodeNumber);
	      graph.addEdge(dummy2NodeNumber, v);
	    }
	  else
	    {
	      // add dummy1 - default dummy vertex
	      if (!dummy1Added)
		{
		  dummy1Added=true;
		  graph.addNode(dummy1NodeNumber);
		  fixedVertices[dummy1NodeNumber]=dummy1;
		}
	      graph.addEdge(v, dummy1NodeNumber);
	      graph.addEdge(dummy1NodeNumber, v);
	    }
	}
    } 
}

void BarycenterDrawing::fixSameNeighbourVertices()
{
  std::map<int, std::vector<int> > neighbourMap=GraphUtil::getNeighbourMap(graph, fixedVertices);
  int mostSameNeighbours=GraphUtil::getMostSameNeighbours(neighbourMap);
  // there are no same neighbour vertices, exit function
  if (mostSameNeighbours==1)
    return;
  std::map<int, struct Point> dummyVerticesMap=GraphUtil::getDummyVerticesMap(fixedVertices, mostSameNeighbours, graph);
  std::map<int, int> dummyMap=GraphUtil::getDummyMap(neighbourMap, graph);
  addDummyForSameNeighbour(dummyMap, dummyVerticesMap);
}

void BarycenterDrawing::addDummyForSameNeighbour(std::map<int, int> dummyMap, std::map<int, struct Point> dummyVerticesMap)
{
  for (std::map<int, int>::const_iterator it=dummyMap.begin(); it!=dummyMap.end(); it++)
    {
      if (it->second!=0)
	{
	  if (!graph.containsNode(it->second))
	    {
	      graph.addNode(it->second); 
	      fixedVertices[it->second]=dummyVerticesMap[it->second];
	    }
	  graph.addEdge(it->first, it->second);
	  graph.addEdge(it->second, it->first);
	}
    }
}

void BarycenterDrawing::removeDummyCoordinates()
{
  std::vector<int> extraVertices;
  for (std::map<int, struct Point>::const_iterator it=coordinates.begin(); it!=coordinates.end(); it++)    
    if (!graph.containsNode(it->first))
      extraVertices.push_back(it->first);
  for (unsigned int i=0; i<extraVertices.size(); i++)
    coordinates.erase(extraVertices[i]);
}
