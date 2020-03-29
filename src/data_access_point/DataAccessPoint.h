#ifndef DataAccessPoint_Class
#define DataAccessPoint_Class

#include <memory>
#include <unordered_map>

#include "../Constants.h"
#include "../event_handler/EventHandler.h"

class DataAccessPoint {

public:

  static std::unique_ptr<DataAccessPoint>& getInstance();

  static void createInstance();

  DataAccessPoint();

  void setAppScreenWidth(const unsigned int appScreenWidth);

  void setAppScreenHeight(const unsigned int appScreenHeight);

  void setPhysicalScreenWidth(const unsigned int physicalScreenWidth);

  void setPhysicalScreenHeight(const unsigned int physicalScreenHeight);

  void setPlatform(Constants::Platform platform);

  const unsigned int& getAppScreenWidth();

  const unsigned int& getAppScreenHeight();

  const unsigned int& getPhysicalScreenWidth();

  const unsigned int& getPhysicalScreenHeight();

  Constants::Platform getPlatform();

  EventHandler& getEventHandler();

#ifdef __DESKTOP__

  Constants::Buttons getKeyCode(const unsigned char& key);

#endif // __DESKTOP__
  
private:

  static std::unique_ptr<DataAccessPoint> instance;

#ifdef __DESKTOP__

  std::unordered_map<char, Constants::Buttons> keyMap;

#endif // __DESKTOP__

  unsigned int physicalScreenWidth = 0;

  unsigned int physicalScreenHeight = 0;

  unsigned int appScreenWidth = 0;

  unsigned int appScreenHeight = 0;

  Constants::Platform platform = Constants::Platform::Unknown;

  EventHandler eventHandler;

};

#endif // !DataAccessPoint_Class
