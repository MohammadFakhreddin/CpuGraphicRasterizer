#include "Path.h"

#include "./../../Constants.h"
#include <string>

std::string Path::generateAssetPath(std::string address){
  #if defined(__ANDROID__)
    return address;
  #elif defined(__APPLE__) || defined(__PLATFORM_WIN__)
    return "assets/" + address;
  #endif
  return "";
};