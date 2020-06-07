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
  directionalLight(std::make_unique<DirectionalLight>(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f)),
  cameraInstance(
    gl,
    Matrix3X1Float(),
    Matrix3X1Float(),
    DataAccessPoint::getInstance()->getAppScreenWidth(),
    DataAccessPoint::getInstance()->getAppScreenHeight(),
    "Plant main camera"
  ),
  ambientLight(std::make_unique<AmbientLight>(0.2f, 0.2f, 0.2f)),
  colorTexture(std::make_unique<ColorTexture>(1.0f, 1.0f, 1.0f)),
  pip(cameraInstance)
{

  {//Creating shape
    shape = FileSystem::loadObject(
      Path::generateAssetPath("plant",".obj"),
      Constants::LightPrecision::perSurface,
      colorTexture.get(),
      false,
      Shape3d::NormalType::fileDefault,
      false
    );
    shape->transformX(float(DataAccessPoint::getInstance()->getAppScreenWidth()) * 0.5f);
    shape->transformY(float(DataAccessPoint::getInstance()->getAppScreenHeight()) * 0.49f);
    shape->transformZ(100.0f);
    shape->scale(5.0f);
  }

  pip.assignAmbientLight(ambientLight.get());
  pip.assignDirectionalLight(directionalLight.get());
  pip.assignShape(shape.get());

}

void PlantScene::update(double deltaTime) {
#ifdef __DESKTOP__
  {//We rotate light by keyboard
    lightRotationX = 0.0f;
    lightRotationY = 0.0f;
    lightRotationZ = 0.0f;
    if (useKeyboardCharacterEvent(GLFW_KEY_A)) {
      lightRotationX += float(deltaTime * lightRotationSpeed * -1.0f);
    }
    if (useKeyboardCharacterEvent(GLFW_KEY_D)) {
      lightRotationX += float(deltaTime * lightRotationSpeed);
    }
    if (useKeyboardCharacterEvent(GLFW_KEY_W)) {
      lightRotationY += float(deltaTime * lightRotationSpeed);
    }
    if (useKeyboardCharacterEvent(GLFW_KEY_S)) {
      lightRotationY += float(deltaTime * lightRotationSpeed * -1.0);
    }
    if (useKeyboardCharacterEvent(GLFW_KEY_C)) {
      lightRotationZ += float(deltaTime * lightRotationSpeed * -1.0 * 0.5);
    }
    if (useKeyboardCharacterEvent(GLFW_KEY_V)) {
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
    if (useKeyboardCharacterEvent(GLFW_KEY_I)) {
      shapeRotationX += float(1.0 * shapeRotationSpeed * deltaTime);
    }
    if (useKeyboardCharacterEvent(GLFW_KEY_Y)) {
      shapeRotationX += float(-1.0 * shapeRotationSpeed * deltaTime);
    }
    if (useKeyboardCharacterEvent(GLFW_KEY_K)) {
      shapeRotationY += float(1.0 * shapeRotationSpeed * deltaTime);
    }
    if (useKeyboardCharacterEvent(GLFW_KEY_H)) {
      shapeRotationY += float(-1.0 * shapeRotationSpeed * deltaTime);
    }
    if (useKeyboardCharacterEvent(GLFW_KEY_U)) {
      shapeRotationZ += float(1.0 * shapeRotationSpeed * deltaTime);
    }
    if (useKeyboardCharacterEvent(GLFW_KEY_J)) {
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
