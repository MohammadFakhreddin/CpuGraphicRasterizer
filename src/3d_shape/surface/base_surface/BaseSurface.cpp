#include "./BaseSurface.h"

#include <cassert>

#include "../../../utils/math/Math.h"
#include "./../../../camera/Camera.h"

unsigned long BaseSurface::getEdgeByIndex(short index) {
  assert(index > -1);
  assert(index < 3);
  switch (index) {
    case 0:
      return edge1;
    case 1:
      return edge2;
    case 2:
      return edge3;
    default:
      Logger::exception("Edge index must be between 0 to 2");
  }
  return edge1;
}

void BaseSurface::update(
  Camera& cameraInstance,
  std::vector<MatrixFloat>& worldPoints,
  std::vector<std::unique_ptr<Light>>& lightSources
) {
  assert(edge1<worldPoints.size() && edge1>=0);
  assert(edge2<worldPoints.size() && edge2>=0);
  assert(edge3<worldPoints.size() && edge3>=0);
  computeNormalVector(worldPoints);
  computeEdgeCenter(worldPoints);
  //TODO This method is cpu demanding
  //if(!isVisibleToCamera(cameraInstance, worldPoints)){
  //  return;
  //}
  computeColorIntensity(lightSources);
}

void BaseSurface::render(
  Camera& cameraInstance,
  std::vector<MatrixFloat>& worldPoints
) {
  computePixelMapData(
    cameraInstance,
    worldPoints
  );
}

void BaseSurface::computePixelMapData(
  Camera& cameraInstance,
  std::vector<MatrixFloat>& worldPoints
){
  Logger::log("computePixelMapData is not implemented correctly");
  assert("False");
}

void BaseSurface::computeColorIntensity(
  std::vector<std::unique_ptr<Light>>& lightSources
) {
  if (lightSources.empty() == false) {
    colorIntensity.setX(0.0f);
    colorIntensity.setY(0.0f);
    colorIntensity.setZ(0.0f);
    for (auto& light : lightSources) {
      light->computeLightIntensity(
        normalVector,
        edgeCenter,
        temporaryColorIntensityPlaceholder
      );
      colorIntensity.setX(
        Math::max(
          temporaryColorIntensityPlaceholder.getX(),
          colorIntensity.getX()
        )
      );
      colorIntensity.setY(
        Math::max(
          temporaryColorIntensityPlaceholder.getY(),
          colorIntensity.getY()
        )
      );
      colorIntensity.setZ(
        Math::max(
          temporaryColorIntensityPlaceholder.getZ(),
          colorIntensity.getZ()
        )
      );
    }
  }
  else {
    colorIntensity.setX(1.0f);
    colorIntensity.setY(1.0f);
    colorIntensity.setZ(1.0f);
  }
}

bool BaseSurface::areEdgesDataValid(int nodesSize) {
  int currentEdge = -1;
  for (int i = 0; i < 3; i++) {
    currentEdge = getEdgeByIndex(i);
    if (currentEdge < 0 || currentEdge >= nodesSize) {
      return false;
    }
  }
  return true;
}

void BaseSurface::computeNormalVector(std::vector<MatrixFloat>& worldPoints){
 //Creating edge vectors for normal vector computing
  edge1To2Vector.setX(worldPoints.at(edge2).get(0, 0) - worldPoints.at(edge1).get(0, 0));
  edge1To2Vector.setY(worldPoints.at(edge2).get(1, 0) - worldPoints.at(edge1).get(1, 0));
  edge1To2Vector.setZ(worldPoints.at(edge2).get(2, 0) - worldPoints.at(edge1).get(2, 0));

  edge2To3Vector.setX(worldPoints.at(edge3).get(0, 0) - worldPoints.at(edge2).get(0, 0));
  edge2To3Vector.setY(worldPoints.at(edge3).get(1, 0) - worldPoints.at(edge2).get(1, 0));
  edge2To3Vector.setZ(worldPoints.at(edge3).get(2, 0) - worldPoints.at(edge2).get(2, 0));
  //Generating normal vector from edge vectors
  normalVector.crossProduct(edge2To3Vector, edge1To2Vector);
}

