#ifndef MonkeyScene_Class
#define MonkeyScene_Class

#include "../base_scene/BaseScene.h"
#include "../../open_gl/OpenGl.h"
#include "../../camera/Camera.h"
#include "../../texture/ColorTexture/ColorTexture.h"
#include "../../shaders/point_light/PointLight.h"
#include "../../pipeline/Pipeline.h"
#include "../../texture/ImageTexture/ImageTexture.h"
#include "../../text/font/Font.h"

class MonkeyScene : public BaseScene {

private:

  static constexpr float shapeTransformSpeed = 1.0f;

  static constexpr float shapeRotationSpeed = 0.01f;

  static constexpr float shapeScaleSpeed = 0.1f;

  static constexpr float lightTransformSpeed = 0.1f;

  static constexpr float cameraTransformSpeed = 0.2f;

  static constexpr float cameraRotationSpeed = 0.0002f;

public:
  
  MonkeyScene(OpenGL& gl);

  void update(double deltaTime) override;

  void render(double deltaTime) override;

private:

  Camera camera;

  std::unique_ptr<Shape3d> monkey;

  std::unique_ptr<PointLight> pointLight;

  std::unique_ptr<AmbientLight> ambientLight;

  std::unique_ptr<ColorTexture> metalColor;

  std::unique_ptr<ColorTexture> whiteColor;

  float cameraTransformX = 0.0f;

  float cameraTransformY = 0.0f;

  float cameraTransformZ = 0.0f;

  float cameraRotationX = 0.0f;

  float cameraRotationY = 0.0f;

  float cameraRotationZ = 0.0f;

  PipeLine pip;

  bool previouseLeftMouseButtonState = false;

  Matrix2X1Int previousMousePosition;

  Matrix2X1Int currentMousePosition;

  Font font;

};

#endif // !MonkeyScene_Class
