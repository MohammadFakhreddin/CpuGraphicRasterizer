#ifndef OpenGl_class
#define OpenGl_class

#include <string>

#if defined(__APPLE__) 
#define GL_SILENCE_DEPRECATION
  /* Defined before OpenGL and GLUT includes to avoid deprecation messages */
  #define __OPENGL__
  #include <GLUT/glut.h>
#elif defined(__ANDROID__)
  #define __GLES__
  #include <GLES2/gl2.h>
#endif

class OpenGL{
  public:
    static void drawPixel(int x,int y,float red,float green,float blue);
    static void drawText(int x,int y,std::string text,float red,float green,float blue);
    static double getElapsedTime();
    static void begin();
    static void end();
};

#endif
