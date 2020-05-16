#include "./RobotScene.h"

#include "../base_scene/BaseScene.h"
#include "../../data_access_point/DataAccessPoint.h"
#include "../../file_system/FileSystem.h"
#include "../../utils/path/Path.h"
#include "../../shaders/directional_light/DirectionalLight.h"
#include "../../shaders/ambient_light/AmbientLight.h"
#include "../../utils/math/Math.h"

RobotScene::RobotScene(OpenGL& gl)
  :
  BaseScene(gl, "RobotScene"),
  ambientLight(std::make_unique<AmbientLight>(0.2f, 0.2f, 0.2f)),
  directionalLight(std::make_unique<DirectionalLight>(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f)),
  cameraInstance(
    gl,
    Matrix3X1Float(),
    Matrix3X1Float(),
    DataAccessPoint::getInstance()->getAppScreenWidth(),
    DataAccessPoint::getInstance()->getAppScreenHeight(),
    "Robot main camera"
  ),
  colorTexture(std::make_unique<ColorTexture>(1.0f, 1.0f, 1.0f)),
  pip(cameraInstance)
{
  colorTexture = std::make_unique<ColorTexture>(1.0f,1.0f,1.0f);
  {//Creating shape
    auto scaleFactor = float(DataAccessPoint::getInstance()->getAppScreenWidth()) / 50.0f;
    shape = FileSystem::loadObject(
      Path::generateAssetPath("robot", ".obj"),
      Constants::LightPrecision::perSurface,
      colorTexture.get(),
      true,
      Shape3d::NormalType::fileDefault,
      false
    );
    shape->transformX(float(DataAccessPoint::getInstance()->getAppScreenWidth()) / 2.0f);
    shape->transformY(float(DataAccessPoint::getInstance()->getAppScreenHeight()) / 2.0f);
    shape->transformZ(100.0f);
    float rotationYValue = Math::piFloat;
    shape->rotateXYZ(0.0f, rotationYValue , 0.0f);
    shape->scale(scaleFactor);
  }
  pip.assignAmbientLight(ambientLight.get());
  pip.assignDirectionalLight(directionalLight.get());
  pip.assignShape(shape.get());
}

void RobotScene::update(double deltaTime) {
#ifdef __DESKTOP__
  {//We rotate light by keyboard
    lightRotationX = 0.0f;
    lightRotationY = 0.0f;
    lightRotationZ = 0.0f;
    if (useScanCodeEvent(GLFW_KEY_A)) {
      lightRotationX += float(deltaTime * lightRotationSpeed * -1.0f);
    }
    if (useScanCodeEvent(GLFW_KEY_D)) {
      lightRotationX += float(deltaTime * lightRotationSpeed);
    }
    if (useScanCodeEvent(GLFW_KEY_W)) {
      lightRotationY += float(deltaTime * lightRotationSpeed);
    }
    if (useScanCodeEvent(GLFW_KEY_S)) {
      lightRotationY += float(deltaTime * lightRotationSpeed * -1.0);
    }
    if (useScanCodeEvent(GLFW_KEY_C)) {
      lightRotationZ += float(deltaTime * lightRotationSpeed * -1.0 * 0.5);
    }
    if (useScanCodeEvent(GLFW_KEY_V)) {
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
    if (useScanCodeEvent(GLFW_KEY_I)) {
      shapeRotationX += float(1.0 * shapeRotationSpeed * deltaTime);
    }
    if (useScanCodeEvent(GLFW_KEY_Y)) {
      shapeRotationX += float(-1.0 * shapeRotationSpeed * deltaTime);
    }
    if (useScanCodeEvent(GLFW_KEY_K)) {
      shapeRotationY += float(1.0 * shapeRotationSpeed * deltaTime);
    }
    if (useScanCodeEvent(GLFW_KEY_H)) {
      shapeRotationY += float(-1.0 * shapeRotationSpeed * deltaTime);
    }
    if (useScanCodeEvent(GLFW_KEY_U)) {
      shapeRotationZ += float(1.0 * shapeRotationSpeed * deltaTime);
    }
    if (useScanCodeEvent(GLFW_KEY_J)) {
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

void RobotScene::render(double deltaTime) {
  cameraInstance.render(deltaTime);
}
