#include "./OpenGl.h"
#include <cassert>
#include "./../Constants.h"
#include "./../utils/log/Logger.h"
#include <string>

OpenGL::OpenGL(unsigned int appScreenWidth,unsigned int appScreenHeight,unsigned int physicalScreenWidth,unsigned int physicalScreenHeight)
:
appScreenWidth(appScreenWidth),
appScreenHeight(appScreenHeight),
physicalScreenWidth(physicalScreenWidth),
physicalScreenHeight(physicalScreenHeight)
{
  Logger::log("AppScreenWidth: " + std::to_string(appScreenWidth));
  Logger::log("AppScreenHeight: " + std::to_string(appScreenHeight));
  init();
}

void OpenGL::notifyScreenSurfaceChanged(
  unsigned int paramAppScreenWidth,
  unsigned int paramAppScreenHeight,
  unsigned int paramPhysicalScreenWidth,
  unsigned int paramPhysicalScreenHeight
){
  this->appScreenWidth = paramAppScreenWidth;
  this->appScreenHeight = paramAppScreenHeight;
  this->physicalScreenWidth = paramPhysicalScreenWidth;
  this->physicalScreenHeight = paramPhysicalScreenHeight;

  init();
}

void OpenGL::init(){
#ifdef __GLES__
  int nrAttributes;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
  Logger::log("Maximum nr of vertex attributes supported:"+std::to_string(nrAttributes));

  float pointSize = (float(physicalScreenWidth)/float(appScreenWidth)) * 2.0f + 0.5f;
  
  const std::string vShaderStr =
    "attribute vec4 aVertexPosition;\n"
    "attribute vec4 aVertexColor;\n"
    "varying vec4 vColor;\n"
    "void main(){\n"
      "vColor=aVertexColor;\n"
      "gl_PointSize = "+std::to_string(pointSize)+";\n"
      "gl_Position = aVertexPosition;\n"
    "}\n";

  const std::string fShaderStr =
    "precision mediump float;\n"
    "varying vec4 vColor;\n"
    "void main(){\n"
      "gl_FragColor=vColor;\n"
    "}\n";

  auto vertexShader = loadShader(GL_VERTEX_SHADER, vShaderStr.c_str());
  auto fragmentShader = loadShader(GL_FRAGMENT_SHADER, fShaderStr.c_str());

  // Create the program object
  programObject = glCreateProgram();
  assert(programObject != 0);

  glAttachShader(programObject, vertexShader);
  glAttachShader(programObject, fragmentShader);
  
  // Link the program
  glLinkProgram(programObject);
  // Check the link status
  GLint linked;
  glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
  if(!linked)
  {
    GLint infoLen = 0;
    glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);

    if(infoLen > 1)
    {
      char* infoLog = new char[infoLen];
      glGetProgramInfoLog(programObject, infoLen, nullptr, infoLog);
      Logger::log("Error linking program:\n"+std::string(infoLog));
      free(infoLog);
    }
    glDeleteProgram(programObject);
  }
  assert(linked);

  // delete the shaders as they're linked into our program now and no longer necessary
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader); 

  glBindAttribLocation(programObject,0,"aVertexPosition");
  pointParamLocation = glGetAttribLocation(programObject,"aVertexPosition");
  assert(pointParamLocation>=0);

  glBindAttribLocation(programObject,1,"aVertexColor");
  colorParamLocation = glGetAttribLocation(programObject,"aVertexColor");
  assert(colorParamLocation>=0);

#endif
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
#if defined(__OPENGL__)
  glPointSize(1.4f);
  glViewport(0,0,appScreenWidth,appScreenHeight);
  glOrtho(-0.5f, float(appScreenWidth) - 0.5f, -0.5f, float(appScreenHeight) - 0.5f, -1.0, 1.0);
#endif
#if defined(__GLES__)
  #ifdef __ANDROID__
  viewPortWidth = physicalScreenWidth * 2;
  viewPortHeight = physicalScreenHeight * 2;
  #endif
  #ifdef __IOS__
  viewPortWidth = physicalScreenWidth * 2;
  viewPortHeight = physicalScreenHeight * 2;
  #endif

  viewPortStartX = int(-1 * (float(viewPortWidth)/2.0f));
  viewPortStartY = int(-1 * (float(viewPortHeight)/2.0f));
  
  xDifValue = float(appScreenWidth)/4.0f;
  yDifValue = float(appScreenHeight)/4.0f;
  projectionX = 2.0f / float(appScreenWidth);
  projectionY = 2.0f / float(appScreenHeight);
#endif
#if defined(__ANDROID__)
  glViewport(viewPortStartX,viewPortStartY,viewPortWidth,viewPortHeight);
#endif
}

#ifdef __GLES__

