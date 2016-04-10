#include "Geo.hpp"

#include <iostream>

Geo::Geo(std::string _objFile, float _height)
{
  m_mesh = ngl::Obj(_objFile);
  m_height = _height;
  m_rotation = ngl::Vec3(0,0,0);
}

Geo::~Geo()
{

}

/*ngl::Transformation Geo::getTransformation()
{
  ngl::Transformation topTransformation;
  topTransformation.reset();

  topTransformation.setRotation(m_rotation);

  //ngl::Vec4 point = topTransformation.getMatrix() * ngl::Vec4(0, m_height, 0, 1);
  //topTransformation.addPosition(0,m_height,0);

  return topTransformation;
}*/

ngl::Vec3 Geo::getTopTranslation()
{
  return ngl::Vec3(0,1,0);
}
