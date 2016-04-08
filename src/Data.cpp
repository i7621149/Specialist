#include "Data.hpp"

Data::Data() :
  mode(SCANNING),
  baseColor(0.9, 0.5, 0.5, 1.0),
  selectedColor(1.0, 0.8, 0.8, 1.0),
  clickedColor(0.3, 1.0, 0.6, 1.0),
  width(1024),
  height(720),
  dwellTime(2.0),
  circleSize(80.0),
  clickMovement(true),
  borderSize(6),
  borderColor(0.3, 0.3, 0.3, 1.0)
{
}
