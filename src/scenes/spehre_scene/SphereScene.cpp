#include "./SphereScene.h"

#include "../../data_access_point/DataAccessPoint.h"

SphereScene::SphereScene(OpenGL& gl)
  :
  BaseScene(gl,"SphereScene"),
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

}

void SphereScene::update(double deltaTime) {

}

void SphereScene::render(double deltaTime) {

}