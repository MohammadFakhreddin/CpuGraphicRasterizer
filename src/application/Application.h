#ifndef Application_class
#define Application_class

#include <memory>

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

  void init(
    Constants::Platform& platform,
    unsigned int& appScreenWidth,
    unsigned int& appScreenHeight,
    unsigned int& physicalDeviceScreenWidth,
    unsigned int& physicalDeviceScreenHeight
  );
  
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
  //For ubuntu only we need system clock
  static constexpr std::chrono::nanoseconds loopTime = std::chrono::nanoseconds(int(1000000000.0 / 90.0));

  supportedClock::time_point currentTime;

  supportedClock::time_point lastTime;

  supportedClock::duration deltaTimeInChrono;

  double deltaTimeInSecond;

  supportedClock::duration sleepTime;

#endif // __DESKTOP__

};

#endif
