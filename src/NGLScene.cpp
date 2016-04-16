#include <QMouseEvent>
#include <QGuiApplication>

#include "NGLScene.hpp"
#include <ngl/NGLInit.h>
#include <ngl/ShaderLib.h>
#include "Data.hpp"
#include <QPoint>
#include <sstream>
#include <fstream>

NGLScene::NGLScene()
{
  m_width = Data::instance()->width;
  m_height = Data::instance()->height;
  setTitle("UI");

  m_mousePos.set(-1, -1);

  m_time = QTime::currentTime();
  m_action = Button::Action::NONE;

  m_frame = 0;
  m_finished = false;
  m_colorSet = Data::instance()->colorSet;
}


NGLScene::~NGLScene()
{
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
}

void NGLScene::resizeGL(QResizeEvent *_event)
{
  m_width=_event->size().width()*devicePixelRatio();
  m_height=_event->size().height()*devicePixelRatio();
  ngl::ShaderLib *shader = ngl::ShaderLib::instance();
  shader->use("buttonShader");
  shader->setRegisteredUniform("resolution", ngl::Vec2(m_width, m_height));
  shader->use("progressShader");
  shader->setRegisteredUniform("resolution", ngl::Vec2(m_width, m_height));
  m_project = ngl::perspective(45.0f, float(m_width)/float(m_height), 0.2f, 20.0f);

}

void NGLScene::resizeGL(int _w , int _h)
{
  m_width=_w*devicePixelRatio();
  m_height=_h*devicePixelRatio();
  ngl::ShaderLib *shader = ngl::ShaderLib::instance();
  shader->use("buttonShader");
  shader->setRegisteredUniform("resolution", ngl::Vec2(m_width, m_height));
  shader->use("progressShader");
  shader->setRegisteredUniform("resolution", ngl::Vec2(m_width, m_height));
  m_project = ngl::perspective(45.0f, float(m_width)/float(m_height), 0.2f, 20.0f);
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
  Data *data = Data::instance();


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

  shader->setRegisteredUniform("simpleGhosting", data->simpleGhosting);



  // we are creating a shader called buttonShader
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

  // now we have associated that data we can link the shader
  shader->linkProgramObject("buttonShader");
  // and make it active ready to load values
  shader->use("buttonShader");

  shader->setRegisteredUniform("clickedColor", data->clickedColor);
  shader->setRegisteredUniform("circleSize", data->circleSize);
  shader->setRegisteredUniform("dwellTime", data->dwellTime);
  shader->setRegisteredUniform("clickMovement", data->clickMovement);
  shader->setRegisteredUniform("borderSize", data->borderSize);
  shader->setRegisteredUniform("loadingBarSize", data->loadingBarSize);
  shader->setRegisteredUniform("borderColor", data->borderColor);
  shader->setRegisteredUniform("loadingBarColor", data->loadingBarColor);
  shader->setRegisteredUniform("mode", data->shaderMode);
  shader->setRegisteredUniform("animLength", data->animLength);
  shader->setRegisteredUniform("originalColor", data->baseColor);
  shader->setRegisteredUniform("hueChangeAmount", data->hueChangeAmount);
  shader->setRegisteredUniform("geoColor", data->geoColor);



  shader->createShaderProgram("progressShader");
  shader->attachShader("prgoressVertex",ngl::ShaderType::VERTEX);
  shader->attachShader("progressFragment",ngl::ShaderType::FRAGMENT);

  shader->loadShaderSource("progressVertex","shaders/ProgressVertex.glsl");
  shader->loadShaderSource("progressFragment","shaders/ProgressFragment.glsl");

  shader->compileShader("progressVertex");
  shader->compileShader("progressFragment");

  shader->attachShaderToProgram("progressShader","progressVertex");
  shader->attachShaderToProgram("progressShader","progressFragment");

  shader->linkProgramObject("progressShader");
  shader->use("progressShader");
  shader->setRegisteredUniform("frame", m_frame);
  shader->setRegisteredUniform("animLength", data->animLength);
  shader->setRegisteredUniform("backgroundColor", data->backgroundColor);
  shader->setRegisteredUniform("progressBarSize", data->progressBarSize);
  shader->setRegisteredUniform("progressBarColor", data->progressBarColor);
  shader->setRegisteredUniform("progressBarSelection", data->progressBarSelection);
  shader->setRegisteredUniform("progressBarBack", data->progressBarBack);
  shader->setRegisteredUniform("borderSize", data->borderSize);
  shader->setRegisteredUniform("borderColor", data->borderColor);


  m_view = ngl::lookAt(ngl::Vec3(5,1,0), ngl::Vec3(0,1,0), ngl::Vec3(0,1,0));

  m_geo.push_back(new Geo("geo/base.obj", 0.05, 360000));
  m_geo.push_back(new Geo("geo/lower.obj", 0.8, 90));
  m_geo.push_back(new Geo("geo/upper.obj", 0.8, 90));
  m_geo.push_back(new Geo("geo/hand.obj", 0.2, 90));

  for(auto &geo : m_geo){
    geo->m_mesh.createVAO();
  }

  createQuad();

  glGenVertexArrays(2, m_vaoIDs);

  glGenBuffers(3, m_vboIDs);

  //loadTextures();

  loadButtons();

  m_timer1 = startTimer(16);
}

