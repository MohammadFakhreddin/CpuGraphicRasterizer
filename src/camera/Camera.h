#ifndef Camera_class
#define Camera_class

#include <vector>
#include <memory>

#include "./../data_types/VectorTemplate.h"
#include "./../open_gl/OpenGl.h"
#include "./../fa_texture/FaTexture.h"
#include "./../shaders/light/Light.h"

class Camera{
public:
    //TODO Store point size as well
    struct DrawPixel{
		float zValue;
		float red;
		float green;
		float blue;
	};

    Camera(
        OpenGL& openGlInstance,
        Light& lightInstance,
        float cameraZLocation,
        float cameraFieldOfView,
        float left,
        float right,
        float top,
        float bottom
    );
    void notifyScreenSurfaceIsChanged(
        float left,
        float right,
        float top,
        float bottom
    );
    void putPixelInMap(
        int x,
        int y,
        float zValue,
        float red,
        float green,
        float blue
    );
    void update(double deltaTime);
    void render(double deltaTime);
    float scaleBasedOnZDistance(float zLocation);
    float getLeft();
    float getRight();
    float getTop();
    float getBottom();
    float getCameraZLocation();
    Light& getLight();
    unsigned int getAppScreenWidth();
    unsigned int getAppScreenHeight();

private:

    static constexpr bool DEBUG_MODE = false;
    
    void initPixelMap();
    void drawLight();

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

    Light& lightInstance;
    //TODO Add transformation and rotation 
};

#endif