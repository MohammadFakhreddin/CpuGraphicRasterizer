#include "./TexturedCubeScene.h"

#include "../../data_access_point/DataAccessPoint.h"

TexturedCubeScene::TexturedCubeScene(
  OpenGL& gl
  )
  :
  BaseScene(gl, "ColoredCubeScene"),
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
    "Texture main camera"
    )
{
  {//Creating shape

    auto appScreenWidth = DataAccessPoint::getInstance()->getAppScreenWidth();

    auto appScreenHeight = DataAccessPoint::getInstance()->getAppScreenHeight();

    auto width = DataAccessPoint::getInstance()->getAppScreenWidth() / 6;

    Logger::log("Creating shape object");

    cube = Shape3d::generateTextured3DCube(
      dice.diceCubeTexture,
      dice.diceCubeEdgeList,
      width,
      width,
      width,
      float(appScreenWidth) / 2.0f,
      float(appScreenHeight) / 2.0f,
      float(cameraInitialZLocation - 500.0f),
      0,
      0,
      0,
      1
    );

  }
}

void TexturedCubeScene::update(double deltaTime) {
  {//Temporary code for auto rotation
    cube->rotateY(float(-1.0f * shapeRotationSpeed * deltaTime * 0.1f));
    cube->rotateX(float(-1.0f * shapeRotationSpeed * deltaTime * 0.1f));
    cube->rotateZ(float(-1.0f * shapeRotationSpeed * deltaTime * 0.1f));
  }
  cube->update(deltaTime, cameraInstance, lightSources);
  cameraInstance.update(deltaTime);
}

void TexturedCubeScene::render(double deltaTime) {
  cube->render(deltaTime, cameraInstance);
  cameraInstance.render(deltaTime);
}