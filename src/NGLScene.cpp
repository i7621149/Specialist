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
  glClearColor(0.8, 0.9, 0.9, 1.0);			   // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // enable multisampling for smoother drawing
  glEnable(GL_MULTISAMPLE);
  std::cout << "generating" << std::endl;

  ngl::ShaderLib *shader=ngl::ShaderLib::instance();



  shader->createShaderProgram("geoShader");
  // now we are going to create empty shaders for Frag and Vert
  shader->attachShader("geoVertex",ngl::ShaderType::VERTEX);
  shader->attachShader("geoFragment",ngl::ShaderType::FRAGMENT);
  // attach the source
  shader->loadShaderSource("geoVertex","shaders/GeoVertex.glsl");
  shader->loadShaderSource("geoFragment","shaders/GeoFragment.glsl");
  // compile the shaders
  shader->compileShader("geoVertex");
  shader->compileShader("geoFragment");
  // add them to the program
  shader->attachShaderToProgram("geoShader","geoVertex");
  shader->attachShaderToProgram("geoShader","geoFragment");
  // now we have associated that data we can link the shader
  shader->linkProgramObject("geoShader");
  // and make it active ready to load values
  shader->use("geoShader");



  // we are creating a shader called Phong
  shader->createShaderProgram("buttonShader");
  // now we are going to create empty shaders for Frag and Vert
  shader->attachShader("buttonVertex",ngl::ShaderType::VERTEX);
  shader->attachShader("buttonFragment",ngl::ShaderType::FRAGMENT);
  shader->attachShader("buttonGeometry", ngl::ShaderType::GEOMETRY);

  // attach the source
  shader->loadShaderSource("buttonVertex","shaders/ButtonVertex.glsl");
  shader->loadShaderSource("buttonFragment","shaders/ButtonFragment.glsl");
  shader->loadShaderSource("buttonGeometry", "shaders/ButtonGeometry.glsl");

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



  ngl::VAOPrimitives::instance()->createCylinder("cylinder1", 1.0, 1.0, 8, 1);
  ngl::VAOPrimitives::instance()->createSphere("sphere1", 1.0, 100);

  glGenVertexArrays(2, m_vaoIDs);

  glGenBuffers(3, m_vboIDs);


  //addButton(ngl::Vec4(-1.0, -1.0, 2.0, 2.0), ngl::Vec4(1.0, 0.0, 0.0, 1.0), "buttons/test1.png", true);

  //toggleFullScreen();

  Data *data = Data::instance();

  switch(data->mode){
    case Data::DWELLING :
      loadDwellingButtons();
    break;
    case Data::SCANNING :
      loadScanningButtons();
    break;
    case Data::TOUCHSCREEN :
      std::cout << "touch!" << std::endl;
    break;
  }

  shader->setRegisteredUniform("clickedColor", data->clickedColor);
  shader->setRegisteredUniform("circleSize", data->circleSize);
  shader->setRegisteredUniform("dwellTime", data->dwellTime);
  shader->setRegisteredUniform("clickMovement", data->clickMovement);
  shader->setRegisteredUniform("borderSize", data->borderSize);
  shader->setRegisteredUniform("borderColor", data->borderColor);
  shader->setRegisteredUniform("mode", data->mode);

  startTimer(16);
}

void NGLScene::loadDwellingButtons()
{
  Data *data = Data::instance();
  m_buttons.clear();

  ngl::Vec2 buttonPos;
  ngl::Vec2 buttonSize;
  ngl::Vec4 buttonColor;

  buttonColor.set(data->baseColor);
  buttonPos.set(-0.2, -0.2);
  buttonSize.set(0.6, 0.6);

  addButton(buttonPos, buttonSize, buttonColor);

  buttonPos.set(-0.9, -0.9);
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
  Data *data = Data::instance();
  m_buttons.clear();

  ngl::Vec2 buttonPos;
  ngl::Vec2 buttonSize;
  ngl::Vec4 buttonColor;

  buttonColor.set(data->baseColor);
  buttonPos.set(0.0, -0.2);
  buttonSize.set(0.6, 0.6);

  addButton(buttonPos, buttonSize, buttonColor);

  buttonPos.set(-0.9, -0.9);
  addButton(buttonPos, buttonSize, buttonColor);

  buttonSize.set(0.6, 0.4);
  buttonPos.set(-0.8, 0.2);
  addButton(buttonPos, buttonSize, buttonColor);

  updateButtonArrays();
  if(m_buttons.size()>0){
    m_currentButton = &m_buttons[0];
    m_currentButton->m_firstSelected = QTime::currentTime().msecsSinceStartOfDay() / 1000.0;
    m_currentButton->m_isSelected = true;
    m_currentButton->m_color.set(data->selectedColor);
  }
}

