#include "./BaseScene.h"

#include <functional>

#include "../../Constants.h"
#include "../../open_gl/OpenGl.h"
#include "../../event_handler/EventHandler.h"
#include "../../utils/log/Logger.h"
#include "../../utils/operators/Operators.h"
#include "../../application/Application.h"

BaseScene::BaseScene(OpenGL& gl,std::string sceneName) 
  : 
  gl(gl),
  sceneName(sceneName)
{
  if (sceneName.empty()) {
    Logger::exception("Scene name is not specified");
  }
  Application::getInstance()->getEventHandler().subscribeToEvent<Constants::Buttons>(
    EventHandler::EventName::keyboardKeyIsPressed,
    sceneName,
    std::bind(&BaseScene::notifyKeyIsPressed,this, std::placeholders::_1)
  );
}

void BaseScene::update(double deltaTime) {
  Logger::exception("BaseScene::update function is not implemented in child class");
}

void BaseScene::render(double deltaTime) {
  Logger::exception("BaseScene::render function is not implemented in child class");
}

void BaseScene::notifyKeyIsPressed(const Constants::Buttons & keyEvent) {
  keyEvents[keyEvent] = true;
}

bool BaseScene::useKeyEvent(const Constants::Buttons & keyEvent) {
  temporaryKeyEventPlaceholder = keyEvents[keyEvent];
  keyEvents[keyEvent] = false;
  return temporaryKeyEventPlaceholder;
}

BaseScene::~BaseScene() {
  if (Application::getInstance()) {
    Application::getInstance()->getEventHandler().unSubscribeFromEvents(
      sceneName
    );
  }
}

std::string BaseScene::getSceneName() {
  return sceneName;
}