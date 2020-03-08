#include "./OpenGl.h"
#include <iostream>

#if defined(__OPENGL__)
  GLint64 lastTime = glutGet(GLUT_ELAPSED_TIME);
#elif defined(__GLES__)
  GLint64 lastTime = 0;
  //TODO
  //glGetInteger64i_v(GL_Times, &lastTime);
#endif

GLint64 currentTime = 0;

void OpenGL::begin(){
  // #ifdef __OPENGL__
    glClear( GL_COLOR_BUFFER_BIT);

  // #else
  //   glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  // #endif
}

void OpenGL::end(){
  glFlush();
}

void OpenGL::drawPixel(int x,int y,float red,float green,float blue){
  // #ifdef __OPENGL__
    glColor3f(red,green,blue);
    glVertex2i(x,y);
  // #else
  //   glColorMask(red,green,blue,1);
  //   glUniform2i(0,x,y);
  //   glDrawArrays(GL_POINTS, 0, 1);
  // #endif
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

double OpenGL::getElapsedTime(){
  lastTime = currentTime;
  #if defined(__OPENGL__)
    currentTime = glutGet(GLUT_ELAPSED_TIME);
  #elif defined(__GLES__)
  //TODO
    currentTime = 0;
  #endif
  return double(currentTime-lastTime);
}