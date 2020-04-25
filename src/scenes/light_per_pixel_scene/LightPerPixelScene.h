#ifndef CarScene_Class
#define CarScene_Class

#include <memory>

#include "../base_scene/BaseScene.h"
#include "../../open_gl/OpenGl.h"
#include "../../camera/Camera.h"
#include "../../shaders/point_light/PointLight.h"
#include "../../texture/ColorTexture/ColorTexture.h"
#include "../../pipeline/Pipeline.h"
#include "../../3d/models/ColoredCube.h"

class LightPerPixelScene : public BaseScene {

private:

  static constexpr float shapeTransformSpeed = 1.0f;

  static constexpr float shapeRotationSpeed = 0.01f;

  static constexpr float shapeScaleSpeed = 0.1f;

  static constexpr float lightTransformSpeed = 0.4f;

public:

  LightPerPixelScene(OpenGL& gl);

  void update(double deltaTime) override;

  void render(double deltaTime) override;

private:

  std::unique_ptr<PointLight> pointLight;

  std::unique_ptr<AmbientLight> ambientLight;

  Camera cameraInstance;

  std::unique_ptr<Shape3d> shape;

  std::unique_ptr<ColorTexture> whiteColor;

  PipeLine pip;

  ColorCube colorCube;

  float lightRotationX = 0.0f;

  float lightRotationY = 0.0f;

  float lightRotationZ = 0.0f;

  float shapeRotationX = 0.0f;

  float shapeRotationY = 0.0f;

  float shapeRotationZ = 0.0f;

};

#endif // !CarScene_Class
