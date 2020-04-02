#ifndef Application_class
#define Application_class

#include <memory>

#include "../Constants.h"
#include "./../3d_models/Dice.h"
#include "../3d_shape/Shape3d.h"
#include "../open_gl/OpenGl.h"
#include "../camera/Camera.h"
#include "../scenes/base_scene/BaseScene.h"

class Application {
public:

  Application(
    Constants::Platform platform,
    unsigned int appScreenWidth,
    unsigned int appScreenHeight,
    unsigned int physicalDeviceScreenWidth,
    unsigned int physicalDeviceScreenHeight
  );

  /**
   * Currently this event is only for android
  */
  void notifyScreenSurfaceChanged(
    unsigned int paramAppScreenWidth,
    unsigned int paramAppScreenHeight,
    unsigned int paramPhysicalScreenWidth,
    unsigned int paramPhysicalScreenHeight,
    bool forceNewAppScreenData
  );
  
  void mainLoop(double deltaTime);

  void navigateToScene(unsigned int sceneIndex);

#ifdef __DESKTOP__
  void notifyKeyIsPressed(Constants::Buttons key);
#endif

private:

  void render(double deltaTime);
  
  void update(double deltaTime);

  BaseScene* currentScene;

  unsigned int sceneIndex = 0;
  //TODO Maybe we need maximum 1 scene alive at same time
  std::vector<std::unique_ptr<BaseScene>> sceneList;

  double currentFps = 0;

  OpenGL openGLInstance;

  _Vec2d<unsigned int> fpsDrawLocation;

  _Vec2d<unsigned int> sceneNameDrawLocation;
};

#endif
