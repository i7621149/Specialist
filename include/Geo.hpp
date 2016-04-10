#ifndef GEO_HPP__
#define GEO_HPP__

#include <string>
#include <ngl/Obj.h>

class Geo
{
public :
  Geo(std::string _objFile);
  ~Geo();

  ngl::Obj m_mesh;
  ngl::Vec3 m_rotation;
};

#endif//GEO_HPP__
