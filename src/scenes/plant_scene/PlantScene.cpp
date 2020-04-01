#include "./PlantScene.h"

#include "../base_scene/BaseScene.h"
#include "../../data_access_point/DataAccessPoint.h"
#include "../../file_system/FileSystem.h"
#include "../../utils/path/Path.h"

PlantScene::PlantScene(OpenGL& gl)
  :
  BaseScene(gl, "PlantScene"),
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
  {//Creating shape
    auto scaleFactor = float(DataAccessPoint::getInstance()->getAppScreenWidth()) / 50.0f;
    shape = FileSystem::loadObjectWithColor(
       Path::generateAssetPath("plant",".obj"),
       Vec3DFloat(1.0f,1.0f,1.0f),
       false
    );
    shape->transformX(float(DataAccessPoint::getInstance()->getAppScreenWidth()) * 0.5f);
    shape->transformY(float(DataAccessPoint::getInstance()->getAppScreenHeight()) * 0.25f);
    shape->transformZ(cameraInitialZLocation - 100.0f);
    shape->scale(5.0f);
  }
  {//Creating light source
    lightSources.emplace_back(
      std::make_unique<DiffuseLight>(
        float(DataAccessPoint::getInstance()->getAppScreenWidth()) / 2.0f,
        float(DataAccessPoint::getInstance()->getAppScreenHeight()),
        cameraInitialZLocation - 1.0f
      )
    );
    light = (DiffuseLight*)lightSources.at(lightSources.size() - 1).get();
  }
}

void PlantScene::update(double deltaTime) {
#ifdef __DESKTOP__
  {//We rotate light by keyboard
    if (useKeyEvent(Constants::Buttons::keyA)) {
      light->transformX(
        float(deltaTime * lightTransformSpeed * -1.0f)
        );
    }
    if (useKeyEvent(Constants::Buttons::keyD)) {
      light->transformX(
        float(deltaTime * lightTransformSpeed)
        );
    }
    if (useKeyEvent(Constants::Buttons::keyW)) {
      light->transformY(
        float(deltaTime * lightTransformSpeed)
        );
    }
    if (useKeyEvent(Constants::Buttons::keyS)) {
      light->transformY(
        float(deltaTime * lightTransformSpeed * -1.0)
        );
    }
    if (useKeyEvent(Constants::Buttons::keyC)) {
      light->transformZ(
        float(deltaTime * lightTransformSpeed * -1.0 * 0.5)
        );
    }
    if (useKeyEvent(Constants::Buttons::keyV)) {
      light->transformZ(
        float(deltaTime * lightTransformSpeed * 1.0 * 0.5)
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

void PlantScene::render(double deltaTime) {
  cameraInstance.render(deltaTime);
}