//Not using right now
void OpenGL::glesOrtho(float left, float right, float top, float bottom, float near, float far){

    projMat[0] = GLfloat(2.0 / (right - left));
    projMat[1] = 0.0;
    projMat[2] = 0.0;
    projMat[3] = 0.0;
    projMat[4] = 0.0;
    projMat[5] = GLfloat(2.0 / (top - bottom));
    projMat[6] = 0.0;
    projMat[7] = 0.0;
    projMat[8] = 0.0;
    projMat[9] = 0.0;
    projMat[10] = (1.0f / (near - far));
    projMat[11] = 0.0;
    projMat[12] = ((left + right) / (left - right));
    projMat[13] = ((top + bottom) / (bottom - top));
    projMat[14] = (near / (near - far));
    projMat[15] = 1;
}

GLuint OpenGL::loadShader(GLenum shaderType, const char* shaderSource){
  GLuint shader = glCreateShader(shaderType);
  if (shader)
  {
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);
    GLint compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled)
    {
      GLint infoLen = 0;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
      if (infoLen)
      {
        char * buf = (char*) malloc(infoLen);
        if (buf)
        {
          glGetShaderInfoLog(shader, infoLen, NULL, buf);
          Logger::log("Could not Compile Shader:"+std::to_string(shaderType)+"\nBuffer:"+std::string(buf));
          free(buf);
        }
        glDeleteShader(shader);
        shader = 0;
      }
    }
  }
  return shader;
}

#endif // GLES

#ifdef __GLES__

GLuint OpenGL::createProgram(const char* vertexSource, const char * fragmentSource)
{
  GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertexSource);
  if (!vertexShader)
  {
    return 0;
  }
  GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentSource);
  if (!fragmentShader)
  {
    return 0;
  }
  GLuint program = glCreateProgram();
  if (program)
  {
    glAttachShader(program , vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    GLint linkStatus = GL_FALSE;
    glGetProgramiv(program , GL_LINK_STATUS, &linkStatus);
    if( linkStatus != GL_TRUE)
    {
      GLint bufLength = 0;
      glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
      if (bufLength)
      {
        char* buf = (char*) malloc(bufLength);
        if (buf)
        {
          glGetProgramInfoLog(program, bufLength, NULL, buf);
          Logger::log("Could not link program:\n"+std::string(buf));
          free(buf);
        }
      }
      glDeleteProgram(program);
      program = 0;
    }
  }
  return program;
}

#endif // GLES

void OpenGL::clear(){
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGL::flush(){
  glFlush();
}

void OpenGL::drawPixel(unsigned int x,unsigned int y,float red,float green,float blue){
  assert(x>=0 && x<appScreenWidth);
  assert(y>=0 && y<appScreenHeight);
  assert(red>=0 && red<=1.0f);
  assert(green>=0 && green<=1.0f);
  assert(blue>=0 && blue<=1.0f);

#ifdef __OPENGL__
  glColor3f(red,green,blue);
  glVertex2i(x,y);
#else
  {
      position[0] = (x - xDifValue) * projectionX;
      position[1] = (y - yDifValue) * projectionY;
      glVertexAttribPointer((GLuint)pointParamLocation,4,GL_FLOAT,GL_FALSE,0,position);
      assert(checkForOpenGlError());
      glEnableVertexAttribArray((GLuint)pointParamLocation);
      assert(checkForOpenGlError());
  }
  {
      color[0] = red;
      color[1] = green;
      color[2] = blue;
      glVertexAttribPointer((GLuint)colorParamLocation,4,GL_FLOAT,GL_FALSE,0,color);
      assert(checkForOpenGlError());
      glEnableVertexAttribArray((GLuint)colorParamLocation);
      assert(checkForOpenGlError());
  }
  glDrawArrays(GL_POINTS,0,1);
  assert(checkForOpenGlError());
  glDisableVertexAttribArray((GLuint)pointParamLocation);
  glDisableVertexAttribArray((GLuint)colorParamLocation);

#endif
}

void OpenGL::beginDrawingPoints(){
#if defined(__GLES__)
  glUseProgram(programObject);
#if defined(__IOS__)
  glViewport(viewPortStartX,viewPortStartY,viewPortWidth,viewPortHeight);
#endif
#else
  glBegin(GL_POINTS);
#endif
}

void OpenGL::resetProgram(){
#if defined(__GLES__)
  glUseProgram(0);
#else
  glEnd();
#endif
}

void OpenGL::drawText(unsigned int x,unsigned int y,const std::string& text,float red,float green,float blue){
  assert(x>=0 && x<appScreenWidth);
  assert(y>=0 && y<appScreenHeight);
#if defined(__OPENGL__)
  glColor3f(red,green,blue);
  glRasterPos2i(x ,y);
  for (auto& i : text)
  {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,i);
  }
#elif defined(__GLES__)
    //TODO
#endif
}

bool OpenGL::checkForOpenGlError(){
  auto error = glGetError();
  if(error==GL_NO_ERROR){
    return true;
  }
  
  Logger::log("OpenGLError:\n"+std::to_string(error));
  
  while ((error = glGetError())!=GL_NO_ERROR)
  {
    Logger::log("OpenGLError:\n"+std::to_string(error));
  }
  
  return false;
}
