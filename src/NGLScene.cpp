#include <QMouseEvent>
#include <QGuiApplication>

#include "NGLScene.hpp"
#include <ngl/NGLInit.h>
#include <iostream>
#include <ngl/ShaderLib.h>
#include "Data.hpp"
#include <QPoint>

NGLScene::NGLScene()
{
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  setTitle("UI");

  m_mode = DWELLING;

  m_mousePos.set(-1, -1);

  m_time = QTime::currentTime();
}


NGLScene::~NGLScene()
{
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
}

void NGLScene::resizeGL(QResizeEvent *_event)
{
  m_width=_event->size().width()*devicePixelRatio();
  m_height=_event->size().height()*devicePixelRatio();
  ngl::ShaderLib::instance()->setRegisteredUniform("resolution", ngl::Vec2(m_width, m_height));
}

void NGLScene::resizeGL(int _w , int _h)
{
  m_width=_w*devicePixelRatio();
  m_height=_h*devicePixelRatio();
  ngl::ShaderLib::instance()->setRegisteredUniform("resolution", ngl::Vec2(m_width, m_height));
}

void NGLScene::initializeGL()
{
  // we need to initialise the NGL lib which will load all of the OpenGL functions, this must
  // be done once we have a valid GL context but before we call any GL commands. If we dont do
  // this everything will crash
  ngl::NGLInit::instance();
  glClearColor(0.0, 0.0, 0.0, 1.0);			   // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // enable multisampling for smoother drawing
  glEnable(GL_MULTISAMPLE);


  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  // we are creating a shader called Phong
  shader->createShaderProgram("buttonShader");
  // now we are going to create empty shaders for Frag and Vert
  shader->attachShader("buttonVertex",ngl::ShaderType::VERTEX);
  shader->attachShader("buttonFragment",ngl::ShaderType::FRAGMENT);
  shader->attachShader("buttonGeometry", ngl::ShaderType::GEOMETRY);

  // attach the source
  shader->loadShaderSource("buttonVertex","shaders/VertexShader.glsl");
  shader->loadShaderSource("buttonFragment","shaders/FragmentShader.glsl");
  shader->loadShaderSource("buttonGeometry", "shaders/GeometryShader.glsl");

  // compile the shaders
  shader->compileShader("buttonVertex");
  shader->compileShader("buttonFragment");
  shader->compileShader("buttonGeometry");
  // add them to the program
  shader->attachShaderToProgram("buttonShader","buttonVertex");
  shader->attachShaderToProgram("buttonShader","buttonFragment");
  shader->attachShaderToProgram("buttonShader","buttonGeometry");

  // layout attribute 0 is the vertex data (x,y,z)
  //shader->bindAttribute("buttonShader",0,"inPosAndSize");
  // attribute 1 is the UV data u,v (if present)
  //shader->bindAttribute("buttonShader",1,"inColor");

  // now we have associated that data we can link the shader
  shader->linkProgramObject("buttonShader");
  // and make it active ready to load values
  shader->use("buttonShader");


  glGenVertexArrays(1, &m_vaoID);

  glGenBuffers(3, m_vboIDs);

  loadDwellingButtons();

  //addButton(ngl::Vec4(-1.0, -1.0, 2.0, 2.0), ngl::Vec4(1.0, 0.0, 0.0, 1.0), "buttons/test1.png", true);

  //toggleFullScreen();

  shader->setRegisteredUniform("circleSize", Data::instance()->circleSize);
  shader->setRegisteredUniform("dwellTime", Data::instance()->dwellTime);

  startTimer(16);
}

void NGLScene::loadDwellingButtons()
{
  m_buttons.clear();

  ngl::Vec2 buttonPos;
  ngl::Vec2 buttonSize;
  ngl::Vec4 buttonColor;

  buttonColor.set(0.0, 0.0, 1.0, 1.0);
  buttonPos.set(-0.2, -0.2);
  buttonSize.set(0.6, 0.6);


  addButton(buttonPos, buttonSize, buttonColor);



  updateButtonArrays();
}

void NGLScene::loadLargeTouchButtons()
{
  m_buttons.clear();

  updateButtonArrays();
}

void NGLScene::loadSmallTouchButtons()
{
  m_buttons.clear();

  updateButtonArrays();
}

void NGLScene::loadScanningButtons()
{
  m_buttons.clear();

  updateButtonArrays();
}

void NGLScene::paintGL()
{
  updateButtonArrays();

  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0,m_width,m_height);

  glBindVertexArray(m_vaoID);
  glDrawArrays(GL_POINTS, 0, m_buttons.size());

  m_frameRenderTime = (m_time.elapsed() - m_lastRenderElapsed) / 1000.0;

  m_lastRenderElapsed = m_time.elapsed();

}

void NGLScene::addButton(ngl::Vec2 _pos, ngl::Vec2 _size, ngl::Vec4 _color)
{
  GLuint textureID = 0;
  //GLuint textureID = loadTexture(_textureFile);
  //std::cout << "textureID: " << textureID << std::endl;
  m_buttons.push_back(Button(_pos, _size, _color, textureID));
}

