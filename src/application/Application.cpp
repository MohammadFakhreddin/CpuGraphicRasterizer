#include "Application.h"

#include <memory>
#include <vector>

#include "../Constants.h"
#include "../open_gl/OpenGl.h"
#include "../3d_shape/Shape3d.h"
#include "../utils/log/Logger.h"
#include "../utils/math/Math.h"
#include "../file_system/FileSystem.h"
#include "../event_handler/EventHandler.h"
#include "../scenes/bunny_scene/BunnyScene.h"

Application* Application::applicationInstance;

void handleKeyboardEvent(unsigned char key, int x, int y)
{
  Application::getInstance()->getEventHandler().emitEvent<Constants::Buttons>(
    EventHandler::EventName::keyboardKeyIsPressed, 
    Constants::keyMap[std::tolower(key)]
  );
}

Application::Application(
  Constants::Platform platform,
  unsigned int paramAppScreenWidth,
  unsigned int paramAppScreenHeight,
  unsigned int physicalDeviceScreenWidth,
  unsigned int physicalDeviceScreenHeight
)
  :
  platform(platform),
  physicalScreenWidth(physicalDeviceScreenWidth),
  physicalScreenHeight(physicalDeviceScreenHeight),
  appScreenWidth(paramAppScreenWidth),
  appScreenHeight(paramAppScreenHeight),
  openGLInstance(
    paramAppScreenWidth,
    paramAppScreenHeight,
    physicalDeviceScreenWidth,
    physicalDeviceScreenHeight
  )
{
  applicationInstance = this;
#ifdef __DESKTOP__
  glutKeyboardFunc(handleKeyboardEvent);
#endif
  currentScene = std::make_unique<BunnyScene>(openGLInstance);
  {//Shape
    //auto scaleFactor = appScreenWidth/4;
    //Logger::log("Creating shape object");
    // shape = Shape3d::generateTextured3DCube(
    // 	dice.diceCubeTexture,
    // 	dice.diceCubeEdgeList,
    // 	width,
    // 	width,
    // 	width,
    // 	float(appScreenWidth)/2.0f,
    // 	float(appScreenHeight)/2.0f,
    // 	float(cameraInitialZLocation - 500),
    // 	0,
    // 	0,
    // 	0,
    // 	1
    // );
    //============================================
    //shape = FileSystem::loadObjectWithColor(
    //  Path::generateAssetPath("bunny",".obj"),
    //  Vec3DFloat(1.0f,1.0f,1.0f),
    //  true
    //);
    //shape->transformX(float(appScreenWidth)/2.0f);
    //shape->transformY(float(appScreenHeight)/2.0f);
    //shape->transformZ(cameraInitialZLocation - 100.0f);
    //shape->scale(scaleFactor);
    //===========================================
    // shape = FileSystem::loadObjectWithColor(
    //    Path::generateAssetPath("robot",".obj"),
    //    Vec3DFloat(1.0f,1.0f,1.0f),
    //    true
    // );
    // shape->transformX(float(appScreenWidth)/2.0f);
    // shape->transformY(float(appScreenHeight)/2.0f);
    // shape->transformZ(cameraInitialZLocation - 100.0f);
    // shape->scale(10.0f);
    //==========================================
    // shape = FileSystem::loadObjectWithColor(
    //   Path::generateAssetPath("plant",".obj"),
    //   Vec3DFloat(1.0f,1.0f,1.0f),
    //   false
    // );
    // shape->transformX(float(appScreenWidth) * 0.5f);
    // shape->transformY(float(appScreenHeight) * 0.25f);
    // shape->transformZ(cameraInitialZLocation - 100.0f);
    // shape->scale(5.0f);
    //==========================================
    //Logger::log("Creating shape was successful");
  }
}

Application* Application::getInstance() {
  return applicationInstance;
}

void Application::notifyScreenSurfaceChanged(
  unsigned int paramAppScreenWidth,
  unsigned int paramAppScreenHeight,
  unsigned int paramPhysicalScreenWidth,
  unsigned int paramPhysicalScreenHeight,
  bool forceNewAppScreenWidthAndHeight
){
  Logger::log("Surface has changed");

  if(forceNewAppScreenWidthAndHeight==true){
    this->appScreenWidth = paramAppScreenWidth;
    this->appScreenHeight = paramAppScreenHeight;
  }

  this->physicalScreenWidth = paramPhysicalScreenWidth;
  this->physicalScreenHeight = paramPhysicalScreenHeight;

  openGLInstance.notifyScreenSurfaceChanged(
    paramAppScreenWidth,
    paramAppScreenHeight,
    paramPhysicalScreenWidth,
    paramPhysicalScreenHeight);

  {//Sending events to all listeners
    EventHandler::ScreenSurfaceChangeEventData data;
    data.appScreenWidth = appScreenWidth;
    data.appScreenHeight = appScreenHeight;
    data.physicalScreenWidth = physicalScreenWidth;
    data.physicalScreenHeight = physicalScreenHeight;
    data.forceNewAppScreenWidthAndHeight = forceNewAppScreenWidthAndHeight;

    eventHandler.emitEvent(
      EventHandler::EventName::screenSurfaceChanged,
      data
    );
  }
}



void Application::render(double deltaTime) {
  openGLInstance.clear();
  {//Drawing current active scene
    currentScene->render(deltaTime);
  }
  {//FPSText
    openGLInstance.drawText(0,0,std::to_string(currentFps),1.0f,1.0f,1.0f);
  }
  {//SceneNameText
    openGLInstance.drawText(0, appScreenHeight - 32, currentScene->getSceneName(), 1.0f, 1.0f, 1.0f);
  }
  // dice.diceCubeTexture->render();
  openGLInstance.flush();
}

void Application::update(double deltaTime) {
  currentScene->update(deltaTime);
}

void Application::mainLoop(double deltaTime){
  deltaTime = Math::min(deltaTime,100.0);
  update(deltaTime);
  render(deltaTime);
  if(deltaTime>0){
    currentFps = 1000.0f/deltaTime;
  }
  assert(openGLInstance.checkForOpenGlError());
}

unsigned int Application::getAppScreenHeight(){
  return appScreenHeight;
}

unsigned int Application::getAppScreenWidth(){
  return appScreenWidth;
}

unsigned int Application::getPhysicalScreenWidth(){
  return physicalScreenWidth;
}

unsigned int Application::getPhysicalScreenHeight(){
  return physicalScreenHeight;
}

EventHandler& Application::getEventHandler() {
  return eventHandler;
}

Application::~Application() {
  applicationInstance = nullptr;
}