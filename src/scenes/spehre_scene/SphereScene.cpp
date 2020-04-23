#include "./SphereScene.h"

#include <memory>

#include "../../data_access_point/DataAccessPoint.h"
#include "../../shaders/directional_light/DirectionalLight.h"
#include "../../shaders/ambient_light/AmbientLight.h"
#include "../../3d/models/ShapeGenerator.h"

SphereScene::SphereScene(OpenGL& gl)
  :
  BaseScene(gl, "SphereScene"),
  camera(
    gl,
    Matrix3X1Float(3, 1, 0.0f),
    Matrix3X1Float(3, 1, 0.0f),
    DataAccessPoint::getInstance()->getAppScreenWidth(),
    DataAccessPoint::getInstance()->getAppScreenHeight(),
    "Plant main camera"
  ),
  sphereColor(std::make_unique<ColorTexture>(1.0f, 1.0f, 1.0f)),
  ambientLight(std::make_unique<AmbientLight>(0.2f, 0.2f, 0.2f)),
  directionalLight(std::make_unique<DirectionalLight>(1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f)),
  pipeLine(camera)
{
  
  auto radius = float((25.0f / 800.f) * DataAccessPoint::getInstance()->getAppScreenWidth());
  //TODO Use simple pointer instead of refrence to unique_ptr
  sphere = ShapeGenerator::sphere(
    Constants::LightPrecision::perSurface,
    sphereColor.get(),
    radius,
    12 * 2,
    24 * 2,
    Vec3DFloat(
      DataAccessPoint::getInstance()->getAppScreenWidth() / 2.0f,
      DataAccessPoint::getInstance()->getAppScreenHeight() / 2.0f,
      cameraInitialZLocation - 100
    ),
    Vec3DFloat(0.0f, 0.0f, 0.0f),
    1.0f
  );

  pipeLine.assignAmbientLight(ambientLight.get());
  pipeLine.assignDirectionalLight(directionalLight.get());
  pipeLine.assignShapes(sphere.get());

}

void SphereScene::update(double deltaTime) {
#ifdef __DESKTOP__
  {//We rotate light by keyboard
    lightRotationX = 0.0f;
    lightRotationY = 0.0f;
    lightRotationZ = 0.0f;
    if (useKeyEvent(Constants::Buttons::keyA)) {
      lightRotationX += float(deltaTime * lightTransformSpeed * -1.0f);
    }
    if (useKeyEvent(Constants::Buttons::keyD)) {
      lightRotationX += float(deltaTime * lightTransformSpeed);
    }
    if (useKeyEvent(Constants::Buttons::keyW)) {
      lightRotationY += float(deltaTime * lightTransformSpeed);
    }
    if (useKeyEvent(Constants::Buttons::keyS)) {
      lightRotationY += float(deltaTime * lightTransformSpeed * -1.0);
    }
    if (useKeyEvent(Constants::Buttons::keyC)) {
      lightRotationZ += float(deltaTime * lightTransformSpeed * -1.0 * 0.5);
    }
    if (useKeyEvent(Constants::Buttons::keyV)) {
      lightRotationZ += float(deltaTime * lightTransformSpeed * 1.0 * 0.5);
    }
    if (lightRotationX != 0 || lightRotationY != 0 || lightRotationZ != 0) {
      directionalLight->rotateXYZ(lightRotationX, lightRotationY, lightRotationZ);
    }
  }
#endif // __DESKTOP__
  pipeLine.update(deltaTime);
}

void SphereScene::render(double deltaTime) {
  camera.render(deltaTime);
}