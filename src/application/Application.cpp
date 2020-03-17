#include "Application.h"
#include <memory>
#include "../Constants.h"
#include "../open_gl/OpenGl.h"
#include <vector>
#include "../3d_shape/Shape3d.h"
#include "../utils/log/Logger.h"
#include "../utils/math/Math.h"

Application::Application(
	Application::Platform platform,
	unsigned int paramAppScreenWidth,
	unsigned int paramAppScreenHeight,
	unsigned int physicalDeviceScreenWidth,
	unsigned int physicalDeviceScreenHeight
)
	:
	platform(platform),
	physicalScreenWidth(physicalDeviceScreenWidth),
	physicalScreenHeight(physicalDeviceScreenHeight),
	appScreenWidth(paramAppScreenWidth),
	appScreenHeight(paramAppScreenHeight),
	openGLInstance(paramAppScreenWidth,paramAppScreenHeight,physicalDeviceScreenWidth,physicalDeviceScreenHeight),
	cameraInstance(
		openGLInstance,
		cameraInitialZLocation,
		cameraInitialMaximumFov,
		0,
		appScreenWidth,
		0,
		appScreenHeight,
		appScreenWidth,
		appScreenHeight
	)
{
	instance = this;
	{//Shape
		auto width = appScreenWidth/5;
		Logger::log("Creating shape object");
		shape = Shape3d::generateTextured3DCube(
			dice.diceCubeTexture,
			dice.diceCubeEdgeList,
			width,
			width,
			width,
			float(appScreenWidth)/2.0f,
			float(appScreenHeight)/2.0f,
			float(width + 500),
			0,
			0,
			0,
			1
		);
		Logger::log("Creating shape was successful");
	}
}

void Application::notifyScreenSurfaceChanged(
	unsigned int appScreenWidth,
	unsigned int appScreenHeight,
	unsigned int physicalScreenWidth,
	unsigned int physicalScreenHeight,
	bool forceNewAppScreenWidthAndHeight
){
	Logger::log("Surface has changed");

	if(forceNewAppScreenWidthAndHeight==true){
		this->appScreenWidth = appScreenWidth;
		this->appScreenHeight = appScreenHeight;
		cameraInstance.notifyScreenSurfaceIsChanged(
			0,
			appScreenWidth,
			0,
			appScreenHeight,
			appScreenWidth,
			appScreenHeight
		);
	}

	this->physicalScreenWidth = physicalScreenWidth;
	this->physicalScreenHeight = physicalScreenHeight;

	openGLInstance.notifyScreenSurfaceChanged(
		appScreenWidth,
		appScreenHeight,
		physicalScreenWidth,
		physicalScreenHeight
	);
}



void Application::render(double deltaTime) {
	openGLInstance.clear();
	cameraInstance.render(deltaTime);
	{//FPSText
		openGLInstance.drawText(0,0,std::to_string(currentFps),1.0f,1.0f,1.0f);
	}
	// dice.diceCubeTexture->render();
	openGLInstance.flush();
}

void Application::update(double deltaTime) {
	if (keyEvents[leftButton]==true) {
		shape->transformX(float(-1.0f * Application::shapeTransformSpeed * deltaTime));
		keyEvents[leftButton] = false;
	}
	if (keyEvents[rightButton]==true) {
		shape->transformX(float(Application::shapeTransformSpeed * deltaTime));
		keyEvents[rightButton] = false;
	}
	if (keyEvents[forwardButton] == true) {
		shape->transformY(float(Application::shapeTransformSpeed * deltaTime));
		keyEvents[forwardButton] = false;
	}
	if (keyEvents[backwardButton] == true) {
		shape->transformY(float(-1 * Application::shapeTransformSpeed * deltaTime));
		keyEvents[backwardButton] = false;
	}
  	if (keyEvents[rotationZLeftButton] == true) {
		shape->rotateZ(float(Application::shapeRotationSpeed * deltaTime));
		keyEvents[rotationZLeftButton] = false;
	}
	if (keyEvents[rotationZRightButton]) {
		shape->rotateZ(float(-1.0f * Application::shapeRotationSpeed * deltaTime));
		keyEvents[rotationZRightButton] = false;
	}
  	if (keyEvents[rotationXLeftButton] == true) {
		shape->rotateX(float(Application::shapeRotationSpeed * deltaTime));
		keyEvents[rotationXLeftButton] = false;
	}
	if (keyEvents[rotationXRightButton]) {
		shape->rotateX(float(-1.0f * Application::shapeRotationSpeed * deltaTime));
		keyEvents[rotationXRightButton] = false;
	}
	if (keyEvents[rotationYLeftButton] == true) {
		shape->rotateY(float(Application::shapeRotationSpeed * deltaTime));
		keyEvents[rotationYLeftButton] = false;
	}
	if (keyEvents[rotationYRightButton]) {
		shape->rotateY(float(-1.0f * Application::shapeRotationSpeed * deltaTime));
		keyEvents[rotationYRightButton] = false;
	}
	if (keyEvents[zoomInButton]) {
		shape->scale(float(Application::shapeScaleSpeed * deltaTime));
		keyEvents[zoomInButton] = false;
	}
	if (keyEvents[zoomOutButton]) {
		shape->scale(float(-1.0f * Application::shapeScaleSpeed * deltaTime));
		keyEvents[zoomOutButton] = false;
	}
	if(keyEvents[forwardZButton]){
		shape->transformZ(float(Application::shapeTransformSpeed * deltaTime));
		keyEvents[forwardZButton] = false;
	}
	if(keyEvents[backwardZButton]){
		shape->transformZ(float(-1.0f * Application::shapeTransformSpeed * deltaTime));
		keyEvents[backwardZButton] = false;
	}
	{//Temporary code for auto rotation
		shape->rotateY(float(-1.0f * Application::shapeRotationSpeed * deltaTime * 0.1f));
		shape->rotateX(float(-1.0f * Application::shapeRotationSpeed * deltaTime * 0.1f));
		shape->rotateZ(float(-1.0f * Application::shapeRotationSpeed * deltaTime * 0.1f));
	}
	shape->update(deltaTime,cameraInstance);
}

void Application::notifyKeyIsPressed(Application::Buttons keyEvent)
{
	keyEvents[keyEvent] = true;
}

Application* Application::getInstance()
{
	return Application::instance;
}

void Application::mainLoop(double deltaTime){
	deltaTime = Math::max(deltaTime,1000.0);
	update(deltaTime);
	render(deltaTime);
	if(deltaTime>0){
		currentFps = 1000.0f/deltaTime;
	}
	assert(openGLInstance.checkForOpenGlError());
}

Application* Application::instance;

unsigned int Application::getAppScreenHeight(){
	return appScreenHeight;
}

unsigned int Application::getAppScreenWidth(){
	return appScreenWidth;
}

unsigned int Application::getPhysicalScreenWidth(){
	return physicalScreenWidth;
}

unsigned int Application::getPhysicalScreenHeight(){
	return physicalScreenHeight;
}