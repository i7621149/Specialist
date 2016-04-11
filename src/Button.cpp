#include "Button.hpp"
#include "Data.hpp"

Button::Button() :
  m_pos(0.2,0.2),
  m_size(0.6, 0.6),
  m_color(1,0,0,0)
{

}

Button::Button(ngl::Vec2 _pos, ngl::Vec2 _size, ngl::Vec4 _color, Action _action, GLuint _textureID) :
  m_action(_action),
  m_pos(_pos),
  m_size(_size),
  m_color(_color),
  m_textureID(_textureID),
  m_isSelected(false),
  m_firstSelected(0),
  m_selectedTime(0),
  m_clicked(0)
{

}

Button::~Button()
{

}

bool Button::isInside(ngl::Vec2 _pos)
{
  if(_pos[0] >= m_pos[0] && _pos[1] >= m_pos[1] && _pos[0] < m_pos[0] + m_size[0] && _pos[1] < m_pos[1] + m_size[1]){
    m_isSelected = true;
  }
  else{
    m_isSelected = false;
  }
  return m_isSelected;
}

Button::Action Button::click(float _time)
{
  if(Data::instance()->mode == Data::DWELLING){
    m_firstSelected = 0;
    m_selectedTime = 0;
  }
  m_clicked = _time;
  return m_action;
}
