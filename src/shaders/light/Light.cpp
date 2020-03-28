#include "./Light.h"

#include <cassert>

#include "./../../utils/log/Logger.h"

void Light::computeLightIntensity(
    Vec3DFloat& surfaceNormalVector,
    Vec3DFloat& surfaceCenter,
    Vec3DFloat& output
){
    Logger::log("Method getLightIntensity is not implemented correctly in child class");
    assert(false);
}

void Light::transformX(float value){
  transformMatrix.set(0, 0, transformMatrix.get(0, 0) + value);
}

void Light::transformY(float value){
  transformMatrix.set(1, 0, transformMatrix.get(1, 0) + value);
}

void Light::transformZ(float value){
  transformMatrix.set(2, 0, transformMatrix.get(2, 0) + value);
}

Light::Light(float transformX, float transformY, float transformZ) {
  transformMatrix.set(0, 0, transformX);
  transformMatrix.set(1, 0, transformY);
  transformMatrix.set(2, 0, transformZ);
}

void Light::render(double deltaTime,Camera& cameraInstance) {
  //TODO Replace this with spehre
  int radius = 10;
  for (int i = -radius; i < radius; i++) {
    for (int j = -radius; j < radius; j++) {
      cameraInstance.putPixelInMap(
        int(worldPoint.get(0,0)) + i,
        int(worldPoint.get(1,0)) + j,
        worldPoint.get(2,0),
        0.7f,
        0.6f,
        0.0f
      );
    }
  }
}

void Light::update(double deltaTime,Camera& cameraInstance) {
  worldPoint = transformMatrix;
  worldPoint -= cameraInstance.getTransformMatrix();
}