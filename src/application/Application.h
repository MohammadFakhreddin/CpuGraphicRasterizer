#ifndef Application_class
#define Application_class

#include <memory>
#include <chrono>

#include "../Constants.h"
#include "../open_gl/OpenGl.h"
#include "../camera/Camera.h"
#include "../scenes/base_scene/BaseScene.h"

class Application {
public:

  Application() = delete;

  Application(
    Constants::Platform platform,
    unsigned int appScreenWidth,
    unsigned int appScreenHeight,
    unsigned int physicalDeviceScreenWidth,
    unsigned int physicalDeviceScreenHeight
  );

#ifdef __OPENGL__
  
  Application(
    GLFWwindow* window,
    Constants::Platform platform,
    unsigned int appScreenWidth,
    unsigned int appScreenHeight,
    unsigned int physicalDeviceScreenWidth,
    unsigned int physicalDeviceScreenHeight
  );

#endif // __OPENGL__

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

  void init();
  
  void mainLoop(double deltaTime);

  void navigateToScene(unsigned int sceneIndex);

#ifdef __DESKTOP__
  
  void notifyKeyIsPressed(const int& key);

  void run();

#endif

private:

  void update(double deltaTime);

  void render(double deltaTime);

  BaseScene* currentScene;

  unsigned int sceneIndex = 0;
  //TODO Maybe we need maximum 1 scene alive at same time
  std::vector<std::unique_ptr<BaseScene>> sceneList;

  double currentFps = 0;

  OpenGL openGLInstance;

  Matrix2X1Float fpsDrawLocation;

  Matrix2X1Float sceneNameDrawLocation;

#ifdef __DESKTOP__

  GLFWwindow* window;

  static constexpr std::chrono::nanoseconds loopTime = std::chrono::nanoseconds(int(1000.0 / 60.0));

  std::chrono::steady_clock::time_point currentTime;

  std::chrono::steady_clock::time_point lastTime;

  std::chrono::steady_clock::duration deltaTime;

  std::chrono::steady_clock::duration sleepTime;

#endif // __DESKTOP__

};

#endif
