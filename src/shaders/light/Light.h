#ifndef Light_class
#define Light_class

#include <vector>

#include "./../../data_types/VectorTemplate.h"
#include "./../../camera/Camera.h"
#include "./../../data_types/MatrixTemplate.h"

class Light{

public:

  Light(
    float transformX,
    float transformY,
    float transformZ
  );

  virtual void computeLightIntensity(
    Vec3DFloat& surfaceNormalVector,
    Vec3DFloat& surfaceCenter,
    Vec3DFloat& output
  );

  void transformX(float value);

  void transformY(float value);

  void transformZ(float value);

  void update(double deltaTime, Camera& cameraInstance);

protected:
  
  MatrixFloat worldPoint = MatrixFloat(3,1,0.0f);

private:

  MatrixFloat transformMatrix = MatrixFloat(3, 1, 0.0f);

};

#endif