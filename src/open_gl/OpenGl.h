#ifndef OpenGl_class
#define OpenGl_class

#include "../Constants.h"
#include <string>

#if defined(__APPLE__) 
  #define GL_SILENCE_DEPRECATION
  /* Defined before OpenGL and GLUT includes to avoid deprecation messages */
  #define __OPENGL__
  #include <GLUT/glut.h>
#elif defined(__ANDROID__)
  #define __GLES__
  #include <GLES2/gl2.h>
#elif defined(__PLATFORM_WIN__)
	#define __OPENGL__
	#include <GL/freeglut.h>
#endif
// #include <GLES3/gl3.h>
// #define __GLES__

class OpenGL{
public:
  OpenGL(unsigned int appScreenWidth,unsigned int appScreenHeight);
  ~OpenGL();
  void drawPixel(int x,int y,float red,float green,float blue);
  void drawText(int x,int y,std::string text,float red,float green,float blue);
  void clear();
  void flush();
  void beginDrawingPoints();
  void resetProgram();
#ifdef __GLES__
  GLuint loadShader(GLenum shaderType, const char* shaderSource);
  GLuint createProgram(const char* vertexSource, const char * fragmentSource);
  GLfloat color[4] = {0,0,0,1.0f};
  GLfloat position[4] = {0,0,0.0f,1.0f};

#endif // GLES
private:
  GLuint programObject;
  GLint colorParamLocation = 0;
  GLint pointParamLocation = 0;
  unsigned int appScreenWidth = 0;
  unsigned int appScreenHeight = 0;
};

#endif