void NGLScene::paintGL()
{
  ngl::ShaderLib *shader = ngl::ShaderLib::instance();
  updateButtonArrays();
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0,m_width,m_height);

  shader->use("geoShader");

  ngl::VAOPrimitives::instance()->draw("teapot");

  shader->use("buttonShader");

  glBindVertexArray(m_vaoIDs[0]);
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
  m_buttonSelectedAndClicked.clear();

  for(Button &button : m_buttons){
    for(int i=0; i<4; i++){
      if(i<2) m_buttonPosAndSizes.push_back(button.m_pos[i]);
      else m_buttonPosAndSizes.push_back(button.m_size[i-2]);
      m_buttonColors.push_back(button.m_color[i]);
    }
    m_buttonSelectedAndClicked.push_back(button.m_selectedTime);
    m_buttonSelectedAndClicked.push_back(button.m_clicked);
  }

  for(int i=0; i<m_buttonSelectedAndClicked.size(); i++){
  }

  glBindVertexArray(m_vaoIDs[0]);

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
  glBufferData(GL_ARRAY_BUFFER, m_buttonSelectedAndClicked.size() * sizeof(float), &(m_buttonSelectedAndClicked[0]), GL_STATIC_DRAW);
  // now fix this to the attribute buffer 0
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, 0, 0);
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

  case Qt::Key_Space : buttonHit();

  default : break;
  }
}

void NGLScene::timerEvent(QTimerEvent *_event)
{
  Data *data = Data::instance();
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  QPoint p = mapFromGlobal(QCursor::pos());
  m_mousePos[0] = (p.x() / float(m_width)  * 2) - 1;
  m_mousePos[1] = (p.y() / float(m_height) * 2) - 1;
  m_mousePos[1] *= -1;

  float currentTime = QTime::currentTime().msecsSinceStartOfDay() / 1000.0;

  switch(Data::instance()->mode){
    case Data::DWELLING :
      m_currentButton = checkButtonMouseOver();
      for(Button &button : m_buttons){
        if(m_currentButton && &button == m_currentButton){
          if(button.m_selectedTime == 0){
            button.m_isSelected = true;
            button.m_firstSelected = currentTime;
            button.m_selectedTime = 0.001;
            button.m_color.set(data->selectedColor);
          }
          else{
            button.m_selectedTime = currentTime - button.m_firstSelected;
            if(button.m_selectedTime > data->dwellTime){
              button.click(currentTime);
            }
          }
        }
        else{
          button.m_color.set(data->baseColor);
          button.m_isSelected = false;
          button.m_firstSelected = 0;
          button.m_selectedTime = 0;
        }
      }
    break;
    case Data::SCANNING :
      if(m_currentButton){
        m_currentButton->m_selectedTime = currentTime - m_currentButton->m_firstSelected;

        if(m_currentButton->m_selectedTime > data->dwellTime){
          m_currentButton->m_isSelected = false;
          m_currentButton->m_selectedTime = 0;
          m_currentButton->m_color.set(data->baseColor);

          if(m_currentButton == &m_buttons[m_buttons.size()-1]){
            m_currentButton = &m_buttons[0];
          }
          else{
            m_currentButton++;
          }

          m_currentButton->m_isSelected = true;
          m_currentButton->m_firstSelected = currentTime;
          m_currentButton->m_color.set(data->selectedColor);
        }
      }
    break;

    default :
      std::cerr << "mode not set" << std::endl;
    break;


  }

  shader->setRegisteredUniform("currentTime", currentTime);
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

void NGLScene::buttonHit()
{
  float currentTime = QTime::currentTime().msecsSinceStartOfDay() / 1000.0;
  m_currentButton->click(currentTime);
}

void NGLScene::toggleFullScreen()
{
  // complex fullscreen shaders can be expensive and slow, so be careful making window fullscreen (or just large)
  m_fullScreen ^= true;
  m_fullScreen ? showFullScreen() : showNormal();
}
