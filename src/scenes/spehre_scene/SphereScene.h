#ifndef SphereScene_Class
#define SphereScene_Class

#include "../base_scene/BaseScene.h"
#include "../../camera/Camera.h"

class SphereScene : public BaseScene {

public:

  SphereScene(OpenGL& gl);

  void update(double deltaTime);

  void render(double deltaTime);

private:

  static constexpr unsigned int cameraInitialMaximumFov = 1000;

  static constexpr unsigned int cameraInitialZLocation = 1000;

  Camera cameraInstance;

};

#endif // !SphereScene_Class
