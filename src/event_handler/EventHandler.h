#ifndef EventHandler_Class
#define EventHandler_Class

#include <unordered_map>
#include <string>
#include <functional>
#include <memory>

class EventHandler {
private:

  struct Listener {
    std::string className;
    std::function<void(void *)> callback;
  };

public:

  enum EventName {
    keyboardKeyIsPressed,
    screenSurfaceChanged
  };

  template<typename T>
  void subscribeToEvent(
    EventName eventName,
    std::string className,
    std::function<void(T)> callback
  ) {
    Listener listener;
    listener.className = className;
    listener.callback = [callback](void * rawDataPointer) {
      T* dataPointer = (T*)rawDataPointer;
      callback(dataPointer[0]);
    };
    eventsMap[eventName][className] = listener;
  }

  void unSubscribeFromEvents(std::string className);

  template<typename T>
  void emitEvent(EventName eventName, T data) {
    auto& listeners = eventsMap[eventName];
    T* pointer = new T[1];
    pointer[0] = data;
    for (auto& listener : listeners) {
      listener.second.callback((void *)pointer);
    }
    delete[] pointer;
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