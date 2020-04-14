#ifndef PointLight_Class
#define PointLight_Class

#include "../light/Light.h"
#include "../../3d_shape/Shape3d.h"
#include "../../texture/ColorTexture/ColorTexture.h"

class PointLight : public Light {

public:

  PointLight(
    const float& radius,
    const float& colorR,
    const float& colorG,
    const float& colorB,
    const float& initialTransformX,
    const float& initialTransformY,
    const float& initialTransformZ,
    const double& constantAttenuation,
    const double& linearAttenuation,
    const double& quadricAttenuation,
    const double& specularIntensity,
    const unsigned int& specularPower
  );

  void transformX(const float& value);

  void transformY(const float& value);

  void transformZ(const float& value);

  void computeLightIntensity(
    const MatrixFloat& surfaceNormalVector,
    const MatrixFloat& surfaceLocation,
    const Camera& cameraInstance,
    MatrixFloat& output
  ) const override;

  void update(double deltaTime, Camera& cameraInstance) override;

private:

  float radius;

  float colorR;

  float colorG;

  float colorB;

  MatrixFloat transform;

  std::unique_ptr<ColorTexture> lightColor;

  std::unique_ptr<Shape3d> sphere;

  MatrixFloat worldPoint;
  //TODO Remove this
  std::vector<std::unique_ptr<Light>> lightSources;

  double constantAttenuation;

  double linearAttenuation;

  double quadricAttenuation;

  double specularIntensity;

  unsigned int specularPower;

};

#endif // !PointLight_Class
