#ifndef AmbientLight_Class
#define AmbientLight_Class

#include "../light/Light.h"

class AmbientLight : public Light {

public:

  AmbientLight(
    const float& colorR,
    const float& colorG,
    const float& colorB
  );

  void computeLightIntensity(
    MatrixFloat& surfaceNormalVector,
    MatrixFloat& surfaceLocation,
    MatrixFloat& output
  ) override;

  void update(double deltaTime, Camera& cameraInstance) override;

private:

  float colorR;

  float colorG;

  float colorB;

};

#endif // !AmbiantLight_Class
