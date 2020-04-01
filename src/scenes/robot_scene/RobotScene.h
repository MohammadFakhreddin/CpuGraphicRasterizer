#ifndef RobotScene_Class
#define RobotScene_Class

#include <memory>

#include "../base_scene/BaseScene.h"
#include "../../open_gl/OpenGl.h"
#include "../../shaders/diffuse_light/DiffuseLight.h"
#include "../../camera/Camera.h"
#include "../../3d_shape/Shape3d.h"

class RobotScene : public BaseScene {

private:

  static constexpr float shapeTransformSpeed = 1.0f;

  static constexpr float shapeRotationSpeed = 0.01f;

  static constexpr float shapeScaleSpeed = 0.1f;

  static constexpr float lightTransformSpeed = 0.2f;

  static constexpr float cameraInitialZLocation = 1000.0f;

  static constexpr float cameraInitialMaximumFov = 1000.0f;

public:

  RobotScene(OpenGL& gl);

  void update(double deltaTime) override;

  void render(double deltaTime) override;

private:

  DiffuseLight* light;

  Camera cameraInstance;

  std::unique_ptr<Shape3d> shape;

  std::vector<std::unique_ptr<Light>> lightSources;

};

#endif // !RobotScene_Class