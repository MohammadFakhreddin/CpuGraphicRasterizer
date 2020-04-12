#include "./DirectionalLight.h"

#include <cassert>

DirectionalLight::DirectionalLight(
  const float& colorR,
  const float& colorG,
  const float& colorB,
  const float& directionX,
  const float& directionY,
  const float& directionZ
)
  :
  colorR(colorR),
  colorG(colorG),
  colorB(colorB)
{

  assert(colorR >= 0 && colorR <= 1.0f);
  assert(colorG >= 0 && colorG <= 1.0f);
  assert(colorB >= 0 && colorB <= 1.0f);

  lightDirection.set(0, 0, directionX);
  lightDirection.set(1, 0, directionY);
  lightDirection.set(2, 0, directionZ);

  lightDirection.hat<float>(lightDirectionHat);
  
  worldLightDirectionHat.assign(lightDirectionHat);

  rotateXYZ(0.0f,0.0f,0.0f);
 
}

//Source: https://en.wikipedia.org/wiki/Computer_graphics_lighting
void DirectionalLight::computeLightIntensity(
    const MatrixFloat& surfaceNormal,
    const MatrixFloat& surfaceLocation,
    MatrixFloat& output
) const {
  
  float lightIntensityFactor = worldLightDirectionHat.dotProduct(surfaceNormal) * -1;
  
  assert(lightIntensityFactor <= 1);
  
  output.set(0, 0, lightIntensityFactor * colorR);
  output.set(1, 0, lightIntensityFactor * colorG);
  output.set(2, 0, lightIntensityFactor * colorB);

}

//This method must be called before other objects update
void DirectionalLight::update(double deltaTime, Camera& cameraInstance) {
  worldLightDirectionHat.assign(lightDirectionHat);
  worldLightDirectionHat.multiply(roationXYZMatrix);
  worldLightDirectionHat.multiply(cameraInstance.getRotationXYZ());
}

void DirectionalLight::rotateXYZ(const float& x, const float& y, const float& z) {
  rotationDegreeMatrix.set(0, 0, rotationDegreeMatrix.get(0, 0) + x);
  rotationDegreeMatrix.set(1, 0, rotationDegreeMatrix.get(1, 0) + y);
  rotationDegreeMatrix.set(2, 0, rotationDegreeMatrix.get(2, 0) + z);
  MatrixFloat::assignAsRotationXYZMatrix(
    roationXYZMatrix, 
    rotationDegreeMatrix.get(0, 0),
    rotationDegreeMatrix.get(1, 0),
    rotationDegreeMatrix.get(2, 0)
  );
}
