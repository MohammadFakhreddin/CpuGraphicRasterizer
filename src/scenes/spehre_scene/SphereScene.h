#ifndef SphereScene_Class
#define SphereScene_Class

#include <memory>

#include "../base_scene/BaseScene.h"
#include "../../camera/Camera.h"
#include "../../3d_shape/Shape3d.h"
#include "../../shaders/directional_light/DirectionalLight.h"
#include "../../texture/ColorTexture/ColorTexture.h"

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

  std::vector<std::unique_ptr<Light>> lightSources;

  std::unique_ptr<ColorTexture> whiteColor;

  DirectionalLight* light;

};

#endif // !SphereScene_Class
