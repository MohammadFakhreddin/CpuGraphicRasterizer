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

  void update(double deltaTime);
  
  void render(double deltaTime);
  
  float scaleBasedOnZDistance(float zLocation);
  
  unsigned int getAppScreenWidth();
  
  unsigned int getAppScreenHeight();

  void transform(float transformX, float transformY, float transformZ);

  void rotateX(float x);

  void rotateY(float y);

  void rotateZ(float z);

  const MatrixFloat& getTransformMatrix();

  const MatrixFloat& getRotationX();

  const MatrixFloat& getRotationY();

  const MatrixFloat& getRotationZ();

private:

  static constexpr bool DEBUG_MODE = false;
  
  std::string cameraName;

  void initPixelMap();

  float cameraFieldOfView;

  float zDefaultValue;

  unsigned int appScreenWidth;

  unsigned int appScreenHeight;

  DrawPixel* pixelMap;

  DrawPixel* currentPixel = nullptr;

  OpenGL& gl;

  MatrixFloat transformMatrix;

  MatrixFloat rotationDegreeMatrix;
  
  MatrixFloat rotationValueXMatrix;
  
  MatrixFloat rotationValueYMatrix;
  
  MatrixFloat rotationValueZMatrix;

  MatrixFloat transformationPlaceholder = MatrixFloat(3, 1, 0.0f);

};

#endif