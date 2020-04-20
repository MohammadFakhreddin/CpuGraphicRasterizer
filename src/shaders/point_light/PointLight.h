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
    const MatrixFloat& paramColor,
    const MatrixFloat& paramTransform,
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
    const MatrixFloat& surfaceNormalVector,
    const MatrixFloat& surfaceLocation,
    const float& specularIntensity,
    MatrixFloat& lightVectorPlaceholder,
    MatrixFloat& lightVectorHatPlaceholder,
    MatrixFloat& lightReflectionPlaceholder,
    MatrixFloat& lightReflectionHatPlaceholder,
    MatrixFloat& cameraVectorPlaceholder,
    MatrixFloat& cameraVectorHatPlaceholder,
    MatrixFloat& output
  ) const ;

  Shape3d* getShape();

private:

  MatrixFloat transform;

  std::unique_ptr<ColorTexture> lightColor;

  MatrixFloat worldPoint;
  
  double constantAttenuation;

  double linearAttenuation;

  double quadricAttenuation;

  unsigned int specularPower;

  MatrixFloat color;

  std::unique_ptr<Shape3d> sphere;

};

#endif // !PointLight_Class
