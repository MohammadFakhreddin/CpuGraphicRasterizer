#ifndef DiffuseLight_class
#define DiffuseLight_class

#include "./../../utils/math/Math.h"
#include "./../../data_types/MatrixTemplate.h"
#include "./../light/Light.h"

class DirectionalLight :public Light {

public:

  DirectionalLight(
    const Vec3DFloat& direction,
    const float& ambiantLight
  );

  void computeLightIntensity(
    MatrixFloat& surfaceNormalVector,
    MatrixFloat& output
  ) override;

  void update(double deltaTime, Camera& cameraInstance) override;

private:

  MatrixFloat lightDirection = MatrixFloat(3, 1, 0.0f);
  
  float dotProductValue = 0.0f;
  
  float lightIntensityFactor = 0.0f;

  float ambiantLight;

};

#endif