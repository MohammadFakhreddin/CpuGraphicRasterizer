#ifndef ColoredCubeScene_Class
#define ColoredCubeScene_Class

#include "../base_scene/BaseScene.h"
#include "../../camera/Camera.h"

class ColoredCubeScene : public BaseScene {

private:

  static constexpr float shapeRotationSpeed = 0.01f;

private:

  Camera cameraInstance;

};

#endif // !ColoredCubeScene_Class
