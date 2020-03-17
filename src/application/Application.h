#ifndef Application_class
#define Application_class

#include <memory>
#include <unordered_map>

#include "../3d_shape/edge/texture_edge/TextureEdge.h"
#include "./../3d_models/Wood.h"
#include "./../3d_models/Sky.h"
#include "./../3d_models/Dice.h"
#include "../3d_shape/Shape3d.h"
#include "../open_gl/OpenGl.h"
#include "../camera/Camera.h"

class Application {
public:
	enum Platform{
		Windows,
		Mac,
		Iphone,
		Android
	};
	enum Buttons
	{
		leftButton,
		rightButton,
		forwardButton,
		backwardButton,
		forwardZButton,
		backwardZButton,
		rotationZRightButton,
		rotationZLeftButton,
		rotationXRightButton,
		rotationXLeftButton,
		rotationYRightButton,
		rotationYLeftButton,
		zoomInButton,
		zoomOutButton
	};

	static constexpr float shapeTransformSpeed = 1.0f;

	static constexpr float shapeRotationSpeed = 0.01f;
	
	static constexpr float cameraInitialZLocation = 0.0f;
	
	static constexpr float cameraInitialMaximumFov = 1000.0f;
	
	static constexpr float shapeScaleSpeed = 0.1f;
	
	Application(
		Application::Platform platform,
		unsigned int appScreenWidth,
		unsigned int appScreenHeight,
		unsigned int physicalDeviceScreenWidth,
		unsigned int physicalDeviceScreenHeight
	);
	/**
	 * Currently this event is only for android
	*/
	void notifyScreenSurfaceChanged(
		unsigned int appScreenWidth,
		unsigned int appScreenHeight,
		unsigned int physicalDeviceScreenWidth,
		unsigned int physicalDeviceScreenHeight,
		bool forceNewAppScreenData
	);
	void mainLoop(double deltaTime);
	void notifyKeyIsPressed(Application::Buttons);
	void putPixelInMap(int x,int y,float zValue,float red,float green,float blue);
	
	static Application* getInstance();

	static Application* instance;

	unsigned int getAppScreenWidth();
	unsigned int getAppScreenHeight();
	unsigned int getPhysicalScreenWidth();
	unsigned int getPhysicalScreenHeight();

	Vec3DFloat& getCameraLocation();

private:

	void render(double deltaTime);
	void update(double deltaTime);
	
	double currentFps = 0;

	OpenGL openGLInstance;
	Camera cameraInstance;

	unsigned int physicalScreenWidth;
	unsigned int physicalScreenHeight;
	unsigned int appScreenWidth;
	unsigned int appScreenHeight;

	Application::Platform platform;

	std::unique_ptr<Shape3d> shape;

	std::unordered_map<Application::Buttons,bool> keyEvents;

	Wood wood;
	Sky sky;
	Dice dice;	
};

#endif
