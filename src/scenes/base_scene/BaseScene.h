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

  const bool& useKeyboardCharacterEvent(const int& keyboardKey);

  const bool& useScanCodeEvent(const int& scanCode);

  const bool& getMouseEvent(const int& mouseButtonName);

  void notifyForNewKeyboardCharacterEvent(const int& keyboardCharacter);

  void notifyForNewScanCodeCharacterEvent(const int& scanCode);

  void mouseButtonPressed(const int& mouseButton);

  void mouseButtonReleased(const int& mouseButton);

#endif

  OpenGL & gl;

  const std::string sceneName;

  bool isPageActive = false;

private:

#ifdef __DESKTOP__

  std::unordered_map<int, bool> keyEvents;

  std::unordered_map<int, bool> mouseEvents;

  std::unordered_map<int,bool> scanCodeEvents;

#endif

  bool temporaryKeyEventPlaceholder = false;

  bool temporaryScanCodeEventPlaceholder = false;

};

#endif // !BaseScene_Class