void NGLScene::createQuad()
{
  // a simple quad object which fills the screen, from Jon's code
  float* vert = new float[18];
  const static float s=1.0;
  vert[0] =-s; vert[1] =  s; vert[2] =-1.0;
  vert[3] = s; vert[4] =  s; vert[5] =-1.0;
  vert[6] = -s; vert[7] = -s; vert[8]= -1.0;

  vert[9] =-s; vert[10]= -s; vert[11]=-1.0;
  vert[12] =s; vert[13]= -s; vert[14]=-1.0;
  vert[15] =s; vert[16]= s; vert[17]=-1.0;
  // allocate a VertexArray
  glGenVertexArrays(1, &m_vaoID);
  // now bind a vertex array object for our verts
  glBindVertexArray(m_vaoID);
  // now we are going to bind this to our vbo
  GLuint vboID;
  glGenBuffers(1, &vboID);
  // now bind this to the VBO buffer
  glBindBuffer(GL_ARRAY_BUFFER, vboID);
  // allocate the buffer datra
  glBufferData(GL_ARRAY_BUFFER, 18*sizeof(GLfloat), vert, GL_STATIC_DRAW);
  // now fix this to the attribute buffer 0
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  // enable and bind this attribute (will be inPosition in the shader)
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);

  // clean up
  delete [] vert;
}

void NGLScene::drawQuad()
{

  // bind and draw our quad
  glBindVertexArray(m_vaoID);

  glDrawArrays(GL_TRIANGLES, 0, 6);
}

void NGLScene::loadButtons()
{
  switch(Data::instance()->mode){
    case Data::DWELLING :
      loadDwellingButtons();
    break;
    case Data::SCANNING :
      loadScanningButtons();
    break;
    case Data::TOUCHLARGE :
      loadLargeTouchButtons();
    break;
    case Data::TOUCHSMALL_L :
    case Data::TOUCHSMALL_R :
      loadSmallTouchButtons();
    break;
    default :
      std::cerr << "DATA MODE ERROR!" << std::endl;
    break;
  }
}

