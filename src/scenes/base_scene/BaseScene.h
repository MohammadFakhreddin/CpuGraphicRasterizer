#ifndef BaseScene_Class
#define BaseScene_Class

#include <unordered_map>
#include <string>

#include "../../Constants.h"
#include "../../open_gl/OpenGl.h"

class BaseScene {

public:

  BaseScene(OpenGL& gl,std::string sceneName);
  
  ~BaseScene();
  
  virtual void update(double deltaTime);
  
  virtual void render(double deltaTime);

#ifdef __DESKTOP__

  void notifyKeyIsPressed(const Constants::Buttons& keyEvent);

#endif // __DESKTOP__

  std::string getSceneName();

protected:

  bool useKeyEvent(const Constants::Buttons& keyEvent);

  OpenGL & gl;

  const std::string sceneName;

private:

  std::unordered_map<Constants::Buttons, bool> keyEvents;

  bool temporaryKeyEventPlaceholder = false;

};

#endif // !BaseScene_Class
