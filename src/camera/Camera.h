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
    int left,
    int right,
    int top,
    int bottom
  );
  void notifyScreenSurfaceIsChanged(
    int paramLeft,
    int paramRight,
    int paramTop,
    int paramBottom);
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
    int getLeft();
    int getRight();
    int getTop();
    int getBottom();
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

  int left;
  int right;
  int top;
  int bottom;

  unsigned int appScreenWidth;
  unsigned int appScreenHeight;

  std::vector<std::vector<DrawPixel>> pixelMap;

  DrawPixel* currentPixel = nullptr;

  OpenGL& openGLInstance;

  Light& lightInstance;
    //TODO Add transformation and rotation 
};

#endif