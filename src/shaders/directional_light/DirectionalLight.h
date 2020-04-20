#ifndef DiffuseLight_class
#define DiffuseLight_class

#include "../../data_types/MatrixTemplate.h"
#include "../../camera/Camera.h"

class DirectionalLight {

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
    const MatrixFloat& normal,
    MatrixFloat& output
  ) const;

  void update(Camera& camera);

  void rotateXYZ(const float& x, const float& y, const float& z);

private:

  MatrixFloat color;

  MatrixFloat lightDirectionHat;

  MatrixFloat worldLightDirectionHat;

  MatrixFloat roationXYZMatrix;

  MatrixFloat rotationDegreeMatrix;

};

#endif