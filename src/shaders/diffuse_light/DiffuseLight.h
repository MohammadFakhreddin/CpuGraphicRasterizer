#ifndef DiffuseLight_class
#define DiffuseLight_class

#include "./../../utils/math/Math.h"
#include "./../../data_types/VectorTemplate.h"
#include "./../light/Light.h"

class DiffuseLight :public Light {

public:

  DiffuseLight(float positionX, float positionY, float positionZ);

  void computeLightIntensity(
    Vec3DFloat& surfaceNormalVector,
    Vec3DFloat& surfaceCenter,
    Vec3DFloat& output
  ) override;

private:

  Vec3DFloat lightDirectionVector;

};

#endif