#include "./EventHandler.h"

#include <unordered_map>

void EventHandler::unSubscribeFromEvents(std::string className) {
  if (eventsMap.empty()) {
    return;
  }
  for (auto& eventObject : eventsMap) {
    eventObject.second.erase(className);
  }
}
