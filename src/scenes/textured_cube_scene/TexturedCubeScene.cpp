#include "./TexturedCubeScene.h"

#include "../../data_access_point/DataAccessPoint.h"
#include "../../3d_models/ShapeGenerator.h"
#include "../../shaders/ambient_light/AmbientLight.h"

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
      Surface::LightPrecision::perSurface,
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

  lightSources.emplace_back(std::make_unique<AmbientLight>(0.2f, 0.2f, 0.2f));
  lightSources.emplace_back(std::make_unique<DirectionalLight>(1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f));
  light = (DirectionalLight*)lightSources.back().get();

}

void TexturedCubeScene::update(double deltaTime) {
#ifdef __DESKTOP__
  {//We rotate light by keyboard
    lightRotationX = 0.0f;
    lightRotationY = 0.0f;
    lightRotationZ = 0.0f;
    if (useKeyEvent(Constants::Buttons::keyA)) {
      lightRotationX += float(deltaTime * lightTransformSpeed * -1.0f);
    }
    if (useKeyEvent(Constants::Buttons::keyD)) {
      lightRotationX += float(deltaTime * lightTransformSpeed);
    }
    if (useKeyEvent(Constants::Buttons::keyW)) {
      lightRotationY += float(deltaTime * lightTransformSpeed);
    }
    if (useKeyEvent(Constants::Buttons::keyS)) {
      lightRotationY += float(deltaTime * lightTransformSpeed * -1.0);
    }
    if (useKeyEvent(Constants::Buttons::keyC)) {
      lightRotationZ += float(deltaTime * lightTransformSpeed * -1.0 * 0.5);
    }
    if (useKeyEvent(Constants::Buttons::keyV)) {
      lightRotationZ += float(deltaTime * lightTransformSpeed * 1.0 * 0.5);
    }
    if (lightRotationX != 0 || lightRotationY != 0 || lightRotationZ != 0) {
      light->rotateXYZ(lightRotationX, lightRotationY, lightRotationZ);
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
  cube->rotateXYZ(
    float(-1.0f * shapeRotationSpeed * deltaTime * 0.1f), 
    float(-1.0f * shapeRotationSpeed * deltaTime * 0.1f), 
    float(-1.0f * shapeRotationSpeed * deltaTime * 0.1f)
  );
  cube->update(deltaTime, cameraInstance, lightSources);
}

void TexturedCubeScene::render(double deltaTime) {
  cameraInstance.render(deltaTime);
}