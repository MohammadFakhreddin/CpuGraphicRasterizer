#include "./Surface.h"

#include <cassert>

Surface::Surface(
  const Constants::LightPrecision lightPrecision,
  Texture* texture,
  const unsigned long edgeIndex[3]
)
  :
  Surface(
    lightPrecision,
    texture,
    edgeIndex[0],
    edgeIndex[1],
    edgeIndex[2]
  )
{}

Surface::Surface(
  const Constants::LightPrecision lightPrecision,
  Texture* texture,
  const unsigned long edgeIndex1,
  const unsigned long edgeIndex2,
  const unsigned long edgeIndex3
)
  :
  texture(texture),
  lightPrecision(lightPrecision)
{

  assert(texture);

  edgeIndices[0] = edgeIndex1;
  normalIndices[0] = 0;
  textureCoordinate[0].setXY(0.0f, 0.0f);

  edgeIndices[1] = edgeIndex2;
  normalIndices[1] = 0;
  textureCoordinate[1].setXY(0.0f, 0.0f);

  edgeIndices[2] = edgeIndex3;
  normalIndices[2] = 0;
  textureCoordinate[2].setXY(0.0f, 0.0f);

}


void Surface::setNormalIndex(const short& edgeNumber, const unsigned long& index) {
  assert(edgeNumber >= 0 && edgeNumber < 3);
  assert(index >= 0);
  normalIndices[edgeNumber] = index;
}

void Surface::setTextureCoordinates(const short& edgeNumber, const float& x, const float& y) {
  assert(edgeNumber >= 0 && edgeNumber < 3);
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
