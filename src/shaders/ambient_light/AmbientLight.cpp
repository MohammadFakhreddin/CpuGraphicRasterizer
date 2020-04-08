#include "./AmbientLight.h"

#include <cassert>

AmbientLight::AmbientLight(
  const float& colorR,
  const float& colorG,
  const float& colorB
) 
  :
  colorR(colorR),
  colorG(colorG),
  colorB(colorB)
{

  assert(colorR >= 0.0f && colorR <= 1.0f);
  assert(colorG >= 0.0f && colorG <= 1.0f);
  assert(colorB >= 0.0f && colorB <= 1.0f);

}

void AmbientLight::computeLightIntensity(
  MatrixFloat& surfaceNormalVector,
  MatrixFloat& surfaceLocation,
  MatrixFloat& output
) {
  output.set(0, 0, colorR);
  output.set(1, 0,colorG);
  output.set(2, 0, colorB);
}

void AmbientLight::update(double deltaTime, Camera& cameraInstance) {}
