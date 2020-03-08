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

class Application {
public:
  struct DrawPixel{
    float zValue;
    float red;
    float green;
    float blue;
  };
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
	static constexpr float cameraZLocation = 200.0f;
	static constexpr float maximumFov = 200.0f;
	static constexpr float shapeScaleSpeed = 0.1f;
	
	static constexpr float drawStepValue = 0.5;

	Application(
		Application::Platform platform,
		unsigned int physicalScreenWidth,
		unsigned int physicalScreenHeight
	);
	void mainLoop();
	void notifyKeyIsPressed(Application::Buttons);
	void putPixelInMap(int x,int y,float zValue,float red,float green,float blue);
	static Application* getInstance();
	void drawLineBetweenPoints(
		float startX,
		float startY,
		float startZ,
		float endX,
		float endY,
		float endZ,
		float red,
		float green,
		float blue
	);
	void drawTextureBetweenPoints(
		std::unique_ptr<FaTexture>& texture,
		float triangleStartX,
		float triangleStartY,
		float triangleStartZ,
		float triangleEndX,
		float triangleEndY,
		float triangleEndZ,
		float textureStartX,
		float textureStartY,
		float textureEndX,
		float textureEndY
	);

	static Application* instance;

private:

	void render(float deltaTime);
	void update(float deltaTime);
	
	double targetFPS = 60;                    
	double timePerFrame = 1000 / targetFPS;
	double currentFps = 0;
	std::string fpsText = "";
	double deltaTime = 0;

private:

	unsigned int physicalScreenWidth;
	unsigned int physicalScreenHeight;

	Application::Platform platform;

	std::unique_ptr<Shape3d> shape;

	std::unordered_map<Application::Buttons,bool> keyEvents;

	DrawPixel* currentPixel;

  Wood wood;

	Sky sky;

	Dice dice;

	std::vector<std::vector<DrawPixel>> pixelMap;
};

#endif
