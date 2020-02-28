#include "Constants.h"
#include "utils/screen/ScreenSize.h"
#include "OpenGlHeaders.h"
#include <memory>
#include "application/Application.h"
#include <iostream>
#include "data_types/MatrixTemplate.h"
#include <string>

std::unique_ptr<Application> application;
double lastTime = glutGet(GLUT_ELAPSED_TIME);
double currentTime = 0;
double targetFPS = 60;                    
double timePerFrame = 1000 / targetFPS;
double currentFps = 0;
std::string fpsText = "";

void update(){
	application->update();
}

void render(){
    glClear( GL_COLOR_BUFFER_BIT);
    application->render();
    {//FPSText
        glColor3f(1,1,1);
        glRasterPos2i(0,0);
        fpsText = std::to_string(currentFps);
        int len = fpsText.length();
        for (int i = 0; i < len; i++)
        {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,fpsText[i]);
        }
    }
    glFlush();
}

void mainLoop() {
    currentTime = glutGet(GLUT_ELAPSED_TIME);
    
    update();
    render();

    currentFps = 1000/(currentTime-lastTime);
    lastTime = currentTime;
}

void timer(int value)
{
	glutTimerFunc(16, timer, 0);
	glutPostRedisplay();
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
	glutInit(&argc,argv);
    glutInitDisplayMode ( GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
#ifdef __DESKTOP__
    unsigned int realScreenHeight = 0;
	unsigned int realScreenWidth = 0;
    ScreenSize::getScreenResolution(
		realScreenWidth,
		realScreenHeight
	);
    glutInitWindowSize(Constants::Window::screenWidth,Constants::Window::screenHeight);
    glutInitWindowPosition(
            (int)(realScreenWidth/2 - Constants::Window::screenWidth/2),
            (int)(realScreenHeight/2 - Constants::Window::screenHeight/2));
#endif
    glutCreateWindow(Constants::Window::appName);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glViewport(0,0,Constants::Window::screenWidth,Constants::Window::screenHeight);
    glOrtho(0.0, Constants::Window::screenWidth, 0.0, Constants::Window::screenHeight, -1.0, 1.0);
    application = std::unique_ptr<Application>(new Application());
    glutTimerFunc(0, timer, 0);
	glutDisplayFunc(mainLoop);
    glutMainLoop();
    return 0;
}