#ifndef SphereScene_Class
#define SphereScene_Class

#include <memory>

#include "../base_scene/BaseScene.h"
#include "../../camera/Camera.h"
#include "../../shaders/directional_light/DirectionalLight.h"
#include "../../texture/ColorTexture/ColorTexture.h"
#include "../../3d/shape/Shape3d.h"

class SphereScene : public BaseScene {

public:

  SphereScene(OpenGL& gl);

  void update(double deltaTime);

  void render(double deltaTime);

private:

  static constexpr unsigned int cameraInitialMaximumFov = 1000;

  static constexpr unsigned int cameraInitialZLocation = 1000;

  static constexpr float shapeRotationSpeed = 0.01f;

  static constexpr float lightTransformSpeed = 0.01f;

  Camera cameraInstance;

  std::unique_ptr<Shape3d> sphere;

  std::unique_ptr<ColorTexture> whiteColor;

  DirectionalLight* light;

  float lightRotationX = 0.0f;

  float lightRotationY = 0.0f;

  float lightRotationZ = 0.0f;

};

#endif // !SphereScene_Class
