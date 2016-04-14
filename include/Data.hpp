#ifndef DATA_HPP__
#define DATA_HPP__

#include <ngl/Util.h>
#include <ngl/Singleton.h>

class Data : public ngl::Singleton<Data>
{
friend class ngl::Singleton<Data>;

public:
  enum UiMode{DWELLING, SCANNING, TOUCHLARGE, TOUCHSMALL_L, TOUCHSMALL_R};
  enum UiShaderMode{CIRCLE, BAR, NONE};
  UiMode mode;
  UiShaderMode shaderMode;
  ngl::Vec4 backgroundColor;
  ngl::Vec4 geoColor;
  ngl::Vec4 ghostColor;
  ngl::Vec4 baseColor;
  ngl::Vec4 selectedColor;
  ngl::Vec4 clickedColor;
  int width;
  int height;
  float dwellTime;
  float circleSize;
  bool clickMovement;
  int borderSize;
  int loadingBarSize;
  ngl::Vec4 borderColor;
  ngl::Vec4 loadingBarColor;
  float rotateAngle;
  int animLength;
  float fps;
  int ghosting;
  bool perspGhosting;
  bool simpleGhosting;

  int progressBarSize;
  ngl::Vec4 progressBarColor;
  ngl::Vec4 progressBarSelection;
  ngl::Vec4 progressBarBack;

  float hueChangeAmount;

  bool colorSet;

  void updateColor(int _hueChange);
  ngl::Vec4 RGBtoHSV(float r, float g, float b);
  ngl::Vec4 HSVtoRGB(float h, float s, float v);
  float MIN(float a, float b, float c);
  float MAX(float a, float b, float c);


private:
  Data();
};

#endif//PROGRAMDATA_HPP__
