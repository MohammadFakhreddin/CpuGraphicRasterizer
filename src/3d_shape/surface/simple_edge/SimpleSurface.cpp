#include "./SimpleSurface.h"
#include "./../../../camera/Camera.h"

SimpleSurface::SimpleSurface(
  int edge1,
  int edge2,
  int edge3,
  float red,
  float green,
  float blue
)
:
  red(red),
  green(green),
  blue(blue)
{
  this->edge1 = edge1;
  this->edge2 = edge2;
  this->edge3 = edge3;
}

void SimpleSurface::computePixelMapData(
  Camera& cameraInstance,
  std::vector<MatrixFloat>* worldPoints
){
  for(int i=0;i<3;i++){
    firstEdge = &worldPoints->at(getEdgeByIndex((i)%3));
    secondEdge = &worldPoints->at(getEdgeByIndex((i+1)%3));
    thirdEdge = &worldPoints->at(getEdgeByIndex((i+2)%3));
    drawLineBetweenPoints(
      cameraInstance,
      firstEdge->get(0,0),
      firstEdge->get(1,0),
      firstEdge->get(2,0),
      secondEdge->get(0,0),
      secondEdge->get(1,0),
      secondEdge->get(2,0),
      red,
      green,
      blue
    );
    drawLineBetweenPoints(
      cameraInstance,
      firstEdge->get(0,0),
      firstEdge->get(1,0),
      firstEdge->get(2,0),
      thirdEdge->get(0,0),
      thirdEdge->get(1,0),
      thirdEdge->get(2,0),
      red,
      green,
      blue
    );
    drawLineBetweenPoints(
      cameraInstance,
      thirdEdge->get(0,0),
      thirdEdge->get(1,0),
      thirdEdge->get(2,0),
      secondEdge->get(0,0),
      secondEdge->get(1,0),
      secondEdge->get(2,0),
      red,
      green,
      blue
    );
  }
}

EdgeType SimpleSurface::getEdgeType(){
  return EdgeType::color;
}

void SimpleSurface::drawLineBetweenPoints(
  Camera& cameraInstance,
  float startX,
  float startY,
  float startZ,
  float endX,
  float endY,
  float endZ,
  float red,
  float green,
  float blue
){
  bool moveByX = true;
  if(abs(startX-endX)<abs(startY-endY)){
    moveByX = false;
  }
  if(moveByX){
    float xDifference = endX - startX;
	if(xDifference==0){
		return;
	}
	float yM = (endY - startY)/xDifference;
    float zM = (endZ - startZ)/xDifference;
    putPixelInMap(
      cameraInstance,
      int(floor(startX)), 
      int(floor(startY)), 
      startZ, 
      red, 
      green, 
      blue
    );
    float stepMoveValue = startX - endX > 0 ? -1 : +1;
	do{
		startX += stepMoveValue;
		startY += yM * stepMoveValue;
		startZ += zM * stepMoveValue;
		putPixelInMap(
      cameraInstance,
      int(floor(startX)),
      int(floor(startY)),
      startZ,red,green,blue
    );
	}while (
		( stepMoveValue > 0 && startX + stepMoveValue < endX ) || 
		( stepMoveValue < 0 && startX - stepMoveValue > endX )
	);
  } else {
    float yDifference = endY - startY;
    if(yDifference==0){
      return;
    }
      float xM = (endX - startX)/yDifference;
      float zM = (endZ - startZ)/yDifference;
      putPixelInMap(
        cameraInstance,
        int(floor(startX)),
        int(floor(startY)),
        startZ,
        red,
        green,
        blue
      );
      float stepMoveValue = startY - endY > 0 ? -1 : +1;
    do{
      startY += stepMoveValue;
      startX += xM * stepMoveValue;
      startZ += zM * stepMoveValue;
      putPixelInMap(
        cameraInstance,
        int(floor(startX)),
        int(floor(startY)),
        startZ,
        red,
        green,
        blue
      );
    }while (
      (stepMoveValue > 0 && startY + stepMoveValue < endY) ||
      ( stepMoveValue <0 && startY - stepMoveValue > endY )
    );
  }
}