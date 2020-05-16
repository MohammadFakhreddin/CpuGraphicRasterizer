#include "Application.h"

#define DEBUG_INPUT

#include <memory>
#include <vector>

#include "../data_access_point/DataAccessPoint.h"
#include "../scenes/monkey_scene/MonkeyScene.h"
#include "../scenes/colored_cube_scene/ColoredCubeScene.h"
#include "../scenes/textured_cube_scene/TexturedCubeScene.h"
#include "../scenes/bunny_scene/BunnyScene.h"
#include "../scenes/plant_scene/PlantScene.h"
#include "../scenes/robot_scene/RobotScene.h"
#include "../scenes/spehre_scene/SphereScene.h"

//  UI Libraries
/*
    Source : https://philippegroarke.com/posts/2018/c++_ui_solutions/
    GUILite
    Juce
*/
//Use this source to assign process to all cpu cores
/*
https://stackoverflow.com/questions/150355/programmatically-find-the-number-of-cores-on-a-machine
*/

#ifdef __DESKTOP__
void handleKeyboardEvent(GLFWwindow* window, int key, int scanCode, int action, int mode)
{
#ifdef DEBUG_INPUT
  Logger::log("key: "+std::to_string(key) + " ScanCode: " + std::to_string(scanCode) + " mode: " + std::to_string(mode));
#endif
  if(action == GLFW_PRESS){
    DataAccessPoint::getInstance()->getEventHandler().emitEvent<int>(
      EventHandler::EventName::keyboardCharacterIsPressed,
      key
    );
    DataAccessPoint::getInstance()->getEventHandler().emitEvent<int>(
      EventHandler::EventName::keyboardScanCodeIsPressed,
      scanCode
    );
  }
}

void handleMouseEvent(int button, int state, int x, int y) {
  DataAccessPoint::getInstance()->getEventHandler().emitEvent<int>(
    state == 1 ? EventHandler::EventName::mouseButtonRelease : EventHandler::EventName::mouseButtonPressed,
    button
  );
  DataAccessPoint::getInstance()->setMousePosition(x,y);
}

void handleMotionEvent(int x, int y){
  DataAccessPoint::getInstance()->setMousePosition(x,y);
}

#endif

Application::Application(
#ifdef __OPENGL__
  GLFWwindow* window,
#endif
  Constants::Platform platform,
  unsigned int appScreenWidth,
  unsigned int appScreenHeight,
  unsigned int physicalDeviceScreenWidth,
  unsigned int physicalDeviceScreenHeight
  )
  :
  openGLInstance(
  #ifdef __OPENGL__
    window,
  #endif
    appScreenWidth,
    appScreenHeight,
    physicalDeviceScreenWidth,
    physicalDeviceScreenHeight
  ),
#ifdef __DESKTOP__
  window(window),
#endif // __DESKTOP__
  fpsDrawLocation(-1.0f, -1.0f),
  sceneNameDrawLocation(-1.0f, 0.9f)
{
  {
    DataAccessPoint::createInstance();

    DataAccessPoint::getInstance()->setAppScreenWidth(appScreenWidth);
    DataAccessPoint::getInstance()->setAppScreenHeight(appScreenHeight);
    DataAccessPoint::getInstance()->setPhysicalScreenWidth(physicalDeviceScreenWidth);
    DataAccessPoint::getInstance()->setPhysicalScreenHeight(physicalDeviceScreenHeight);
    DataAccessPoint::getInstance()->setPlatform(platform);
  }

#ifdef __DESKTOP__
  glfwSetKeyCallback(window, handleKeyboardEvent);
  GLFWmousebuttonfun(handleMouseEvent);
  GLFWcursorposfun(handleMotionEvent);
#endif
  {
    //sceneList.emplace_back(std::make_unique<LightPerPixelScene>(openGLInstance));
    sceneList.emplace_back(std::make_unique<MonkeyScene>(openGLInstance));
    sceneList.emplace_back(std::make_unique<BunnyScene>(openGLInstance));
    sceneList.emplace_back(std::make_unique<SphereScene>(openGLInstance));
    sceneList.emplace_back(std::make_unique<ColoredCubeScene>(openGLInstance));
    sceneList.emplace_back(std::make_unique<TexturedCubeScene>(openGLInstance));
    sceneList.emplace_back(std::make_unique<RobotScene>(openGLInstance));
    sceneList.emplace_back(std::make_unique<PlantScene>(openGLInstance));
    navigateToScene(0);
  }

#ifdef __DESKTOP__
  DataAccessPoint::getInstance()->getEventHandler().subscribeToEvent<int>(
    EventHandler::EventName::keyboardScanCodeIsPressed,
    "Application",
    std::bind(&Application::notifyKeyIsPressed, this, std::placeholders::_1)
  );
#endif // __DESKTOP__

}

