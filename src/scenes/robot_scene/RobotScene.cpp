#include "./RobotScene.h"

#include "../base_scene/BaseScene.h"
#include "../../data_access_point/DataAccessPoint.h"
#include "../../file_system/FileSystem.h"
#include "../../utils/path/Path.h"
#include "../../shaders/directional_light/DirectionalLight.h"

RobotScene::RobotScene(OpenGL& gl)
  :
  BaseScene(gl, "RobotScene"),
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
    "Robot main camera"
  )
{
  colorTexture = std::make_unique<ColorTexture>(1.0f,1.0f,1.0f);
  {//Creating shape
    auto scaleFactor = float(DataAccessPoint::getInstance()->getAppScreenWidth()) / 50.0f;
    shape = FileSystem::loadObjectWithColor(
      Path::generateAssetPath("robot", ".obj"),
      (std::unique_ptr<Texture>&)colorTexture,
      true,
      true,
      false
      );
    shape->transformX(float(DataAccessPoint::getInstance()->getAppScreenWidth()) / 2.0f);
    shape->transformY(float(DataAccessPoint::getInstance()->getAppScreenHeight()) / 2.0f);
    shape->transformZ(cameraInitialZLocation - 100.0f);
    shape->scale(scaleFactor);
  }
  {//Creating light source
    lightSources.emplace_back(std::make_unique<DirectionalLight>(1.0f,1.0f,1.0f,0.2f));
    light = (DirectionalLight*)lightSources.at(lightSources.size() - 1).get();
  }
}

void RobotScene::update(double deltaTime) {
#ifdef __DESKTOP__
  {//We rotate light by keyboard
    if (useKeyEvent(Constants::Buttons::keyA)) {
      light->rotateX(
        float(deltaTime * lightRotationSpeed * -1.0f)
      );
    }
    if (useKeyEvent(Constants::Buttons::keyD)) {
      light->rotateX(
        float(deltaTime * lightRotationSpeed)
      );
    }
    if (useKeyEvent(Constants::Buttons::keyW)) {
      light->rotateY(
        float(deltaTime * lightRotationSpeed)
      );
    }
    if (useKeyEvent(Constants::Buttons::keyS)) {
      light->rotateY(
        float(deltaTime * lightRotationSpeed * -1.0)
      );
    }
    if (useKeyEvent(Constants::Buttons::keyC)) {
      light->rotateZ(
        float(deltaTime * lightRotationSpeed * -1.0)
      );
    }
    if (useKeyEvent(Constants::Buttons::keyV)) {
      light->rotateZ(
        float(deltaTime * lightRotationSpeed * 1.0)
      );
    }
  }
  {//Rotating shape by keyboard
    if (useKeyEvent(Constants::Buttons::keyU)) {
      shape->rotateZ(float(1.0 * shapeRotationSpeed * deltaTime));
    }
    if (useKeyEvent(Constants::Buttons::keyJ)) {
      shape->rotateZ(float(-1.0 * shapeRotationSpeed * deltaTime));
    }
    if (useKeyEvent(Constants::Buttons::keyK)) {
      shape->rotateY(float(1.0 * shapeRotationSpeed * deltaTime));
    }
    if (useKeyEvent(Constants::Buttons::keyH)) {
      shape->rotateY(float(-1.0 * shapeRotationSpeed * deltaTime));
    }
    if (useKeyEvent(Constants::Buttons::keyI)) {
      shape->rotateX(float(1.0 * shapeRotationSpeed * deltaTime));
    }
    if (useKeyEvent(Constants::Buttons::keyY)) {
      shape->rotateX(float(-1.0 * shapeRotationSpeed * deltaTime));
    }
  }
#endif
  {//Updating light
    for (unsigned int i = 0; i < lightSources.size(); i++) {
      lightSources.at(i)->update(deltaTime, cameraInstance);
    }
  }
  shape->update(deltaTime, cameraInstance, lightSources);
}

void RobotScene::render(double deltaTime) {
  cameraInstance.render(deltaTime);
}