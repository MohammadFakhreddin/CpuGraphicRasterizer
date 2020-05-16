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
  
  DataAccessPoint::getInstance()->getEventHandler().subscribeToEvent<int>(
    EventHandler::EventName::keyboardCharacterIsPressed,
    sceneName,
    std::bind(&BaseScene::notifyForNewKeyboardCharacterEvent,this, std::placeholders::_1)
  );
  
  DataAccessPoint::getInstance()->getEventHandler().subscribeToEvent<int>(
    EventHandler::EventName::keyboardScanCodeIsPressed,
    sceneName,
    std::bind(&BaseScene::notifyForNewScanCodeCharacterEvent,this, std::placeholders::_1)
  );

  DataAccessPoint::getInstance()->getEventHandler().subscribeToEvent<std::string>(
    EventHandler::EventName::activeSceneChanged,
    sceneName,
    std::bind(&BaseScene::onActiveSceneChanged,this,std::placeholders::_1)
  );

  DataAccessPoint::getInstance()->getEventHandler().subscribeToEvent<int>(
    EventHandler::EventName::mouseButtonPressed,
    sceneName,
    std::bind(&BaseScene::mouseButtonPressed,this,std::placeholders::_1)
  );

  DataAccessPoint::getInstance()->getEventHandler().subscribeToEvent<int>(
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
void BaseScene::notifyForNewKeyboardCharacterEvent(const int & keyEvent) {
  if (isPageActive == false) {
    return;
  }
  keyEvents[keyEvent] = true;
}

void BaseScene::notifyForNewScanCodeCharacterEvent(const int& scanCode){
  if(isPageActive == false){
    return;
  }
  scanCodeEvents[scanCode] = true;
}

const bool& BaseScene::useKeyboardCharacterEvent(const int& keyboardKey){
  temporaryKeyEventPlaceholder = keyEvents[keyboardKey];
  keyEvents[keyboardKey] = false;
  return temporaryKeyEventPlaceholder;
}

const bool& BaseScene::useScanCodeEvent(const int& scanCode){
  temporaryScanCodeEventPlaceholder = scanCodeEvents[scanCode];
  scanCodeEvents[scanCode] = false;
  return temporaryScanCodeEventPlaceholder;
}

const bool& BaseScene::getMouseEvent(const int& mouseButtonName){
  return mouseEvents[mouseButtonName];
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

void BaseScene::mouseButtonPressed(const int& buttonName){
  mouseEvents[buttonName] = true;
}

void BaseScene::mouseButtonReleased(const int& buttonName){
  mouseEvents[buttonName] = false;
}