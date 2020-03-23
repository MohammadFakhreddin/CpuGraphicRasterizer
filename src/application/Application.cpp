#include "Application.h"

#include <memory>
#include <vector>

#include "../Constants.h"
#include "../open_gl/OpenGl.h"
#include "../3d_shape/Shape3d.h"
#include "../utils/log/Logger.h"
#include "../utils/math/Math.h"
#include "../file_system/FileSystem.h"

void handleKeyboardEvent(unsigned char key, int x, int y)
{
  if (std::tolower(key) == 'a') {
    Application::getInstance()->notifyKeyIsPressed(Application::Buttons::keyA);
  }
  if (std::tolower(key) == 'd') {
    Application::getInstance()->notifyKeyIsPressed(Application::Buttons::keyD);
  }
  if (std::tolower(key) == 'w') {
    Application::getInstance()->notifyKeyIsPressed(Application::Buttons::keyW);
  }
  if (std::tolower(key) == 's') {
    Application::getInstance()->notifyKeyIsPressed(Application::Buttons::keyS);
  }
  if (std::tolower(key) == 'c') {
    Application::getInstance()->notifyKeyIsPressed(Application::Buttons::keyC);
  }
  if(std::tolower(key) == 'v'){
    Application::getInstance()->notifyKeyIsPressed(Application::Buttons::keyV);
  }
  if(std::tolower(key) == 'u'){
    Application::getInstance()->notifyKeyIsPressed(Application::Buttons::keyU);
  }
  if(std::tolower(key) == 'h'){
    Application::getInstance()->notifyKeyIsPressed(Application::Buttons::keyH);
  }
  if(std::tolower(key) == 'j'){
    Application::getInstance()->notifyKeyIsPressed(Application::Buttons::keyJ);
  }
  if(std::tolower(key) == 'k'){
    Application::getInstance()->notifyKeyIsPressed(Application::Buttons::keyK);
  }
  if(std::tolower(key) == 'y'){
    Application::getInstance()->notifyKeyIsPressed(Application::Buttons::keyY);
  }
  if(std::tolower(key) == 'i'){
    Application::getInstance()->notifyKeyIsPressed(Application::Buttons::keyI);
  }
}

Application::Application(
  Application::Platform platform,
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
  light(
    float(paramAppScreenWidth)/2.0f,
    float(paramAppScreenHeight)/2.0f,
    cameraInitialZLocation - 50
  ),
  openGLInstance(
    paramAppScreenWidth,
    paramAppScreenHeight,
    physicalDeviceScreenWidth,
    physicalDeviceScreenHeight
  ),
  cameraInstance(
    openGLInstance,
    light,
    cameraInitialZLocation,
    cameraInitialMaximumFov,
    0,
    (int)paramAppScreenWidth,
    0,
    (int)paramAppScreenHeight
  )
{
  instance = this;
  {//Shape
    Logger::log("Creating shape object");
    auto width = appScreenWidth/5;
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
    // shape = FileSystem::loadObjectWithColor(
    //    Path::generateAssetPath("bunny",".obj"),
    //    Vec3DFloat(1.0f,1.0f,1.0f)
    // );
    // shape->transformX(float(appScreenWidth)/2.0f);
    // shape->transformY(float(appScreenHeight)/2.0f);
    // shape->transformZ(cameraInitialZLocation - 100.0f);
    // shape->scale(200.0f);
    shape = FileSystem::loadObjectWithColor(
       Path::generateAssetPath("robot",".obj"),
       Vec3DFloat(1.0f,1.0f,1.0f)
    );
    shape->transformX(float(appScreenWidth)/2.0f);
    shape->transformY(float(appScreenHeight)/2.0f);
    shape->transformZ(cameraInitialZLocation - 100.0f);
    shape->scale(10.0f);
    Logger::log("Creating shape was successful");
  }
#ifdef __DESKTOP__
  glutKeyboardFunc(handleKeyboardEvent);
#endif
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
    cameraInstance.notifyScreenSurfaceIsChanged(
        0, (int)paramAppScreenWidth,
        0, (int)paramAppScreenHeight);
  }

  this->physicalScreenWidth = paramPhysicalScreenWidth;
  this->physicalScreenHeight = paramPhysicalScreenHeight;

  openGLInstance.notifyScreenSurfaceChanged(
    paramAppScreenWidth,
    paramAppScreenHeight,
    paramPhysicalScreenWidth,
    paramPhysicalScreenHeight);
}



