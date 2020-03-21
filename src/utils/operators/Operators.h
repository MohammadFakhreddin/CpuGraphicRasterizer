#ifndef Operators_class
#define Operators_class

#include <string>

class Operators{
public:
  static std::string toLowerCase(std::string value){
    std::string result = "";
    if(value.length()>0){
      for(auto& character:value){
        result += std::tolower(character);
      }
    }
    return result;
  }
};

#endif