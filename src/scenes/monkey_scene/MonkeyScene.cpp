#include "./MonkeyScene.h"

#include "../base_scene/BaseScene.h"
#include "../../data_access_point/DataAccessPoint.h"
#include "../../file_system/FileSystem.h"
#include "../../utils/path/Path.h"
#include "../../shaders/directional_light/DirectionalLight.h"
#include "../../shaders/ambient_light/AmbientLight.h"

MonkeyScene::MonkeyScene(OpenGL& gl) 
  :
  BaseScene(gl,"MonkeyScene"),
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
    "Monkey main camera"
  )
{
  
  whiteColor = std::make_unique<ColorTexture>(
    1.0f, 1.0f, 1.0f
  );
  
  {//Creating shape
    auto scaleFactor = float(DataAccessPoint::getInstance()->getAppScreenWidth()) / 10.0f;
    shape = FileSystem::loadObject(
      Path::generateAssetPath("monkey", ".obj"),
      Surface::LightPrecision::perSurface,
      (std::unique_ptr<Texture>&)whiteColor,
      true,
      Shape3d::NormalType::fileDefault,
      false
    );
    shape->transformX(float(DataAccessPoint::getInstance()->getAppScreenWidth()) / 2.0f);
    shape->transformY(float(DataAccessPoint::getInstance()->getAppScreenHeight()) / 2.0f);
    shape->transformZ(cameraInitialZLocation - 500.0f);
    shape->scale(scaleFactor);
  }

  {//Creating light source
    //lightSources.emplace_back(std::make_unique<DirectionalLight>(1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f));
    lightSources.emplace_back(std::make_unique<AmbientLight>(0.05f, 0.05f, 0.05f));
    lightSources.emplace_back(std::make_unique<PointLight>(
      2.0f, 
      255.0f/256.0f, 214.0f/256.0f, 170.0f/256.0f,
      float(DataAccessPoint::getInstance()->getAppScreenWidth()) / 2.0f 
        - float(DataAccessPoint::getInstance()->getAppScreenWidth()) / 5.0f,
      float(DataAccessPoint::getInstance()->getAppScreenHeight()) / 2.0f
        - float(DataAccessPoint::getInstance()->getAppScreenHeight()) / 5.0f,
      cameraInitialZLocation - 400.0f,
      1.0f/cameraInstance.getCamerFieldOfView() * 1000,
      1.0f/cameraInstance.getCamerFieldOfView() * 1000,
      1.0f/cameraInstance.getCamerFieldOfView() * 1000,
      4.0f,
      2
    ));
    light = (PointLight*)lightSources.back().get();
  }
}

void MonkeyScene::update(double deltaTime) {
#ifdef __DESKTOP__
  {//We rotate light by keyboard
    if (useKeyEvent(Constants::Buttons::keyA)) {
      light->transformX(float(deltaTime * lightTransformSpeed * -1.0f));
    }
    if (useKeyEvent(Constants::Buttons::keyD)) {
      light->transformX(float(deltaTime * lightTransformSpeed));
    }
    if (useKeyEvent(Constants::Buttons::keyW)) {
      light->transformY(float(deltaTime * lightTransformSpeed));
    }
    if (useKeyEvent(Constants::Buttons::keyS)) {
      light->transformY(float(deltaTime * lightTransformSpeed * -1.0));
    }
    if (useKeyEvent(Constants::Buttons::keyC)) {
      light->transformZ(float(deltaTime * lightTransformSpeed * -1.0 * 0.5));
    }
    if (useKeyEvent(Constants::Buttons::keyV)) {
      light->transformZ(float(deltaTime * lightTransformSpeed * 1.0 * 0.5));
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
      //We need seperate rotation methods as well
      shape->rotateXYZ(shapeRotationX, shapeRotationY, shapeRotationZ);
    }
  }
#endif
  shape->rotateXYZ(
    float(-1.0f * shapeRotationSpeed * deltaTime * 0.1f),
    float(-1.0f * shapeRotationSpeed * deltaTime * 0.1f),
    float(-1.0f * shapeRotationSpeed * deltaTime * 0.1f)
  );
  {//Updating light
    for (auto& light:lightSources) {
      light->update(deltaTime, cameraInstance);
    }
  }
  shape->update(deltaTime, cameraInstance, lightSources);
}

void MonkeyScene::render(double deltaTime) {
  cameraInstance.render(deltaTime);
}