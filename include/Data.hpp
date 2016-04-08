#ifndef DATA_HPP__
#define DATA_HPP__

#include <ngl/Util.h>
#include <ngl/Singleton.h>

class Data : public ngl::Singleton<Data>
{
friend class ngl::Singleton<Data>;

public:
  enum uiMode{DWELLING, SCANNING, TOUCHSCREEN};
  uiMode mode;
  ngl::Vec4 baseColor;
  ngl::Vec4 selectedColor;
  ngl::Vec4 clickedColor;
  int width;
  int height;
  float dwellTime;
  float circleSize;
  bool clickMovement;
  int borderSize;
  ngl::Vec4 borderColor;
private:
  Data();
};

#endif//PROGRAMDATA_HPP__
