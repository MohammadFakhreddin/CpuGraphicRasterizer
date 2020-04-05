#ifndef ColoredCubeScene_Class
#define ColoredCubeScene_Class

#include <memory>
#include <vector>

#include "../base_scene/BaseScene.h"
#include "../../camera/Camera.h"
#include "../../3d_shape/Shape3d.h"
#include "../../open_gl/OpenGl.h"
#include "../../3d_models/ColoredCube.h"
#include "../../shaders/directional_light/DirectionalLight.h"

class ColoredCubeScene : public BaseScene {

private:

  static constexpr float shapeRotationSpeed = 0.01f;

  static constexpr float cameraInitialZLocation = 1000.0f;

  static constexpr float cameraInitialMaximumFov = 1000.0f;

  static constexpr float lightTransformSpeed = 0.01f;

public:

  ColoredCubeScene(OpenGL& gl);

  void update(double deltaTime) override;

  void render(double deltaTime) override;

private:

  Camera cameraInstance;

  std::unique_ptr<Shape3d> cube;

  ColorCube colorCube;

  std::vector<std::unique_ptr<Light>> lightSources;

  DirectionalLight* light;

};

#endif // !ColoredCubeScene_Class
