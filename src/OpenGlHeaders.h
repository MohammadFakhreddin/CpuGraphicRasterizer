#pragma once

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
/* Defined before OpenGL and GLUT includes to avoid deprecation messages */
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif