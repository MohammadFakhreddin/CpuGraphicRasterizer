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
#include "../data_access_point/DataAccessPoint.h"

void handleKeyboardEvent(unsigned char key, int x, int y)
{
  DataAccessPoint::getInstance()->getEventHandler().emitEvent<Constants::Buttons>(
    EventHandler::EventName::keyboardKeyIsPressed,
    DataAccessPoint::getInstance()->getKeyCode(key)
  );
}

Application::Application(
  Constants::Platform platform,
  unsigned int appScreenWidth,
  unsigned int appScreenHeight,
  unsigned int physicalDeviceScreenWidth,
  unsigned int physicalDeviceScreenHeight
  )
  :
  openGLInstance(
    appScreenWidth,
    appScreenHeight,
    physicalDeviceScreenWidth,
    physicalDeviceScreenHeight
    ),
  fpsDrawLocation(0.0f, 0.0f),
  sceneNameDrawLocation(0.0f, appScreenHeight - 32)
{

  DataAccessPoint::createInstance();
  
  DataAccessPoint::getInstance()->setAppScreenWidth(appScreenWidth);
  DataAccessPoint::getInstance()->setAppScreenHeight(appScreenHeight);
  DataAccessPoint::getInstance()->setPhysicalScreenWidth(physicalDeviceScreenWidth);
  DataAccessPoint::getInstance()->setPhysicalScreenHeight(physicalDeviceScreenHeight);
  DataAccessPoint::getInstance()->setPlatform(platform);

#ifdef __DESKTOP__
  glutKeyboardFunc(handleKeyboardEvent);
  //TODO We need a singleton user class to store data in it

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

void Application::notifyScreenSurfaceChanged(
  unsigned int paramAppScreenWidth,
  unsigned int paramAppScreenHeight,
  unsigned int paramPhysicalScreenWidth,
  unsigned int paramPhysicalScreenHeight,
  bool forceNewAppScreenWidthAndHeight
){
  Logger::log("Surface has changed");

  if(forceNewAppScreenWidthAndHeight==true){
    DataAccessPoint::getInstance()->setPhysicalScreenWidth(paramAppScreenWidth);
    DataAccessPoint::getInstance()->setAppScreenHeight(paramAppScreenHeight);

    sceneNameDrawLocation.setY(paramAppScreenHeight - 32);
  }

  DataAccessPoint::getInstance()->setPhysicalScreenWidth(paramPhysicalScreenWidth);
  DataAccessPoint::getInstance()->setPhysicalScreenHeight(paramPhysicalScreenHeight);
  
  openGLInstance.notifyScreenSurfaceChanged(
    paramAppScreenWidth,
    paramAppScreenHeight,
    paramPhysicalScreenWidth,
    paramPhysicalScreenHeight);

  {//Sending events to all listeners
    DataAccessPoint::getInstance()->getEventHandler().emitEvent<bool>(
      EventHandler::EventName::screenSurfaceChanged,
      forceNewAppScreenWidthAndHeight
    );
  }
}



void Application::render(double deltaTime) {
  openGLInstance.clear();
  {//Drawing current active scene
    currentScene->render(deltaTime);
  }
  {//FPSText
    openGLInstance.drawText(
      fpsDrawLocation.getX(),
      fpsDrawLocation.getY(),
      std::to_string(currentFps),
      1.0f,1.0f,1.0f
    );
  }
  {//SceneNameText
    openGLInstance.drawText(
      sceneNameDrawLocation.getX(),
      sceneNameDrawLocation.getY(),
      currentScene->getSceneName(), 
      1.0f, 1.0f, 1.0f
    );
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