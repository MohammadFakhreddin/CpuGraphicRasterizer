#ifndef EventHandler_Class
#define EventHandler_Class

#include <unordered_map>
#include <string>
#include <functional>
#include <memory>
#include <any>

class EventHandler {
private:

  struct Listener {
    std::string className;
    std::function<void(std::any)> callback;
  };

public:

  enum EventName {
    keyboardKeyIsPressed,
    screenSurfaceChanged
  };

  struct ScreenSurfaceChangeEventData {
    unsigned int appScreenWidth;
    unsigned int appScreenHeight;
    unsigned int physicalScreenWidth;
    unsigned int physicalScreenHeight;
    bool forceNewAppScreenWidthAndHeight;
  };

  template<typename T>
  void subscribeToEvent(
    EventName eventName,
    std::string className,
    std::function<void(T)> callback
  ) {
    Listener listener;
    listener.className = className;
    listener.callback = [callback](std::any param) {
      callback(std::any_cast<T>(param));
    };
    eventsMap[eventName][className] = listener;
  }

  void unSubscribeFromEvents(std::string className);

  template<typename T>
  void emitEvent(EventName eventName, T data) {
    auto& listeners = eventsMap[eventName];
    for (auto& listener : listeners) {
      listener.second.callback(std::make_any<T>(data));
    }
  }

private:

  std::unordered_map<
    EventName,
    std::unordered_map<
      std::string,
      Listener
    >
  > eventsMap;

};

#endif