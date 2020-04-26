#define STB_IMAGE_IMPLEMENTATION
#include "../src/libs/stb_image/open_gl_stb_image.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "../src/libs/tiny_obj_loader/tiny_obj_loader.h"

#include "../src/open_gl/OpenGl.h"
#include <memory>
#include <CoreGraphics/CGDisplayConfiguration.h>
#include "../src/application/Application.h"

std::unique_ptr<Application> application;

int currentTime = 0;
int lastTime = 0;
int deltaTime = 0;

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
	unsigned int appScreenWidth = 800;
	unsigned int appScreenHeight = 600;
	
	glutInitWindowSize(appScreenWidth,appScreenHeight);
	glutInitWindowPosition(
			(int)(realScreenWidth/2 - appScreenWidth/2),
			(int)(realScreenHeight/2 - appScreenHeight/2));
	glutCreateWindow(Constants::Window::appName);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
	application = std::make_unique<Application>(
		Constants::Platform::Mac,
		appScreenWidth,
		appScreenHeight,
		realScreenWidth,
		realScreenHeight
	);
	glutTimerFunc(0, timer, 0);
	glutDisplayFunc(mainLoop);
	glutMainLoop();
	return 0;
}