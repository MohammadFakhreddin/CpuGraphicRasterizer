#define STB_IMAGE_IMPLEMENTATION
#include "../src/libs/stb_image/open_gl_stb_image.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "../src/libs/tiny_obj_loader/tiny_obj_loader.h"

#include <thread>
#include <chrono>
#include <CoreGraphics/CGDisplayConfiguration.h>

#include "../src/application/Application.h"
#include "../src/open_gl/OpenGl.h"
#include "../src/utils/log/Logger.h"

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

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

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
	
	glfwMakeContextCurrent(window);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		Logger::log("Failed to initialize OpenGL context");
		return -1;
	}

	glfwSetWindowPos(
		window, 
		(int)(realScreenWidth / 2.0f - appScreenWidth / 2.0f), 
		(int)(realScreenHeight / 2.0f - appScreenHeight / 2.0f)
	);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque

	Application* application = new Application(
		window,
		Constants::Platform::Mac,
		appScreenWidth,
		appScreenHeight,
		realScreenWidth,
		realScreenHeight
	);

	application->run();

	delete application;
	
	glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
	
	return 0;
}