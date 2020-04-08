#include "../point_light/PointLight.h"

#include <cassert>

#include "../../3d_models/ShapeGenerator.h"


PointLight::PointLight(
  const float& radius,
  const float& colorR,
  const float& colorG,
  const float& colorB,
  const float& initialTransformX,
  const float& initialTransformY,
  const float& initialTransformZ
) :
  radius(radius),
  colorR(colorR),
  colorG(colorG),
  colorB(colorB)
{

  assert(radius >= 0);
  assert(colorR >= 0 && colorR <= 1.0f);
  assert(colorG >= 0 && colorG <= 1.0f);
  assert(colorB >= 0 && colorB <= 1.0f);

  lightColor = std::make_unique<ColorTexture>(colorR, colorG, colorB);

  sphere = ShapeGenerator::sphere(
    (std::unique_ptr<Texture>&)lightColor,
    radius, 12, 24,
    Vec3DFloat(0.0f, 0.0f, 0.0f),
    Vec3DFloat(0.0f, 0.0f, 0.0f),
    1.0f
  );

  transformX(initialTransformX);
  transformY(initialTransformY);
  transformZ(initialTransformZ);
}

void PointLight::transformX(const float& value) {
  transform.set(0, 0, transform.get(0, 0) + value);
  sphere->transformX(value);
}

void PointLight::transformY(const float& value) {
  transform.set(1, 0, transform.get(1, 0) + value);
  sphere->transformY(value);
}

void PointLight::transformZ(const float& value) {
  transform.set(2, 0, transform.get(2, 0) + value);
  sphere->transformZ(value);
}

void PointLight::update(double deltaTime, Camera& cameraInstance) {

  worldPoint.assign(transform);

  worldPoint.sum(cameraInstance.getTransformMatrix());

  sphere->update(deltaTime, cameraInstance, lightSources);

}
//I use y = 1/(x*x + 1) instead of y = 1/(x*x + x + 1)
void PointLight::computeLightIntensity(
  MatrixFloat& surfaceNormalVector,
  MatrixFloat& surfaceLocation,
  MatrixFloat& output
) {

  assert(surfaceNormalVector.getWidth() == 3);
  assert(surfaceNormalVector.getHeight() == 1);
  assert(surfaceLocation.getWidth() == 3);
  assert(surfaceLocation.getHeight() == 1);

  //distanceMatrix.assign(worldPoint);
  //TODO Assign radius as light factor too
  //distanceMatrix.minus(surfaceLocation);
  distanceMatrix.set(0, 0, surfaceLocation.get(0, 0) - worldPoint.get(0, 0));
  distanceMatrix.set(1, 0, surfaceLocation.get(1, 0) - worldPoint.get(1, 0));
  distanceMatrix.set(2, 0, surfaceLocation.get(2, 0) - worldPoint.get(2, 0));

  //squareDistance = distanceMatrix.squareSize<double>();

  distanceMatrix.hat<float>(distanceMatrixHat);

  lightIntensity = distanceMatrixHat.dotProduct(surfaceNormalVector) * -1.0;
  //double squareValue = 1.0 / (squareDistance + 1.0);
  //lightIntensity *= squareValue;

  assert(lightIntensity <= 1);

  output.set(0, 0, float(lightIntensity * colorR));
  output.set(1, 0, float(lightIntensity * colorG));
  output.set(2, 0, float(lightIntensity * colorB));
  
}