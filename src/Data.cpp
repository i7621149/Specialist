#include "Data.hpp"
#include <iostream>

Data::Data() :
  mode(DWELLING),
  shaderMode(CIRCLE),
  backgroundColor(0.6, 0.8, 0.9, 1.0),
  geoColor(0.9, 0.5, 0.7, 1.0),
  ghostColor(geoColor + ngl::Vec4(0.1, 0.1, 0.1, 0.0)),
  baseColor(0.5, 0.5, 0.9, 1.0),
  selectedColor(0.7, 0.7, 1.0, 1.0),
  clickedColor(0.6, 1.0, 0.6, 1.0),
  width(1024),
  height(720),
  dwellTime(1.0),
  circleSize(40.0),
  clickMovement(false),
  borderSize(5),
  loadingBarSize(15),
  borderColor(0.3, 0.3, 0.3, 1.0),
  loadingBarColor(selectedColor - ngl::Vec4(0.1, 0.1, 0.0, 0.0)),
  rotateAngle(30),
  animLength(12),
  fps(animLength),
  ghosting(animLength),
  perspGhosting(true),
  simpleGhosting(false),
  progressBarSize(loadingBarSize),
  progressBarColor(baseColor),
  progressBarSelection(clickedColor),
  progressBarBack(loadingBarColor),
  hueChangeAmount(36),
  colorSet(false)
{
  if(mode == TOUCHSMALL){
    shaderMode = NONE;
  }

  ghostColor.m_w = 0;
}

void Data::updateColor(int _hueChange)
{
  ngl::Vec4 newColHSV = RGBtoHSV(geoColor[0], geoColor[1], geoColor[2]);

  std::cout << "hsvbefore: " << newColHSV[0] << ", " << newColHSV[1] << ", " << newColHSV[2] << std::endl;

  newColHSV[0] += _hueChange * hueChangeAmount;
  if(newColHSV[0] > 360.0){
    newColHSV[0] -= 360.0;
  }
  if(newColHSV[0] < 0){
    newColHSV[0] += 360;
  }
  std::cout << "hueafter: " << newColHSV[0] << ", " << newColHSV[1] << ", " << newColHSV[2] << std::endl;
  geoColor = HSVtoRGB(newColHSV[0], newColHSV[1], newColHSV[2]);
  std::cout << "rgbafter: " << geoColor[0] << ", " << geoColor[1] << ", " << geoColor[2] << std::endl;
  ghostColor = geoColor + ngl::Vec4(0.1, 0.1, 0.1, 0.0);
  ghostColor.m_w = 0;
}

ngl::Vec4 Data::RGBtoHSV( float r, float g, float b )
{
  float min, max, delta;
  float h, s, v;

  min = MIN( r, g, b );
  max = MAX( r, g, b );
  v = max;				// v

  delta = max - min;

  if( max != 0 )
    s = delta / max;		// s
  else {
    // r = g = b = 0		// s = 0, v is undefined
    s = 0;
    h = 0;
    return ngl::Vec4(h, s, v, 1.0);
  }

  if( r == max )
    h = ( g - b ) / delta;		// between yellow & magenta
  else if( g == max )
    h = 2 + ( b - r ) / delta;	// between cyan & yellow
  else
    h = 4 + ( r - g ) / delta;	// between magenta & cyan

  h *= 60;				// degrees
  if( h < 0 )
    h += 360;
  return ngl::Vec4(h, s, v, 1.0);
}

ngl::Vec4 Data::HSVtoRGB( float h, float s, float v)
{
  int i;
  float f, p, q, t;
  float r, g, b;

  if( s == 0 ) {
    // achromatic (grey)
    r = g = b = v;
    return ngl::Vec4(r, g, b, 1.0);
  }

  h /= 60;			// sector 0 to 5
  i = floor( h );
  f = h - i;			// factorial part of h
  p = v * ( 1 - s );
  q = v * ( 1 - s * f );
  t = v * ( 1 - s * ( 1 - f ) );

  switch( i ) {
    case 0:
      r = v;
      g = t;
      b = p;
      break;
    case 1:
      r = q;
      g = v;
      b = p;
      break;
    case 2:
      r = p;
      g = v;
      b = t;
      break;
    case 3:
      r = p;
      g = q;
      b = v;
      break;
    case 4:
      r = t;
      g = p;
      b = v;
      break;
    default:		// case 5:
      r = v;
      g = p;
      b = q;
      break;
  }
  return ngl::Vec4(r, g, b, 1.0);

}

float Data::MIN(float a, float b, float c)
{
  if(a < b && a < c) return a;
  if(b < c) return b;
  return c;
}

float Data::MAX(float a, float b, float c)
{
  if(a > b && a > c) return a;
  if(b > c) return b;
  return c;
}
