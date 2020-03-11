#define __PLATFORM_WIN__
#include "../src/Constants.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../src/fa_texture/stb_image_headers.h"

#include "../src/open_gl/OpenGl.h"
#include <memory>
#include <iostream>
#include <string>
#include "../src/application/Application.h"
#include <windows.h>

std::unique_ptr<Application> application;

void handleKeyboardEvent(unsigned char key, int x, int y)
{
	if (!Application::getInstance()) {
		return;
	}
	if (key == 'a' || key == 'A') {
		Application::getInstance()->notifyKeyIsPressed(Application::Buttons::leftButton);
	}
	if (key == 'd' || key == 'D') {
		Application::getInstance()->notifyKeyIsPressed(Application::Buttons::rightButton);
	}
	if (key == 'w' || key == 'W') {
		Application::getInstance()->notifyKeyIsPressed(Application::Buttons::forwardButton);
	}
	if (key == 's' || key == 'S') {
		Application::getInstance()->notifyKeyIsPressed(Application::Buttons::backwardButton);
	}
	if (key == 'e' || key == 'E') {
		Application::getInstance()->notifyKeyIsPressed(Application::Buttons::rotationZRightButton);
	}
	if (key == 'q' || key == 'Q') {
		Application::getInstance()->notifyKeyIsPressed(Application::Buttons::rotationZLeftButton);
	}
	if (key == 'r' || key == 'R') {
		Application::getInstance()->notifyKeyIsPressed(Application::Buttons::rotationXRightButton);
	}
	if (key == 't' || key == 'T') {
		Application::getInstance()->notifyKeyIsPressed(Application::Buttons::rotationXLeftButton);
	}
	if (key == 'f' || key == 'F') {
		Application::getInstance()->notifyKeyIsPressed(Application::Buttons::rotationYRightButton);
	}
	if (key == 'g' || key == 'G') {
		Application::getInstance()->notifyKeyIsPressed(Application::Buttons::rotationYLeftButton);
	}
	if (key == 'x' || key == 'X') {
		Application::getInstance()->notifyKeyIsPressed(Application::Buttons::zoomInButton);
	}
	if (key == 'c' || key == 'C') {
		Application::getInstance()->notifyKeyIsPressed(Application::Buttons::zoomOutButton);
	}
	if(key == 'v' || key == 'V'){
		Application::getInstance()->notifyKeyIsPressed(Application::Buttons::forwardZButton);
	}
	if(key == 'b' || key == 'B'){
		Application::getInstance()->notifyKeyIsPressed(Application::Buttons::backwardZButton);
	}
}

double currentTime = 0;
double lastTime = glutGet(GLUT_ELAPSED_TIME);
double deltaTime = 0;

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
    glutKeyboardFunc(handleKeyboardEvent);
    glutTimerFunc(0, timer, 0);
    glutDisplayFunc(mainLoop);
    glutMainLoop();
    return 0;
}