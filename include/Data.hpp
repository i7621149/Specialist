#ifndef DATA_HPP__
#define DATA_HPP__

#include <ngl/Util.h>
#include <ngl/Singleton.h>

class Data : public ngl::Singleton<Data>
{
friend class ngl::Singleton<Data>;

public:
  ngl::Vec4 baseColor;
  ngl::Vec4 selectedColor;
  int width;
  int height;
  float dwellTime;
  float circleSize;
private:
  Data();
};

#endif//PROGRAMDATA_HPP__
