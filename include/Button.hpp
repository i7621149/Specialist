#ifndef BUTTON_HPP__
#define BUTTON_HPP__

#include <ngl/Vec4.h>
#include <QWidget>

class Button //: QWidget
{
//Q_OBJECT

public :
  Button();
  Button(ngl::Vec2 _pos, ngl::Vec2 _size, ngl::Vec4 _color, GLuint _textureID);
  ~Button();

  bool isInside(ngl::Vec2 _pos);

  ngl::Vec2 m_pos;
  ngl::Vec2 m_size;
  ngl::Vec4 m_color;
  GLuint m_textureID;

  float m_selectedTime;
  //virtual void isClicked(ngl::Vec2 _mousePos);
};

#endif
