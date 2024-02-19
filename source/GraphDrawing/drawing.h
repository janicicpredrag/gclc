#ifndef __drawing_h
#define __drawing_h
#include "graph.h"
#include "graph_util.h"
#include "settings.h"

//! Abstract drawing class that's used for graph drawing
class Drawing
{
 protected:
  std::string file_name;                    // file name
  Graph graph;                         // graph 
  std::map<int, struct Point> coordinates;  // coordinates of the nodes in the final drawing
  Settings settings;                   // drawing settings
  bool isValid;                        // indicating if graph can be drawn
  //! Opens a file
  /*! \param ofstream& file stream (output argument)
   * \return boolean value indicating if file was sucessfully opened
   */
  bool openFile(std::ofstream&);
  //! Defines nodes in GCLC
  /*! After this method is applied, beside defining nodes in GCLC, coordinates
   * are scaled and offseted.
   * \param ofstream& reference to file
   * \param Graph labeled graph
   */
  void defineNodesGCLC(std::ofstream&, Graph);
  //! Draws line in GCLC
  /*!\param ofstream& reference to file 
   * \param int source graph node number
   * \param int destination graph node number
   */
  void drawLineGCLC(std::ofstream&, int, int);
  //! Draws arc in GCLC
  /*!\param ofstream& reference to file 
   * \param int source graph node number
   * \param int destination graph node number
   */
  void drawArcGCLC(std::ofstream&, int, int);
  //! Writes obtained drawing to the file
  /*! \param ofstream& reference to the file
   */
  void writeDrawingToFileGCLC(std::ofstream&);
  //! Connects nodes in GCLC
  /*!\param ofstream& reference to file
   * \param Graph labeled graph
   */
  virtual void makeConnectionsGCLC(std::ofstream&, Graph)=0;
  //! Scales and offsets coordinates
  void scaleAndOffsetCoordinates();
  //! Computes coordinates
  virtual void computeCoordinates()=0;
 public:
  //! A constructor
  /*!
   * \param string filename
   * \param Graph graph that should be drawn
   * \param Settings drawing settings
   * drawing should be largest possible to fit the canvas)
   */
  Drawing(std::string, Graph, Settings);
  //! Virtual destructor
  virtual ~Drawing(){}
  //! Gets file name
  /*! \return filename */
  std::string getFileName() const;
  //! Gets the graph that should be drawn
  /*!\return graph */  
  Graph getGraph();
  //! Gets scaling factor
  /*! \return scaling factor */
  float getScalingFactor() const;
  //! Sets filename
  /*! \param string new filename */
  void setFileName(std::string _filename);
  //! Sets scaling factor
  /*! \param float new scaling factor */
  void setScalingFactor(float _scalingFactor);
  //! Sets all coordinates to zero
  void setZeroCoordinates();
  //! Returns boolean value indicating if this edge should be drawn as arc.
  /*! This function is used only in ArcLayeredDrawing and BaricentricDrawing.
   * \param int first nodes node number
   * \param int second nodes node number
   * \return boolean value indicating if the edge should be drawn as an arc
   */
  virtual bool isArc(int, int){return false;}
  //! Function that does the drawing
  /*! In Drawing class this function is defined as pure virual function, but
   * in every inherited class for different types of drawing, this function
   * is used for the specific drawing of the graph.
   */
  bool draw();
  std::map<int, struct Point> getCoordinates();
};

#endif
