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
    const MatrixFloat& surfaceNormalVector,
    const MatrixFloat& surfaceLocation,
    const Camera& cameraInstance,
    MatrixFloat& output
  ) const override;

  void update(double deltaTime, Camera& cameraInstance) override;

private:

  float colorR;

  float colorG;

  float colorB;

};

#endif // !AmbiantLight_Class
