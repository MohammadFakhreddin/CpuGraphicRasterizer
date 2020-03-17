#include "./SimpleEdge.h"
#include "./../../../camera/Camera.h"

SimpleEdge::SimpleEdge(
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

void SimpleEdge::render(
  Camera& cameraInstance,
  std::vector<MatrixFloat>* worldPoints,
  float transformCenterX,
  float transformCenterY,
  float transformCenterZ
){
  for(int i=0;i<3;i++){
    firstEdge = &worldPoints->at(getEdgeByIndex((i)%3));
    secondEdge = &worldPoints->at(getEdgeByIndex((i+1)%3));
    thirdEdge = &worldPoints->at(getEdgeByIndex((i+2)%3));
    cameraInstance.drawLineBetweenPoints(
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
    cameraInstance.drawLineBetweenPoints(
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
    cameraInstance.drawLineBetweenPoints(
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

EdgeType SimpleEdge::getEdgeType(){
  return EdgeType::color;
}

