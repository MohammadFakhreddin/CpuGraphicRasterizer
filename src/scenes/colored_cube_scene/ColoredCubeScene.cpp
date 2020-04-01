#include "./ColoredCubeScene.h"

#include "../../3d_models/ShapeGenerator.h"
#include "../../data_access_point/DataAccessPoint.h"

ColoredCubeScene::ColoredCubeScene(
  OpenGL& gl
) 
  :
  BaseScene(gl,"ColoredCubeScene"),
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
    "Color main camera"
  )
{
  {//Creating shape
    
    auto appScreenWidth = DataAccessPoint::getInstance()->getAppScreenWidth();
    
    auto appScreenHeight = DataAccessPoint::getInstance()->getAppScreenHeight();

    float width = DataAccessPoint::getInstance()->getAppScreenWidth()/6.0f;
    
    Logger::log("Creating shape object");
    
    cube = ShapeGenerator::generateColored3DCube(
      width,
      width,
      width,
      float(appScreenWidth)/2.0f,
      float(appScreenHeight)/2.0f,
      float(cameraInitialZLocation - 500.0f),
      0,
      0,
      0,
      1
    );

  }
}

void ColoredCubeScene::update(double deltaTime) {
  {//Temporary code for auto rotation
    cube->rotateY(float(-1.0f * shapeRotationSpeed * deltaTime * 0.1f));
    cube->rotateX(float(-1.0f * shapeRotationSpeed * deltaTime * 0.1f));
    cube->rotateZ(float(-1.0f * shapeRotationSpeed * deltaTime * 0.1f));
  }
  cube->update(deltaTime,cameraInstance,lightSources);
}

void ColoredCubeScene::render(double deltaTime) {
  cameraInstance.render(deltaTime);
}