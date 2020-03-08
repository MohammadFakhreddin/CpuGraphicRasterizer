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

void OpenGL::clear(){
  glClear( GL_COLOR_BUFFER_BIT);
}

void OpenGL::flush(){
  glFlush();
}

void OpenGL::drawPixel(int x,int y,float red,float green,float blue){
  glColor3f(red,green,blue);
  glVertex2i(x,y);
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

void OpenGL::beginDrawingPoints(){
  glBegin(GL_POINTS);
}

void OpenGL::end(){
  glEnd();
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