void NGLScene::loadDwellingButtons()
{
  Data *data = Data::instance();
  m_buttons.clear();

  ngl::Vec2 buttonPos;
  ngl::Vec2 buttonSize;
  ngl::Vec4 buttonColor;

  buttonColor.set(data->baseColor);
  if(m_colorSet){

    float borderX = 50.0;
    float borderY = 50.0;

    borderX = borderX/m_width;
    borderY = borderY/m_height;

    float firstX = borderX * 2 - 1.0;
    float firstY = borderY * 2 - 1.0;
    float firstW = 0.7;


    float keyX = firstX;
    float keyY = firstY;
    keyX += borderX + firstW;

    float keyX2 = (keyX * -1);

    buttonPos.set(firstX, firstY);
    buttonSize.set(firstW, 0.35);
    addButton(buttonPos, buttonSize, buttonColor, Button::Action::SPIN_CW);

    buttonPos.set(keyX, keyY);
    buttonSize.set(keyX2 - keyX, 0.35);
    addButton(buttonPos, buttonSize, buttonColor, Button::Action::KEY);

    buttonPos.set(firstX, firstY);
    buttonSize.set(firstW, 0.35);

    buttonPos.m_x *= -1;
    buttonPos.m_x -= buttonSize.m_x;
    addButton(buttonPos, buttonSize, buttonColor, Button::Action::SPIN_CCW);

    buttonSize.set(0.5, 0.35);
    buttonPos.m_x = firstX;
    buttonPos.m_y += borderY + buttonSize.m_y;
    addButton(buttonPos, buttonSize, buttonColor, Button::Action::ROTATE_1_L);

    buttonPos.m_x *= -1;
    buttonPos.m_x -= buttonSize.m_x;
    addButton(buttonPos, buttonSize, buttonColor, Button::Action::ROTATE_1_R);

    buttonPos.m_x *= -1;
    buttonPos.m_x -= buttonSize.m_x;
    buttonPos.m_y += borderY + buttonSize.m_y;
    addButton(buttonPos, buttonSize, buttonColor, Button::Action::ROTATE_2_L);

    buttonPos.m_x *= -1;
    buttonPos.m_x -= buttonSize.m_x;
    addButton(buttonPos, buttonSize, buttonColor, Button::Action::ROTATE_2_R);

    buttonPos.m_x *= -1;
    buttonPos.m_x -= buttonSize.m_x;
    buttonPos.m_y += borderY + buttonSize.m_y;
    addButton(buttonPos, buttonSize, buttonColor, Button::Action::ROTATE_3_L);

    buttonPos.m_x *= -1;
    buttonPos.m_x -= buttonSize.m_x;
    addButton(buttonPos, buttonSize, buttonColor, Button::Action::ROTATE_3_R);
  }
  else{
    float borderX = 50.0;
    float borderY = 50.0;

    borderX = borderX/m_width;
    borderY = borderY/m_height;

    float firstX = borderX * 2 - 1.0;
    float firstY = borderY * 2 - 1.0;
    float firstW = 0.5;


    float keyX = firstX;
    float keyY = firstY;
    keyX += borderX + firstW;

    float keyX2 = (keyX * -1);

    buttonPos.set(firstX, firstY);
    buttonSize.set(firstW, 0.7);
    addButton(buttonPos, buttonSize, buttonColor, Button::Action::COLOR_L);

    buttonPos.set(keyX, keyY);
    buttonSize.set(keyX2 - keyX, 0.4);
    addButton(buttonPos, buttonSize, buttonColor, Button::Action::COLOR_SET);

    buttonPos.set(firstX, firstY);
    buttonSize.set(firstW, 0.7);

    buttonPos.m_x *= -1;
    buttonPos.m_x -= buttonSize.m_x;
    addButton(buttonPos, buttonSize, buttonColor, Button::Action::COLOR_R);
  }

  updateButtonArrays();
}

void NGLScene::loadLargeTouchButtons()
{
  loadDwellingButtons();
}

