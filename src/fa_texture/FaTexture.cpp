#include "FaTexture.h"
#include <iostream>
#include "stb_image.h"
#include <assert.h>
#include "../application/Application.h"


FaTexture::FaTexture(
  std::string address,
  float virtualImageWidth,
  float virtualImageHeight
)
:
virtualImageWidth(virtualImageWidth),
virtualImageHeight(virtualImageHeight)
{
  assert(virtualImageWidth>0);
  assert(virtualImageHeight>0);
  data = stbi_load(address.c_str(), &width, &height, &numberOfChannels, STBI_rgb);
  assert(data);
  dataLength = width * height * numberOfChannels;
};

FaTexture::~FaTexture(){
  if(data){
    delete data;
  }
}

int FaTexture::getWidth(){
  return width;
};

int FaTexture::getHeight(){
  return height;
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
  assert(positionX<virtualImageWidth);
  assert(positionX>=0);
  assert(positionY<virtualImageHeight);
  assert(positionY>=0);
  float realPositionX = scaleX * positionX;
  float realPositionY = scaleY * positionY;
  //TODO Create a filter for it
  int rawRed;
  int rawGreen;
  int rawBlue;
  getPixelForPosition(
    realPositionX,
    realPositionY,
    &rawRed,
    &rawGreen,
    &rawBlue
  );
  *red = float(rawRed)/255.0f;
  *green = float(rawGreen)/255.0f;
  *blue = float(rawBlue)/255.0f;
}

void FaTexture::getPixelForPosition(
  int positionX,
  int positionY,
  int* red,
  int* green,
  int* blue
){
  int index = numberOfChannels * (positionY * width + positionX);
  assert(index+numberOfChannels<=dataLength);
  *red = data[index + 0];
  *green = data[index + 1];
  *blue = data[index + 2];
}

void FaTexture::render(){
  int red = 0;
  int green = 0;
  int blue = 0;
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