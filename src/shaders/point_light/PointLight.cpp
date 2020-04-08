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
  const float& initialTransformZ,
  const float& cameraFieldOfView,
  const float& attenuation
) :
  radius(radius),
  colorR(colorR),
  colorG(colorG),
  colorB(colorB),
  cameraFieldOfView(cameraFieldOfView),
  attenuation(attenuation)
{

  assert(radius >= 0);
  assert(colorR >= 0 && colorR <= 1.0f);
  assert(colorG >= 0 && colorG <= 1.0f);
  assert(colorB >= 0 && colorB <= 1.0f);

  lightColor = std::make_unique<ColorTexture>(colorR, colorG, colorB);

  sphere = ShapeGenerator::sphere(
    (std::unique_ptr<Texture>&)lightColor,
    radius, 12 * 2, 24 * 2,
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
  worldPoint.minus(cameraInstance.getTransformMatrix());
  sphere->update(deltaTime, cameraInstance, lightSources);

}
//I use y = 1/(x*x + x + 1)
void PointLight::computeLightIntensity(
  MatrixFloat& surfaceNormalVector,
  MatrixFloat& surfaceLocation,
  MatrixFloat& output
) {

  assert(surfaceNormalVector.getWidth() == 3);
  assert(surfaceNormalVector.getHeight() == 1);
  assert(surfaceLocation.getWidth() == 3);
  assert(surfaceLocation.getHeight() == 1);
  
  distanceMatrix.set(0, 0, surfaceLocation.get(0, 0) - worldPoint.get(0, 0));
  distanceMatrix.set(1, 0, surfaceLocation.get(1, 0) - worldPoint.get(1, 0));
  distanceMatrix.set(2, 0, surfaceLocation.get(2, 0) - worldPoint.get(2, 0));

  squareDistance = distanceMatrix.squareSize<double>();

  distance = sqrt(squareDistance);

  distanceMatrixHat.set(0, 0, float(double(distanceMatrix.get(0, 0)) / distance));
  distanceMatrixHat.set(1, 0, float(double(distanceMatrix.get(1, 0)) / distance));
  distanceMatrixHat.set(2, 0, float(double(distanceMatrix.get(2, 0)) / distance));

  angleFactor = distanceMatrixHat.dotProduct(surfaceNormalVector) * -1.0;

  distanceFactor = cameraFieldOfView / ((long long)(squareDistance + distance + 1));

  lightIntensity = Math::clamp(distanceFactor * angleFactor * attenuation, 0.0, 1.0);

  if (lightIntensity > 0) {
    output.set(0, 0, float(lightIntensity * colorR));
    output.set(1, 0, float(lightIntensity * colorG));
    output.set(2, 0, float(lightIntensity * colorB));
  }
  
}