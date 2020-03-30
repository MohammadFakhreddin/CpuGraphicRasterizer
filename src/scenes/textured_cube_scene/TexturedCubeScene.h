#ifndef TexturedCubeScene_Class
#define TexturedCubeScene_Class

#include <memory>

#include "../base_scene/BaseScene.h"
#include "../../camera/Camera.h"
#include "../../3d_shape/Shape3d.h"
#include "../../open_gl/OpenGl.h"
#include "../../3d_models/Dice.h"

class TexturedCubeScene : public BaseScene {

private:

  static constexpr float shapeRotationSpeed = 0.01f;

  static constexpr float cameraInitialZLocation = 1000.0f;

  static constexpr float cameraInitialMaximumFov = 1000.0f;

public:

  TexturedCubeScene(OpenGL& gl);

  void update(double deltaTime) override;

  void render(double deltaTime) override;

private:

  Camera cameraInstance;

  std::unique_ptr<Shape3d> cube;

  std::vector<std::unique_ptr<Light>> lightSources;

  Dice dice;

};

#endif // !TexturedCubeScene_Class
