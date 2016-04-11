#ifndef GEO_HPP__
#define GEO_HPP__

#include <string>
#include <ngl/Obj.h>
#include <ngl/Vec4.h>
#include <ngl/Transformation.h>

class Geo
{
public :
  Geo(std::string _objFile, float _height, float _maxAngle);
  ~Geo();

  ngl::Vec3 getTopTranslation();

  ngl::Obj m_mesh;
  float m_height;
  ngl::Vec3 m_localRotation;
  ngl::Vec3 m_rotation;
  float m_maxAngle;
};

#endif//GEO_HPP__
