#ifndef MonkeyScene_Class
#define MonkeyScene_Class

#include "../base_scene/BaseScene.h"
#include "../../open_gl/OpenGl.h"
#include "../../camera/Camera.h"
#include "../../texture/ColorTexture/ColorTexture.h"
#include "../../shaders/point_light/PointLight.h"
#include "../../pipeline/Pipeline.h"
#include "../../texture/ImageTexture/ImageTexture.h"
#ifdef __OPENGL__
#include "../../text/font/Font.h"
#endif

#define MONKEY_SCENE_SPEED_FACTOR 100

class MonkeyScene : public BaseScene {

private:

  static constexpr float shapeTransformSpeed = 1.0f * MONKEY_SCENE_SPEED_FACTOR;

  static constexpr float shapeRotationSpeed = 0.01f * MONKEY_SCENE_SPEED_FACTOR;

  static constexpr float shapeScaleSpeed = 0.1f * MONKEY_SCENE_SPEED_FACTOR;

  static constexpr float lightTransformSpeed = 0.1f * MONKEY_SCENE_SPEED_FACTOR;

  static constexpr float cameraTransformSpeed = 0.2f * MONKEY_SCENE_SPEED_FACTOR;

  static constexpr float cameraRotationSpeed = 0.0002f * MONKEY_SCENE_SPEED_FACTOR;

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
#ifdef __OPENGL__
  std::unique_ptr<Font> font;
#endif
};

#endif // !MonkeyScene_Class
