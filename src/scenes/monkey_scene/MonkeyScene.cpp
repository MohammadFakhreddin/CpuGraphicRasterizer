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
  camera(
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
    - 10.0f,
    float(DataAccessPoint::getInstance()->getAppScreenHeight()) / 2.0f
    - 10.0f,
    90.0f,
    1.1f,
    float(1.0 / double(10000)),
    float(1.0 / double(10000)),
    30
  )),
  pip(camera)
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
    monkey->setSpecularIntensity(0.5f);
    monkey->rotateY(float(Math::piDouble));
  }

  pip.assignAmbientLight(ambientLight.get());
  pip.assignPointLight(pointLight.get());
  pip.assignShapes(monkey.get());

}

void MonkeyScene::update(double deltaTime) {
#ifdef __DESKTOP__
  //{//We rotate light by keyboard
  //  if (useKeyEvent(Constants::KeyboardButtons::keyK)) {
  //    pointLight->transformX(float(deltaTime * lightTransformSpeed * 1.0f));
  //  }
  //  if (useKeyEvent(Constants::KeyboardButtons::keyH)) {
  //    pointLight->transformX(float(deltaTime * lightTransformSpeed * -1.0f));
  //  }
  //  if (useKeyEvent(Constants::KeyboardButtons::keyU)) {
  //    pointLight->transformY(float(deltaTime * lightTransformSpeed * 1.0f));
  //  }
  //  if (useKeyEvent(Constants::KeyboardButtons::keyJ)) {
  //    pointLight->transformY(float(deltaTime * lightTransformSpeed * -1.0f));
  //  }
  //  if (useKeyEvent(Constants::KeyboardButtons::keyY)) {
  //    pointLight->transformZ(float(deltaTime * lightTransformSpeed * -1.0f));
  //  }
  //  if (useKeyEvent(Constants::KeyboardButtons::keyI)) {
  //    pointLight->transformZ(float(deltaTime * lightTransformSpeed * 1.0f));
  //  }
  //}
  {//We rotate cmaera by keyboard and mouse
    cameraTransformX = 0.0f;
    if (useKeyEvent(Constants::KeyboardButtons::keyA)) {
      cameraTransformX += float(-1.0f * deltaTime * cameraTransformSpeed);
    }
    if (useKeyEvent(Constants::KeyboardButtons::keyD)) {
      cameraTransformX += float(1.0f * deltaTime * cameraTransformSpeed);
    }
    cameraTransformY = 0.0f;
    if (useKeyEvent(Constants::KeyboardButtons::keyW)) {
      cameraTransformY += float(1.0f * deltaTime * cameraTransformSpeed);
    }
    if (useKeyEvent(Constants::KeyboardButtons::keyS)) {
      cameraTransformY += float(-1.0f * deltaTime * cameraTransformSpeed);
    }
    cameraTransformZ = 0.0f;
    if (useKeyEvent(Constants::KeyboardButtons::keyC)) {
      cameraTransformZ += float(+1.0f * deltaTime * cameraTransformSpeed);
    }
    if (useKeyEvent(Constants::KeyboardButtons::keyV)) {
      cameraTransformZ += float(-1.0f * deltaTime * cameraTransformSpeed);
    }
    camera.transformXYZ(cameraTransformX, cameraTransformY, cameraTransformZ);
  }
  {
    cameraRotationX = 0.0f;
    if (useKeyEvent(Constants::KeyboardButtons::keyF)) {
      cameraRotationX += float(-1.0f * deltaTime * cameraRotationSpeed);
    }
    if(useKeyEvent(Constants::KeyboardButtons::keyH)){
      cameraRotationX += float(1.0f * deltaTime * cameraRotationSpeed);
    }
    cameraRotationY = 0.0f;
    if (useKeyEvent(Constants::KeyboardButtons::keyT)) {
      cameraRotationY += float(-1.0f * deltaTime * cameraRotationSpeed);
    }
    if (useKeyEvent(Constants::KeyboardButtons::keyG)) {
      cameraRotationY += float(1.0f * deltaTime * cameraRotationSpeed);
    }
    cameraRotationZ = 0.0f;
    if (useKeyEvent(Constants::KeyboardButtons::keyY)) {
      cameraRotationZ += float(-1.0f * deltaTime * cameraRotationSpeed);
    }
    if (useKeyEvent(Constants::KeyboardButtons::keyR)) {
      cameraRotationZ += float(1.0f * deltaTime * cameraRotationSpeed);
    }
    camera.rotateXYZ(cameraRotationX, cameraRotationY, cameraRotationZ);
  }
#endif
  pip.update(deltaTime);
}

void MonkeyScene::render(double deltaTime) {
  camera.render(deltaTime);
}