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

  rotateXYZ(0.0f,0.0f,0.0f);
 
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