void NGLScene::loadSmallTouchButtons()
{
  Data *data = Data::instance();
  m_buttons.clear();

  ngl::Vec2 buttonPos;
  ngl::Vec2 buttonSize;
  ngl::Vec4 buttonColor;
  float sf = 0.3;
  ngl::Vec2 one;

  if(data->mode == Data::TOUCHSMALL_L){
    one = ngl::Vec2(-0.65, -0.65);
  }
  else{
    one = ngl::Vec2(0.65, -0.65);
  }


  buttonColor.set(data->baseColor);
  if(m_colorSet){

    float borderX = 50.0;
    float borderY = 50.0;

    borderX = borderX/m_width;
    borderY = borderY/m_height;

    float firstX = borderX * 2 - 1.0;
    float firstY = borderY * 2 - 1.0;
    float firstW = 0.7;

    float keyX = firstX;
    float keyY = firstY;
    keyX += borderX + firstW;

    float keyX2 = (keyX * -1);

    buttonPos.set(firstX, firstY);
    buttonSize.set(firstW, 0.35);
    addButton(one+sf*buttonPos, sf*buttonSize, buttonColor, Button::Action::SPIN_CW);

    buttonPos.set(keyX, keyY);
    buttonSize.set(keyX2 - keyX, 0.35);
    addButton(one+sf*buttonPos, sf*buttonSize, buttonColor, Button::Action::KEY);

    buttonPos.set(firstX, firstY);
    buttonSize.set(firstW, 0.35);

    buttonPos.m_x *= -1;
    buttonPos.m_x -= buttonSize.m_x;
    addButton(one+sf*buttonPos, sf*buttonSize, buttonColor, Button::Action::SPIN_CCW);

    buttonPos.m_x = firstX;
    buttonPos.m_y += borderY + buttonSize.m_y;
    addButton(one+sf*buttonPos, sf*buttonSize, buttonColor, Button::Action::ROTATE_1_L);

    buttonPos.m_x *= -1;
    buttonPos.m_x -= buttonSize.m_x;
    addButton(one+sf*buttonPos, sf*buttonSize, buttonColor, Button::Action::ROTATE_1_R);

    buttonPos.m_x *= -1;
    buttonPos.m_x -= buttonSize.m_x;
    buttonPos.m_y += borderY + buttonSize.m_y;
    addButton(one+sf*buttonPos, sf*buttonSize, buttonColor, Button::Action::ROTATE_2_L);

    buttonPos.m_x *= -1;
    buttonPos.m_x -= buttonSize.m_x;
    addButton(one+sf*buttonPos, sf*buttonSize, buttonColor, Button::Action::ROTATE_2_R);

    buttonPos.m_x *= -1;
    buttonPos.m_x -= buttonSize.m_x;
    buttonPos.m_y += borderY + buttonSize.m_y;
    addButton(one+sf*buttonPos, sf*buttonSize, buttonColor, Button::Action::ROTATE_3_L);

    buttonPos.m_x *= -1;
    buttonPos.m_x -= buttonSize.m_x;
    addButton(one+sf*buttonPos, sf*buttonSize, buttonColor, Button::Action::ROTATE_3_R);
  }
  else{
    float borderX = 50.0;
    float borderY = 50.0;

    borderX = borderX/m_width;
    borderY = borderY/m_height;

    float firstX = borderX * 2 - 1.0;
    float firstY = borderY * 2 - 1.0;
    float firstW = 0.5;


    float keyX = firstX;
    float keyY = firstY;
    keyX += borderX + firstW;

    float keyX2 = (keyX * -1);

    buttonPos.set(firstX, firstY);
    buttonSize.set(firstW, 0.7);
    addButton(one+sf*buttonPos, sf*buttonSize, buttonColor, Button::Action::COLOR_L);

    buttonPos.set(keyX, keyY);
    buttonSize.set(keyX2 - keyX, 0.4);
    addButton(one+sf*buttonPos, sf*buttonSize, buttonColor, Button::Action::COLOR_SET);

    buttonPos.set(firstX, firstY);
    buttonSize.set(firstW, 0.7);

    buttonPos.m_x *= -1;
    buttonPos.m_x -= buttonSize.m_x;
    addButton(one+sf*buttonPos, sf*buttonSize, buttonColor, Button::Action::COLOR_R);
  }

  updateButtonArrays();
}

