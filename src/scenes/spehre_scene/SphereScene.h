#ifndef SphereScene_Class
#define SphereScene_Class

#include <memory>

#include "../base_scene/BaseScene.h"
#include "../../camera/Camera.h"
#include "../../3d_shape/Shape3d.h"

class SphereScene : public BaseScene {

public:

  SphereScene(OpenGL& gl);

  void update(double deltaTime);

  void render(double deltaTime);

private:

  static constexpr unsigned int cameraInitialMaximumFov = 1000;

  static constexpr unsigned int cameraInitialZLocation = 1000;

  static constexpr float shapeRotationSpeed = 0.01f;

  Camera cameraInstance;

  std::unique_ptr<Shape3d> sphere;

  std::vector<std::unique_ptr<Light>> lightSources;

};

#endif // !SphereScene_Class
