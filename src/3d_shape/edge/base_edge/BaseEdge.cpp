#include "./BaseEdge.h"
#include <iostream>

int BaseEdge::getEdgeByIndex(int index){
  assert(index>-1);
  assert(index<3);
  switch (index)
  {
  case 0:
    return edge1;
  case 1:
    return edge2;
  case 2:
    return edge3;
  }
  return -1;
}

EdgeType BaseEdge::getEdgeType(){
  return EdgeType::base;
}

void BaseEdge::render(std::vector<MatrixFloat>* worldPoints){
  std::cout<<"Rendering"<<std::endl;
}

  void BaseEdge::assignToObject(BaseEdge const& other){
    edge1 = other.edge1;
    edge2 = other.edge2;
    edge3 = other.edge3;
    red = other.red;
    green = other.green;
    blue = other.blue;
  }

  BaseEdge& BaseEdge::operator=(BaseEdge const& other) { 
    assignToObject(other);
    return *this; 
  }

  BaseEdge::BaseEdge(BaseEdge const& other) {
    assignToObject(other);
  }