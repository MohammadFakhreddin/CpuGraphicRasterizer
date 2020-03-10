#define STB_IMAGE_IMPLEMENTATION
#include "../src/fa_texture/stb_image_headers.h"

#include "../src/Constants.h"
#include "../src/open_gl/OpenGl.h"
#include <memory>
#include <iostream>
#include <string>
#include <CoreGraphics/CGDisplayConfiguration.h>
#include "../src/application/Application.h"

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
	auto mainDisplayId = CGMainDisplayID();
  unsigned int realScreenWidth = CGDisplayPixelsWide(mainDisplayId);
  unsigned int realScreenHeight = CGDisplayPixelsHigh(mainDisplayId);
  glutInitWindowSize(Constants::Window::screenWidth,Constants::Window::screenHeight);
  glutInitWindowPosition(
          (int)(realScreenWidth/2 - Constants::Window::screenWidth/2),
          (int)(realScreenHeight/2 - Constants::Window::screenHeight/2));
  glutCreateWindow(Constants::Window::appName);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
  // glViewport(0,0,Constants::Window::screenWidth,Constants::Window::screenHeight);
  // glOrtho(-0.5f, Constants::Window::screenWidth-0.5f, -0.5f, Constants::Window::screenHeight-0.5f, -1.0, 1.0);
  application = std::unique_ptr<Application>(new Application(
      Application::Platform::Mac,
      realScreenWidth,
      realScreenHeight
  ));
  glutKeyboardFunc(handleKeyboardEvent);
  glutTimerFunc(0, timer, 0);
  glutDisplayFunc(mainLoop);
  glutMainLoop();
  return 0;
}