#ifndef CarScene_Class
#define CarScene_Class

#include <memory>

#include "../base_scene/BaseScene.h"
#include "../../open_gl/OpenGl.h"
#include "../../camera/Camera.h"
#include "../../3d_shape/Shape3d.h"
#include "../../shaders/point_light/PointLight.h"
#include "../../texture/ColorTexture/ColorTexture.h"
#include "../../3d_models/ColoredCube.h"

class LightPerPixelScene : public BaseScene {

private:

  static constexpr float shapeTransformSpeed = 1.0f;

  static constexpr float shapeRotationSpeed = 0.01f;

  static constexpr float shapeScaleSpeed = 0.1f;

  static constexpr float lightTransformSpeed = 0.4f;

  static constexpr float cameraInitialZLocation = 1000.0f;

  static constexpr float cameraInitialMaximumFov = 1000.0f;

public:

  LightPerPixelScene(OpenGL& gl);

  void update(double deltaTime) override;

  void render(double deltaTime) override;

private:

  PointLight* light;

  Camera cameraInstance;

  std::unique_ptr<Shape3d> shape;

  std::vector<std::unique_ptr<Light>> lightSources;

  std::unique_ptr<ColorTexture> whiteColor;

  float lightRotationX = 0.0f;

  float lightRotationY = 0.0f;

  float lightRotationZ = 0.0f;

  float shapeRotationX = 0.0f;

  float shapeRotationY = 0.0f;

  float shapeRotationZ = 0.0f;

  ColorCube colorCube;

};

#endif // !CarScene_Class
