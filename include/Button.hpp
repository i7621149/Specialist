#ifndef BUTTON_HPP__
#define BUTTON_HPP__

#include <ngl/Vec4.h>
#include <QWidget>

class Button //: QWidget
{
//Q_OBJECT

public :
  enum Action {NONE, SPIN_CW, SPIN_CCW, ROTATE_1_R, ROTATE_1_L, ROTATE_2_R, ROTATE_2_L, ROTATE_3_R, ROTATE_3_L, KEY, FINISH};

  Button();
  Button(ngl::Vec2 _pos, ngl::Vec2 _size, ngl::Vec4 _color, Action _action, GLuint _textureID);
  ~Button();

  bool isInside(ngl::Vec2 _pos);

  Action click(float _time);
  Action m_action;

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

#endif//BUTTON_HPP__
