#ifndef Light_class
#define Light_class

#include <vector>

#include "./../../camera/Camera.h"
#include "./../../data_types/MatrixTemplate.h"

class Light{

public:

  virtual ~Light() = default;

  virtual void computeLightIntensity(
    MatrixFloat& surfaceNormalVector,
    MatrixFloat& output
  );

  virtual void update(double deltaTime, Camera& cameraInstance);

};

#endif