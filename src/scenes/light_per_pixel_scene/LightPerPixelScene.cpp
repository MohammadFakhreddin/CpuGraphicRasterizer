#include "./LightPerPixelScene.h"

#include "../base_scene/BaseScene.h"
#include "../../data_access_point/DataAccessPoint.h"
#include "../../utils/path/Path.h"
#include "../../shaders/ambient_light/AmbientLight.h"
#include "../../shaders/point_light/PointLight.h"

LightPerPixelScene::LightPerPixelScene(OpenGL& gl)
  :
  BaseScene(gl, "CarScene"),
  cameraInstance(
    gl,
    MatrixFloat(3, 1, 0.0f),
    MatrixFloat(3, 1, 0.0f),
    DataAccessPoint::getInstance()->getAppScreenWidth(),
    DataAccessPoint::getInstance()->getAppScreenHeight(),
    "Light per pixel scene main camera"
  ),
  whiteColor(std::make_unique<ColorTexture>(
    1.0f, 1.0f, 1.0f
  )),
  ambientLight(std::make_unique<AmbientLight>(0.05f, 0.05f, 0.05f)),
  pointLight(std::make_unique<PointLight>(
    2.0f,
    255.0f / 256.0f, 214.0f / 256.0f, 170.0f / 256.0f,
    float(DataAccessPoint::getInstance()->getAppScreenWidth()) / 30.0f,
    float(DataAccessPoint::getInstance()->getAppScreenHeight()) - float(DataAccessPoint::getInstance()->getAppScreenWidth()) / 30.0f,
    cameraInitialZLocation - 80.0f,
    1.0f,1.0f,1.0f,2
  )),
  pip(cameraInstance)
{
  
  {//Creating shape
    float width = DataAccessPoint::getInstance()->getAppScreenWidth()/5.0f;
    shape = colorCube.generateCube(Constants::LightPrecision::perPixel, width, width, 1, 0, 0, 0, 0, 0, 0, 1);
    shape->transformX(float(DataAccessPoint::getInstance()->getAppScreenWidth()) / 2.0f);
    shape->transformY(float(DataAccessPoint::getInstance()->getAppScreenHeight()) / 2.0f);
    shape->transformZ(cameraInitialZLocation - 500.0f);
  }

  pip.assignShapes(shape.get());
  pip.assignAmbientLight(ambientLight.get());
  pip.assignPointLight(pointLight.get());

}

void LightPerPixelScene::update(double deltaTime) {
#ifdef __DESKTOP__
  {//We rotate light by keyboard
    if (useKeyEvent(Constants::Buttons::keyA)) {
      pointLight->transformX(float(deltaTime * lightTransformSpeed * -1.0f));
    }
    if (useKeyEvent(Constants::Buttons::keyD)) {
      pointLight->transformX(float(deltaTime * lightTransformSpeed));
    }
    if (useKeyEvent(Constants::Buttons::keyW)) {
      pointLight->transformY(float(deltaTime * lightTransformSpeed));
    }
    if (useKeyEvent(Constants::Buttons::keyS)) {
      pointLight->transformY(float(deltaTime * lightTransformSpeed * -1.0));
    }
    if (useKeyEvent(Constants::Buttons::keyC)) {
      pointLight->transformZ(float(deltaTime * lightTransformSpeed * -1.0 * 0.5));
    }
    if (useKeyEvent(Constants::Buttons::keyV)) {
      pointLight->transformZ(float(deltaTime * lightTransformSpeed * 1.0 * 0.5));
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

void LightPerPixelScene::render(double deltaTime) {
  cameraInstance.render(deltaTime);
}
