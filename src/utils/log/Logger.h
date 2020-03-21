#ifndef Logger_class
#define Logger_class

#include <string>
#include "../../Constants.h"
#include <iostream>
#ifdef __ANDROID__
#include "../../../android/cpp/AndroidEnvironment.h"
#endif
#ifdef __IOS__
#include "../../../ios/ThreeDimentionalCube/ThreeDimentionalCube/IPhoneHelperAbstraction.h"
#endif

class Logger{
public:
  static void log(std::string text){
#if defined(__DESKTOP__)
    std::cout<<text<<std::endl;  
#elif defined(__ANDROID__)
    AndroidEnvironment::getInstance()->log(text);
#elif defined(__IOS__)
    IPhoneHelperAbstraction::getInstance()->callObjectiveCToLog(text);
#endif
  }
  static void exception(std::string text){
    //TODO Check and improve this function
    Logger::log(text);
    throw std::runtime_error(text);
  }
};

#endif
