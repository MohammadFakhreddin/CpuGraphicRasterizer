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

  void onActiveSceneChanged(const std::string& sceneName);

  std::string getSceneName();

protected:

#ifdef __DESKTOP__

  const bool& useKeyEvent(const Constants::KeyboardButtons& keyEvent);

  const bool& getMouseEvent(const Constants::MouseButtonName& mouseButtonName);

  void notifyKeyIsPressed(const Constants::KeyboardButtons& keyEvent);

  void mouseButtonPressed(const Constants::MouseButtonName& mouseButton);

  void mouseButtonReleased(const Constants::MouseButtonName& mouseButton);

#endif

  OpenGL & gl;

  const std::string sceneName;

  bool isPageActive = false;

private:

#ifdef __DESKTOP__

  std::unordered_map<Constants::KeyboardButtons, bool> keyEvents;

  std::unordered_map<Constants::MouseButtonName, bool> mouseEvents;

#endif

  bool temporaryKeyEventPlaceholder = false;

};

#endif // !BaseScene_Class