void NGLScene::loadScanningButtons()
{
  Data *data = Data::instance();
  loadDwellingButtons();

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
  Data *data = Data::instance();
  updateButtonArrays();
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0,m_width,m_height);

  shader->use("progressShader");
  shader->setRegisteredUniform("colorSet", m_colorSet && !m_finished);

  drawQuad();

  glClear(GL_DEPTH_BUFFER_BIT);


  if(!m_finished){
    shader->use("geoShader");
    if(data->perspGhosting){
      shader->setRegisteredUniform("color", data->geoColor);
      shader->setRegisteredUniform("alpha", (float)1.0);
      m_transform.reset();
      for(unsigned int i = 0; i<m_geo.size(); i++){
        m_transform.setRotation(m_geo[i]->m_rotation);
        loadMatricesToShader();
        m_geo[i]->m_mesh.draw();
        m_transform.addPosition(m_geo[i]->getTopTranslation());
      }
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    int geoIndex = 0;
    int trueGhosting = data->ghosting + 1;
    m_transform.reset();
    for(int j=m_rotHistory.size()-m_geo.size(); j>=0; j-=4){
      for(int i=j; i < j+m_geo.size(); i++){
        if((m_rotHistory.size()-1-i)/m_geo.size() < data->ghosting){
          float alpha = 1.0 - (float((m_rotHistory.size()-1-i)/4 + 1.0)/trueGhosting);
          alpha *= alpha;
          shader->setRegisteredUniform("color", data->ghostColor);

          shader->setRegisteredUniform("alpha", alpha);

          Geo *geo = m_geo[geoIndex];
          ngl::Vec3 rotation = m_rotHistory[i];
          ngl::Vec3 translation;
          if(i % 4 == 0){
            m_transform.reset();
          }
          translation = m_transHistory[i];
          m_transform.setRotation(rotation);
          loadMatricesToShader();
          geo->m_mesh.draw();
          m_transform.addPosition(translation);
          geoIndex++;
          if(geoIndex == m_geo.size()){
            geoIndex = 0;
            m_transform.reset();
          }
        }
      }
    }
    glDisable(GL_BLEND);
    glClear(GL_DEPTH_BUFFER_BIT);

    if(!data->perspGhosting){
      shader->setRegisteredUniform("color", data->geoColor);
      shader->setRegisteredUniform("alpha", (float)1.0);
      m_transform.reset();
      for(unsigned int i = 0; i<m_geo.size(); i++){
        m_transform.setRotation(m_geo[i]->m_rotation);
        loadMatricesToShader();
        m_geo[i]->m_mesh.draw();
        m_transform.addPosition(m_geo[i]->getTopTranslation());
      }
    }
  }
  else{
    shader->use("geoShader");
    shader->setRegisteredUniform("color", data->geoColor);
    shader->setRegisteredUniform("alpha", (float)1.0);
    m_frame %= data->animLength;
    m_transform.reset();
    for(unsigned int i = 0; i<m_geo.size(); i++){
      m_transform.setRotation(m_rotHistory[(m_frame*4)+i]);
      loadMatricesToShader();
      m_geo[i]->m_mesh.draw();
      m_transform.addPosition(m_transHistory[(m_frame*4)+i]);
    }
  }

  shader->use("buttonShader");
  glClear(GL_DEPTH_BUFFER_BIT);
  glBindVertexArray(m_vaoIDs[0]);
  glDrawArrays(GL_POINTS, 0, m_buttons.size());
  glBindVertexArray(0);
  m_frameRenderTime = (m_time.elapsed() - m_lastRenderElapsed) / 1000.0;

  m_lastRenderElapsed = m_time.elapsed();
}

void NGLScene::addButton(ngl::Vec2 _pos, ngl::Vec2 _size, ngl::Vec4 _color, Button::Action _action)
{
  GLuint textureID = 0;
  //GLuint textureID = loadTexture(_textureFile);
  //std::cout << "textureID: " << textureID << std::endl;
  m_buttons.push_back(Button(_pos, _size, _color, _action, textureID));
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
  }
  return textureID;
}

void NGLScene::updateButtonArrays()
{
  m_buttonPosAndSizes.clear();
  m_buttonColors.clear();
  m_buttonSelectedClickedAction.clear();

  for(Button &button : m_buttons){
    for(int i=0; i<4; i++){
      if(i<2) m_buttonPosAndSizes.push_back(button.m_pos[i]);
      else m_buttonPosAndSizes.push_back(button.m_size[i-2]);
      m_buttonColors.push_back(button.m_color[i]);
    }
    m_buttonSelectedClickedAction.push_back(button.m_selectedTime);
    m_buttonSelectedClickedAction.push_back(button.m_clicked);
    m_buttonSelectedClickedAction.push_back(float(button.m_action));
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
  glBufferData(GL_ARRAY_BUFFER, m_buttonSelectedClickedAction.size() * sizeof(float), &(m_buttonSelectedClickedAction[0]), GL_STATIC_DRAW);
  // now fix this to the attribute buffer 0
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, 0, 0);
  // enable and bind this attribute (will be inPosition in the shader)
  glEnableVertexAttribArray(2);

  //glBindBuffer(GL_ARRAY_BUFFER, m_vboIDs[3]);
  //glBufferData(GL_ARRAY_BUFFER, m_buttonActions.size() * sizeof(float), &(m_buttonActions[0]), GL_STATIC_DRAW);
  // now fix this to the attribute buffer 0
  //glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 0, 0);
  // enable and bind this attribute (will be inPosition in the shader)
  //glEnableVertexAttribArray(3);

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
  Data *data = Data::instance();
  if(data->mode == Data::TOUCHLARGE || data->mode == Data::TOUCHSMALL_L || data->mode == Data::TOUCHSMALL_R){
    if(_event->button() == Qt::LeftButton){
      m_currentButton = checkButtonMouseOver();
      if(m_currentButton){
        m_action = m_currentButton->click(QTime::currentTime().msecsSinceStartOfDay() / 1000.0);
      }
    }
  }
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
  case Qt::Key_Escape : std::cout << "Quitting, not saving animation" << std::endl; QGuiApplication::exit(EXIT_SUCCESS); break;
  case Qt::Key_F : toggleFullScreen(); break;
  case Qt::Key_Space : if(!_event->isAutoRepeat() && Data::instance()->mode == Data::SCANNING) buttonHit();

  default : break;
  }
}

