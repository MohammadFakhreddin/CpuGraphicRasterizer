#include "./BunnyScene.h"

#include "../base_scene/BaseScene.h"
#include "../../data_access_point/DataAccessPoint.h"
#include "../../file_system/FileSystem.h"
#include "../../utils/path/Path.h"

BunnyScene::BunnyScene(OpenGL& gl)
  : 
  BaseScene(gl, "BunnyScene"),
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
    "Bunny main camera"
  )
{
  whiteColor = std::make_unique<ColorTexture>(
    1.0f,1.0f,1.0f  
  );
  {//Creating shape
    auto scaleFactor = float(DataAccessPoint::getInstance()->getAppScreenWidth()) / 4.0f;
    shape = FileSystem::loadObject(
      Path::generateAssetPath("bunny", ".obj"),
      (std::unique_ptr<Texture>&)whiteColor,
      true,
      Shape3d::NormalType::smooth,
      false
    );
    shape->transformX(float(DataAccessPoint::getInstance()->getAppScreenWidth()) / 2.0f);
    shape->transformY(float(DataAccessPoint::getInstance()->getAppScreenHeight()) / 2.0f);
    shape->transformZ(cameraInitialZLocation - 100.0f);
    shape->scale(scaleFactor);
  }
  {//Creating light source
    lightSources.emplace_back(
      std::make_unique<DirectionalLight>(1.0f,1.0f,1.0f,0.2f));
    light = (DirectionalLight*)lightSources.at(lightSources.size()-1).get();
  }
}

void BunnyScene::update(double deltaTime) {
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
  {//Rotating shape by keyboard
    if (useKeyEvent(Constants::Buttons::keyI)) {
      shape->rotateZ(float(1.0 * shapeRotationSpeed * deltaTime));
    }
    if (useKeyEvent(Constants::Buttons::keyY)) {
      shape->rotateZ(float(-1.0 * shapeRotationSpeed * deltaTime));
    }
    if (useKeyEvent(Constants::Buttons::keyK)) {
      shape->rotateY(float(1.0 * shapeRotationSpeed * deltaTime));
    }
    if (useKeyEvent(Constants::Buttons::keyH)) {
      shape->rotateY(float(-1.0 * shapeRotationSpeed * deltaTime));
    }
    if (useKeyEvent(Constants::Buttons::keyU)) {
      shape->rotateX(float(1.0 * shapeRotationSpeed * deltaTime));
    }
    if (useKeyEvent(Constants::Buttons::keyJ)) {
      shape->rotateX(float(-1.0 * shapeRotationSpeed * deltaTime));
    }
  }
#endif
  {//Temporary code for auto rotation
   shape->rotateY(float(-1.0f * shapeRotationSpeed * deltaTime * 0.1f));
   shape->rotateX(float(-1.0f * shapeRotationSpeed * deltaTime * 0.1f));
   shape->rotateZ(float(-1.0f * shapeRotationSpeed * deltaTime * 0.1f));
  }
  {//Updating light
    for (unsigned int i = 0; i < lightSources.size(); i++) {
      lightSources.at(i)->update(deltaTime, cameraInstance);
    }
  }
  shape->update(deltaTime, cameraInstance, lightSources);
}

void BunnyScene::render(double deltaTime) {
  cameraInstance.render(deltaTime);
}