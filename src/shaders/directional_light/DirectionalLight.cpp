#include "./DirectionalLight.h"

DirectionalLight::DirectionalLight(
  const Vec3DFloat& direction,
  const float& ambiantLight
)
  :
  ambiantLight(ambiantLight)
{
  auto directionHat = direction.hat();
  lightDirection.set(0, 0, directionHat.getX());
  lightDirection.set(1, 0, directionHat.getY());
  lightDirection.set(2, 0, directionHat.getZ());
}

//Source: https://en.wikipedia.org/wiki/Computer_graphics_lighting
void DirectionalLight::computeLightIntensity(
    MatrixFloat& surfaceNormal,
    MatrixFloat& output
){
  
  assert(
    surfaceNormal.size() == 1 && 
    "DiffuseLight::computeLightIntensity normal vectors must have size of 1"
  );

  dotProductValue = lightDirection.dotProduct(surfaceNormal) * -1;
  
  lightIntensityFactor = Math::max(dotProductValue, ambiantLight);
  
  output.set(0, 0, lightIntensityFactor);
  output.set(1, 0, lightIntensityFactor);
  output.set(2, 0, lightIntensityFactor);

}

void DirectionalLight::update(double deltaTime, Camera& cameraInstance) {}