#include "./BaseScene.h"

#include <functional>

#include "../../Constants.h"
#include "../../open_gl/OpenGl.h"
#include "../../event_handler/EventHandler.h"
#include "../../utils/log/Logger.h"
#include "../../utils/operators/Operators.h"
#include "../../data_access_point/DataAccessPoint.h"

BaseScene::BaseScene(OpenGL& gl,std::string sceneName) 
  : 
  gl(gl),
  sceneName(sceneName)
{
  if (sceneName.empty()) {
    Logger::exception("Scene name is not specified");
  }

#ifdef __DESKTOP__
  //TODO Move all events to data access point
  DataAccessPoint::getInstance()->getEventHandler().subscribeToEvent<Constants::KeyboardButtons>(
    EventHandler::EventName::keyboardKeyIsPressed,
    sceneName,
    std::bind(&BaseScene::notifyKeyIsPressed,this, std::placeholders::_1)
  );
  
  DataAccessPoint::getInstance()->getEventHandler().subscribeToEvent<std::string>(
    EventHandler::EventName::activeSceneChanged,
    sceneName,
    std::bind(&BaseScene::onActiveSceneChanged,this,std::placeholders::_1)
  );

  DataAccessPoint::getInstance()->getEventHandler().subscribeToEvent<Constants::MouseButtonName>(
    EventHandler::EventName::mouseButtonPressed,
    sceneName,
    std::bind(&BaseScene::mouseButtonPressed,this,std::placeholders::_1)
  );

  DataAccessPoint::getInstance()->getEventHandler().subscribeToEvent<Constants::MouseButtonName>(
    EventHandler::EventName::mouseButtonRelease,
    sceneName,
    std::bind(&BaseScene::mouseButtonReleased,this,std::placeholders::_1)
  );
#endif // __DESKTOP__

}

void BaseScene::update(double deltaTime) {
  Logger::exception("BaseScene::update function is not implemented in child class");
}

void BaseScene::render(double deltaTime) {
  Logger::exception("BaseScene::render function is not implemented in child class");
}

#ifdef __DESKTOP__
void BaseScene::notifyKeyIsPressed(const Constants::KeyboardButtons & keyEvent) {
  if (isPageActive == false) {
    return;
  }
  keyEvents[keyEvent] = true;
}
#endif // __DESKTOP__

#ifdef __DESKTOP__
bool BaseScene::useKeyEvent(const Constants::KeyboardButtons & keyEvent) {
  temporaryKeyEventPlaceholder = keyEvents[keyEvent];
  keyEvents[keyEvent] = false;
  return temporaryKeyEventPlaceholder;
}
#endif // __DESKTOP__


BaseScene::~BaseScene() {
  if (DataAccessPoint::getInstance()) {
    DataAccessPoint::getInstance()->getEventHandler().unSubscribeFromEvents(
      sceneName
    );
  }
}

std::string BaseScene::getSceneName() {
  return sceneName;
}

void BaseScene::onActiveSceneChanged(const std::string& sceneName) {
  isPageActive = this->sceneName == sceneName;
}

void BaseScene::mouseButtonPressed(const Constants::MouseButtonName& buttonName){
  mouseEvents[buttonName] = true;
}

void BaseScene::mouseButtonReleased(const Constants::MouseButtonName& buttonName){
  mouseEvents[buttonName] = false;
}