void Application::render(double deltaTime) {
  openGLInstance.clear();
  cameraInstance.render(deltaTime);
  {//FPSText
    openGLInstance.drawText(0,0,std::to_string(currentFps),1.0f,1.0f,1.0f);
  }
  // dice.diceCubeTexture->render();
  openGLInstance.flush();
}

void Application::update(double deltaTime) {
  {//We rotate light by keyboard
    if(keyEvents[Buttons::keyA]){
      cameraInstance.getLight().transformX(
          float(deltaTime * Application::lightTransformSpeed * -1.0f)
      );
      keyEvents[Buttons::keyA] = false;
    }
    if(keyEvents[Buttons::keyD]){
      cameraInstance.getLight().transformX(
          float(deltaTime * Application::lightTransformSpeed)
      );
      keyEvents[Buttons::keyD] = false;
    }
    if(keyEvents[Buttons::keyW]){
      cameraInstance.getLight().transformY(
          float(deltaTime *  Application::lightTransformSpeed)
      );
      keyEvents[Buttons::keyW] = false;
    }
    if(keyEvents[Buttons::keyS]){
      cameraInstance.getLight().transformY(
          float(deltaTime *  Application::lightTransformSpeed * -1.0)
      );
      keyEvents[Buttons::keyS] = false;
    }
    if(keyEvents[Buttons::keyC]){
      cameraInstance.getLight().transformZ(
          float(deltaTime *  Application::lightTransformSpeed * -1.0 * 0.5)
      );
      keyEvents[Buttons::keyC] = false;
    }
    if(keyEvents[Buttons::keyV]){
      cameraInstance.getLight().transformZ(
          float(deltaTime *  Application::lightTransformSpeed * 1.0 * 0.5)
      );
      keyEvents[Buttons::keyV] = false;
    }
  }
  {//Rotating shape by keyboard
    if(keyEvents[Buttons::keyU]){
      shape->rotateZ(float(1.0 * Application::shapeRotationSpeed * deltaTime));
      keyEvents[Buttons::keyU] = false;
    }
    if(keyEvents[Buttons::keyJ]){
      shape->rotateZ(float(-1.0 * Application::shapeRotationSpeed * deltaTime));
      keyEvents[Buttons::keyJ] = false;
    }
    if(keyEvents[Buttons::keyK]){
      shape->rotateY(float(1.0 * Application::shapeRotationSpeed * deltaTime));
      keyEvents[Buttons::keyK] = false;
    }
    if(keyEvents[Buttons::keyH]){
      shape->rotateY(float(-1.0 * Application::shapeRotationSpeed * deltaTime));
      keyEvents[Buttons::keyH] = false;
    }
    if(keyEvents[Buttons::keyI]){
      shape->rotateX(float(1.0 * Application::shapeRotationSpeed * deltaTime));
      keyEvents[Buttons::keyI] = false;
    }
    if(keyEvents[Buttons::keyY]){
      shape->rotateX(float(-1.0 * Application::shapeRotationSpeed * deltaTime));
      keyEvents[Buttons::keyY] = false;
    }
  }
  {//Temporary code for auto rotation
    // shape->rotateY(float(-1.0f * Application::shapeRotationSpeed * deltaTime * 0.1f));
    // shape->rotateX(float(-1.0f * Application::shapeRotationSpeed * deltaTime * 0.1f));
    // shape->rotateZ(float(-1.0f * Application::shapeRotationSpeed * deltaTime * 0.1f));
  }
  shape->update(deltaTime,cameraInstance);
  cameraInstance.update(deltaTime);
}

void Application::notifyKeyIsPressed(Application::Buttons keyEvent)
{
  keyEvents[keyEvent] = true;
}

Application* Application::getInstance()
{
  return Application::instance;
}

void Application::mainLoop(double deltaTime){
  deltaTime = Math::max(deltaTime,100.0);
  update(deltaTime);
  render(deltaTime);
  if(deltaTime>0){
    currentFps = 1000.0f/deltaTime;
  }
  assert(openGLInstance.checkForOpenGlError());
}

Application* Application::instance;

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