GLuint NGLScene::loadTexture(std::string _textureFile)
{
  GLuint textureID = -1;

  QImage image;
  // load given texture files
  bool loaded=image.load(_textureFile.c_str());
  if(loaded == true)
  {
    int width=image.width();
    int height=image.height();

    unsigned char *data = new unsigned char[ width*height*3];
    unsigned int index=0;
    // reversed height so it loads as is used in shader
    for( int y=height-1; y>=0; --y)
    {
      for( int x=0; x<width; ++x)
      {
        // getting RGB from the image
        QRgb colour=image.pixel(x,y);
        data[index++]=qRed(colour);
        data[index++]=qGreen(colour);
        data[index++]=qBlue(colour);
      }
    }

    // set active texture unit
    glActiveTexture(GL_TEXTURE0);


    glGenTextures(1, &textureID);

    // is this the right place??
    glBindTexture(GL_TEXTURE_2D, textureID);

    // load texture
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);

    // setting up mipmap parameters
    // not sure whether the first should be GL_LINEAR or GL_LINEAR_MIPMAP_LINEAR?
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    // set wrapping parameters for textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // creating mipmaps, needs to be done after texture is loaded
    glGenerateMipmap(GL_TEXTURE_2D);

    // calculate texture channel name
    // this allows for easier adding of textures, as they can all be called iChannelX where X is their number

    // get texture unit location and set uniform up
    GLuint texLocation = glGetUniformLocation(ngl::ShaderLib::instance()->getProgramID("buttonShader"), "tex");

    glUniform1i(texLocation, textureID);

    // clean up
    delete[] data;
  }
  else{
    std::cerr << _textureFile << " was not found" << std::endl;
    exit(EXIT_FAILURE);
  }
  return textureID;
}

void NGLScene::updateButtonArrays()
{

  m_buttonPosAndSizes.clear();
  m_buttonColors.clear();
  m_buttonSelectedTimes.clear();

  for(auto &button : m_buttons){
    for(int i=0; i<4; i++){
      if(i<2) m_buttonPosAndSizes.push_back(button.m_pos[i]);
      else m_buttonPosAndSizes.push_back(button.m_size[i-2]);
      m_buttonColors.push_back(button.m_color[i]);
    }
    m_buttonSelectedTimes.push_back(button.m_selectedTime);
  }

  glBindVertexArray(m_vaoID);

  glBindBuffer(GL_ARRAY_BUFFER, m_vboIDs[0]);
  glBufferData(GL_ARRAY_BUFFER, m_buttonPosAndSizes.size() * sizeof(float), &(m_buttonPosAndSizes[0]), GL_STATIC_DRAW);
  // now fix this to the attribute buffer 0
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
  // enable this attribute (will be inPosition in the shader)
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, m_vboIDs[1]);
  glBufferData(GL_ARRAY_BUFFER, m_buttonColors.size() * sizeof(float), &(m_buttonColors[0]), GL_STATIC_DRAW);
  // now fix this to the attribute buffer 0
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
  // enable and bind this attribute (will be inPosition in the shader)
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, m_vboIDs[2]);
  glBufferData(GL_ARRAY_BUFFER, m_buttonSelectedTimes.size() * sizeof(float), &(m_buttonSelectedTimes[0]), GL_STATIC_DRAW);
  // now fix this to the attribute buffer 0
  glVertexAttribPointer(2, 1, GL_FLOAT, GL_TRUE, 0, 0);
  // enable and bind this attribute (will be inPosition in the shader)
  glEnableVertexAttribArray(2);

  // unbund vertex array
  glBindVertexArray(0);
}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mouseMoveEvent (QMouseEvent * _event)
{

}


//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mousePressEvent ( QMouseEvent * _event)
{

}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::mouseReleaseEvent ( QMouseEvent * _event )
{

}

//----------------------------------------------------------------------------------------------------------------------
void NGLScene::wheelEvent(QWheelEvent *_event)
{

}
//----------------------------------------------------------------------------------------------------------------------

void NGLScene::keyPressEvent(QKeyEvent *_event)
{
  // this method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
  switch (_event->key())
  {
  // escape key to quite
  case Qt::Key_Escape : QGuiApplication::exit(EXIT_SUCCESS); break;
  case Qt::Key_F : toggleFullScreen(); break;

  default : break;
  }
}

void NGLScene::timerEvent(QTimerEvent *_event)
{
  QPoint p = mapFromGlobal(QCursor::pos());
  m_mousePos[0] = (p.x() / float(m_width)  * 2) - 1;
  m_mousePos[1] = (p.y() / float(m_height) * 2) - 1;
  m_mousePos[1] *= -1;


  switch(m_mode){
    case DWELLING :
      m_currentButton = checkButtonMouseOver();
      for(Button &button : m_buttons){
        if(m_currentButton && &button == m_currentButton){
          button.m_color.set(0.0, 1.0, 0.0, 1.0);
          button.m_selectedTime+=m_frameRenderTime/Data::instance()->dwellTime;
        }
        else{
          button.m_color.set(0.0, 0.0, 1.0, 1.0);
          button.m_selectedTime = 0;
        }
      }
      break;

  }


  update();
}

Button *NGLScene::checkButtonMouseOver()
{
  Button *buttonPtr = nullptr;
  for(Button &button : m_buttons){
    if(button.isInside(m_mousePos)){
      buttonPtr = &button;
    }
  }
  return buttonPtr;
}

void NGLScene::toggleFullScreen()
{
  // complex fullscreen shaders can be expensive and slow, so be careful making window fullscreen (or just large)
  m_fullScreen ^= true;
  m_fullScreen ? showFullScreen() : showNormal();
}
