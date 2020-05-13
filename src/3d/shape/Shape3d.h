#ifndef Shape3d_class
#define Shape3d_class

#include <vector>
#include <memory>

#include "../surface/Surface.h"

class Shape3d
{

public:

  enum class NormalType {
    smooth,
    sharp,
    fileDefault
  };

  static std::vector<Matrix3X1Float> generateNormals(
    std::vector<std::unique_ptr<Surface>>& surfaceList,
    std::vector<Matrix3X1Float>& nodes,
    NormalType normalType
  );

  Shape3d(
    std::vector<Matrix3X1Float>& paramNodes,
    std::vector<std::unique_ptr<Surface>>& paramSurfaces,
    std::vector<Matrix3X1Float>& paramNormals
  );

  Shape3d(
    std::vector<Matrix3X1Float>& paramNodes,
    std::vector<std::unique_ptr<Surface>>& paramSurfaces,
    std::vector<Matrix3X1Float>& paramNormals,
    const float paramSpecularIntensity
  );

  Shape3d(
    std::vector<Matrix3X1Float>& paramNodes,
    std::vector<std::unique_ptr<Surface>>& paramSurfaces,
    std::vector<Matrix3X1Float>& paramNormals,
    const float paramSpecularIntensity,
    float paramInitialTransformX,
    float paramInitialTransformY,
    float paramInitialTransformZ
  );

  Shape3d(
    std::vector<Matrix3X1Float>& paramNodes,
    std::vector<std::unique_ptr<Surface>>& paramSurfaces,
    std::vector<Matrix3X1Float>& paramNormals,
    const float paramSpecularIntensity,
    float paramTransformX,
    float paramTransformY,
    float paramTransformZ,
    float paramRotationDegreeX,
    float paramRotationDegreeY,
    float paramRotationDegreeZ,
    float paramScaleValue
  );

  void setSpecularIntensity(const float& value);

  void transformX(const float& x);

  void transformY(const float& y);

  void transformZ(const float& z);

  void transformXYZ(const float& x, const float& y, const float& z);

  void resetTransform();

  void scale(float value);

  void resetScale();

  void rotateX(const float& x);

  void rotateY(const float& x);

  void rotateZ(const float& x);

  void rotateXYZ(const float& x, const float& y, const float& z);

  bool checkDataValidation();

  void setTransparencyColor(const float& colorR, const float& colorG, const float& colorB);

  void setTransparencyColorStatus(const bool& enable);

  std::vector<Matrix3X1Float> nodes;
  
  std::vector<std::unique_ptr<Surface>>surfaces;

  std::vector<Matrix3X1Float> normals;
  
  std::vector<Matrix4X1Float> worldPoints;

  std::vector<Matrix4X1Float> worldNormals;

  Matrix3X1Float transformValue;

  Matrix3X1Float rotationValue;

  float scaleValue = 0.0f;

  float specularIntensity;
  
  Matrix4X4Float transformMatrix;

  Matrix4X4Float rotationXYZMatrix;

  Matrix4X4Float scaleMatrix;

  Matrix3X1Float transparencyColor;

  bool transparencyColorIsEnabled;

};

#endif