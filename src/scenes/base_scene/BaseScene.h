#ifndef BaseScene_Class
#define BaseScene_Class

#include <unordered_map>
#include <string>

#include "../../Constants.h"
#include "../../open_gl/OpenGl.h"

class BaseScene {

public:

  BaseScene(OpenGL& gl,std::string sceneName);
  
  virtual ~BaseScene();
  
  virtual void update(double deltaTime);
  
  virtual void render(double deltaTime);

#ifdef __DESKTOP__

  void notifyKeyIsPressed(const Constants::KeyboardButtons& keyEvent);

#endif // __DESKTOP__

  void onActiveSceneChanged(const std::string& sceneName);

  std::string getSceneName();

protected:

#ifdef __DESKTOP__

  bool useKeyEvent(const Constants::KeyboardButtons& keyEvent);

#endif

  OpenGL & gl;

  const std::string sceneName;

  bool isPageActive = false;

private:

#ifdef __DESKTOP__

  std::unordered_map<Constants::KeyboardButtons, bool> keyEvents;

  std::unordered_map<Constants::MouseButtonName, Constants::MouseEvent> mouseEvents;

#endif

  bool temporaryKeyEventPlaceholder = false;

};

#endif // !BaseScene_Class
