#ifndef GEO_HPP__
#define GEO_HPP__

#include <string>
#include <ngl/Obj.h>
#include <ngl/Vec4.h>
#include <ngl/Transformation.h>

class Geo
{
public :
  Geo(std::string _objFile, float _height);
  ~Geo();

  ngl::Vec3 getTopTranslation();

  ngl::Obj m_mesh;
  float m_height;
  ngl::Vec3 m_rotation;
};

#endif//GEO_HPP__
