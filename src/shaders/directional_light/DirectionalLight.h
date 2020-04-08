#ifndef DiffuseLight_class
#define DiffuseLight_class

#include "./../../utils/math/Math.h"
#include "./../../data_types/MatrixTemplate.h"
#include "./../light/Light.h"

class DirectionalLight :public Light {

public:
  //TODO We need to have light color
  DirectionalLight(
    const float& colorR,
    const float& colorG,
    const float& colorB,
    const float& directionX,
    const float& directionY,
    const float& directionZ
  );

  void computeLightIntensity(
    MatrixFloat& surfaceNormalVector,
    MatrixFloat& surfaceLocation,
    MatrixFloat& output
  ) override;

  void update(double deltaTime, Camera& cameraInstance) override;

  void rotateXYZ(const float& x,const float& y,const float& z);

private:

  float colorR;

  float colorG;

  float colorB;

  MatrixFloat lightDirection = MatrixFloat(3, 1, 0.0f);

  MatrixFloat lightDirectionHat = MatrixFloat(3, 1, 0.0f);

  MatrixFloat worldLightDirectionHat = MatrixFloat(3, 1, 0.0f);

  MatrixFloat roationXYZMatrix = MatrixFloat(3, 3, 0.0f);

  MatrixFloat rotationDegreeMatrix = MatrixFloat(3, 1, 0.0f);
  
  float lightIntensityFactor = 0.0f;

};

#endif