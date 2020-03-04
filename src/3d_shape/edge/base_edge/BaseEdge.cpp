#include "./BaseEdge.h"
#include <iostream>
#include <assert.h>

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
  assert(false);
}

bool BaseEdge::areEdgesDataValid(int nodesSize){
  int currentEdge = -1;
  for(int i=0;i<3;i++){
    currentEdge = getEdgeByIndex(i);
    if(currentEdge<0||currentEdge>=nodesSize){
      return false;
    }
  }
  return true;
}