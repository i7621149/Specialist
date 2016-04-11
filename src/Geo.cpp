#include "Geo.hpp"

#include <iostream>

Geo::Geo(std::string _objFile, float _height, float _maxAngle) :
  m_mesh(_objFile)
{
  m_height = _height;
  m_localRotation = ngl::Vec3(0,0,0);
  m_rotation = ngl::Vec3(0,0,0);
  m_maxAngle = _maxAngle;
}

Geo::~Geo()
{

}

ngl::Vec3 Geo::getTopTranslation()
{
  ngl::Vec4 point(0,m_height,0,1);
  ngl::Mat4 rotation;
  rotation.rotateX(m_rotation.m_x);

  point = point * rotation;

  rotation.identity();
  rotation.rotateY(m_rotation.m_y);

  point = point * rotation;

  rotation.identity();
  rotation.rotateZ(m_rotation.m_z);

  point = point * rotation;

  return point.toVec3();
}
