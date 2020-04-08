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
    const float& cameraFieldOfView,
    const float& attenuation
  );

  void transformX(const float& value);

  void transformY(const float& value);

  void transformZ(const float& value);

  void computeLightIntensity(
    MatrixFloat& surfaceNormalVector,
    MatrixFloat& surfaceLocation,
    MatrixFloat& output
  ) override;

  void update(double deltaTime, Camera& cameraInstance) override;

private:

  float radius;

  float colorR;

  float colorG;

  float colorB;

  MatrixFloat transform = MatrixFloat(3, 1, 0.0f);

  std::unique_ptr<ColorTexture> lightColor;

  std::unique_ptr<Shape3d> sphere;

  MatrixFloat worldPoint = MatrixFloat(3, 1, 0.0f);
  //TODO Remove this
  std::vector<std::unique_ptr<Light>> lightSources;

  MatrixFloat distanceMatrix = MatrixFloat(3, 1, 0.0f);

  MatrixFloat distanceMatrixHat = MatrixFloat(3, 1, 0.0f);

  double squareDistance = 0.0;

  double distance = 0.0f;

  double angleFactor = 0.0;

  double distanceFactor = 0.0;

  double lightIntensity = 0.0;

  double computationPercision = 1000.0;

  float cameraFieldOfView;

  float attenuation;

};

#endif // !PointLight_Class
