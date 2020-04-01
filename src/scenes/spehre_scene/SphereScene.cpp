#include "./SphereScene.h"

#include <memory>

#include "../../data_access_point/DataAccessPoint.h"
#include "../../3d_models/ShapeGenerator.h"

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

  sphere = ShapeGenerator::sphere(
    10,
    12,
    24,
    Vec3DFloat(1.0f, 1.0f, 1.0f),
    Vec3DFloat(
      DataAccessPoint::getInstance()->getAppScreenWidth() / 2.0f,
      DataAccessPoint::getInstance()->getAppScreenHeight() / 2.0f,
      cameraInitialZLocation - 100
    ),
    Vec3DFloat(0.0f,0.0f,0.0f),
    1.0f
  );
}

void SphereScene::update(double deltaTime) {
  {//Temporary code for auto rotation
    sphere->rotateY(float(-1.0f * shapeRotationSpeed * deltaTime * 0.1f));
    sphere->rotateX(float(-1.0f * shapeRotationSpeed * deltaTime * 0.1f));
    sphere->rotateZ(float(-1.0f * shapeRotationSpeed * deltaTime * 0.1f));
  }
  sphere->update(deltaTime, cameraInstance, lightSources);
  cameraInstance.update(deltaTime);
}

void SphereScene::render(double deltaTime) {
  sphere->render(deltaTime, cameraInstance);
  cameraInstance.render(deltaTime);
}