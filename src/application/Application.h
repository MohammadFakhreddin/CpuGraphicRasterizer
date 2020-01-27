#pragma once

#include <memory>
#include <unordered_map>
// #include "../3d_shape/Shape3d.h"

class Application {
public:
	enum Buttons
	{
		leftButton,
		rightButton,
		forwardButton,
		backwardButton,
		rotationZRightButton,
		rotationZLeftButton,
		rotationXRightButton,
		rotationXLeftButton,
		rotationYRightButton,
		rotationYLeftButton,
		zoomInButton,
		zoomOutButton
	};
	static constexpr float shapeTransformSpeed = 10.0f;
	static constexpr float shapeRotationSpeed = 0.1f;
	static constexpr float shapeScaleSpeed = 0.01f;
	Application();
	void render();
	void update();
	void notifyKeyIsPressed(Application::Buttons);
	static Application* getInstance();
private:
	// std::unique_ptr<Shape3d> shape;
	static Application* instance;
	std::unordered_map<Application::Buttons,bool> keyEvents;
};