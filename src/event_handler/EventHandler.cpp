#include "./EventHandler.h"

#include <unordered_map>

std::unique_ptr<EventHandler> EventHandler::instance;

std::unique_ptr<EventHandler>& EventHandler::getInstance() {
  if (!instance) {
    instance = std::make_unique<EventHandler>();
  }
  return instance;
}

void EventHandler::unSubscribeFromEvents(std::string className) {
  if (eventsMap.empty()) {
    return;
  }
  for (auto& eventObject : eventsMap) {
    eventObject.second.erase(className);
  }
}
