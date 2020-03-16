#include "FaTexture.h"
#include "stb_image_headers.h"
#include <cassert>
#include "../application/Application.h"


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
  
  auto tempData = STBImageHelper::loadTexture(address, &width, &height, &numberOfChannels);
  assert(tempData);
  //For data protection we copy temp data so external events won't effect data
  dataLength = (unsigned int)(width * height * numberOfChannels);
  data = new float[dataLength];
  for(int i=0;i<dataLength;i++){
    data[i] = float(tempData[i])/255.0f;
  }

  scaleX = float(width) / virtualImageWidth;
  scaleY = float(height) / virtualImageHeight;
};

FaTexture::~FaTexture(){
  if(data){
    delete[] data;
  }
}

int FaTexture::getRealImageWidth(){
  return width;
};

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
  int index = numberOfChannels * (positionY * width + positionX);
  assert(index+numberOfChannels<=dataLength);
  *red = data[index + 0];
  *green = data[index + 1];
  *blue = data[index + 2];
}

/**
 * This method is currently for debug purposes
 * We need spritesheet in future
*/
void FaTexture::render(){
  float red = 0;
  float green = 0;
  float blue = 0;
  for(int i=0;i<width;i++){
    for(int j=0;j<height;j++){
      getPixelForPosition(
        i,
        j,
        &red,
        &green,
        &blue
      );
      assert(red>=0&&red<=255);
      assert(blue>=0&&blue<=255);
      assert(green>=0&&green<=255);
      Application::getInstance()->putPixelInMap(
        i,
        j,
        -100,
        float(red)/255.0f,
        float(green)/255.0f,
        float(blue)/255.0f
      );
    }
  }
}

std::string FaTexture::getAddress(){
  return address;
}