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
    12 * 1,
    24 * 1,
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
    if (useKeyEvent(Constants::Buttons::keyA)) {
      light->rotateX(
        float(deltaTime * lightTransformSpeed * -1.0f)
      );
    }
    if (useKeyEvent(Constants::Buttons::keyD)) {
      light->rotateX(
        float(deltaTime * lightTransformSpeed)
      );
    }
    if (useKeyEvent(Constants::Buttons::keyW)) {
      light->rotateY(
        float(deltaTime * lightTransformSpeed)
      );
    }
    if (useKeyEvent(Constants::Buttons::keyS)) {
      light->rotateY(
        float(deltaTime * lightTransformSpeed * -1.0)
      );
    }
    if (useKeyEvent(Constants::Buttons::keyC)) {
      light->rotateZ(
        float(deltaTime * lightTransformSpeed * -1.0 * 0.5)
      );
    }
    if (useKeyEvent(Constants::Buttons::keyV)) {
      light->rotateZ(
        float(deltaTime * lightTransformSpeed * 1.0 * 0.5)
      );
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
    sphere->rotateY(float(-1.0f * shapeRotationSpeed * deltaTime * 0.1f));
    sphere->rotateX(float(-1.0f * shapeRotationSpeed * deltaTime * 0.1f));
    sphere->rotateZ(float(-1.0f * shapeRotationSpeed * deltaTime * 0.1f));
  }
  sphere->update(deltaTime, cameraInstance, lightSources);
}

void SphereScene::render(double deltaTime) {
  cameraInstance.render(deltaTime);
}