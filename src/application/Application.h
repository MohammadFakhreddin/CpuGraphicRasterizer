#ifndef Application_class
#define Application_class

#include <memory>
#include <unordered_map>
#include "../3d_shape/Shape3d.h"
#include "../fa_texture/FaTexture.h"

class Application {
public:
  struct DrawPixel{
    float zValue;
    float red;
    float green;
    float blue;
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
	static constexpr float cameraZLocation = 200.0f;
	static constexpr float maximumFov = 200.0f;
	static constexpr float shapeScaleSpeed = 0.1f;
	Application();
	void render(float deltaTime);
	void update(float deltaTime);
	void notifyKeyIsPressed(Application::Buttons);
	void putPixelInMap(int x,int y,float zValue,float red,float green,float blue);
	static Application* getInstance();
	std::vector<std::vector<DrawPixel>> pixelMap;
private:
	std::unique_ptr<Shape3d> shape;
	static Application* instance;
	std::unordered_map<Application::Buttons,bool> keyEvents;
	DrawPixel* currentPixel;
	FaTexture cubeTexture = FaTexture("src/assets/wood.png",1,1);
};

#endif