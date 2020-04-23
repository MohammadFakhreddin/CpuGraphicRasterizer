#ifndef PointLight_Class
#define PointLight_Class

#include "../../texture/ColorTexture/ColorTexture.h"
#include "../../3d/shape/Shape3d.h"

class PointLight{

public:

  PointLight(
    const float& radius,
    const float& colorR,
    const float& colorG,
    const float& colorB,
    const float& transformX,
    const float& transformY,
    const float& transformZ,
    const float& constantAttenuation,
    const float& linearAttenuation,
    const float& quadricAttenuation,
    const unsigned int& specularPower
  );

  PointLight(
    const float& paramRadius,
    const Matrix3X1Float& paramColor,
    const Matrix3X1Float& paramTransform,
    const float& paramConstantAttenuation,
    const float& paramLinearAttenuation,
    const float& paramQuadricAttenuation,
    const unsigned int& paramSpecularPower
  );

  void transformX(const float& value);

  void transformY(const float& value);

  void transformZ(const float& value);

  void computeLightIntensity(
    const Camera& cameraInstance,
    const Matrix4X1Float& surfaceNormalVector,
    const Matrix4X1Float& surfaceLocation,
    const float& specularIntensity,
    Matrix4X1Float& lightVectorPlaceholder,
    Matrix4X1Float& lightVectorHatPlaceholder,
    Matrix4X1Float& lightReflectionPlaceholder,
    Matrix4X1Float& lightReflectionHatPlaceholder,
    Matrix4X1Float& cameraVectorPlaceholder,
    Matrix4X1Float& cameraVectorHatPlaceholder,
    Matrix4X1Float& output
  ) const ;

  Shape3d* getShape();

private:

  Matrix3X1Float transform;

  std::unique_ptr<ColorTexture> lightColor;

  Matrix4X1Float worldPoint;
  
  double constantAttenuation;

  double linearAttenuation;

  double quadricAttenuation;

  unsigned int specularPower;

  Matrix3X1Float color;

  std::unique_ptr<Shape3d> sphere;

};

#endif // !PointLight_Class
