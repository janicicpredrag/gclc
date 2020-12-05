#ifdef WIN32
// disable warnings about long names
//  #pragma warning( disable : 4786)
#endif

#include "settings.h"

Settings::Settings(DrawingType _drawingType, DrawingDirection _drawingDirection, DrawingFormat _drawingFormat, float _scaling_factor, 
		   float _offset_factorX, float _offset_factorY, double _eps, int _max_iterations)
{
  drawingType=_drawingType;
  drawingDirection=_drawingDirection;
  drawingFormat=_drawingFormat;
  scaling_factor=_scaling_factor;
  offset_factorX=_offset_factorX;
  offset_factorY=_offset_factorY;
  eps=_eps;
  max_iterations=_max_iterations;
}

DrawingType Settings::getDrawingType() const
{
  return drawingType;
}

void Settings::setDrawingType(DrawingType _drawingType)
{
  drawingType=_drawingType;
}

DrawingDirection Settings::getDrawingDirection() const
{
  return drawingDirection;
}

void Settings::setDrawingDirection(DrawingDirection _drawingDirection)
{
  drawingDirection=_drawingDirection;
}

DrawingFormat Settings::getDrawingFormat() const
{
  return drawingFormat;
}

void Settings::setDrawingFormat(DrawingFormat _drawingFormat)
{
  drawingFormat=_drawingFormat;
}

float Settings::getScalingFactor() const
{
  return scaling_factor;
}

void Settings::setScalingFactor(float _scaling_factor)
{
  scaling_factor=_scaling_factor;
}

float Settings::getOffsetFactorX() const
{
  return offset_factorX;
}

float Settings::getOffsetFactorY() const
{
  return offset_factorY;
}

void Settings::setOffsetFactorX(float _offset_factorX)
{
  offset_factorX=_offset_factorX;
}

void Settings::setOffsetFactorY(float _offset_factorY)
{
  offset_factorY=_offset_factorY;
}

double Settings::getAccuracy() const
{
  return eps;
}

void Settings::setAccuracy(double _eps)
{
  eps=_eps;
}

int Settings::getMaxIterations() const
{
  return max_iterations;
}

void Settings::setMaxIterations(int _max_iterations)
{
  max_iterations=_max_iterations;
}
