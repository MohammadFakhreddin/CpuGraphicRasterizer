#ifndef Camera_class
#define Camera_class

#include <vector>
#include <memory>

#include "./../data_types/VectorTemplate.h"
#include "./../open_gl/OpenGl.h"
#include "./../data_types/MatrixTemplate.h"

class Camera{

public:

 float endZ = 1000;

 float startZ = 0;

 float zDistance = endZ - startZ;

  Camera(
    OpenGL& paramGl,
    Matrix3X1Float paramTransform,
    Matrix3X1Float paramRotation,
    unsigned int paramAppScreenWidth,
    unsigned int paramAppScreenHeight,
    std::string paramCameraName
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

  void render(const double& deltaTime);
  
  float scaleBasedOnZDistance(const float& zLocation);
  
  const unsigned int& getAppScreenWidth() const ;
  
  const unsigned int& getAppScreenHeight() const ;

  void transformXYZ(const float& transformX, const float& transformY, const float& transformZ);

  void rotateXYZ(const float& x, const float& y, const float& z);

  void assignToPixel(
    const unsigned int& index,
    const float& zValue,
    const float& red,
    const float& green,
    const float& blue
  );

  bool isVisibleToCamera(
    std::vector<Matrix4X1Float>& worldPoints,
    std::vector<Matrix4X1Float>& normals,
    const unsigned long edgeIndices[3],
    const unsigned long normalVectorIndices[3],
    Matrix4X1Float& cameraVectorPlaceholder
  );

  void generateCameraToPointVector(
    const Matrix4X1Float& worldPoint,
    Matrix4X1Float& output
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
    Matrix2X1Float& stepValueMatrix
  ) const;

  void calculateStepValueBasedOnStepCount(
    const float& differenceX,
    const float& differenceY,
    const float& differenceZ,
    const unsigned int& stepCount,
    Matrix3X1Float& stepValueMatrix
  ) const;

  void calculateStepValueBasedOnStepCount(
    const Matrix3X1Float& difference,
    const unsigned int& stepCount,
    Matrix3X1Float& stepValueMatrix
  ) const;

  void calculateStepValueBasedOnStepCount(
    const Matrix4X1Float& differenceMatrix,
    const unsigned int& stepCount,
    Matrix4X1Float& stepValueMatrix
  ) const;

  void calculateStepValueBasedOnStepCount(
    const float& differenceX,
    const float& differenceY,
    const float& differenceZ,
    const unsigned int& stepCount,
    Matrix4X1Float& stepValueMatrix
  ) const;


  Matrix4X4Float transformInverseMatrix;

  Matrix4X4Float rotationXMatrix;

  Matrix4X4Float rotationYMatrix;

  Matrix4X4Float rotationZMatrix;

  Matrix4X4Float rotationInverseMatrix;

  Matrix4X4Float projection;

  Matrix3X1Float transformInverseValue;

  Matrix3X1Float rotationInverseDegree;

  Matrix3X1Float rotationDegree;

  Matrix4X1Float screenCenter;

private:

  /**
   *
   * Jump value in draw steps
   *
  */
  //TODO It must be 1, Current value is cpu expensive
  static constexpr float drawStepValue = 0.5;

  std::string cameraName;

  void initPixelMap();

  unsigned int appScreenWidth;

  unsigned int appScreenHeight;

  unsigned int pixelMapSize;

  GLfloat* pointsDefaultValue;
  GLfloat* points;
  
  GLfloat* colorsDefaultValue;
  GLfloat* colors;

  size_t sizeOfColors;
  size_t sizeOfPoints;

  OpenGL& gl;

  float startX;

  float endX;

  float startY;

  float endY;

  Matrix4X1Float position;

  Matrix4X1Float newTransform;

};

#endif