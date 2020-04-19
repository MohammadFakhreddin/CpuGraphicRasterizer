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

  struct Params {
    OpenGL& gl;
    MatrixFloat transform;
    MatrixFloat rotation;
    unsigned int appScreenWidth;
    unsigned int appScreenHeight;
    std::string cameraName;
  };

  Camera(
    OpenGL& gl,
    MatrixFloat transform,
    MatrixFloat rotation,
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
  
  float scaleBasedOnZDistance(const float& zLocation);
  
  const unsigned int& getAppScreenWidth() const ;
  
  const unsigned int& getAppScreenHeight() const ;

  void transform(float transformX, float transformY, float transformZ);

  void rotateXYZ(const float& x, const float& y, const float& z);

  const MatrixFloat& getTransformMatrix();

  const MatrixFloat& getRotationInverseXYZ();

  void assignToPixel(
    const unsigned int& index,
    const float& zValue,
    const float& red,
    const float& green,
    const float& blue
  );

  const unsigned int& getCameraCenterX() const ;

  const unsigned int& getCameraCenterY() const ;

  bool isVisibleToCamera(
    std::vector<MatrixFloat>& worldPoints,
    std::vector<MatrixFloat>& normals,
    const unsigned long edgeIndices[3],
    const unsigned long normalVectorIndices[3],
    MatrixFloat& cameraVectorPlaceholder
  );

  void generateCameraToPointVector(
    const MatrixFloat& worldPoint,
    MatrixFloat& output
  ) const;

  void calculateStepCount(
    const float& difference,
    unsigned int* totalStepCount
  ) const;

  void calculateStepValueBasedOnStepCount(
    const float& difference,
    const unsigned int& stepCount,
    float* stepValue
  ) const;

  void calculateStepValueBasedOnStepCount(
    const float& differenceX,
    const float& differenceY,
    const unsigned int& stepCount,
    MatrixFloat& stepValueMatrix
  ) const;

  void calculateStepValueBasedOnStepCount(
    const float& differenceX,
    const float& differenceY,
    const float& differenceZ,
    const unsigned int& stepCount,
    MatrixFloat& stepValueMatrix
  ) const;

  void calculateStepValueBasedOnStepCount(
    const MatrixFloat& differenceMatrix,
    const unsigned int& stepCount,
    MatrixFloat& stepValueMatrix
  ) const;

private:

  /**
   *
   * Jump value in draw steps
   *
  */
  //TODO It must be 1, Current value is cpu expensive
  static constexpr float drawStepValue = 0.5;


  static constexpr float zDefaultValue = 0;

  std::string cameraName;

  void initPixelMap();

  unsigned int appScreenWidth;

  unsigned int appScreenHeight;

  unsigned int pixelMapSize;

  DrawPixel* pixelMap;

  OpenGL& gl;

  MatrixFloat transformMatrix;

  MatrixFloat rotationInverseDegreeMatrix;
  
  MatrixFloat rotationInverseValueXYZMatrix;

  MatrixFloat transformationPlaceholder;

  unsigned int cameraCenterX;

  unsigned int cameraCenterY;

  unsigned int cameraStartX;

  unsigned int cameraEndX;

  unsigned int cameraStartY;

  unsigned int cameraEndY;

};

#endif