void NGLScene::timerEvent(QTimerEvent *_event)
{
  Data *data = Data::instance();
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  if(_event->timerId() == m_timer1){
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
                m_action = button.click(currentTime);
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
        if(m_buttons.size() == 1){
          m_currentButton = &(m_buttons[0]);
        }
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

      case Data::TOUCHLARGE :
      case Data::TOUCHSMALL_L :
      case Data::TOUCHSMALL_R :
      break;

      default :
        std::cerr << "mode not set" << std::endl;
      break;
    }

    int start = 0;
    int dir = 0;
    int rotIndex = 0;
    int hueChange = 0;
    switch(m_action){
      case Button::Action::NONE :
      break;
      case Button::Action::SPIN_CCW :
        if(m_geo[0]->m_localRotation[1] < (m_geo[0]->m_maxAngle)){
          rotIndex = 1;
          start = 0;
          dir += 1;
        }
      break;
      case Button::Action::SPIN_CW :
        if(m_geo[0]->m_localRotation[1] > -(m_geo[0]->m_maxAngle)){
          rotIndex = 1;
          start = 0;
          dir -= 1;
        }
      break;
      case Button::Action::ROTATE_1_R :
          rotIndex = 0;
          if(m_geo[1]->m_localRotation[0] > -(m_geo[1]->m_maxAngle)){
            start = 1;
            dir -= 1;
          }
      break;
      case Button::Action::ROTATE_1_L :
          rotIndex = 0;
          if(m_geo[1]->m_localRotation[0] < m_geo[1]->m_maxAngle){
            start = 1;
            dir += 1;
          }
      break;
      case Button::Action::ROTATE_2_R :
          rotIndex = 0;
          if(m_geo[2]->m_localRotation[0] > -(m_geo[2]->m_maxAngle)){
            start = 2;
            dir -= 1;
          }
      break;
      case Button::Action::ROTATE_2_L :
          rotIndex = 0;
          if(m_geo[2]->m_localRotation[0] < m_geo[2]->m_maxAngle){
            start = 2;
            dir += 1;
          }
      break;
      case Button::Action::ROTATE_3_R :
          rotIndex = 0;
          if(m_geo[3]->m_localRotation[0] > -(m_geo[3]->m_maxAngle)){
            start = 3;
            dir -= 1;
          }
      break;
      case Button::Action::ROTATE_3_L :
          rotIndex = 0;
          if(m_geo[3]->m_localRotation[0] < m_geo[3]->m_maxAngle){
            start = 3;
            dir += 1;
          }
      break;
      case Button::Action::KEY :
        writeKey();
      break;
      case Button::Action::FINISH :
        exit(writeOut());
      break;
      case Button::Action::COLOR_L :
        hueChange = 1;
      break;
      case Button::Action::COLOR_R :
        hueChange = -1;
      break;
      case Button::Action::COLOR_SET :
        m_timer3 = startTimer(data->dwellTime/5 * 1000);
      break;
      default :
        std::cout << "ERROR: button action" << std::endl;
      break;
    }

    if(hueChange){
      data->updateColor(hueChange);
      shader->use("buttonShader");
      shader->setRegisteredUniform("geoColor", data->geoColor);
    }

    m_geo[start]->m_localRotation[rotIndex] += data->rotateAngle * dir;
    for(unsigned int i = start; i<m_geo.size(); i++){
        m_geo[i]->m_rotation[rotIndex] += data->rotateAngle * dir;
    }

    m_action = Button::Action::NONE;

    shader->use("buttonShader");
    shader->setRegisteredUniform("currentTime", currentTime);
    update();
  }
  else if(_event->timerId() == m_timer2){
   m_frame++;
  }
  else if(_event->timerId() == m_timer3){
    m_colorSet = true;
    loadButtons();
    killTimer(m_timer3);
  }
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
  m_action = m_currentButton->click(currentTime);
}

