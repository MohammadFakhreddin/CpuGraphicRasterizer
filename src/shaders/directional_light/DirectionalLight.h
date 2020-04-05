#ifndef DiffuseLight_class
#define DiffuseLight_class

#include "./../../utils/math/Math.h"
#include "./../../data_types/MatrixTemplate.h"
#include "./../light/Light.h"

class DirectionalLight :public Light {

public:

  DirectionalLight(
    const float& directionX,
    const float& directionY,
    const float& directionZ,
    const float& ambiantLight
  );

  void computeLightIntensity(
    MatrixFloat& surfaceNormalVector,
    MatrixFloat& output
  ) override;

  void update(double deltaTime, Camera& cameraInstance) override;

  void rotateX(float value);

  void rotateY(float value);

  void rotateZ(float value);

private:

  MatrixFloat lightDirection = MatrixFloat(3, 1, 0.0f);

  MatrixFloat lightDirectionHat = MatrixFloat(3, 1, 0.0f);

  MatrixFloat worldLightDirectionHat = MatrixFloat(3, 1, 0.0f);

  MatrixFloat roationXMatrix = MatrixFloat(3, 3, 0.0f);

  MatrixFloat rotationYMatrix = MatrixFloat(3, 3, 0.0f);

  MatrixFloat rotationZMatrix = MatrixFloat(3, 3, 0.0f);

  MatrixFloat rotationDegreeMatrix = MatrixFloat(3, 1, 0.0f);
  
  double dotProductValue = 0.0f;
  
  float lightIntensityFactor = 0.0f;

  float ambiantLight;

};

#endif