#include "./TexturedCubeScene.h"

#include "../../data_access_point/DataAccessPoint.h"
#include "../../3d_models/ShapeGenerator.h"

TexturedCubeScene::TexturedCubeScene(
  OpenGL& gl
  )
  :
  BaseScene(gl, "TexturedCubeScene"),
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

    cube = dice.generateCube(
      float(width), 
      float(width), 
      float(width), 
      float(appScreenWidth) / 2.0f,
      float(appScreenHeight) / 2.0f,
      float(cameraInitialZLocation - 500.0f),
      0.0f,
      0.0f,
      0.0f,
      1.0f
    );

  }

  lightSources.emplace_back(std::make_unique<DirectionalLight>(-1.0f, -1.0f, -1.0f, 0.05f));
  light = (DirectionalLight*)lightSources.back().get();

}

void TexturedCubeScene::update(double deltaTime) {
#ifdef __DESKTOP__
  {//We rotate light by keyboard
    if (useKeyEvent(Constants::Buttons::keyA)) {
      light->rotateX(
        float(deltaTime * lightTransformSpeed * -1.0f)
      );
    }
    if (useKeyEvent(Constants::Buttons::keyD)) {
      light->rotateX(
        float(deltaTime * lightTransformSpeed)
      );
    }
    if (useKeyEvent(Constants::Buttons::keyW)) {
      light->rotateY(
        float(deltaTime * lightTransformSpeed)
      );
    }
    if (useKeyEvent(Constants::Buttons::keyS)) {
      light->rotateY(
        float(deltaTime * lightTransformSpeed * -1.0)
      );
    }
    if (useKeyEvent(Constants::Buttons::keyC)) {
      light->rotateZ(
        float(deltaTime * lightTransformSpeed * -1.0 * 0.5)
      );
    }
    if (useKeyEvent(Constants::Buttons::keyV)) {
      light->rotateZ(
        float(deltaTime * lightTransformSpeed * 1.0 * 0.5)
      );
    }
  }
#endif // __DESKTOP__
  {//Light sources
    if (!lightSources.empty()) {
      for (auto& lightSource : lightSources) {
        lightSource->update(deltaTime, cameraInstance);
      }
    }
  }
  {//Temporary code for auto rotation
    cube->rotateY(float(-1.0f * shapeRotationSpeed * deltaTime * 0.1f));
    cube->rotateX(float(-1.0f * shapeRotationSpeed * deltaTime * 0.1f));
    cube->rotateZ(float(-1.0f * shapeRotationSpeed * deltaTime * 0.1f));
  }
  cube->update(deltaTime, cameraInstance, lightSources);
}

void TexturedCubeScene::render(double deltaTime) {
  cameraInstance.render(deltaTime);
}