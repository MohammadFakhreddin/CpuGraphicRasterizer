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
  DataAccessPoint::getInstance()->getEventHandler().subscribeToEvent<Constants::Buttons>(
    EventHandler::EventName::keyboardKeyIsPressed,
    sceneName,
    std::bind(&BaseScene::notifyKeyIsPressed,this, std::placeholders::_1)
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
void BaseScene::notifyKeyIsPressed(const Constants::Buttons & keyEvent) {
  keyEvents[keyEvent] = true;
}
#endif // __DESKTOP__

#ifdef __DESKTOP__
bool BaseScene::useKeyEvent(const Constants::Buttons & keyEvent) {
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