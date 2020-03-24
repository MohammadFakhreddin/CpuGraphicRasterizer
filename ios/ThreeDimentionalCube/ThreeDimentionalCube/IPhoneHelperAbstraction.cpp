#include "./IPhoneHelperAbstraction.h"

#include <string>

IPhoneHelperAbstraction* IPhoneHelperAbstraction::instance = nullptr;

unsigned char * IPhoneHelperAbstraction::callObjectiveCToLoadImage(
  std::string imageName,
  int * width,
  int * height,
  int * numberOfChannels
){
  auto imageData = loadImage(objectiveCIPhoneHelperInstance,imageName);
  *width = (int)imageData.width;
  *height = (int)imageData.height;
  *numberOfChannels = (int)imageData.numberOfChannels;
  return imageData.pixels;
};

unsigned char * IPhoneHelperAbstraction::callObjectiveCToLoadTextFile(
                                                                      std::string fileName
                                                                      ){
    return loadTextFile(objectiveCIPhoneHelperInstance,fileName);
}

void IPhoneHelperAbstraction::callObjectiveCToLog(std::string text){
    log(objectiveCIPhoneHelperInstance,text);
}

IPhoneHelperAbstraction::IPhoneHelperAbstraction(void * objectiveCIPhoneHelperInstance)
:
objectiveCIPhoneHelperInstance(objectiveCIPhoneHelperInstance)
{
    instance = this;
};

IPhoneHelperAbstraction* IPhoneHelperAbstraction::getInstance(){
    return instance;
};


std::string IPhoneHelperAbstraction::callObjectiveCToGetPathNameForResource(std::string fileName, std::string extension){
    return getPathNameForResource(
                                  objectiveCIPhoneHelperInstance,
                                  fileName,
                                  extension);
};
