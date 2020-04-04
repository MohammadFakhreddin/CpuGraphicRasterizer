#include "./ImageTexture.h"


#include <cassert>
#include <memory>

#include "../../file_system/FileSystem.h"
#include "../../camera/Camera.h"
#include "../../data_types/FaObject.h"
#include "../../camera/Camera.h"

ImageTexture::ImageTexture(
  std::string address,
  float virtualImageWidth,
  float virtualImageHeight
  )
  :
  address(address),
  virtualImageWidth(virtualImageWidth),
  virtualImageHeight(virtualImageHeight)
{
  assert(virtualImageWidth > 0);
  assert(virtualImageHeight > 0);

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

ImageTexture::~ImageTexture() {
  delete[] data;
}

int ImageTexture::getRealImageWidth() {
  return width;
}

int ImageTexture::getRealImageHeight() {
  return height;
}

float ImageTexture::getVirtualImageWidth() {
  return virtualImageWidth;
}

float ImageTexture::getVirtualImageHeight() {
  return virtualImageHeight;
}

int ImageTexture::getNumberOfChannels() {
  return numberOfChannels;
}

void ImageTexture::getPixelForPosition(
  const float& x,
  const float& y,
  float* red,
  float* green,
  float* blue
) {
  assert(data);
  if (
    x < 0 ||
    x >= virtualImageWidth ||
    y < 0 ||
    y >= virtualImageHeight
    ) {
    return;
  }
  assert(positionX < virtualImageWidth);
  assert(positionX >= 0);
  assert(positionY < virtualImageHeight);
  assert(positionY >= 0);
  
  realPositionX = unsigned int(scaleX * x);
  realPositionY = unsigned int(scaleY * y);
  
  getDirectPixelColor(
    realPositionX,
    realPositionY,
    red,
    green,
    blue
    );
}

void ImageTexture::getDirectPixelColor(
  const unsigned int& positionX,
  const unsigned int& positionY,
  float* red,
  float* green,
  float* blue
  ) {
  assert(positionX >= 0 && positionX < width && "FaImageTexture::getPixelForPosition positionX must be between 0 and width");
  assert(positionY >= 0 && positionY < height && "FaImageTexture::getPixelForPosition positionY must be between 0 and height");

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
void ImageTexture::update(double deltaTime, Camera& cameraInstance) {
  float red = 0;
  float green = 0;
  float blue = 0;
  for (unsigned int i = 0; i < width; i++) {
    for (unsigned int j = 0; j < height; j++) {
      getPixelForPosition(
        i,
        j,
        &red,
        &green,
        &blue
      );
      assert(red >= 0 && red <= 255);
      assert(blue >= 0 && blue <= 255);
      assert(green >= 0 && green <= 255);
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

std::string ImageTexture::getAddress() {
  return address;
}
