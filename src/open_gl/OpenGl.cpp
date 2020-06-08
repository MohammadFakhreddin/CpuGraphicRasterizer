#include "./OpenGl.h"
#include <cassert>
#include "./../Constants.h"
#include "./../utils/log/Logger.h"
#include <string>

OpenGL::OpenGL(
  unsigned int appScreenWidth, 
  unsigned int appScreenHeight, 
  unsigned int physicalScreenWidth, 
  unsigned int physicalScreenHeight
)
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

#ifdef __OPENGL__
OpenGL::OpenGL(
  GLFWwindow* window,
  unsigned int appScreenWidth,
  unsigned int appScreenHeight,
  unsigned int physicalScreenWidth,
  unsigned int physicalScreenHeight
)
:
  window(window),
  appScreenWidth(appScreenWidth),
  appScreenHeight(appScreenHeight),
  physicalScreenWidth(physicalScreenWidth),
  physicalScreenHeight(physicalScreenHeight)
{
  Logger::log("AppScreenWidth: " + std::to_string(appScreenWidth));
  Logger::log("AppScreenHeight: " + std::to_string(appScreenHeight));
  init();
}
#endif

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
  int nrAttributes;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
  Logger::log("Maximum nr of vertex attributes supported:"+std::to_string(nrAttributes));
#ifdef __OPENGL__
  float pointSize = (float(physicalScreenWidth) / float(appScreenWidth)) * 1.2f + 0.5f;
  glPointSize(pointSize);
#endif
  const std::string vShaderStr =
    "attribute vec4 aVertexPosition;\n"
    "attribute vec4 aVertexColor;\n"
    "varying vec4 vColor;\n"
    "void main(){\n"
    "vColor=aVertexColor;\n"
  #ifdef __GLES__
    "gl_PointSize = "+std::to_string(pointSize)+";\n"
  #endif
    "gl_Position = vec4(aVertexPosition.xy,0,1);\n"
  "}\n";

  const std::string fShaderStr =
  #ifdef __GLES__
    "precision mediump float;\n"
  #endif
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

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
#if defined(__GLES__)
  
  viewPortWidth = physicalScreenWidth * 2;
  viewPortHeight = physicalScreenHeight * 2;

  viewPortStartX = int(-1 * (float(viewPortWidth)/2.0f));
  viewPortStartY = int(-1 * (float(viewPortHeight)/2.0f));

#endif
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

void OpenGL::clear(){
  glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGL::flush(){
  glFlush();
}

void OpenGL::drawPixel(
  const GLfloat* position,
  const GLfloat* color,
  const int& pixelCount
){
  {
    glVertexAttribPointer(pointParamLocation,4,GL_FLOAT,GL_FALSE,0,position);
    assert(checkForOpenGlError());
    glEnableVertexAttribArray(pointParamLocation);
    assert(checkForOpenGlError());
  }
  {
    glVertexAttribPointer(colorParamLocation,4,GL_FLOAT,GL_FALSE,0,color);
    assert(checkForOpenGlError());
    glEnableVertexAttribArray(colorParamLocation);
    assert(checkForOpenGlError());
  }
  glDrawArrays(GL_POINTS,0,pixelCount);
  assert(checkForOpenGlError());
  glDisableVertexAttribArray(pointParamLocation);
  glDisableVertexAttribArray(colorParamLocation);
}

void OpenGL::beginDrawingPoints(){
  glUseProgram(programObject);
#ifdef __GLES__
  glViewport(viewPortStartX,viewPortStartY,viewPortWidth,viewPortHeight);
#endif
#ifdef __OPENGL__
  glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);
  glViewport(0, 0, bufferWidth, bufferHeight);
#endif
  //glClear(GL_COLOR_BUFFER_BIT );
}

void OpenGL::resetProgram(){
  glUseProgram(0);
}

bool OpenGL::checkForOpenGlError(){
  GLenum errorCode;
  std::string error = "";
  while ((errorCode = glGetError()) != GL_NO_ERROR)
  {
    switch (errorCode)
    {
      case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
      case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
      case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
      case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
      case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
      case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
      case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
    }
    Logger::log("OpenGLError:\n" + error);
  }
  return errorCode == GL_NO_ERROR;
}
