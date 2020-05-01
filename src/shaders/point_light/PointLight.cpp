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
    Constants::LightPrecision::none,
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
  const Camera& camera,
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
  lightVectorPlaceholder.assign(worldPoint);
  lightVectorPlaceholder.minus(surfaceLocation);
  lightVectorPlaceholder.setW(0.0f);
  /*lightVectorPlaceholder.set(0, 0, worldPoint.get(0, 0) - surfaceLocation.get(0, 0));
  lightVectorPlaceholder.set(1, 0, worldPoint.get(1, 0) - surfaceLocation.get(1, 0));
  lightVectorPlaceholder.set(2, 0, worldPoint.get(2, 0) - surfaceLocation.get(2, 0));*/

  double squareDistance = lightVectorPlaceholder.squareSize<double>();
  assert(std::isnan(squareDistance) == false);

  double distance = sqrt(squareDistance);
  assert(std::isnan(distance) == false);
  //Light hat
  lightVectorHatPlaceholder.setX(float(double(lightVectorPlaceholder.getX()) / distance));
  lightVectorHatPlaceholder.setY(float(double(lightVectorPlaceholder.getY()) / distance));
  lightVectorHatPlaceholder.setZ(float(double(lightVectorPlaceholder.getZ()) / distance));

  double angleFactor = lightVectorHatPlaceholder.dotProduct(surfaceNormalVector);
  assert(std::isnan(angleFactor) == false);
  assert(angleFactor >= -1.0 && angleFactor <= 1.0);

  if (angleFactor <= 0) {
    output.setX(0.0f);
    output.setY(0.0f);
    output.setZ(0.0f);
    output.setW(0.0f);
    return;
  }

  double distanceFactor = 1.0 / (
    squareDistance * quadricAttenuation +
    distance * linearAttenuation +
    constantAttenuation
  );
  assert(std::isnan(distanceFactor) == false);
  //TODO We need memory pool
  //TODO I need a memory manger that I request objects from it
  //Like matrix& a = Memory.RequestMatrix; and then I free it

  double lightIntensity = angleFactor * distanceFactor;
  assert(std::isnan(lightIntensity) == false);
  assert(lightIntensity >= 0 && lightIntensity <= 1);

  if (specularIntensity > 0) {
    //LightReflection
    lightReflectionPlaceholder.assign(surfaceNormalVector);
    lightReflectionPlaceholder.multiply(float(2.0 * angleFactor));
    lightReflectionPlaceholder.minus(lightVectorHatPlaceholder);

    //Camera vector
    camera.generateCameraToPointVector(
      worldPoint,
      cameraVectorPlaceholder
    );

    cameraVectorPlaceholder.hat(cameraVectorHatPlaceholder);

    lightReflectionPlaceholder.hat(lightReflectionHatPlaceholder);

    double specularDotProduct = double(
      -1.0 * lightReflectionHatPlaceholder.dotProduct(cameraVectorHatPlaceholder)
    );

    assert(std::isnan(specularDotProduct) == false);
    assert(specularDotProduct >= -1);
    assert(specularDotProduct <= 1);

    if (specularDotProduct > 0) {
      //TODO Write a pow function to constantly cast value to float to prevent overflow
      double specularHighlight = specularIntensity * pow(
        specularDotProduct,
        specularPower
      );
      assert(std::isnan(specularHighlight) == false);
      lightIntensity += float(specularHighlight);
      assert(std::isnan(lightIntensity) == false);
    }
  }

  lightIntensity = Math::clamp(lightIntensity, 0, 1);

  output.assign(color);

  output.multiply(float(lightIntensity));

  output.setW(0.0f);

}

void PointLight::update(Camera& camera) {
  //TOOD Merge all update methods with shape pipeLine
  worldPoint.assign(sphere->transformValue);
  
  worldPoint.multiply(camera.transformInverseMatrix);

  worldPoint.minus(camera.screenCenter);

  worldPoint.multiply(camera.rotationInverseMatrix);

  float scaleValue = camera.scaleBasedOnZDistance(worldPoint.getZ());

  worldPoint.setX(worldPoint.getX() * scaleValue);

  worldPoint.setY(worldPoint.getY() * scaleValue);

  worldPoint.sum(camera.screenCenter);

}

Shape3d* PointLight::getShape() {
  return sphere.get();
}