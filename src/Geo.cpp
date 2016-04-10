#include "Geo.hpp"

Geo::Geo(std::string _objFile)
{
  m_mesh = ngl::Obj(_objFile);
  m_rotation = ngl::Vec3(0,0,0);
}

Geo::~Geo()
{

}
