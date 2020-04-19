#include "./PlantScene.h"

#include "../base_scene/BaseScene.h"
#include "../../data_access_point/DataAccessPoint.h"
#include "../../file_system/FileSystem.h"
#include "../../utils/path/Path.h"
#include "../../shaders/directional_light/DirectionalLight.h"
#include "../../shaders/ambient_light/AmbientLight.h"

PlantScene::PlantScene(OpenGL& gl)
  :
  BaseScene(gl, "PlantScene"),
  cameraInstance(
    gl,
    MatrixFloat(3, 1, 0.0f),
    MatrixFloat(3, 1, 0.0f),
    DataAccessPoint::getInstance()->getAppScreenWidth(),
    DataAccessPoint::getInstance()->getAppScreenHeight(),
    "Plant main camera"
  ),
  ambientLight(std::make_unique<AmbientLight>(0.2f, 0.2f, 0.2f)),
  directionalLight(std::make_unique<DirectionalLight>(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f)),
  colorTexture(std::make_unique<ColorTexture>(1.0f, 1.0f, 1.0f)),
  pip(cameraInstance)
{

  {//Creating shape
    shape = FileSystem::loadObject(
      Path::generateAssetPath("plant",".obj"),
      Constants::LightPrecision::perSurface,
      (std::unique_ptr<Texture>&)colorTexture,
      false,
      Shape3d::NormalType::fileDefault,
      false
    );
    shape->transformX(float(DataAccessPoint::getInstance()->getAppScreenWidth()) * 0.5f);
    shape->transformY(float(DataAccessPoint::getInstance()->getAppScreenHeight()) * 0.25f);
    shape->transformZ(cameraInitialZLocation - 100.0f);
    shape->scale(5.0f);
  }

  pip.assignAmbientLight(ambientLight.get());
  pip.assignDirectionalLight(directionalLight.get());
  pip.assignShapes(shape.get());

}

void PlantScene::update(double deltaTime) {
#ifdef __DESKTOP__
  {//We rotate light by keyboard
    lightRotationX = 0.0f;
    lightRotationY = 0.0f;
    lightRotationZ = 0.0f;
    if (useKeyEvent(Constants::Buttons::keyA)) {
      lightRotationX += float(deltaTime * lightRotationSpeed * -1.0f);
    }
    if (useKeyEvent(Constants::Buttons::keyD)) {
      lightRotationX += float(deltaTime * lightRotationSpeed);
    }
    if (useKeyEvent(Constants::Buttons::keyW)) {
      lightRotationY += float(deltaTime * lightRotationSpeed);
    }
    if (useKeyEvent(Constants::Buttons::keyS)) {
      lightRotationY += float(deltaTime * lightRotationSpeed * -1.0);
    }
    if (useKeyEvent(Constants::Buttons::keyC)) {
      lightRotationZ += float(deltaTime * lightRotationSpeed * -1.0 * 0.5);
    }
    if (useKeyEvent(Constants::Buttons::keyV)) {
      lightRotationZ += float(deltaTime * lightRotationSpeed * 1.0 * 0.5);
    }
    if (lightRotationX != 0 || lightRotationY != 0 || lightRotationZ != 0) {
      directionalLight->rotateXYZ(lightRotationX, lightRotationY, lightRotationZ);
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
  pip.update(deltaTime);
}

void PlantScene::render(double deltaTime) {
  cameraInstance.render(deltaTime);
}
