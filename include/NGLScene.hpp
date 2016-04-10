#ifndef NGLSCENE_H__
#define NGLSCENE_H__
#include <ngl/Camera.h>
#include <ngl/Colour.h>
#include <ngl/Light.h>
#include <ngl/Transformation.h>
#include <ngl/Text.h>
#include <QOpenGLWindow>

#include "Button.hpp"
#include <vector>
#include <QTime>
#include <ngl/Obj.h>
#include "Geo.hpp"
//----------------------------------------------------------------------------------------------------------------------
/// @file NGLScene.h
/// @brief this class inherits from the Qt OpenGLWindow and allows us to use NGL to draw OpenGL
/// @author Jonathan Macey
/// @version 1.0
/// @date 10/9/13
/// Revision History :
/// This is an initial version used for the new NGL6 / Qt 5 demos
/// @class NGLScene
/// @brief our main glwindow widget for NGL applications all drawing elements are
/// put in this file
//----------------------------------------------------------------------------------------------------------------------

class NGLScene : public QOpenGLWindow
{
public:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief ctor for our NGL drawing class
  /// @param [in] parent the parent window to the class
  //----------------------------------------------------------------------------------------------------------------------
  NGLScene();
  //-----------------------------------------------------------------------------------------------------------r-----------
  /// @brief dtor must close down ngl and release OpenGL resources
  //----------------------------------------------------------------------------------------------------------------------
  ~NGLScene();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the initialize class is called once when the window is created and we have a valid GL context
  /// use this to setup any default GL stuff
  //----------------------------------------------------------------------------------------------------------------------
  void initializeGL();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this is called everytime we want to draw the scene
  //----------------------------------------------------------------------------------------------------------------------
  void paintGL();

  void addButton(ngl::Vec2 _pos, ngl::Vec2 _size, ngl::Vec4 _color, Button::Action _action);

  void updateButtonArrays();

  GLuint loadTexture(std::string _textureFile);

  void toggleFullScreen();

  Button *checkButtonMouseOver();

  void loadDwellingButtons();
  void loadLargeTouchButtons();
  void loadSmallTouchButtons();
  void loadScanningButtons();

  void buttonHit();

  void loadMatricesToShader();

private:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this is called everytime we resize the window
  //----------------------------------------------------------------------------------------------------------------------
  // Qt 5.5.1 must have this implemented and uses it
  void resizeGL(QResizeEvent *_event);
  // Qt 5.x uses this instead! http://doc.qt.io/qt-5/qopenglwindow.html#resizeGL
  void resizeGL(int _w, int _h);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Qt Event called when a key is pressed
  /// @param [in] _event the Qt event to query for size etc
  //----------------------------------------------------------------------------------------------------------------------
  void keyPressEvent(QKeyEvent *_event);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this method is called every time a mouse is moved
  /// @param _event the Qt Event structure
  //----------------------------------------------------------------------------------------------------------------------
  void mouseMoveEvent (QMouseEvent * _event );
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this method is called everytime the mouse button is pressed
  /// inherited from QObject and overridden here.
  /// @param _event the Qt Event structure
  //----------------------------------------------------------------------------------------------------------------------
  void mousePressEvent ( QMouseEvent *_event);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this method is called everytime the mouse button is released
  /// inherited from QObject and overridden here.
  /// @param _event the Qt Event structure
  //----------------------------------------------------------------------------------------------------------------------
  void mouseReleaseEvent ( QMouseEvent *_event );

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief this method is called everytime the mouse wheel is moved
  /// inherited from QObject and overridden here.
  /// @param _event the Qt Event structure
  //----------------------------------------------------------------------------------------------------------------------
  void wheelEvent( QWheelEvent *_event);

  void timerEvent(QTimerEvent *_event);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief window width
  //----------------------------------------------------------------------------------------------------------------------
  int m_width;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief window height
  //----------------------------------------------------------------------------------------------------------------------
  int m_height;

  bool m_fullScreen;

  GLuint m_vaoIDs[2];

  GLuint m_vboIDs[3];

  std::vector<Button> m_buttons;

  std::vector<float> m_buttonPosAndSizes;

  std::vector<float> m_buttonColors;

  std::vector<float> m_buttonSelectedAndClicked;

  ngl::Vec2 m_mousePos;

  Button *m_currentButton;

  QTime m_time;

  int m_lastRenderElapsed;

  float m_frameRenderTime;

  std::vector<Geo *>m_geo;

  ngl::Transformation m_transform;

  ngl::Mat4 m_view;

  ngl::Mat4 m_project;

  Button::Action m_action;
};



#endif//NGLSCENE_HPP__
