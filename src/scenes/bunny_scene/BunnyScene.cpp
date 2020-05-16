#include "./BunnyScene.h"

#include "../base_scene/BaseScene.h"
#include "../../data_access_point/DataAccessPoint.h"
#include "../../file_system/FileSystem.h"
#include "../../utils/path/Path.h"
#include "../../shaders/ambient_light/AmbientLight.h"

BunnyScene::BunnyScene(OpenGL& gl)
  : 
  BaseScene(gl, "BunnyScene"),
  cameraInstance(
    gl,
    Matrix3X1Float(),
    Matrix3X1Float(),
    DataAccessPoint::getInstance()->getAppScreenWidth(),
    DataAccessPoint::getInstance()->getAppScreenHeight(),
    "Bunny main camera"
  ),
  ambientLight(std::make_unique<AmbientLight>(0.2f, 0.2f, 0.2f)),
  directionalLight(std::make_unique<DirectionalLight>(1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f)),
  pip(cameraInstance)
{
  whiteColor = std::make_unique<ColorTexture>(
    1.0f,1.0f,1.0f  
  );
  {//Creating shape
    auto scaleFactor = float(DataAccessPoint::getInstance()->getAppScreenWidth()) / 4.0f;
    bunnyShape = FileSystem::loadObject(
      Path::generateAssetPath("bunny", ".obj"),
      Constants::LightPrecision::perSurface,
      whiteColor.get(),
      true,
      Shape3d::NormalType::smooth,
      false
    );
    bunnyShape->transformX(float(DataAccessPoint::getInstance()->getAppScreenWidth()) / 2.0f);
    bunnyShape->transformY(float(DataAccessPoint::getInstance()->getAppScreenHeight()) / 2.0f);
    bunnyShape->transformZ(100.0f);
    bunnyShape->scale(scaleFactor);
  }
 
  pip.assignAmbientLight(ambientLight.get());
  pip.assignDirectionalLight(directionalLight.get());
  pip.assignShape(bunnyShape.get());

}

void BunnyScene::update(double deltaTime) {
#ifdef __DESKTOP__
  {//We rotate light by keyboard
    lightRotationX = 0.0f;
    lightRotationY = 0.0f;
    lightRotationZ = 0.0f;
    if (useScanCodeEvent(GLFW_KEY_A)) {
      lightRotationX += float(deltaTime * lightTransformSpeed * -1.0f);
    }
    if (useScanCodeEvent(GLFW_KEY_D)) {
      lightRotationX += float(deltaTime * lightTransformSpeed);
    }
    if (useScanCodeEvent(GLFW_KEY_W)) {
      lightRotationY += float(deltaTime * lightTransformSpeed);
    }
    if (useScanCodeEvent(GLFW_KEY_S)) {
      lightRotationY += float(deltaTime * lightTransformSpeed * -1.0);
    }
    if (useScanCodeEvent(GLFW_KEY_C)) {
      lightRotationZ += float(deltaTime * lightTransformSpeed * -1.0 * 0.5);
    }
    if (useScanCodeEvent(GLFW_KEY_V)) {
      lightRotationZ += float(deltaTime * lightTransformSpeed * 1.0 * 0.5);
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
      //We need separate rotation methods as well
      bunnyShape->rotateXYZ(shapeRotationX, shapeRotationY, shapeRotationZ);
    }
  }
#endif
  bunnyShape->rotateXYZ(
    float(-1.0f * shapeRotationSpeed * deltaTime * 0.1f), 
    float(-1.0f * shapeRotationSpeed * deltaTime * 0.1f),
    float(-1.0f * shapeRotationSpeed * deltaTime * 0.1f)
  );
  pip.update(deltaTime);
}

void BunnyScene::render(double deltaTime) {
  cameraInstance.render(deltaTime);
}
