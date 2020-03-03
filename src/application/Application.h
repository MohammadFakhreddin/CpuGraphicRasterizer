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

	static constexpr char* cubeTextureAddress = "src/assets/wood.png";
	static constexpr float cubeTextureVirtualWidth = 100;
	static constexpr float cubeTextureVirtualHeight = 100;

	Application();
	void render(float deltaTime);
	void update(float deltaTime);
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
private:
	std::unique_ptr<Shape3d> shape;
	static Application* instance;
	std::unordered_map<Application::Buttons,bool> keyEvents;
	DrawPixel* currentPixel;
	std::unique_ptr<FaTexture> cubeTexture = std::unique_ptr<FaTexture>(
		new FaTexture(
			cubeTextureAddress,
			cubeTextureVirtualWidth,
			cubeTextureVirtualHeight
		)
	);
	std::vector<std::vector<DrawPixel>> pixelMap;
};

#endif