#include "./DirectionalLight.h"

#include <cassert>

#include "../../data_types/MatrixTemplate.h"
#include "../../camera/Camera.h"

DirectionalLight::DirectionalLight(
  const float& colorR,
  const float& colorG,
  const float& colorB,
  const float& directionX,
  const float& directionY,
  const float& directionZ
)
  :
  color(3, 1, 0.0f),
  lightDirectionHat(3, 1, 0.0f),
  worldLightDirectionHat(3, 1, 0.0f),
  roationXYZMatrix(3, 3, 0.0f),
  rotationDegreeMatrix(3, 1, 0.0f)
{

  assert(colorR >= 0 && colorR <= 1.0f);
  assert(colorG >= 0 && colorG <= 1.0f);
  assert(colorB >= 0 && colorB <= 1.0f);

  color.set(0, 0, colorR);
  color.set(1, 0, colorG);
  color.set(2, 0, colorB);

  MatrixFloat lightDirection(3, 1, 0.0f);
  lightDirection.set(0, 0, directionX);
  lightDirection.set(1, 0, directionY);
  lightDirection.set(2, 0, directionZ);

  lightDirection.hat<float>(lightDirectionHat);

  worldLightDirectionHat.assign(lightDirectionHat);

  rotateXYZ(0.0f, 0.0f, 0.0f);

}

//Source: https://en.wikipedia.org/wiki/Computer_graphics_lighting
void DirectionalLight::computeLightIntensity(
    const MatrixFloat& normal,
    MatrixFloat& output
) const {
  
  float lightIntensityFactor = worldLightDirectionHat.dotProduct(normal) * -1;
  
  assert(lightIntensityFactor <= 1);
  
  output.assign(color);
  output.multiply(lightIntensityFactor);

}

//This method must be called before other objects update
void DirectionalLight::update(Camera& camera) {
  worldLightDirectionHat.assign(lightDirectionHat);
  worldLightDirectionHat.multiply(roationXYZMatrix);
  worldLightDirectionHat.multiply(camera.getRotationInverseXYZ());
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
