#define __PLATFORM_WIN__
#include "../src/Constants.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../src/libs/stb_image/open_gl_stb_image.h"

#include "../src/open_gl/OpenGl.h"
#include <memory>
#include <iostream>
#include <string>
#include "../src/application/Application.h"
#include <windows.h>

std::unique_ptr<Application> application;

unsigned int currentTime = 0;
unsigned int lastTime = glutGet(GLUT_ELAPSED_TIME);
unsigned int deltaTime = 0;

void mainLoop() {
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;
  	application->mainLoop(deltaTime);
}

void timer(int value)
{
	glutTimerFunc(16, timer, 0);
	glutPostRedisplay();
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
	glutInit(&argc,argv);
    glutInitDisplayMode ( GLUT_SINGLE | GLUT_RGB );
    unsigned int realScreenWidth = (int)GetSystemMetrics(SM_CXSCREEN);
    unsigned int realScreenHeight = (int)GetSystemMetrics(SM_CYSCREEN);
	unsigned int appScreenWidth = 800;
	unsigned int appScreenHeight = 600;
    glutInitWindowSize(appScreenWidth,appScreenHeight);
    glutInitWindowPosition(
            (int)(realScreenWidth/2 - appScreenWidth/2),
            (int)(realScreenHeight/2 - appScreenHeight/2));
    glutCreateWindow(Constants::Window::appName);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    application = std::unique_ptr<Application>(new Application(
			Application::Platform::Windows,
			appScreenWidth,
			appScreenHeight,
			realScreenWidth,
			realScreenHeight
    ));
    glutTimerFunc(0, timer, 0);
    glutDisplayFunc(mainLoop);
    glutMainLoop();
    return 0;
}