#include "./MonkeyScene.h"

#include "../base_scene/BaseScene.h"
#include "../../data_access_point/DataAccessPoint.h"
#include "../../file_system/FileSystem.h"
#include "../../utils/path/Path.h"
#include "../../shaders/directional_light/DirectionalLight.h"
#include "../../shaders/ambient_light/AmbientLight.h"

MonkeyScene::MonkeyScene(OpenGL& gl)
  :
  BaseScene(gl, "MonkeyScene"),
  cameraInstance(
    gl,
    Matrix3X1Float(),
    Matrix3X1Float(),
    DataAccessPoint::getInstance()->getAppScreenWidth(),
    DataAccessPoint::getInstance()->getAppScreenHeight(),
    "Monkey main camera"
  ),
  ambientLight(std::make_unique<AmbientLight>(0.05f, 0.05f, 0.05f)),
  metalColor(std::make_unique<ColorTexture>(171.0f / 255.0f, 171.0f / 255.0f, 171.0f / 255.0f)),
  pointLight(std::make_unique<PointLight>(
    1.1f,
    255.0f / 256.0f, 214.0f / 256.0f, 170.0f / 256.0f,
    float(DataAccessPoint::getInstance()->getAppScreenWidth()) / 2.0f
    - float(DataAccessPoint::getInstance()->getAppScreenWidth()) / 5.0f,
    float(DataAccessPoint::getInstance()->getAppScreenHeight()) / 2.0f
    - float(DataAccessPoint::getInstance()->getAppScreenHeight()) / 5.0f,
    50.0f,
    1.7f,
    float(1.0 / double(100)),
    float(1.0 / double(10000)),
    30
  )),
  pip(cameraInstance)
{

  {//Creating shape
    auto scaleFactor = float(DataAccessPoint::getInstance()->getAppScreenWidth()) / 50.0f;
    monkey = FileSystem::loadObject(
      Path::generateAssetPath("monkey", ".obj"),
      Constants::LightPrecision::perSurface,
      metalColor.get(),
      true,
      Shape3d::NormalType::fileDefault,
      false
    );
    monkey->transformX(float(DataAccessPoint::getInstance()->getAppScreenWidth()) / 2.0f);
    monkey->transformY(float(DataAccessPoint::getInstance()->getAppScreenHeight()) / 2.0f);
    monkey->transformZ(100.0f);
    monkey->scale(scaleFactor);
    monkey->setSpecularIntensity(0.6f);
  }

  pip.assignAmbientLight(ambientLight.get());
  pip.assignPointLight(pointLight.get());
  pip.assignShapes(monkey.get());

  cameraInstance.rotateXYZ(0, Math::piDouble, 0);
}

void MonkeyScene::update(double deltaTime) {
#ifdef __DESKTOP__
  {//We rotate light by keyboard
    if (useKeyEvent(Constants::Buttons::keyA)) {
      pointLight->transformX(float(deltaTime * lightTransformSpeed * -1.0f));
    }
    if (useKeyEvent(Constants::Buttons::keyD)) {
      pointLight->transformX(float(deltaTime * lightTransformSpeed));
    }
    if (useKeyEvent(Constants::Buttons::keyW)) {
      pointLight->transformY(float(deltaTime * lightTransformSpeed));
    }
    if (useKeyEvent(Constants::Buttons::keyS)) {
      pointLight->transformY(float(deltaTime * lightTransformSpeed * -1.0));
    }
    if (useKeyEvent(Constants::Buttons::keyC)) {
      pointLight->transformZ(float(deltaTime * lightTransformSpeed * -1.0 * 0.5));
    }
    if (useKeyEvent(Constants::Buttons::keyV)) {
      pointLight->transformZ(float(deltaTime * lightTransformSpeed * 1.0 * 0.5));
    }
  }
  {//Rotating shape by keyboard
    shapeRotationX = 0.0f;
    shapeRotationY = 0.0f;
    shapeRotationZ = 0.0f;
    if (useKeyEvent(Constants::Buttons::keyI)) {
      shapeRotationX += float(1.0 * shapeRotationSpeed * deltaTime);
    }
    if (useKeyEvent(Constants::Buttons::keyY)) {
      shapeRotationX += float(-1.0 * shapeRotationSpeed * deltaTime);
    }
    if (useKeyEvent(Constants::Buttons::keyK)) {
      shapeRotationY += float(1.0 * shapeRotationSpeed * deltaTime);
    }
    if (useKeyEvent(Constants::Buttons::keyH)) {
      shapeRotationY += float(-1.0 * shapeRotationSpeed * deltaTime);
    }
    if (useKeyEvent(Constants::Buttons::keyU)) {
      shapeRotationZ += float(1.0 * shapeRotationSpeed * deltaTime);
    }
    if (useKeyEvent(Constants::Buttons::keyJ)) {
      shapeRotationZ += float(-1.0 * shapeRotationSpeed * deltaTime);
    }
    if (shapeRotationX != 0 || shapeRotationY != 0 || shapeRotationZ != 0) {
      //We need separate rotation methods as well
      monkey->rotateXYZ(shapeRotationX, shapeRotationY, shapeRotationZ);
    }
  }
#endif
  pip.update(deltaTime);
}

void MonkeyScene::render(double deltaTime) {
  cameraInstance.render(deltaTime);
}