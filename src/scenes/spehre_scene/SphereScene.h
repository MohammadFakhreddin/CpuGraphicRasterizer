#ifndef SphereScene_Class
#define SphereScene_Class

#include <memory>

#include "../base_scene/BaseScene.h"
#include "../../camera/Camera.h"
#include "../../shaders/directional_light/DirectionalLight.h"
#include "../../texture/ColorTexture/ColorTexture.h"
#include "../../3d/shape/Shape3d.h"
#include "../../pipeline/Pipeline.h"

#define SPHERE_SCENE_SPEED 100

class SphereScene : public BaseScene {

public:

  SphereScene(OpenGL& gl);

  void update(double deltaTime);

  void render(double deltaTime);

private:

  static constexpr float shapeRotationSpeed = 0.01f * SPHERE_SCENE_SPEED;

  static constexpr float lightTransformSpeed = 0.01f * SPHERE_SCENE_SPEED;

  Camera camera;

  std::unique_ptr<Shape3d> sphere;

  std::unique_ptr<ColorTexture> sphereColor;

  std::unique_ptr<DirectionalLight> directionalLight;

  std::unique_ptr<AmbientLight> ambientLight;

  float lightRotationX = 0.0f;

  float lightRotationY = 0.0f;

  float lightRotationZ = 0.0f;

  PipeLine pipeLine;

};

#endif // !SphereScene_Class
