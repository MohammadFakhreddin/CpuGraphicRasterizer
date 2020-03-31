#include "./FaTexture.h"

#include <cassert>
#include <memory>

#include "../file_system/FileSystem.h"
#include "../camera/Camera.h"
#include "./../data_types/FaObject.h"
#include "./../camera/Camera.h"

FaTexture::FaTexture(
  std::string address,
  float virtualImageWidth,
  float virtualImageHeight
)
    :
    virtualImageWidth(virtualImageWidth),
    virtualImageHeight(virtualImageHeight),
    address(address)
{
  assert(virtualImageWidth>0);
  assert(virtualImageHeight>0);
  
  auto tempData = FileSystem::loadTexture(address, &width, &height, &numberOfChannels);
  assert(tempData);
  //For data protection we copy temp data so external events won't effect data
  dataLength = (unsigned int)(width * height * numberOfChannels);

  assert(dataLength % numberOfChannels == 0);

  data = new float[dataLength];
  for (unsigned int i = 0; i < dataLength; i++) {
    data[i] = float(tempData[i]) / 255.0f;
  }

#ifndef __ANDROID__
  //Removing temporary data
  delete[] tempData;
#endif

  scaleX = float(width) / virtualImageWidth;
  scaleY = float(height) / virtualImageHeight;
}

FaTexture::~FaTexture() {
  delete[] data;
}

int FaTexture::getRealImageWidth(){
  return width;
}

int FaTexture::getRealImageHeight(){
  return height;
}

float FaTexture::getVirtualImageWidth(){
  return virtualImageWidth;
}

float FaTexture::getVirtualImageHeight(){
  return virtualImageHeight;
}

int FaTexture::getNumberOfChannels(){
  return numberOfChannels;
}

void FaTexture::getColorForPosition(
  float positionX,
  float positionY,
  float* red,
  float* green,
  float* blue
){
  assert(data);
  if(
    positionX<0 || 
    positionX>=virtualImageWidth || 
    positionY<0 || 
    positionY>=virtualImageHeight
  ){
    return;
  }
  assert(positionX<virtualImageWidth);
  assert(positionX>=0);
  assert(positionY<virtualImageHeight);
  assert(positionY>=0);
  int realPositionX = int(scaleX * positionX);
  int realPositionY = int(scaleY * positionY);
  getPixelForPosition(
    realPositionX,
    realPositionY,
    red,
    green,
    blue
  );
}

void FaTexture::getPixelForPosition(
  const unsigned int& positionX,
  const unsigned int& positionY,
  float* red,
  float* green,
  float* blue
){
  assert(positionX >= 0 && positionX < width && "FaTexture::getPixelForPosition positionX must be between 0 and width");
  assert(positionY >= 0 && positionY < height && "FaTexture::getPixelForPosition positionY must be between 0 and height");
  
  currentPosition = (positionY * width + positionX) * numberOfChannels;
  
  *red = data[currentPosition];
  *green = data[currentPosition + 1];
  *blue = data[currentPosition + 2];
}

/**
 * This method is currently for debug purposes
 * We need spritesheet in future
*/
//TODO Move this code for spritesheet
void FaTexture::render(double deltaTime,Camera& cameraInstance){
  float red = 0;
  float green = 0;
  float blue = 0;
  for(unsigned int i=0;i<width;i++){
    for(unsigned int j=0;j<height;j++){
      getPixelForPosition(
        i,
        j,
        &red,
        &green,
        &blue
      );
      assert( red>=0 && red<=255 );
      assert( blue>=0 && blue<=255 );
      assert( green>=0 && green<=255 );
      cameraInstance.putPixelInMap(
        i,
        j,
        -1,
        red,
        green,
        blue
      );
    }
  }
}

std::string FaTexture::getAddress(){
  return address;
}