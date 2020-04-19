#include "./Surface.h"

#include <cassert>

Surface::Surface(
  const Constants::LightPrecision lightPrecision,
  Texture* texture,
  const unsigned long edgeIndex[3],
  const float specularIntensity
)
  :
  texture(texture),
  lightPrecision(lightPrecision),
  cameraVectorPlaceholder(3,1,0.0f)
{

  assert(texture);

  for (unsigned short index = 0; index < 3; index++) {
    colorIntensity[index].reset(3, 1, 0.0f);
    edgeIndices[index] = edgeIndex[index];
    setTextureCoordinates(index, 0, 0);
  }

}

void Surface::setNormalIndex(const short& edgeNumber, const unsigned long& index) {
  assert(edgeNumber >= 0 && edgeNumber < edgeCount);
  assert(index >= 0);
  normalIndices[edgeNumber] = index;
}

void Surface::setTextureCoordinates(const short& edgeNumber, const float& x, const float& y) {
  assert(edgeNumber >= 0 && edgeNumber < edgeCount);
  textureCoordinate[edgeNumber].setXY(x,y);
}

const unsigned long& Surface::getEdgeByIndex(const unsigned short& index) {
  assert(index < 3);
  return edgeIndices[index];
}

const unsigned long& Surface::getNormalIndex(const unsigned short& index) {
  assert(index < 3);
  return normalIndices[index];
}

bool Surface::areEdgeAndNormalsValid(
  const unsigned long& worldPointsSize,
  const unsigned long& normalsSize
) {
  for (unsigned short i = 0; i < 3; i++) {
    if (
      edgeIndices[i] < 0 ||
      edgeIndices[i] >= worldPointsSize ||
      normalIndices[i] < 0 ||
      normalIndices[i] >= normalsSize
    ) {
      return false;
    }
  }
  return true;
}
