#define STB_IMAGE_IMPLEMENTATION
#include "../src/libs/stb_image/open_gl_stb_image.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "../src/libs/tiny_obj_loader/tiny_obj_loader.h"

#include <memory>
#include <iostream>
#include <string>
#include <X11/Xlib.h>

#include "../src/Constants.h"
#include "../src/open_gl/OpenGl.h"
#include "../src/application/Application.h"

std::unique_ptr<Application> application = nullptr;

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

  //Get screen width and height
  Display* display = XOpenDisplay(NULL);
  Screen*  screen = DefaultScreenOfDisplay(display);
  unsigned int realScreenWidth = (int)screen->width;
  unsigned int realScreenHeight = (int)screen->height;
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

	glfwSetWindowPos(
		window,
		(int)(realScreenWidth / 2.0f - appScreenWidth / 2.0f),
		(int)(realScreenHeight / 2.0f - appScreenHeight / 2.0f)
	);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		Logger::log("Failed to initialize OpenGL context");
		return -1;
	}
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
	
	application = std::make_unique<Application>(
		window,
		Constants::Platform::Windows,
		appScreenWidth,
		appScreenHeight,
		realScreenWidth,
		realScreenHeight
	);

	application->run();

	glfwDestroyWindow(window);
	
	glfwTerminate();
	
	exit(EXIT_SUCCESS);

	return 0;

}