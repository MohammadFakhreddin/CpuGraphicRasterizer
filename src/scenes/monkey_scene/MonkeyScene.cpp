#include "./MonkeyScene.h"

#include <codecvt>

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
  pip(camera),
  font(
    Path::generateAssetPath("font/BYekan", ".ttf"),
    200,
    &DataAccessPoint::getInstance()->getFreeType(),
    DataAccessPoint::getInstance()->getSpecialCharacters()
  ),
  whiteColor(std::make_unique<ColorTexture>(1.0f,1.0f,1.0f))
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
  pip.assignShape(monkey.get());

}

void MonkeyScene::update(double deltaTime) {
#ifdef __DESKTOP__
  {//We rotate light by keyboard
   if (useKeyEvent(Constants::KeyboardButtons::keyK)) {
     pointLight->transformX(float(deltaTime * lightTransformSpeed * 1.0f));
   }
   if (useKeyEvent(Constants::KeyboardButtons::keyH)) {
     pointLight->transformX(float(deltaTime * lightTransformSpeed * -1.0f));
   }
   if (useKeyEvent(Constants::KeyboardButtons::keyU)) {
     pointLight->transformY(float(deltaTime * lightTransformSpeed * 1.0f));
   }
   if (useKeyEvent(Constants::KeyboardButtons::keyJ)) {
     pointLight->transformY(float(deltaTime * lightTransformSpeed * -1.0f));
   }
   if (useKeyEvent(Constants::KeyboardButtons::keyY)) {
     pointLight->transformZ(float(deltaTime * lightTransformSpeed * -1.0f));
   }
   if (useKeyEvent(Constants::KeyboardButtons::keyI)) {
     pointLight->transformZ(float(deltaTime * lightTransformSpeed * 1.0f));
   }
  }
  {//Transform of camera
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
  {//Rotation of camera
    if(getMouseEvent(Constants::MouseButtonName::left)){
      cameraRotationX = 0.0f;
      cameraRotationY = 0.0f;
      cameraRotationZ = 0.0f;
      if(previouseLeftMouseButtonState==false){
        previousMousePosition.assign(DataAccessPoint::getInstance()->getMousePosition());
        previouseLeftMouseButtonState = true;
      }else if(previouseLeftMouseButtonState==true){
        currentMousePosition.assign(DataAccessPoint::getInstance()->getMousePosition());
        cameraRotationX = float(
          -1.0f * 
          deltaTime * 
          (
            currentMousePosition.getY() - previousMousePosition.getY()
          ) * cameraRotationSpeed
        );
        cameraRotationY = float(
          +1.0f * 
          deltaTime *
          (
            currentMousePosition.getX() - previousMousePosition.getX()
          ) * cameraRotationSpeed
        );
        camera.rotateXYZ(cameraRotationX, cameraRotationY, cameraRotationZ);
        previousMousePosition.assign(currentMousePosition);
      }
    }else
    {
      previouseLeftMouseButtonState = false;
    }
    
  }
#endif
  pip.update(deltaTime);
  font.drawText(pip, U"به نام خدا", 100, 100, Font::PositionMode::leftTop, whiteColor.get());
  
}

void MonkeyScene::render(double deltaTime) {
  camera.render(deltaTime);
}