#include "../point_light/PointLight.h"

#include <cassert>

#include "../../data_types/MatrixTemplate.h"
#include "../../camera/Camera.h"
#include "../../texture/ColorTexture/ColorTexture.h"
#include "../../3d/shape/Shape3d.h"
#include "../../3d/models/ShapeGenerator.h"


PointLight::PointLight(
  const float& radius,
  const float& colorR,
  const float& colorG,
  const float& colorB,
  const float& transformX,
  const float& transformY,
  const float& transformZ,
  const float& constantAttenuation,
  const float& linearAttenuation,
  const float& quadricAttenuation,
  const unsigned int& specularPower
)
  :
  PointLight(
    radius,
    Matrix3X1Float({ colorR,colorG,colorB }),
    Matrix3X1Float({ transformX ,transformY ,transformZ }),
    constantAttenuation,
    linearAttenuation,
    quadricAttenuation,
    specularPower
  )
{
}

PointLight::PointLight(
  const float& paramRadius,
  const Matrix3X1Float& paramColor,
  const Matrix3X1Float& paramTransform,
  const float& paramConstantAttenuation,
  const float& paramLinearAttenuation,
  const float& paramQuadricAttenuation,
  const unsigned int& paramSpecularPower
)
  :
  constantAttenuation(paramConstantAttenuation),
  linearAttenuation(paramLinearAttenuation),
  quadricAttenuation(paramQuadricAttenuation),
  specularPower(paramSpecularPower)
{

  assert(paramRadius >= 0);
  
  color.assign(paramColor);
  
  assert(color.getR() >= 0 && color.getR() <= 1.0f);
  assert(color.getG() >= 0 && color.getG() <= 1.0f);
  assert(color.getB() >= 0 && color.getB() <= 1.0f);
  assert(constantAttenuation > 0);
  assert(linearAttenuation > 0);
  assert(quadricAttenuation > 0);
  assert(specularPower >= 2);

  lightColor = std::make_unique<ColorTexture>(paramColor);

  sphere = ShapeGenerator::sphere(
    Constants::LightPrecision::perSurface,
    lightColor.get(),
    paramRadius,
    12 * 2, 24 * 2,
    Vec3DFloat(0.0f, 0.0f, 0.0f),
    Vec3DFloat(0.0f, 0.0f, 0.0f),
    1.0f
  );

  transformX(paramTransform.getX());
  transformY(paramTransform.getY());
  transformZ(paramTransform.getZ());

}

void PointLight::transformX(const float& value) {
  sphere->transformX(value);
}

void PointLight::transformY(const float& value) {
  sphere->transformY(value);
}

void PointLight::transformZ(const float& value) {
  sphere->transformZ(value);
}

//I use y = 1/(x*x + x + 1)
void PointLight::computeLightIntensity(
  const Camera& cameraInstance,
  const Matrix4X1Float& surfaceNormalVector,
  const Matrix4X1Float& surfaceLocation,
  const float& specularIntensity,
  Matrix4X1Float& lightVectorPlaceholder,
  Matrix4X1Float& lightVectorHatPlaceholder,
  Matrix4X1Float& lightReflectionPlaceholder,
  Matrix4X1Float& lightReflectionHatPlaceholder,
  Matrix4X1Float& cameraVectorPlaceholder,
  Matrix4X1Float& cameraVectorHatPlaceholder,
  Matrix4X1Float& output
) const {

  //Light vector
  lightVectorPlaceholder.set(0, 0, worldPoint.get(0, 0) - surfaceLocation.get(0, 0));
  lightVectorPlaceholder.set(1, 0, worldPoint.get(1, 0) - surfaceLocation.get(1, 0));
  lightVectorPlaceholder.set(2, 0, worldPoint.get(2, 0) - surfaceLocation.get(2, 0));

  double squareDistance = output.squareSize<double>();

  double distance = sqrt(squareDistance);
  //Light hat
  lightVectorHatPlaceholder.set(0, 0, float(double(lightVectorPlaceholder.get(0, 0)) / distance));
  lightVectorHatPlaceholder.set(1, 0, float(double(lightVectorPlaceholder.get(1, 0)) / distance));
  lightVectorHatPlaceholder.set(2, 0, float(double(lightVectorPlaceholder.get(2, 0)) / distance));

  double angleFactor = output.dotProduct(surfaceNormalVector);

  if (angleFactor <= 0) {
    output.set(0, 0, 0.0f);
    output.set(1, 0, 0.0f);
    output.set(2, 0, 0.0f);
    return;
  }

  double distanceFactor = 1.0f / (
    squareDistance * quadricAttenuation +
    distance * linearAttenuation +
    constantAttenuation
  );
  //TODO We need memory pool
  //TODO I need a memory manger that I request objects from it
  //Like matrix& a = Memory.RequestMatrix; and then I free it
  double lightIntensity = distanceFactor * angleFactor;
  assert(lightIntensity >= 0 && lightIntensity <= 1);

  //LightReflection
  lightReflectionPlaceholder.assign(surfaceNormalVector);
  lightReflectionPlaceholder.multiply(2 * angleFactor);
  lightReflectionPlaceholder.minus(lightVectorHatPlaceholder);

  //Camera vector
  cameraInstance.generateCameraToPointVector(
    worldPoint, 
    cameraVectorPlaceholder
  );

  cameraVectorPlaceholder.hat(cameraVectorHatPlaceholder);

  lightReflectionPlaceholder.hat(lightReflectionHatPlaceholder);

  double specularDotProduct = double(
    -1.0 * lightReflectionHatPlaceholder.dotProduct(cameraVectorHatPlaceholder)
  );
  
  assert(specularDotProduct >= -1);
  assert(specularDotProduct <= 1);
  
  if (specularDotProduct > 0) {
    //TODO Write a pow function to constantly cast value to float to prevent overflow
    double specularHighlight = specularIntensity * pow(
      specularDotProduct,
      specularPower
    );
    lightIntensity += float(specularHighlight);
  }

  lightIntensity = Math::clamp(lightIntensity, 0, 1);

  output.assign(color);

  output.multiply(lightIntensity);

}

Shape3d* PointLight::getShape() {
  return sphere.get();
}