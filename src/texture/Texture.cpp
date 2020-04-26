#include "./Texture.h"

#include <cassert>
#include <memory>

#include "../file_system/FileSystem.h"
#include "../camera/Camera.h"
#include "./../data_types/FaObject.h"
#include "./../camera/Camera.h"

void Texture::getPixelForPosition(
  const float& positionX,
  const float& positionY,
  float* red,
  float* green,
  float* blue
) const {
  Logger::exception("Texture::getPixelForPosition is not correctly implemented in child class");
}

/**
 * This method is currently for debug purposes
 * We need spritesheet in future
*/
void Texture::update(double deltaTime, Camera& cameraInstance) {}

Texture::~Texture(){}