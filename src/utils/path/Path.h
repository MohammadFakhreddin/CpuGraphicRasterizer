#ifndef Path_class
#define Path_class

#include "./../../Constants.h"
#include <string>

class Path {
public:
  static std::string generateAssetPath(std::string address){
    #if defined(__ANDROID__)
      return address;
    #elif defined(__DESKTOP__)
      return "assets/" + address;
    #endif
    return "";
  };
};

#endif