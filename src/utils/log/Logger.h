#ifndef Logger_class
#define Logger_class

#include <string>
#include "../../Constants.h"
#include <iostream>
#ifdef __ANDROID__
#include "../../../android/app/src/main/cpp/AndroidEnvironment.h"
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
};

#endif
