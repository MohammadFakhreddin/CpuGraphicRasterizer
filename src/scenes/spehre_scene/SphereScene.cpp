#include "./SphereScene.h"

#include <memory>

#include "../../data_access_point/DataAccessPoint.h"
#include "../../3d_models/ShapeGenerator.h"
#include "../../shaders/directional_light/DirectionalLight.h"

SphereScene::SphereScene(OpenGL& gl)
  :
  BaseScene(gl, "SphereScene"),
  cameraInstance(
    gl,
    cameraInitialMaximumFov,
    0,
    0,
    cameraInitialZLocation,
    0,
    0,
    0,
    DataAccessPoint::getInstance()->getAppScreenWidth(),
    DataAccessPoint::getInstance()->getAppScreenHeight(),
    "Plant main camera"
  )
{
  whiteColor = std::make_unique<ColorTexture>(1.0f, 1.0f, 1.0f);
  auto radius = float((25.0f / 800.f) * DataAccessPoint::getInstance()->getAppScreenWidth());
  sphere = ShapeGenerator::sphere(
    (std::unique_ptr<Texture>&)whiteColor,
    radius,
    12 * 2,
    24 * 2,
    Vec3DFloat(1.0f, 1.0f, 1.0f),
    Vec3DFloat(
      DataAccessPoint::getInstance()->getAppScreenWidth() / 2.0f,
      DataAccessPoint::getInstance()->getAppScreenHeight() / 2.0f,
      cameraInitialZLocation - 100
    ),
    Vec3DFloat(0.0f, 0.0f, 0.0f),
    1.0f
  );

  lightSources.emplace_back(std::make_unique<DirectionalLight>(-1.0f, -1.0f, -1.0f, 0.05f));
  light = (DirectionalLight*)lightSources.back().get();
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
      light->rotateXYZ(lightRotationX, lightRotationY, lightRotationZ);
    }
  }
#endif // __DESKTOP__
  {//Light sources
    if (!lightSources.empty()) {
      for (auto& lightSource : lightSources) {
        lightSource->update(deltaTime, cameraInstance);
      }
    }
  }
  {//Temporary code for auto rotation
    sphere->rotateXYZ(
      float(-1.0f * shapeRotationSpeed * deltaTime * 0.1f), 
      float(-1.0f * shapeRotationSpeed * deltaTime * 0.1f), 
      float(-1.0f * shapeRotationSpeed * deltaTime * 0.1f)
    );
  }
  sphere->update(deltaTime, cameraInstance, lightSources);
}

void SphereScene::render(double deltaTime) {
  cameraInstance.render(deltaTime);
}