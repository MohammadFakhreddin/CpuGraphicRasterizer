#ifndef DiffuseLight_class
#define DiffuseLight_class

#include "../../data_types/MatrixTemplate.h"
#include "../../camera/Camera.h"

class DirectionalLight {

public:
  
  DirectionalLight(
    const Matrix3X1Float& color,
    const Matrix3X1Float& direction
  );

  DirectionalLight(
    const float& colorR,
    const float& colorG,
    const float& colorB,
    const float& directionX,
    const float& directionY,
    const float& directionZ
  );

  void computeLightIntensity(
    const Matrix4X1Float& normal,
    Matrix4X1Float& output
  ) const;

  void update(const Camera& camera);

  void rotateXYZ(const float& x, const float& y, const float& z);

private:

  Matrix3X1Float color;

  Matrix3X1Float lightDirectionHat;

  Matrix4X1Float worldLightDirectionHat;

  Matrix3X1Float rotationDegree;

  Matrix4X4Float rotationXYZMatrix;


};

#endif