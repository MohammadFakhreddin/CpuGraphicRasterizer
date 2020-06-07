#include "./TexturedCubeScene.h"

#include "../../data_access_point/DataAccessPoint.h"
#include "../../shaders/ambient_light/AmbientLight.h"

TexturedCubeScene::TexturedCubeScene(
  OpenGL& gl
  )
  :
  BaseScene(gl, "TexturedCubeScene"),
  cameraInstance(
    gl,
    Matrix3X1Float(),
    Matrix3X1Float(),
    DataAccessPoint::getInstance()->getAppScreenWidth(),
    DataAccessPoint::getInstance()->getAppScreenHeight(),
    "Texture main camera"
  ),
  ambientLight(std::make_unique<AmbientLight>(0.2f, 0.2f, 0.2f)),
  directionalLight(std::make_unique<DirectionalLight>(1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f)),
  pip(cameraInstance)
{
  {//Creating shape
    auto appScreenWidth = DataAccessPoint::getInstance()->getAppScreenWidth();
    auto appScreenHeight = DataAccessPoint::getInstance()->getAppScreenHeight();
    auto width = DataAccessPoint::getInstance()->getAppScreenWidth() / 30.0f;
    Logger::log("Creating shape object");
    cube = dice.generateCube(
      Constants::LightPrecision::perSurface,
      float(width), 
      float(width), 
      float(width), 
      float(appScreenWidth) / 2.0f,
      float(appScreenHeight) / 2.0f,
      float(100.0f),
      0.0f,
      0.0f,
      0.0f,
      1.0f
    );
  }

  pip.assignAmbientLight(ambientLight.get());
  pip.assignDirectionalLight(directionalLight.get());
  pip.assignShape(cube.get());

}

void TexturedCubeScene::update(double deltaTime) {
#ifdef __DESKTOP__
  {//We rotate light by keyboard
    lightRotationX = 0.0f;
    lightRotationY = 0.0f;
    lightRotationZ = 0.0f;
    if (useKeyboardCharacterEvent(GLFW_KEY_A)) {
      lightRotationX += float(deltaTime * lightTransformSpeed * -1.0f);
    }
    if (useKeyboardCharacterEvent(GLFW_KEY_D)) {
      lightRotationX += float(deltaTime * lightTransformSpeed);
    }
    if (useKeyboardCharacterEvent(GLFW_KEY_W)) {
      lightRotationY += float(deltaTime * lightTransformSpeed);
    }
    if (useKeyboardCharacterEvent(GLFW_KEY_S)) {
      lightRotationY += float(deltaTime * lightTransformSpeed * -1.0);
    }
    if (useKeyboardCharacterEvent(GLFW_KEY_C)) {
      lightRotationZ += float(deltaTime * lightTransformSpeed * -1.0 * 0.5);
    }
    if (useKeyboardCharacterEvent(GLFW_KEY_V)) {
      lightRotationZ += float(deltaTime * lightTransformSpeed * 1.0 * 0.5);
    }
    if (lightRotationX != 0 || lightRotationY != 0 || lightRotationZ != 0) {
      directionalLight->rotateXYZ(lightRotationX, lightRotationY, lightRotationZ);
    }
  }
#endif // __DESKTOP__
  cube->rotateXYZ(
    float(-1.0f * shapeRotationSpeed * deltaTime * 0.1f), 
    float(-1.0f * shapeRotationSpeed * deltaTime * 0.1f), 
    float(-1.0f * shapeRotationSpeed * deltaTime * 0.1f)
  );
  pip.update(deltaTime);
}

void TexturedCubeScene::render(double deltaTime) {
  cameraInstance.render(deltaTime);
}