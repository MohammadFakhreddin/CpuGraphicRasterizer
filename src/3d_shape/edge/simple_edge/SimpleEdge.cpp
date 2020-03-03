#include "./SimpleEdge.h"
#include "./../../../application/Application.h"

SimpleEdge::SimpleEdge(
  int edge1,
  int edge2,
  int edge3,
  float red,
  float green,
  float blue
)
:
  edge1(edge1),
  edge2(edge2),
  edge3(edge3),
  red(red),
  green(green),
  blue(blue)
{}

void SimpleEdge::render(std::vector<MatrixFloat>* worldPoints){
  for(int i=0;i<3;i++){
    firstEdge = &worldPoints->at(getEdgeByIndex((i)%3));
    secondEdge = &worldPoints->at(getEdgeByIndex((i+1)%3));
    thirdEdge = &worldPoints->at(getEdgeByIndex((i+2)%3));
    Application::getInstance()->drawLineBetweenPoints(
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
    Application::getInstance()->drawLineBetweenPoints(
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
    Application::getInstance()->drawLineBetweenPoints(
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

SimpleEdge::SimpleEdge(SimpleEdge const& other){
  assignToObject(other);
}

SimpleEdge& SimpleEdge::operator=(SimpleEdge const& other){
  assignToObject(other);
  return *this; 
}

void SimpleEdge::assignToObject(SimpleEdge const& other){
  edge1 = other.edge1;
  edge2 = other.edge2;
  edge3 = other.edge3;
  red = other.red;
  green = other.green;
  blue = other.blue;
}
