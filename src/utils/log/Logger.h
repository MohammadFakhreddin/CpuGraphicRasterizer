#ifndef Logger_class
#define Logger_class

#include <string>
#include <iostream>
#include <cassert>
#include <codecvt>

#include "../../Constants.h"
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
    //TODO We have to throw custom exception
    Logger::log(text);
    assert(false);
  }
};

#endif
