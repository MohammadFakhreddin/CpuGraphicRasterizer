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

EdgeType BaseSurface::getEdgeType() {
  return EdgeType::base;
}

void BaseSurface::render(
  Camera& cameraInstance,
  std::vector<MatrixFloat> *worldPoints
) {
  assert(edge1<worldPoints->size() && edge1>=0);
  assert(edge2<worldPoints->size() && edge2>=0);
  assert(edge3<worldPoints->size() && edge3>=0);
  computeNormalVector(worldPoints);
  computeEdgeCenter(worldPoints);
  if(!isVisibleToCamera(cameraInstance, worldPoints)){
    return;
  }
  cameraInstance.getLight().computeLightIntensity(
    normalVector,
    edgeCenter,
    colorIntensity
  );
  computePixelMapData(
    cameraInstance,
    worldPoints
  );
}

void BaseSurface::computePixelMapData(
  Camera& cameraInstance,
  std::vector<MatrixFloat>* worldPoints
){
  Logger::log("computePixelMapData is not implemented correctly");
  assert("False");
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

void BaseSurface::computeNormalVector(std::vector<MatrixFloat>* worldPoints){
 //Creating edge vectors for normal vector computing
  edge1To2Vector.setX(worldPoints->at(edge2).get(0, 0) - worldPoints->at(edge1).get(0, 0));
  edge1To2Vector.setY(worldPoints->at(edge2).get(1, 0) - worldPoints->at(edge1).get(1, 0));
  edge1To2Vector.setZ(worldPoints->at(edge2).get(2, 0) - worldPoints->at(edge1).get(2, 0));

  edge2To3Vector.setX(worldPoints->at(edge3).get(0, 0) - worldPoints->at(edge2).get(0, 0));
  edge2To3Vector.setY(worldPoints->at(edge3).get(1, 0) - worldPoints->at(edge2).get(1, 0));
  edge2To3Vector.setZ(worldPoints->at(edge3).get(2, 0) - worldPoints->at(edge2).get(2, 0));
  //Generating normal vector from edge vectors
  normalVector.crossProduct(edge2To3Vector, edge1To2Vector);
}

void BaseSurface::calculateStepCountAndStepValue(
  Camera& cameraInstance,
  float difference,
  unsigned int* totalStepCount,
  float* stepValue
){
  *totalStepCount = Math::max(
      (unsigned int)ceil(abs(difference/drawStepValue)),
      cameraInstance.getAppScreenWidth()
  );
  *stepValue = difference/float(*totalStepCount);
}

void BaseSurface::computeEdgeCenter(
    std::vector<MatrixFloat>* worldPoints
){
  edgeCenter.setX((
    worldPoints->at(edge1).get(0, 0) +
    worldPoints->at(edge2).get(0, 0) +
    worldPoints->at(edge3).get(0, 0))
  / 3.0f);
  edgeCenter.setY((
    worldPoints->at(edge1).get(1, 0) +
    worldPoints->at(edge2).get(1, 0) +
    worldPoints->at(edge3).get(1, 0)
  ) / 3.0f);
  edgeCenter.setZ((
    worldPoints->at(edge1).get(2, 0) +
    worldPoints->at(edge2).get(2, 0) +
    worldPoints->at(edge3).get(2, 0)
  ) / 3.0f);
}

bool BaseSurface::isVisibleToCamera(
    Camera& cameraInstance,
    std::vector<MatrixFloat> *worldPoints
) {
  //TODO Check for cliping too
  //Making camera vector
  cameraVector.setX(
    Math::clamp(
      edgeCenter.getX(),
      cameraInstance.getLeft(),
      cameraInstance.getRight()
    ) - edgeCenter.getX()
  );
  cameraVector.setY(
      Math::clamp(
      edgeCenter.getY(),
      cameraInstance.getTop(),
      cameraInstance.getBottom()
    ) - edgeCenter.getY()
  );
  cameraVector.setZ(edgeCenter.getZ() - cameraInstance.getCameraZLocation());

  dotProductValue = normalVector.dotProduct(cameraVector);

  return dotProductValue < 0.0f;
}

void BaseSurface::putPixelInMap(
  Camera& cameraInstance,
  int x,
  int y,
  float zValue,
  float red,
  float green,
  float blue
){
  assert(red>=0 && red<=1.0f);
  assert(green>=0 && green<=1.0f);
  assert(blue>=0 && blue<=1.0f);
  cameraInstance.putPixelInMap(
    x,
    y,
    zValue,
    Math::clamp(red * colorIntensity.getX(),0.0f,1.0f),
    Math::clamp(green * colorIntensity.getY(),0.0f,1.0f),
    Math::clamp(blue * colorIntensity.getZ(),0.0f,1.0f)
  );
}