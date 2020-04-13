#ifndef Camera_class
#define Camera_class

#include <vector>
#include <memory>

#include "./../data_types/VectorTemplate.h"
#include "./../open_gl/OpenGl.h"
#include "./../data_types/MatrixTemplate.h"

class Camera{
public:
  
  //TODO Store point size as well
  struct DrawPixel{
    unsigned int x;
    unsigned int y;
    float zValue;
    float red;
    float green;
    float blue;
  };

  Camera(
    OpenGL& gl,
    float cameraFieldOfView,
    float transformX,
    float transformY,
    float transformZ,
    float rotationDegreeX,
    float rotationDegreeY,
    float rotationDegreeZ,
    unsigned int appScreenWidth,
    unsigned int appScreenHeight,
    std::string cameraName
  );

  ~Camera();
  
  void notifyScreenSurfaceIsChanged(
   bool usingNewAppScreenWidthAndHeightIsForced
  );

  void putPixelInMap(
    const int& x,
    const int& y,
    const float& zValue,
    const float& red,
    const float& green,
    const float& blue
  );

  void render(double deltaTime);
  
  float scaleBasedOnZDistance(float zLocation);
  
  unsigned int getAppScreenWidth();
  
  unsigned int getAppScreenHeight();

  void transform(float transformX, float transformY, float transformZ);

  void rotateXYZ(const float& x, const float& y, const float& z);

  const MatrixFloat& getTransformMatrix();

  const MatrixFloat& getRotationXYZ();

  float getCamerFieldOfView();

  void assignToPixel(
    const unsigned int& index,
    const float& zValue,
    const float& red,
    const float& green,
    const float& blue
  );

private:

  static constexpr bool DEBUG_MODE = false;
  
  std::string cameraName;

  void initPixelMap();

  float cameraFieldOfView;

  float zDefaultValue;

  unsigned int appScreenWidth;

  unsigned int appScreenHeight;

  unsigned int pixelMapSize;

  DrawPixel* pixelMap;

  DrawPixel* currentPixel = nullptr;

  OpenGL& gl;

  MatrixFloat transformMatrix;

  MatrixFloat rotationDegreeMatrix;
  
  MatrixFloat rotationValueXYZMatrix;

  MatrixFloat transformationPlaceholder;

};

#endif