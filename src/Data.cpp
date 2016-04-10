#include "Data.hpp"

Data::Data() :
  mode(TOUCHLARGE),
  shaderMode(CIRCLE),
  baseColor(0.5, 0.5, 0.9, 1.0),
  selectedColor(0.7, 0.7, 1.0, 1.0),
  clickedColor(0.6, 1.0, 0.6, 1.0),
  width(1024),
  height(720),
  dwellTime(2.0),
  circleSize(40.0),
  clickMovement(true),
  borderSize(5),
  loadingBarSize(height),
  borderColor(0.3, 0.3, 0.3, 1.0),
  loadingBarColor(selectedColor - ngl::Vec4(0.1, 0.1, 0.0, 0.0)),
  rotateAngle(15)
{

}
