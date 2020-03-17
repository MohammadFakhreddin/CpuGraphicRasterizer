#ifndef Camera_class
#define Camera_class

#include <vector>
#include <memory>

#include "./../data_types/VectorTemplate.h"
#include "./../open_gl/OpenGl.h"
#include "./../fa_texture/FaTexture.h"

class Camera{
public:

    struct DrawPixel{
		float zValue;
		float red;
		float green;
		float blue;
	};

    Camera(
        OpenGL& openGlInstance,
        float cameraZLocation,
        float cameraFieldOfView,
        float left,
        float right,
        float top,
        float bottom,
        unsigned int appScreenWidth,
        unsigned int appScreenHeight
    );
    void notifyScreenSurfaceIsChanged(
        float left,
        float right,
        float top,
        float bottom,
        unsigned int appScreenWidth,
        unsigned int appScreenHeight    
    );
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
    void putPixelInMap(
        int x,
        int y,
        float zValue,
        float red,
        float green,
        float blue
    );
    void render(double deltaTime);
    float scaleBasedOnZDistance(float zLocation);
    float getLeft();
    float getRight();
    float getTop();
    float getBottom();
    float getCameraZLocation();
    unsigned int calculateStepCount(float difference);
    float calculateStepValue(float difference);
private:

    void initPixelMap();

    static constexpr float drawStepValue = 0.5;

    float cameraZLocation;
    float cameraFieldOfView;

    float left;
    float right;
    float top;
    float bottom;

    unsigned int appScreenWidth;
    unsigned int appScreenHeight;

    std::vector<std::vector<DrawPixel>> pixelMap;
    DrawPixel* currentPixel;

    OpenGL& openGLInstance;
    //TODO Add transformation and rotation 
};

#endif