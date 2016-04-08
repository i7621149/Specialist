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
  void click(float _time);

  ngl::Vec2 m_pos;
  ngl::Vec2 m_size;
  ngl::Vec4 m_color;
  GLuint m_textureID;

  bool m_isSelected;
  float m_firstSelected;
  float m_selectedTime;

  float m_clicked;
  //virtual void isClicked(ngl::Vec2 _mousePos);
};

#endif