void NGLScene::toggleFullScreen()
{
  // complex fullscreen shaders can be expensive and slow, so be careful making window fullscreen (or just large)
  m_fullScreen ^= true;
  m_fullScreen ? showFullScreen() : showNormal();
}

void NGLScene::loadMatricesToShader()
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;

  MVP = m_transform.getMatrix() * m_view * m_project;
  normalMatrix = m_transform.getMatrix();
  normalMatrix.inverse();

  shader->setRegisteredUniform("MVP", MVP);
  shader->setRegisteredUniform("normalMatrix", normalMatrix);
}

void NGLScene::writeKey()
{
  std::cout << "KEY!" << std::endl;
  ngl::Vec3 translation(0,0,0);
  for(unsigned int i=0; i<m_geo.size(); i++ ){
    m_rotHistory.push_back(m_geo[i]->m_rotation);
    m_localRotHistory.push_back(m_geo[i]->m_localRotation);
    if(i>0){
      translation += m_geo[i-1]->getTopTranslation();
    }
    else{
      translation.set(0,0,0);
    }
    m_transHistory.push_back(m_geo[i]->getTopTranslation());
  }
  if(m_rotHistory.size()/4 == Data::instance()->animLength){
    //exit(writeOut());
    m_buttons.clear();

    std::cout << "adding" << std::endl;

    ngl::Vec2 buttonPos;
    ngl::Vec2 buttonSize;
    float borderX = 50.0;
    float borderY = 50.0;

    borderX = borderX/m_width;
    borderY = borderY/m_height;

    float keyX = borderX * 2 - 1.0;
    float keyY = borderY * 2 - 1.0;

    float keyX2 = (keyX * -1);

    buttonPos.set(keyX, keyY);
    buttonSize.set(0.5, 0.55);

    if(Data::instance()->mode == Data::TOUCHSMALL_R){
      buttonPos.m_x *= -1;
      buttonPos.m_x -= buttonSize.m_x;
    }

    addButton(buttonPos, buttonSize, Data::instance()->baseColor, Button::Action::FINISH);

    updateButtonArrays();
    m_finished = true;
    m_timer2 = startTimer(1000/Data::instance()->fps);
  }
  else if(m_rotHistory.size()/4 > Data::instance()->animLength){
    std::cerr << "ERROR, size of history too large" << std::endl;
  }
  m_frame++;
  ngl::ShaderLib::instance()->use("progressShader");
  ngl::ShaderLib::instance()->setRegisteredUniform("frame", m_frame);
}

int NGLScene::writeOut()
{
  std::cout << "saving Animation" << std::endl;
  std::ostringstream convertStream;
  std::ofstream file;

  std::cout << "history length" << m_rotHistory.size() << std::endl;

  for(unsigned int i=0; i<m_rotHistory.size(); i++){
    convertStream << m_rotHistory[i].m_x << " " << m_rotHistory[i].m_y << " " << m_rotHistory[i].m_z<< "\n";
    convertStream << m_transHistory[i].m_x << " " << m_transHistory[i].m_y << " " << m_transHistory[i].m_z<< "\n";
  }
  convertStream << Data::instance()->fps << "\n";
  convertStream << Data::instance()->geoColor[0] << " " << Data::instance()->geoColor[1] << " " << Data::instance()->geoColor[2];

  file.open("Out.txt");
  file << convertStream.str() << std::endl;

  file.close();
  return EXIT_SUCCESS;
}
