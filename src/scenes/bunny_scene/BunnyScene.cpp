#include "./BunnyScene.h"

#include "../base_scene/BaseScene.h"
#include "../../application/Application.h"
#include "../../file_system/FileSystem.h"

BunnyScene::BunnyScene(OpenGL& gl)
: BaseScene(gl,"BunnyScene"),
  light(
    float(Application::getInstance()->getAppScreenWidth())/2.0f,
    float(Application::getInstance()->getAppScreenHeight()),
    cameraInitialZLocation - 50
  ),
  cameraInstance(
    gl,
    light,
    cameraInitialZLocation,
    cameraInitialMaximumFov,
    0,
    (int)Application::getInstance()->getAppScreenWidth(),
    0,
    (int)Application::getInstance()->getAppScreenHeight()
  )
{
  auto scaleFactor = float(Application::getInstance()->getAppScreenWidth()) / 4.0f;
  shape = FileSystem::loadObjectWithColor(
    Path::generateAssetPath("bunny",".obj"),
    Vec3DFloat(1.0f,1.0f,1.0f),
    true
  );
  shape->transformX(float(Application::getInstance()->getAppScreenWidth())/2.0f);
  shape->transformY(float(Application::getInstance()->getAppScreenHeight())/2.0f);
  shape->transformZ(cameraInitialZLocation - 100.0f);
  shape->scale(scaleFactor);
}

void BunnyScene::update(double deltaTime) {
  {//We rotate light by keyboard
    if (useKeyEvent(Constants::Buttons::keyA)) {
      cameraInstance.getLight().transformX(
        float(deltaTime * lightTransformSpeed * -1.0f)
      );
    }
    if (useKeyEvent(Constants::Buttons::keyD)) {
      cameraInstance.getLight().transformX(
        float(deltaTime * lightTransformSpeed)
      );
    }
    if (useKeyEvent(Constants::Buttons::keyW)) {
      cameraInstance.getLight().transformY(
        float(deltaTime * lightTransformSpeed)
      );
    }
    if (useKeyEvent(Constants::Buttons::keyS)) {
      cameraInstance.getLight().transformY(
        float(deltaTime * lightTransformSpeed * -1.0)
      );
    }
    if (useKeyEvent(Constants::Buttons::keyC)) {
      cameraInstance.getLight().transformZ(
        float(deltaTime * lightTransformSpeed * -1.0 * 0.5)
      );
    }
    if (useKeyEvent(Constants::Buttons::keyV)) {
      cameraInstance.getLight().transformZ(
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
  {//Temporary code for auto rotation
    shape->rotateY(float(-1.0f * shapeRotationSpeed * deltaTime * 0.1f));
    shape->rotateX(float(-1.0f * shapeRotationSpeed * deltaTime * 0.1f));
    shape->rotateZ(float(-1.0f * shapeRotationSpeed * deltaTime * 0.1f));
  }
  shape->update(deltaTime, cameraInstance);
  cameraInstance.update(deltaTime);
}

void BunnyScene::render(double deltaTime) {
  cameraInstance.render(deltaTime);
}