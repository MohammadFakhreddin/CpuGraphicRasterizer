#include "../point_light/PointLight.h"

#include <cassert>

#include "../../3d_models/ShapeGenerator.h"
#include "../ambient_light/AmbientLight.h"


PointLight::PointLight(
  const float& radius,
  const float& colorR,
  const float& colorG,
  const float& colorB,
  const float& initialTransformX,
  const float& initialTransformY,
  const float& initialTransformZ,
  const float& cameraFieldOfView,
  const float& constantAttenuation,
  const float& linearAttenuation,
  const float& quadricAttenuation,
  const float& specularIntensity,
  const unsigned int& specularPower
) :
  radius(radius),
  colorR(colorR),
  colorG(colorG),
  colorB(colorB),
  cameraFieldOfView(cameraFieldOfView),
  constantAttenuation(constantAttenuation),
  linearAttenuation(linearAttenuation),
  quadricAttenuation(quadricAttenuation),
  specularIntensity(specularIntensity),
  specularPower(specularPower)
{

  assert(radius >= 0);
  assert(colorR >= 0 && colorR <= 1.0f);
  assert(colorG >= 0 && colorG <= 1.0f);
  assert(colorB >= 0 && colorB <= 1.0f);
  assert(constantAttenuation >= 1);
  assert(linearAttenuation > 0);
  assert(quadricAttenuation > 0);
  assert(specularIntensity > 0);
  assert(specularPower >= 2);

  lightColor = std::make_unique<ColorTexture>(colorR, colorG, colorB);

  lightSources.emplace_back(std::make_unique<AmbientLight>(colorR, colorG, colorB));

  sphere = ShapeGenerator::sphere(
    Surface::LightPercision::perSurface,
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
  const MatrixFloat& surfaceNormalVector,
  const MatrixFloat& surfaceLocation,
  MatrixFloat& output
) const {

  assert(surfaceNormalVector.getWidth() == 3);
  assert(surfaceNormalVector.getHeight() == 1);
  assert(surfaceLocation.getWidth() == 3);
  assert(surfaceLocation.getHeight() == 1);
  //Light vector
  output.set(0, 0, surfaceLocation.get(0, 0) - worldPoint.get(0, 0));
  output.set(1, 0, surfaceLocation.get(1, 0) - worldPoint.get(1, 0));
  output.set(2, 0, surfaceLocation.get(2, 0) - worldPoint.get(2, 0));

  double squareDistance = output.squareSize<double>();

  double distance = sqrt(squareDistance);
  //Light hat
  output.set(0, 0, float(double(output.get(0, 0)) / distance));
  output.set(1, 0, float(double(output.get(1, 0)) / distance));
  output.set(2, 0, float(double(output.get(2, 0)) / distance));

  double angleFactor = output.dotProduct(surfaceNormalVector) * -1.0;

  double distanceFactor = cameraFieldOfView / ((long long)
    (
      squareDistance * quadricAttenuation +
      distance * linearAttenuation +
      constantAttenuation
    )
  );
  //TODO We need memory pool
  //TODO I need a memory manger that I request objects from it
  //Like matrix& a = Memory.RequestMatrix; and then I free it
  double lightIntensity = Math::min(distanceFactor * angleFactor, 1.0f);

  //LightReflection
  MatrixFloat lightReflection = MatrixFloat(3, 1, 0.0f);
  lightReflection.assign(surfaceNormalVector);
  lightReflection.multiply(2 * angleFactor);
  lightReflection.minus(output);

  //Camera vector
  MatrixFloat cameraVector = MatrixFloat(3, 1, 0.0f);
  cameraVector.set(2, 0, worldPoint.get(2,0));
  MatrixDouble cameraVectorHat = MatrixDouble(3, 1, 0.0f);
  cameraVector.hat(cameraVectorHat);

  MatrixDouble lightReflectionHat = MatrixDouble(3, 1, 0.0f);

  lightReflection.hat(lightReflectionHat);
  
  double specularDotProduct = double(-1.0 * lightReflectionHat.dotProduct(cameraVectorHat));
  if (specularDotProduct > 0) {
    double specularHighlight = Math::max(specularIntensity * pow(
      specularDotProduct,
      specularPower
    ), 0.0f);
    lightIntensity += float(specularHighlight);
  }

  if (lightIntensity > 0) {
    output.set(0, 0, Math::clamp(float(lightIntensity * colorR), 0.0f, 1.0f));
    output.set(1, 0, Math::clamp(float(lightIntensity * colorG), 0.0f, 1.0f));
    output.set(2, 0, Math::clamp(float(lightIntensity * colorB), 0.0f, 1.0f));
  }
  else {
    output.set(0, 0, 0.0f);
    output.set(1, 0, 0.0f);
    output.set(2, 0, 0.0f);
  }
  
}