void BaseSurface::calculateStepCountAndStepValue(
  Camera& cameraInstance,
  float difference,
  unsigned int* totalStepCount,
  float* stepValue
){
  assert(difference != 0 && "Difference must be above 0 in BaseSurface::calculateStepCountAndStepValue");
  assert(drawStepValue != 0 && "Draw step value must be above 0 in BaseSurface::calculateStepCountAndStepValue");
  *totalStepCount = Math::min(
      (unsigned int)abs(difference/drawStepValue),
      cameraInstance.getAppScreenWidth()
  );
  *stepValue = difference/float(*totalStepCount);
}

void BaseSurface::computeEdgeCenter(
    std::vector<MatrixFloat>& worldPoints
){
  edgeCenter.setX((
    worldPoints.at(edge1).get(0, 0) +
    worldPoints.at(edge2).get(0, 0) +
    worldPoints.at(edge3).get(0, 0))
  / 3.0f);
  edgeCenter.setY((
    worldPoints.at(edge1).get(1, 0) +
    worldPoints.at(edge2).get(1, 0) +
    worldPoints.at(edge3).get(1, 0)
  ) / 3.0f);
  edgeCenter.setZ((
    worldPoints.at(edge1).get(2, 0) +
    worldPoints.at(edge2).get(2, 0) +
    worldPoints.at(edge3).get(2, 0)
  ) / 3.0f);
}

bool BaseSurface::isVisibleToCamera(
    Camera& cameraInstance,
    std::vector<MatrixFloat>& worldPoints
) {
  float halfScreenWidth = cameraInstance.getAppScreenWidth() / 2.0f;
  float halfScreenHeight = cameraInstance.getAppScreenHeight() / 2.0f;
  float cameraX = halfScreenWidth;
  float cameraY = halfScreenHeight;
  auto left = 0;
  auto right = cameraInstance.getAppScreenWidth();
  auto top = 0;
  auto bottom = cameraInstance.getAppScreenHeight();
  if (
    (
      worldPoints.at(edge1).get(0, 0) < left &&
      worldPoints.at(edge2).get(0, 0) < left &&
      worldPoints.at(edge3).get(0, 0) < left
    ) || (
      worldPoints.at(edge1).get(0, 0) >= right &&
      worldPoints.at(edge2).get(0, 0) >= right &&
      worldPoints.at(edge3).get(0, 0) >= right
    ) ||
    (
      worldPoints.at(edge1).get(1, 0) >= bottom &&
      worldPoints.at(edge2).get(1, 0) >= bottom &&
      worldPoints.at(edge3).get(1, 0) >= bottom
    ) ||
    (
      worldPoints.at(edge1).get(1, 0) < top &&
      worldPoints.at(edge2).get(1, 0) < top &&
      worldPoints.at(edge3).get(1, 0) < top
    )
  ) {
    return false;
  }
  //TODO Check for cliping too
  //Making camera vector
  cameraVector.setX(
    Math::clamp(
      edgeCenter.getX(),
      left,
      right
    ) - edgeCenter.getX()
  );
  cameraVector.setY(
    Math::clamp(
      edgeCenter.getY(),
      top,
      bottom
    ) - edgeCenter.getY()
  );
  cameraVector.setZ(edgeCenter.getZ());

  dotProductValue = normalVector.dotProduct(cameraVector);

  return dotProductValue < 0.0f;
}

void BaseSurface::putPixelInMap(
  Camera& cameraInstance,
  const int& x,
  const int& y,
  const float& zValue,
  float& red,
  float& green,
  float& blue
){
  cameraInstance.putPixelInMap(
    x,
    y,
    zValue,
    red * colorIntensity.getX(),
    green * colorIntensity.getY(),
    blue * colorIntensity.getZ()  
  );
}