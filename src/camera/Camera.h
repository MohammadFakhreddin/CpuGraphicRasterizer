#ifndef Camera_class
#define Camera_class

#include <vector>
#include <memory>

#include "./../data_types/VectorTemplate.h"
#include "./../open_gl/OpenGl.h"
#include "../event_handler/EventHandler.h"
#include "./../fa_texture/FaTexture.h"
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

  // TODO Camera must contain following information
  // 1- cameraLocation
  // 2- cameraBorders (Left,Right,Top,Bottom)
  // 3- cameraDirectionVector
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
    EventHandler::ScreenSurfaceChangeEventData data
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
  
  unsigned int getAppScreenWidth();
  
  unsigned int getAppScreenHeight();

  void Camera::transform(float transformX, float transformY, float transformZ);

  void Camera::rotateX(float x);

  void Camera::rotateY(float y);

  void Camera::rotateZ(float z);

  const MatrixFloat& Camera::getTransformMatrix();

  const MatrixFloat& Camera::getRotationX();

  const MatrixFloat& Camera::getRotationY();

  const MatrixFloat& Camera::getRotationZ();

private:

  static constexpr bool DEBUG_MODE = false;
  
  std::string cameraName;

  void initPixelMap();

  float cameraFieldOfView;

  float zDefaultValue;

  unsigned int appScreenWidth;
  unsigned int appScreenHeight;

  //std::vector<std::vector<DrawPixel>> pixelMap;
  DrawPixel* pixelMap;

  DrawPixel* currentPixel = nullptr;

  OpenGL& gl;

  MatrixFloat transformMatrix;

  MatrixFloat rotationDegreeMatrix;
  
  MatrixFloat rotationValueXMatrix;
  
  MatrixFloat rotationValueYMatrix;
  
  MatrixFloat rotationValueZMatrix;

  MatrixFloat transformationPlacholder = MatrixFloat(3, 1, 0.0f);

};

#endif