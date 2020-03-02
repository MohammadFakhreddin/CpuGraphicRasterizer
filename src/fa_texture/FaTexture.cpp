#include "FaTexture.h"
#include <iostream>
#include "stb_image.h"


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
  data = stbi_load(address.c_str(), &width, &height, &numberOfChannels, STBI_rgb_alpha);
  assert(data);
};

FaTexture::~FaTexture(){
  delete data;
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
  
}

void FaTexture::print(){
  std::cout<<"Printing texture data:";
  for(int i=0;i<width;i++){
    for(int j=0;j<height;j++){
      std::cout<<data[i*height+j]<<' ';
    }
    std::cout<<std::endl;
  }
}