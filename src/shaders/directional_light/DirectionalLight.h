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
    const MatrixFloat& surfaceNormalVector,
    const MatrixFloat& surfaceLocation,
    MatrixFloat& output
  ) const override;

  void update(double deltaTime, Camera& cameraInstance) override;

  void rotateXYZ(const float& x,const float& y,const float& z);

private:

  float colorR;

  float colorG;

  float colorB;

  MatrixFloat lightDirectionHat;

  MatrixFloat worldLightDirectionHat;

  MatrixFloat roationXYZMatrix;

  MatrixFloat rotationDegreeMatrix;
  
};

#endif