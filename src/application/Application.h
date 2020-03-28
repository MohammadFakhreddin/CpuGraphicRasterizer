#ifndef Application_class
#define Application_class

#include <memory>

#include "../Constants.h"
#include "./../3d_models/Wood.h"
#include "./../3d_models/Sky.h"
#include "./../3d_models/Dice.h"
#include "../3d_shape/Shape3d.h"
#include "../open_gl/OpenGl.h"
#include "../camera/Camera.h"
#include "../shaders/diffuse_light/DiffuseLight.h"
#include "../scenes/base_scene/BaseScene.h"
#include "../event_handler/EventHandler.h"

class Application {
public:

  static Application* getInstance();

  Application(
    Constants::Platform platform,
    unsigned int appScreenWidth,
    unsigned int appScreenHeight,
    unsigned int physicalDeviceScreenWidth,
    unsigned int physicalDeviceScreenHeight
  );

  ~Application();
  
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
  
  unsigned int getAppScreenWidth();
  
  unsigned int getAppScreenHeight();
  
  unsigned int getPhysicalScreenWidth();
  
  unsigned int getPhysicalScreenHeight();

  EventHandler& getEventHandler();

private:

  static Application* applicationInstance;

  EventHandler eventHandler;

  void render(double deltaTime);
  
  void update(double deltaTime);

  std::unique_ptr<BaseScene> currentScene;

  double currentFps = 0;

  OpenGL openGLInstance;
  
  unsigned int physicalScreenWidth;
  
  unsigned int physicalScreenHeight;
  
  unsigned int appScreenWidth;
  
  unsigned int appScreenHeight;

  Constants::Platform platform;

  /*

  Wood wood;
  
  Sky sky;
  
  Dice dice;*/
};

#endif