void Application::notifyScreenSurfaceChanged(
  unsigned int paramAppScreenWidth,
  unsigned int paramAppScreenHeight,
  unsigned int paramPhysicalScreenWidth,
  unsigned int paramPhysicalScreenHeight,
  bool forceNewAppScreenWidthAndHeight
){
  Logger::log("Surface has changed");

  if(forceNewAppScreenWidthAndHeight){
    DataAccessPoint::getInstance()->setPhysicalScreenWidth(paramAppScreenWidth);
    DataAccessPoint::getInstance()->setAppScreenHeight(paramAppScreenHeight);

    sceneNameDrawLocation.setY(0.9f);
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

void Application::update(double deltaTime) {
  currentScene->update(deltaTime);
}

void Application::render(double deltaTime) {
  openGLInstance.clear();
  {//Drawing current active scene
    currentScene->render(deltaTime);
  }
  //TODO Use font instead
  // {//FPSText
  //   openGLInstance.drawText(
  //     fpsDrawLocation.getX(),
  //     fpsDrawLocation.getY(),
  //     std::to_string(currentFps),
  //     1.0f,1.0f,1.0f
  //   );
  // }
  // {//SceneNameText
  //   openGLInstance.drawText(
  //     sceneNameDrawLocation.getX(),
  //     sceneNameDrawLocation.getY(),
  //     currentScene->getSceneName(), 
  //     1.0f, 1.0f, 1.0f
  //   );
  // }
  openGLInstance.flush();
}

void Application::mainLoop(double deltaTime){
  deltaTime = fmin(deltaTime,100.0);
  update(deltaTime);
  render(deltaTime);
  if(deltaTime>0){
    currentFps = 1000.0f/deltaTime;
  }
  assert(openGLInstance.checkForOpenGlError());
}

#ifdef __DESKTOP__
void Application::run() {
  while (!glfwWindowShouldClose(window))
  {
    currentTime = glfwGetTime();
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;
    mainLoop(deltaTime);
    sleepTime = loopTime - deltaTime;
    if (sleepTime > 0.0) {
      std::this_thread::sleep_for(std::chrono::nanoseconds(int(1000 * sleepTime)));
    }
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}
#endif // __DESKTOP__

void Application::navigateToScene(unsigned int sceneIndex) {
  assert(sceneIndex >= 0 && sceneIndex <= sceneList.size());
  currentScene = sceneList.at(sceneIndex).get();
  DataAccessPoint::getInstance()->getEventHandler().emitEvent<std::string>(
    EventHandler::EventName::activeSceneChanged,
    currentScene->getSceneName()
  );
}

#ifdef __DESKTOP__
void Application::notifyKeyIsPressed(const int& key) {
  if (key == GLFW_KEY_TAB) {
    sceneIndex++;
    if (sceneIndex >= sceneList.size()) {
      sceneIndex = 0;
    }
    navigateToScene(sceneIndex);
  }
}
#endif
