#include "./DirectionalLight.h"

DirectionalLight::DirectionalLight(
  const float& directionX,
  const float& directionY,
  const float& directionZ,
  const float& ambiantLight
)
  :
  ambiantLight(ambiantLight)
{

  lightDirection.set(0, 0, directionX);
  lightDirection.set(1, 0, directionY);
  lightDirection.set(2, 0, directionZ);

  lightDirectionHat.assign(lightDirection.hat<float>());
  
  worldLightDirectionHat.assign(lightDirectionHat);

  rotateX(0.0f);
  rotateY(0.0f);
  rotateZ(0.0f);

}

//Source: https://en.wikipedia.org/wiki/Computer_graphics_lighting
void DirectionalLight::computeLightIntensity(
    MatrixFloat& surfaceNormal,
    MatrixFloat& output
){
  
  dotProductValue = worldLightDirectionHat.dotProduct(surfaceNormal) * -1;
  
  lightIntensityFactor = float(Math::max(dotProductValue, ambiantLight));
  
  output.set(0, 0, lightIntensityFactor);
  output.set(1, 0, lightIntensityFactor);
  output.set(2, 0, lightIntensityFactor);

}
//This method must be called before other objects update
void DirectionalLight::update(double deltaTime, Camera& cameraInstance) {

  worldLightDirectionHat.assign(lightDirectionHat);

  worldLightDirectionHat.multiply(roationXMatrix);
  worldLightDirectionHat.multiply(rotationYMatrix);
  worldLightDirectionHat.multiply(rotationZMatrix);

  worldLightDirectionHat.multiply(cameraInstance.getRotationX());
  worldLightDirectionHat.multiply(cameraInstance.getRotationY());
  worldLightDirectionHat.multiply(cameraInstance.getRotationZ());

}

void DirectionalLight::rotateX(float value) {
  rotationDegreeMatrix.set(0, 0, rotationDegreeMatrix.get(0, 0) + value);
  MatrixFloat::assignAsRotationXMatrix(roationXMatrix, rotationDegreeMatrix.get(0, 0));
}

void DirectionalLight::rotateY(float value) {
  rotationDegreeMatrix.set(1, 0, rotationDegreeMatrix.get(1, 0) + value);
  MatrixFloat::assignAsRoationYMatrix(rotationYMatrix, rotationDegreeMatrix.get(1, 0));
}

void DirectionalLight::rotateZ(float value) {
  rotationDegreeMatrix.set(2, 0, rotationDegreeMatrix.get(2, 0) + value);
  MatrixFloat::assignAsRotationZMatrix(rotationZMatrix, rotationDegreeMatrix.get(2, 0));
}