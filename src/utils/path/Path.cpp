#include "Path.h"

#include <string>

std::string Path::generateAssetPath(std::string address){
  #if defined(__ANDROID__)
    return address;
  #elif defined(__APPLE__)
    return "assets/" + address;
  #endif
  return "";
};