#ifndef Application_class
#define Application_class

#include <memory>
#include <unordered_map>

#include "./../3d_models/Wood.h"
#include "./../3d_models/Sky.h"
#include "./../3d_models/Dice.h"
#include "../3d_shape/Shape3d.h"
#include "../open_gl/OpenGl.h"
#include "../camera/Camera.h"
#include "../shaders/diffuse_light/DiffuseLight.h"

class Application {
public:
  enum Platform{
    Windows,
    Mac,
    Iphone,
    Android
  };
  enum Buttons
  {
    keyA,
    keyD,
    keyW,
    keyS,
    keyE,
    keyQ,
    keyR,
    keyT,
    keyF,
    keyG,
    keyX,
    keyC,
    keyV,
    keyB,
    keyU,
    keyH,
    keyJ,
    keyK,
    keyY,
    keyI
  };

  static constexpr float shapeTransformSpeed = 1.0f;
  static constexpr float shapeRotationSpeed = 0.01f;
  static constexpr float shapeScaleSpeed = 0.1f;

  static constexpr float lightTransformSpeed = 0.2f;

  static constexpr float cameraInitialZLocation = 1000.0f;
  static constexpr float cameraInitialMaximumFov = -1000.0f;

  static Application* getInstance();
  static Application* instance;

  Application(
          Application::Platform platform,
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
  void notifyKeyIsPressed(Application::Buttons);
  unsigned int getAppScreenWidth();
  unsigned int getAppScreenHeight();
  unsigned int getPhysicalScreenWidth();
  unsigned int getPhysicalScreenHeight();

private:

  void render(double deltaTime);
  void update(double deltaTime);

  double currentFps = 0;

  DiffuseLight light;
  OpenGL openGLInstance;
  Camera cameraInstance;

  unsigned int physicalScreenWidth;
  unsigned int physicalScreenHeight;
  unsigned int appScreenWidth;
  unsigned int appScreenHeight;

  Application::Platform platform;

  std::unique_ptr<Shape3d> shape;

  std::unordered_map<Application::Buttons,bool> keyEvents;

  Wood wood;
  Sky sky;
  Dice dice;
};

#endif
