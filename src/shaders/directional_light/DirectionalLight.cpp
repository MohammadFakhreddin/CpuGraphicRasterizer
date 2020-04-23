#include "./DirectionalLight.h"

#include <cassert>

#include "../../data_types/MatrixTemplate.h"
#include "../../camera/Camera.h"

DirectionalLight::DirectionalLight(
  const Matrix3X1Float& color,
  const Matrix3X1Float& direction
)
  :
  DirectionalLight(
    color.getR(),
    color.getG(),
    color.getB(),
    direction.getX(),
    direction.getY(),
    direction.getZ()
  )
{}

DirectionalLight::DirectionalLight(
  const float& colorR,
  const float& colorG,
  const float& colorB,
  const float& directionX,
  const float& directionY,
  const float& directionZ
)
{

  assert(colorR >= 0 && colorR <= 1.0f);
  assert(colorG >= 0 && colorG <= 1.0f);
  assert(colorB >= 0 && colorB <= 1.0f);

  color.set(0, 0, colorR);
  color.set(1, 0, colorG);
  color.set(2, 0, colorB);

  Matrix3X1Float lightDirection;
  lightDirection.setX(directionX);
  lightDirection.setY(directionY);
  lightDirection.setZ(directionZ);

  lightDirection.hat(lightDirectionHat);

  worldLightDirectionHat.assign(lightDirectionHat);

  rotateXYZ(0.0f, 0.0f, 0.0f);

}

//Source: https://en.wikipedia.org/wiki/Computer_graphics_lighting
void DirectionalLight::computeLightIntensity(
  const Matrix4X1Float& normal,
  Matrix4X1Float& output
) const {

  float lightIntensityFactor = worldLightDirectionHat.dotProduct(normal) * -1;

  assert(lightIntensityFactor <= 1);

  output.assign(color);
  output.multiply(lightIntensityFactor);
}

//This method must be called before other objects update
void DirectionalLight::update(const Camera& camera) {

  worldLightDirectionHat.assign(lightDirectionHat);
  worldLightDirectionHat.multiply(rotationXYZMatrix);
  worldLightDirectionHat.multiply(camera.rotationInverseMatrix);

}

void DirectionalLight::rotateXYZ(const float& x, const float& y, const float& z) {

  rotationDegree.setX(rotationDegree.getX() + x);
  rotationDegree.setY(rotationDegree.getY() + y);
  rotationDegree.setZ(rotationDegree.getZ() + z);
  
  Matrix4X4Float::assignRotationXYZ(
    rotationXYZMatrix, 
    rotationDegree.getX(),
    rotationDegree.getY(),
    rotationDegree.getZ()
  );

}
