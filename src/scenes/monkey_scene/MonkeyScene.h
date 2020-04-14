#ifndef MonkeyScene_Class
#define MonkeyScene_Class

#include "../base_scene/BaseScene.h"
#include "../../open_gl/OpenGl.h"
#include "../../camera/Camera.h"
#include "../../3d_shape/Shape3d.h"
#include "../../texture/ColorTexture/ColorTexture.h"
#include "../../shaders/point_light/PointLight.h"

class MonkeyScene : public BaseScene {

private:

  static constexpr float shapeTransformSpeed = 1.0f;

  static constexpr float shapeRotationSpeed = 0.01f;

  static constexpr float shapeScaleSpeed = 0.1f;

  static constexpr float lightTransformSpeed = 0.2f;

  static constexpr float cameraInitialZLocation = 1000.0f;

  static constexpr float cameraInitialMaximumFov = 1000.0f;

public:
  
  MonkeyScene(OpenGL& gl);

  void update(double deltaTime) override;

  void render(double deltaTime) override;

private:

  Camera cameraInstance;

  std::unique_ptr<Shape3d> shape;

  std::vector<std::unique_ptr<Light>> lightSources;

  PointLight* light;

  std::unique_ptr<ColorTexture> metalColor = std::make_unique<ColorTexture>(
    171.0f / 255.0f, 171.0f / 255.0f, 171.0f / 255.0f
  );

  float shapeRotationX = 0.0f;

  float shapeRotationY = 0.0f;

  float shapeRotationZ = 0.0f;

};

#endif // !MonkeyScene_Class
