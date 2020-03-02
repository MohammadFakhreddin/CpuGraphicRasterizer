#include "Application.h"
#include <memory>
#include "../Constants.h"
#include "../OpenGlHeaders.h"
#include <vector>

Application* Application::instance;

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

Application::Application()
{
	instance = this;
	glutKeyboardFunc(handleKeyboardEvent);
	shape = Shape3d::generate3DCube(
		100,
		100,
		100,
		Constants::Window::screenWidth/2,
		Constants::Window::screenHeight/2,
		0,
		0,
		0,
		0,
		1
	);
	{//Creating pixelMap
		for(int i=0;i<Constants::Window::screenWidth;i++){
			std::vector<DrawPixel> innerMap;
			pixelMap.emplace_back(innerMap);
			for(int j=0;j<Constants::Window::screenHeight;j++){
				DrawPixel drawPixel;
				drawPixel.zValue = maximumFov;
				pixelMap.at(i).emplace_back(drawPixel);
			}
		}
	}
}

void Application::putPixelInMap(int x,int y,float zValue,float red,float green,float blue){
	//TODO Optimize and find better solution
	if(x<0||x>=Constants::Window::screenWidth||y<0||y>=Constants::Window::screenHeight){
		return;
	}
	assert(x<Constants::Window::screenWidth);
	assert(y<Constants::Window::screenHeight);
	currentPixel = &pixelMap.at(x).at(y);
	if(currentPixel->zValue>zValue){
		currentPixel->blue = blue;
		currentPixel->green = green;
		currentPixel->red = red;
		currentPixel->zValue = zValue;
	}
} 

void Application::render(float deltaTime) {
	glBegin(GL_POINTS);
	for(int i=0;i<Constants::Window::screenWidth;i++){
		for(int j=0;j<Constants::Window::screenHeight;j++){
			currentPixel = &pixelMap.at(i).at(j); 
			if(currentPixel->blue!=0 || currentPixel->green!=0 || currentPixel->red!=0){
				glColor3f(currentPixel->red,currentPixel->green,currentPixel->blue);
				glVertex2i(i,j);
				currentPixel->blue = 0;
				currentPixel->red = 0;
				currentPixel->green = 0;
				currentPixel->zValue = maximumFov;
			}
		}
	}
	glEnd();
}

void Application::update(float deltaTime) {
	if (keyEvents[leftButton]==true) {
		shape->transformX(-1 * Application::shapeTransformSpeed * deltaTime);
		keyEvents[leftButton] = false;
	}
	if (keyEvents[rightButton]==true) {
		shape->transformX(Application::shapeTransformSpeed * deltaTime);
		keyEvents[rightButton] = false;
	}
	if (keyEvents[forwardButton] == true) {
		shape->transformY(Application::shapeTransformSpeed * deltaTime);
		keyEvents[forwardButton] = false;
	}
	if (keyEvents[backwardButton] == true) {
		shape->transformY(-1 * Application::shapeTransformSpeed * deltaTime);
		keyEvents[backwardButton] = false;
	}
  //TODO Add transformZ (It must be like scale factor)
	if (keyEvents[rotationZLeftButton] == true) {
		shape->rotateZ(Application::shapeRotationSpeed * deltaTime);
		keyEvents[rotationZLeftButton] = false;
	}
	if (keyEvents[rotationZRightButton]) {
		shape->rotateZ(-1* Application::shapeRotationSpeed * deltaTime);
		keyEvents[rotationZRightButton] = false;
	}
  if (keyEvents[rotationXLeftButton] == true) {
		shape->rotateX(Application::shapeRotationSpeed * deltaTime);
		keyEvents[rotationXLeftButton] = false;
	}
	if (keyEvents[rotationXRightButton]) {
		shape->rotateX(-1* Application::shapeRotationSpeed * deltaTime);
		keyEvents[rotationXRightButton] = false;
	}
  if (keyEvents[rotationYLeftButton] == true) {
		shape->rotateY(Application::shapeRotationSpeed * deltaTime);
		keyEvents[rotationYLeftButton] = false;
	}
	if (keyEvents[rotationYRightButton]) {
		shape->rotateY(-1* Application::shapeRotationSpeed * deltaTime);
		keyEvents[rotationYRightButton] = false;
	}
	if (keyEvents[zoomInButton]) {
		shape->scale(Application::shapeScaleSpeed * deltaTime);
		keyEvents[zoomInButton] = false;
	}
	if (keyEvents[zoomOutButton]) {
		shape->scale(-1 * Application::shapeScaleSpeed * deltaTime);
		keyEvents[zoomOutButton] = false;
	}
	if(keyEvents[forwardZButton]){
		shape->transformZ(Application::shapeTransformSpeed * deltaTime);
		keyEvents[forwardZButton] = false;
	}
	if(keyEvents[backwardZButton]){
		shape->transformZ(-1 * Application::shapeTransformSpeed * deltaTime);
		keyEvents[backwardZButton] = false;
	}
	shape->update(deltaTime);
}

void Application::notifyKeyIsPressed(Application::Buttons keyEvent)
{
	keyEvents[keyEvent] = true;
}

Application* Application::getInstance()
{
	return Application::instance;
}
