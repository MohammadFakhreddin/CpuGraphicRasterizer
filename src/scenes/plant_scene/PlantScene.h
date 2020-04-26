#ifndef PlantScene_Class
#define PlantScene_Class

#include <memory>

#include "../base_scene/BaseScene.h"
#include "../../open_gl/OpenGl.h"
#include "../../camera/Camera.h"
#include "../../shaders/directional_light/DirectionalLight.h"
#include "../../texture/ColorTexture/ColorTexture.h"
#include "../../pipeline/Pipeline.h"

class PlantScene : public BaseScene {

private:

  static constexpr float shapeTransformSpeed = 1.0f;

  static constexpr float shapeRotationSpeed = 0.01f;

  static constexpr float shapeScaleSpeed = 0.1f;

  static constexpr float lightRotationSpeed = 0.01f;

  static constexpr float cameraInitialZLocation = 1000.0f;

  static constexpr float cameraInitialMaximumFov = 1000.0f;

public:

  PlantScene(OpenGL& gl);

  void update(double deltaTime) override;

  void render(double deltaTime) override;

private:

  std::unique_ptr<DirectionalLight> directionalLight;

  std::unique_ptr<AmbientLight> ambientLight;

  Camera cameraInstance;

  std::unique_ptr<Shape3d> shape;

  std::unique_ptr<ColorTexture> colorTexture;

  PipeLine pip;

  float lightRotationX = 0.0f;

  float lightRotationY = 0.0f;

  float lightRotationZ = 0.0f;

  float shapeRotationX = 0.0f;

  float shapeRotationY = 0.0f;

  float shapeRotationZ = 0.0f;

};

#endif // !PlantScene_Class
