#ifndef __settings_h
#define __settings_h

enum DrawingType{
  BASE,
  STRAIGHT_LINE,
  ARC_LAYERED,
  BARYCENTER
  };

enum DrawingDirection{
  DEFAULT,
  UP_DOWN,
  DOWN_UP,
  LEFT_RIGHT,
  RIGHT_LEFT
};

enum DrawingFormat{
  GCLC,
  TEX
};

//! Base class for settings of graph drawings
class Settings
{
 private:
  DrawingType drawingType;           // drawing type
  DrawingDirection drawingDirection; // drawing direction
  DrawingFormat drawingFormat;       // drawing format
  float scaling_factor;              // scaling factor
  float offset_factorX;              // x offset factor
  float offset_factorY;              // y offset factor
  double eps;                        // accuracy
  int max_iterations;                // maximum number of iterations
 public:
  //! A constructor
  /*! \param DrawingType drawing type indicator
   * \param DrawingDirection drawing direction indicator
   * \param DrawingFormat drawing format
   * \param float scaling factor
   * \param float x offset factor
   * \param float y offset factor
   * \param double accuracy
   * \param int maximum number of iterations
   */
  Settings(DrawingType=BASE, DrawingDirection=DEFAULT, DrawingFormat=GCLC, float=1, float=0, float=0, double=0.01, int=15);
  //! Gets drawing type
  /*!\return DrawingType drawing type
   */
  DrawingType getDrawingType() const;
  //! Sets drawing type
  /*! \param DrawingType drawing type
   */
  void setDrawingType(DrawingType);
  //! Gets drawing direction
  /*! \return DrawingDirection drawing direction
   */
  DrawingDirection getDrawingDirection() const;
  //! Sets drawing direction
  /*! \param DrawingDirection drawing direction
   */
  void setDrawingDirection(DrawingDirection);
  //! Gets drawing format
  /*! \return DrawingFormat drawing direction
   */
  DrawingFormat getDrawingFormat() const;
  //! Sets drawing format
  /*! \param DrawingFormat drawing format
   */
  void setDrawingFormat(DrawingFormat);
  //! Gets scaling factor
  /*! \return float scaling factor
   */
  float getScalingFactor() const;
  //! Sets scaling factor
  /*! \param float scaling factor
   */
  void setScalingFactor(float);
  //! Gets x offset factor
  /*! \return float offset factor
   */
  float getOffsetFactorX() const;
  //! Gets y offset factor
  /*! \return float offset factor
   */
  float getOffsetFactorY() const;
  //! Sets x offset factor
  /*! \param float offset factor
   */
  void setOffsetFactorX(float);
  //! Sets y offset factor
  /*! \param float offset factor
   */
  void setOffsetFactorY(float);
  //! Gets accuracy
  /*! \return double accuracy
   */  
  double getAccuracy() const;
  //! Sets accuracy
  /*! \param double accuracy
   */
  void setAccuracy(double);
  //! Gets maximum number of iterations
  /*! \return int maximum number of iterations
   */
  int getMaxIterations() const;
  //! Sets maximum number of iterations
  /*! \param int maximum number of iterations
   */
  void setMaxIterations(int);
};

#endif
