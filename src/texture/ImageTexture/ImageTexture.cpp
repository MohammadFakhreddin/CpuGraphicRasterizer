#include "./ImageTexture.h"


#include <cassert>
#include <memory>

#include "../../file_system/FileSystem.h"
#include "../../camera/Camera.h"
#include "../../data_types/FaObject.h"
#include "../../camera/Camera.h"

ImageTexture::ImageTexture(
  const std::string& address,
  const float& virtualImageWidth,
  const float& virtualImageHeight
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
    assert(data[i] >= 0.0f && data[i] <= 1.0f);
  }

#ifndef __ANDROID__
  //Removing temporary data
  delete[] tempData;
#endif

  scaleX = float(width) / virtualImageWidth;
  scaleY = float(height) / virtualImageHeight;
}

ImageTexture::ImageTexture(
  unsigned char* tempData, 
  const int& numberOfChannels,
  const int& realImageWidth, 
  const int& realImageHeight, 
  const float& virtualImageWidth, 
  const float& virtualImageHeight
)
  :
  address(address),
  virtualImageWidth(virtualImageWidth),
  virtualImageHeight(virtualImageHeight),
  width(realImageWidth),
  height(realImageHeight),
  numberOfChannels(3)
{
  assert(virtualImageWidth > 0);
  assert(virtualImageHeight > 0);

  //For data protection we copy temp data so external events won't effect data
  dataLength = (unsigned int)(width * height);

  assert(dataLength % numberOfChannels == 0);

  data = new float[dataLength];
  for (unsigned int i = 0; i < dataLength; i++) {
    data[i] = float(tempData[i]) / 255.0f;
    assert(data[i] >= 0.0f && data[i] <= 1.0f);
  }
  //Removing temporary data
  delete[] tempData;

  scaleX = 1.0f;
  scaleY = 1.0f;
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
) const {
  assert(data);
  if (
    x < 0 ||
    x >= virtualImageWidth ||
    y < 0 ||
    y >= virtualImageHeight
    ) {
    return;
  }
  assert(x < virtualImageWidth);
  assert(x >= 0);
  assert(y < virtualImageHeight);
  assert(y >= 0);
   
  getDirectPixelColor(
    ((unsigned int)(scaleY * y) * width + (unsigned int)(scaleX * x)) * numberOfChannels,
    red,
    green,
    blue
  );
}

void ImageTexture::getDirectPixelColor(
  const unsigned int& position,
  float* red,
  float* green,
  float* blue
) const {

  assert(position < (unsigned int)(width* height* numberOfChannels));

  *red = data[position];
  *green = data[position + 1];
  *blue = data[position + 2];

  if (filter != nullptr) {
    (*filter)(red, green, blue);
  }

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
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      getPixelForPosition(
        (float)i,
        (float)j,
        &red,
        &green,
        &blue
      );
      assert(red >= 0 && red <= 1);
      assert(blue >= 0 && blue <= 1);
      assert(green >= 0 && green <= 1);
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

void ImageTexture::assignFilter(
  std::function<void(float* red, float* green, float* blue)>* filter
)
{
  this->filter = filter;
}
