#include "./OpenGl.h"
#include <assert.h>
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

#ifdef __GLES__
  int nrAttributes;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
  Logger::log("Maximum nr of vertex attributes supported:"+std::to_string(nrAttributes));

  const char* vShaderStr = 
    "attribute vec4 aVertexPosition;\n"
    "attribute vec4 aVertexColor;\n"
    "varying vec4 vColor;\n"
    "void main(){\n"
      "vColor=aVertexColor;\n"
      #ifdef __ANDROID__
      "gl_PointSize = 14.1;\n"
      #endif
      #ifdef __IOS__
      "gl_PointSize = 3.0;\n"
      #endif
      "gl_Position = aVertexPosition;\n"
    "}\n";

  const char*  fShaderStr =
    "precision mediump float;\n"
    "varying vec4 vColor;\n"
    "void main(){\n"
      "gl_FragColor=vColor;\n"
    "}\n";

  auto vertexShader = loadShader(GL_VERTEX_SHADER, vShaderStr);
  auto fragmentShader = loadShader(GL_FRAGMENT_SHADER, fShaderStr);

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
  glPointSize(1.2f);
  glViewport(0,0,appScreenWidth,appScreenHeight);
  glOrtho(-0.5f, appScreenWidth - 0.5f, -0.5f, appScreenHeight - 0.5f, -1.0, 1.0);
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
  glViewport(startPointX,startPointY,viewPortWidth,viewPortHeight);
#endif
}

OpenGL::~OpenGL(){
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

void OpenGL::drawPixel(int x,int y,float red,float green,float blue){

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
      assert(glGetError()==GL_NO_ERROR);
      glEnableVertexAttribArray((GLuint)pointParamLocation);
      assert(glGetError()==GL_NO_ERROR);
  }
  {
      color[0] = red;
      color[1] = green;
      color[2] = blue;
      glVertexAttribPointer((GLuint)colorParamLocation,4,GL_FLOAT,GL_FALSE,0,color);
      assert(glGetError()==GL_NO_ERROR);
      glEnableVertexAttribArray((GLuint)colorParamLocation);
      assert(glGetError()==GL_NO_ERROR);
  }
  glDrawArrays(GL_POINTS,0,1);
  assert(glGetError()==GL_NO_ERROR);
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

void OpenGL::drawText(int x,int y,std::string text,float red,float green,float blue){
#if defined(__OPENGL__)
  glColor3f(red,green,blue);
  glRasterPos2i(x ,y);
  for (int i = 0; i < text.length(); i++)
  {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,text[i]);
  }
#elif defined(__GLES__)
    //TODO
#endif
}
