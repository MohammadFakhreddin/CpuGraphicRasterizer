#ifndef OpenGl_class
#define OpenGl_class

#include <string>

#include "../Constants.h"
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
  /**
   * 
   * Called when orientation is changed or app re-gaines focus
   * 
  */
  void notifyScreenSurfaceChanged(
    unsigned int appScreenWidth,
    unsigned int appScreenHeight,
    unsigned int physicalScreenWidth,
    unsigned int physicalScreenHeight
  );
  void init();
  void drawPixel(const unsigned int& x,const unsigned int& y,const float& red,const float& green,const float& blue);
  void drawText(const unsigned int& x,const unsigned int& y,const std::string& text,const float& red,const float& green,const float& blue);
  void clear();
  void flush();
  void beginDrawingPoints();
  void resetProgram();
  //We use this method for both assertion in debug and printing existing errors
  bool checkForOpenGlError();
#ifdef __GLES__
  GLuint loadShader(GLenum shaderType, const char* shaderSource);
  GLuint createProgram(const char* vertexSource, const char * fragmentSource);
  GLfloat color[4] = {0,0,0,1.0f};
  GLfloat position[4] = {0,0,0.0f,1.0f};
#endif // GLES
private:
  unsigned int appScreenWidth = 0;
  unsigned int appScreenHeight = 0;
  unsigned int physicalScreenWidth = 0;
  unsigned int physicalScreenHeight = 0;
#if defined(__GLES__)
  int viewPortWidth = 0;
  int viewPortHeight = 0;
  int viewPortStartX = 0;
  int viewPortStartY = 0;
  GLuint programObject = 0;
  GLint pointParamLocation = -1;
  GLint colorParamLocation = -1;
#endif
};

#endif
