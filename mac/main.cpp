#define STB_IMAGE_IMPLEMENTATION
#include "../src/libs/stb_image/open_gl_stb_image.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "../src/libs/tiny_obj_loader/tiny_obj_loader.h"

#include <memory>
#include <thread>
#include <chrono>

#include <CoreGraphics/CGDisplayConfiguration.h>
#include "../src/application/Application.h"
#include "../src/open_gl/OpenGl.h"
#include "../src/utils/log/Logger.h"

const double loopTime = 1.0 / 60.0;

std::unique_ptr<Application> application;

double currentTime = 0.0;
double lastTime = 0.0;
double deltaTime = 0.0;
double sleepTime = 0.0;

void mainLoop() {
    currentTime = glfwGetTime();
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;
    application->mainLoop(deltaTime);
	sleepTime = loopTime - deltaTime;
	if(sleepTime>0.0){
		std::this_thread::sleep_for(std::chrono::nanoseconds(int(1000 * sleepTime)));
	}
}

void errorCallback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}


/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
	if (!glfwInit())
	{
		Logger::log("Initing glfw failed");
		exit(EXIT_FAILURE);
		return -1;
	}

  	glfwSetErrorCallback(errorCallback);

	auto mainDisplayId = CGMainDisplayID();
	unsigned int realScreenWidth = CGDisplayPixelsWide(mainDisplayId);
	unsigned int realScreenHeight = CGDisplayPixelsHigh(mainDisplayId);
	unsigned int appScreenWidth = 800;
	unsigned int appScreenHeight = 600;
	
	GLFWwindow* window = glfwCreateWindow(appScreenWidth, appScreenHeight, Constants::Window::appName, NULL, NULL);
	if (!window)
	{
		Logger::log("Creating window failed");
		glfwTerminate();
        exit(EXIT_FAILURE);
		return -1;
	}
	glfwSetWindowPos(window,(int)(realScreenWidth/2 - appScreenWidth/2),(int)(realScreenHeight/2 - appScreenHeight/2));
	glfwMakeContextCurrent(window);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
	application = std::make_unique<Application>(
		window,
		Constants::Platform::Mac,
		appScreenWidth,
		appScreenHeight,
		realScreenWidth,
		realScreenHeight
	);

	while (!glfwWindowShouldClose(window))
    {
		mainLoop();
		glfwSwapBuffers(window);
        glfwPollEvents();
	}
	
	glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
	
	return 0;
}