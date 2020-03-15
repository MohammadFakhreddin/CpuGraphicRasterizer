#ifndef OpenGl_class
#define OpenGl_class

#include "../Constants.h"
#include <string>

#if defined(__PLATFORM_MAC__) 
  #define GL_SILENCE_DEPRECATION
  /* Defined before OpenGL and GLUT includes to avoid deprecation messages */
  #define __OPENGL__
  #include <GLUT/glut.h>
#elif defined(__IOS__)
  #define __GLES__
  #include <GLKit/GLKit.h>
#elif defined(__ANDROID__)
  #define __GLES__
  #include <GLES2/gl2.h>
#elif defined(__PLATFORM_WIN__)
	#define __OPENGL__
	#include <GL/freeglut.h>
#elif defined(__PLATFORM_LINUX__)
  #define __OPENGL__
  #include <GL/freeglut.h>
#endif

class OpenGL{
public:
  OpenGL(unsigned int appScreenWidth,unsigned int appScreenHeight,unsigned int physicalScreenWidth,unsigned int physicalScreenHeight);
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
  GLfloat projMat[16];
  void glesOrtho(float left, float right, float top, float bottom, float near, float far);
#endif // GLES
private:
  GLuint programObject;
  GLint colorParamLocation = 0;
  GLint pointParamLocation = 0;
  unsigned int appScreenWidth = 0;
  unsigned int appScreenHeight = 0;
  unsigned int physicalScreenWidth = 0;
  unsigned int physicalScreenHeight = 0;
  #if defined(__GLES__)
  int viewPortWidth = 0;
  int viewPortHeight = 0;
  int viewPortStartX = 0;
  int viewPortStartY = 0;
  float xDifValue = 0;
  float yDifValue = 0;
  float projectionX = 0;
  float projectionY = 0;
  #endif
};

#endif
