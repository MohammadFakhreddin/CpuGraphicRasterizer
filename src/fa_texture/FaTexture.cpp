#include "./FaTexture.h"

#include <cassert>
#include <memory>

#include "../file_system/FileSystem.h"
#include "../camera/Camera.h"
#include "./../data_types/FaObject.h"

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
  data = std::unique_ptr<FaObject<float>[]>(new FaObject<float>[dataLength]);
  for(int i=0;i<dataLength;i++){
    data[i].setValue(float(tempData[i])/255.0f);
  }
#ifndef __ANDROID__
  //Removing temporary data
  delete[] tempData;
#endif

  scaleX = float(width) / virtualImageWidth;
  scaleY = float(height) / virtualImageHeight;
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
  int positionX,
  int positionY,
  float* red,
  float* green,
  float* blue
){
  unsigned int index = numberOfChannels * (positionY * width + positionX);
  assert(index+numberOfChannels<=dataLength);
  *red = data[index + 0].getValue();
  *green = data[index + 1].getValue();
  *blue = data[index + 2].getValue();
}

/**
 * This method is currently for debug purposes
 * We need spritesheet in future
*/
//TODO Move this code for spritesheet
// void FaTexture::render(double deltaTime,Camera& cameraInstance){
//   float red = 0;
//   float green = 0;
//   float blue = 0;
//   for(int i=0;i<width;i++){
//     for(int j=0;j<height;j++){
//       getPixelForPosition(
//         i,
//         j,
//         &red,
//         &green,
//         &blue
//       );
//       assert( red>=0 && red<=255 );
//       assert( blue>=0 && blue<=255 );
//       assert( green>=0 && green<=255 );
//       cameraInstance.putPixelInMap(
//         i,
//         j,
//         -100,
//         red,
//         green,
//         blue
//       );
//     }
//   }
// }

std::string FaTexture::getAddress(){
  return address;
}