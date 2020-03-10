#include "./OpenGl.h"
#include <iostream>
#include "./../Constants.h"

OpenGL::OpenGL(){

#ifdef __GLES__
  int nrAttributes;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
  std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

  const char* vShaderStr = 
    "uniform vec4 aVertexPosition;\n"
    "uniform vec4 aVertexColor;\n"
    "varying vec4 vColor;\n"
    "void main(){\n"
      "vColor=aVertexColor;\n"
      "gl_PointSize = 100.0;\n"
      "gl_Position = aVertexPosition;\n"
    "}\n";

  const char*  fShaderStr =
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
      glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
      std::cout<<"Error linking program:\n"<<infoLog<<std::endl;
      free(infoLog);
    }
    glDeleteProgram(programObject);
  }
  assert(linked);

  // delete the shaders as they're linked into our program now and no longer necessery
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader); 

  pointParamLocation = glGetUniformLocation(programObject,"aVertexPosition");
  assert(pointParamLocation>=0);

  colorParamLocation = glGetUniformLocation(programObject,"aVertexColor");
  assert(colorParamLocation>=0);
#endif

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
  glViewport(0,0,Constants::Window::screenWidth,Constants::Window::screenHeight);
#ifdef __OPENGL__
  glOrtho(-0.5f, Constants::Window::screenWidth-0.5f, -0.5f, Constants::Window::screenHeight-0.5f, -1.0, 1.0);
#endif 

}

OpenGL::~OpenGL(){
}

#ifdef GLES

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
          std::cout<<"Could not Compile Shader "<<shaderType<<"  Buffer:"<<buf<<std::endl;
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
          std::cout<<"Could not link program:\n"<<buf<<std::endl;
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
  glClear( GL_COLOR_BUFFER_BIT);
}

void OpenGL::flush(){
  glFlush();
}

void OpenGL::drawPixel(int x,int y,float red,float green,float blue){
#ifdef __OPENGL__
  glColor3f(red,green,blue);
  glVertex2i(x,y);
#else
  {
    glEnableVertexAttribArray(pointParamLocation);
    glUniform4f(pointParamLocation,x,y,0,1);
  }
  {
    glEnableVertexAttribArray(colorParamLocation);
    glUniform4f(colorParamLocation,red,green,blue,1.0f);
  }
  glDrawArrays(GL_POINTS,0,1);
  glDisableVertexAttribArray(pointParamLocation);
  glDisableVertexAttribArray(colorParamLocation);
#endif
}

void OpenGL::beginDrawingPoints(){
#if defined(__GLES__)
  glUseProgram(programObject);
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
  glRasterPos2i(x,y);
  for (int i = 0; i < text.length(); i++)
  {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,text[i]);
  }
#elif defined(__GLES__)
    //TODO
#endif
}