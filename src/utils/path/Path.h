#ifndef Path_class
#define Path_class

#include "./../../Constants.h"
#include <string>

class Path {
public:
  static std::string generateAssetPath(std::string address,std::string extension){
    #if defined(__ANDROID__)
      return address + extension;
    #elif defined(__DESKTOP__)
      return "assets/" + address + extension;
    #elif defined(__IOS__)
      return address;
    #endif
    return "";
  };
};

#endif