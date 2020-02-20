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
}

void Application::render() {
  shape->render();     
}

void Application::update() {
	if (keyEvents[leftButton]==true) {
		shape->transformX(-1*Application::shapeTransformSpeed);
		keyEvents[leftButton] = false;
	}
	if (keyEvents[rightButton]==true) {
		shape->transformX(Application::shapeTransformSpeed);
		keyEvents[rightButton] = false;
	}
	if (keyEvents[forwardButton] == true) {
		shape->transformY(Application::shapeTransformSpeed);
		keyEvents[forwardButton] = false;
	}
	if (keyEvents[backwardButton] == true) {
		shape->transformY(-1 * Application::shapeTransformSpeed);
		keyEvents[backwardButton] = false;
	}
  //TODO Add transformZ (It must be like scale factor)
	if (keyEvents[rotationZLeftButton] == true) {
		shape->rotateZ(Application::shapeRotationSpeed);
		keyEvents[rotationZLeftButton] = false;
	}
	if (keyEvents[rotationZRightButton]) {
		shape->rotateZ(-1* Application::shapeRotationSpeed);
		keyEvents[rotationZRightButton] = false;
	}
  if (keyEvents[rotationXLeftButton] == true) {
		shape->rotateX(Application::shapeRotationSpeed);
		keyEvents[rotationXLeftButton] = false;
	}
	if (keyEvents[rotationXRightButton]) {
		shape->rotateX(-1* Application::shapeRotationSpeed);
		keyEvents[rotationXRightButton] = false;
	}
  if (keyEvents[rotationYLeftButton] == true) {
		shape->rotateY(Application::shapeRotationSpeed);
		keyEvents[rotationYLeftButton] = false;
	}
	if (keyEvents[rotationYRightButton]) {
		shape->rotateY(-1* Application::shapeRotationSpeed);
		keyEvents[rotationYRightButton] = false;
	}
	if (keyEvents[zoomInButton]) {
		shape->scale(Application::shapeScaleSpeed);
		keyEvents[zoomInButton] = false;
	}
	if (keyEvents[zoomOutButton]) {
		shape->scale(-1 * Application::shapeScaleSpeed);
		keyEvents[zoomOutButton] = false;
	}
	if(keyEvents[forwardZButton]){
		shape->transformZ(Application::shapeTransformSpeed);
		keyEvents[forwardZButton] = false;
	}
	if(keyEvents[backwardZButton]){
		shape->transformZ(-1 * Application::shapeTransformSpeed);
		keyEvents[backwardZButton] = false;
	}
	shape->update();
}

void Application::notifyKeyIsPressed(Application::Buttons keyEvent)
{
	keyEvents[keyEvent] = true;
}

Application* Application::getInstance()
{
	return Application::instance;
}
