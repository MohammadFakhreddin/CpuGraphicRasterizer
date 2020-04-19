#ifndef PointLight_Class
#define PointLight_Class

#include "../../texture/ColorTexture/ColorTexture.h"
#include "../../3d/shape/Shape3d.h"

class PointLight{

public:

  PointLight(
    float radius,
    MatrixFloat color,
    MatrixFloat transform,
    float constantAttenuation,
    float linearAttenuation,
    float quadricAttenuation,
    unsigned int specularPower
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
