#include "Application.h"
#include <memory>
#include "../Constants.h"
#include "../open_gl/OpenGl.h"
#include <vector>
#include "../3d_shape/Shape3d.h"
#include "../utils/log/Logger.h"
#include "../utils/math/Math.h"

void handleKeyboardEvent(unsigned char key, int x, int y)
{
	if (key == 'a' || key == 'A') {
		Application::getInstance()->notifyKeyIsPressed(Application::Buttons::keyA);
	}
	if (key == 'd' || key == 'D') {
		Application::getInstance()->notifyKeyIsPressed(Application::Buttons::keyD);
	}
	if (key == 'w' || key == 'W') {
		Application::getInstance()->notifyKeyIsPressed(Application::Buttons::keyW);
	}
	if (key == 's' || key == 'S') {
		Application::getInstance()->notifyKeyIsPressed(Application::Buttons::keyS);
	}
	if (key == 'e' || key == 'E') {
		Application::getInstance()->notifyKeyIsPressed(Application::Buttons::keyE);
	}
	if (key == 'q' || key == 'Q') {
		Application::getInstance()->notifyKeyIsPressed(Application::Buttons::keyQ);
	}
	if (key == 'r' || key == 'R') {
		Application::getInstance()->notifyKeyIsPressed(Application::Buttons::keyR);
	}
	if (key == 't' || key == 'T') {
		Application::getInstance()->notifyKeyIsPressed(Application::Buttons::keyT);
	}
	if (key == 'f' || key == 'F') {
		Application::getInstance()->notifyKeyIsPressed(Application::Buttons::keyF);
	}
	if (key == 'g' || key == 'G') {
		Application::getInstance()->notifyKeyIsPressed(Application::Buttons::keyG);
	}
	if (key == 'x' || key == 'X') {
		Application::getInstance()->notifyKeyIsPressed(Application::Buttons::keyX);
	}
	if (key == 'c' || key == 'C') {
		Application::getInstance()->notifyKeyIsPressed(Application::Buttons::keyC);
	}
	if(key == 'v' || key == 'V'){
		Application::getInstance()->notifyKeyIsPressed(Application::Buttons::keyV);
	}
	if(key == 'b' || key == 'B'){
		Application::getInstance()->notifyKeyIsPressed(Application::Buttons::keyB);
	}
}

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
	light(paramAppScreenWidth/2,paramAppScreenHeight/2,cameraInitialZLocation + (cameraInitialMaximumFov/2)),
	openGLInstance(paramAppScreenWidth,paramAppScreenHeight,physicalDeviceScreenWidth,physicalDeviceScreenHeight),
	cameraInstance(
		openGLInstance,
		light,
		cameraInitialZLocation,
		cameraInitialMaximumFov,
		0,
		paramAppScreenWidth,
		0,
		paramAppScreenHeight
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
#ifdef __DESKTOP__
	glutKeyboardFunc(handleKeyboardEvent);
#endif
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
	{//We rotate light by keyboard
		if(keyEvents[Buttons::keyA]){
			cameraInstance.getLight().transformX(
				deltaTime * Application::lightTransformSpeed * -1
			);
			keyEvents[Buttons::keyA] = false;
		}
		if(keyEvents[Buttons::keyD]){
			cameraInstance.getLight().transformX(
				deltaTime * Application::lightTransformSpeed * 1
			);
			keyEvents[Buttons::keyD] = false;
		}
		if(keyEvents[Buttons::keyW]){
			cameraInstance.getLight().transformY(
				deltaTime *  Application::lightTransformSpeed * 1
			);
			keyEvents[Buttons::keyW] = false;
		}
		if(keyEvents[Buttons::keyS]){
			cameraInstance.getLight().transformY(
				deltaTime *  Application::lightTransformSpeed * -1
			);
			keyEvents[Buttons::keyS] = false;
		}
		if(keyEvents[Buttons::keyC]){
			cameraInstance.getLight().transformZ(
				deltaTime *  Application::lightTransformSpeed * -1 * 0.5
			);
			keyEvents[Buttons::keyC] = false;
		}
		if(keyEvents[Buttons::keyV]){
			cameraInstance.getLight().transformZ(
				deltaTime *  Application::lightTransformSpeed * 1 * 0.5
			);
			keyEvents[Buttons::keyV] = false;	
		}
	}
	{//Temporary code for auto rotation
		shape->rotateY(float(-1.0f * Application::shapeRotationSpeed * deltaTime * 0.1f));
		shape->rotateX(float(-1.0f * Application::shapeRotationSpeed * deltaTime * 0.1f));
		shape->rotateZ(float(-1.0f * Application::shapeRotationSpeed * deltaTime * 0.1f));
	}
	shape->update(deltaTime,cameraInstance);
	cameraInstance.update(deltaTime);
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