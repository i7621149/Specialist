#include "Button.hpp"
#include <iostream>

Button::Button() :
  m_pos(0.2,0.2),
  m_size(0.6, 0.6),
  m_color(1,0,0,0)
{

}

Button::Button(ngl::Vec2 _pos, ngl::Vec2 _size, ngl::Vec4 _color, GLuint _textureID) :
  m_pos(_pos),
  m_size(_size),
  m_color(_color),
  m_textureID(_textureID),
  m_selectedTime(0)
{

}

Button::~Button()
{

}

bool Button::isInside(ngl::Vec2 _pos)
{
  if(_pos[0] >= m_pos[0] && _pos[1] >= m_pos[1] && _pos[0] < m_pos[0] + m_size[0] && _pos[1] < m_pos[1] + m_size[1]){
    return true;
  }
  else{
    return false;
  }
}
