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
    Matrix3X1Float(),
    Matrix3X1Float(),
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
  sphere = ShapeGenerator::sphere(
    Constants::LightPrecision::perSurface,
    sphereColor.get(),
    radius,
    12 * 2,
    24 * 2,
    Vec3DFloat(
      DataAccessPoint::getInstance()->getAppScreenWidth() / 2.0f,
      DataAccessPoint::getInstance()->getAppScreenHeight() / 2.0f,
      100.0f
    ),
    Vec3DFloat(0.0f, Math::piFloat/2.0f, 0.0f),
    1.0f
  );

  pipeLine.assignAmbientLight(ambientLight.get());
  pipeLine.assignDirectionalLight(directionalLight.get());
  pipeLine.assignShape(sphere.get());
  
}

void SphereScene::update(double deltaTime) {
#ifdef __DESKTOP__
  {//We rotate light by keyboard
    lightRotationX = 0.0f;
    lightRotationY = 0.0f;
    lightRotationZ = 0.0f;
    if (useScanCodeEvent(GLFW_KEY_A)) {
      lightRotationX += float(deltaTime * lightTransformSpeed * -1.0f);
    }
    if (useScanCodeEvent(GLFW_KEY_D)) {
      lightRotationX += float(deltaTime * lightTransformSpeed);
    }
    if (useScanCodeEvent(GLFW_KEY_W)) {
      lightRotationY += float(deltaTime * lightTransformSpeed);
    }
    if (useScanCodeEvent(GLFW_KEY_S)) {
      lightRotationY += float(deltaTime * lightTransformSpeed * -1.0);
    }
    if (useScanCodeEvent(GLFW_KEY_C)) {
      lightRotationZ += float(deltaTime * lightTransformSpeed * -1.0 * 0.5);
    }
    if (useScanCodeEvent(GLFW_KEY_V)) {
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