#include "./Light.h"

#include <cassert>

#include "./../../utils/log/Logger.h"

void Light::computeLightIntensity(
  const MatrixFloat& surfaceNormalVector, 
  const MatrixFloat& surfaceLocation, 
  MatrixFloat& output
) const
{
  Logger::log("Method getLightIntensity is not implemented correctly in child class");
  assert(false);
}

void Light::update(double deltaTime,Camera& cameraInstance) {
  Logger::log("Method getLightIntensity is not implemented correctly in child class");
  assert(false);
}