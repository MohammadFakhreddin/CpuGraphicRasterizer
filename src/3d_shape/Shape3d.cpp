#include "Shape3d.h"
#include <assert.h>
#include "../OpenGlHeaders.h"
#include <math.h>
#include <iostream>

Edge::Edge(int first,int second)
:
first(first),
second(second)
{}

int Edge::getByIndex(int index){
  assert(index>-1);
  assert(index<2);
  if(index==1){
    return second;
  }
  return first;
}

int Edge::getFirst(){
  return first;
}

int Edge::getSecond(){
  return second;
}

Shape3d::Shape3d(std::vector<MatrixFloat> nodes,std::vector<Edge> edges)
  :
  nodes(nodes),
  edges(edges),
  transformMatrix(3,1,0.0f),
  rotationDegreeMatrix(3,1,0.0f),
  rotationValueXMatrix(3,3,std::vector<std::vector<float>>{
    std::vector<float>{cosf(0),-sinf(0),0},
    std::vector<float>{sinf(0),cosf(0),0},
    std::vector<float>{0,0,1}
  }),
  rotationValueYMatrix(3,3,std::vector<std::vector<float>>{
    std::vector<float>{cosf(0),0,sinf(0)},
    std::vector<float>{0,1,0},
    std::vector<float>{-sinf(0),0,cosf(0)}
  }),
  rotationValueZMatrix(3,3,std::vector<std::vector<float>>{
    std::vector<float>{1,0,0},
    std::vector<float>{0,cosf(0),sinf(0)},
    std::vector<float>{0,-sinf(0),cosf(0)}
  }),
  scaleValueMatrix(3,3,std::vector<std::vector<float>>{
    std::vector<float>{1,0,0},
    std::vector<float>{0,1,0},
    std::vector<float>{0,0,1}
  })
{
  assert(nodes.size()>0);
  assert(edges.size()>0);
  assert(checkForNodesValidation());
  if(nodes.size()>0){
    for(auto& node:nodes){
      worldPoints.emplace_back(node.clone());
    }
  }
}

bool Shape3d::checkForNodesValidation(){
  if(nodes.size()>0){
    for(auto& node:nodes){
      if(node.getWidth()!=3||node.getHeight()!=1){
        return false;
      }
    }
  }
  return true;
}

void Shape3d::update(){
  if(nodes.size()>0){
    for(int i=0;i<nodes.size();i++){
      worldPoints.at(i) = nodes[i] * 
        rotationValueXMatrix *
        rotationValueYMatrix *
        rotationValueZMatrix *
        scaleValueMatrix + 
        transformMatrix;
    }
  }
}

void Shape3d::render(){
  if(nodes.size()>0 && edges.size()>0){
    for(auto& edge:edges){
      glBegin(GL_LINES);
      glVertex2f(worldPoints.at(edge.getByIndex(0)).get(0,0),worldPoints.at(edge.getByIndex(0)).get(1,0));
      glVertex2f(worldPoints.at(edge.getByIndex(1)).get(0,0),worldPoints.at(edge.getByIndex(1)).get(1,0));
      glEnd();
    }
  }
}

void Shape3d::transformX(float x){
  transformMatrix.set(0,0,transformMatrix.get(0,0) + x);
}

void Shape3d::transformY(float y){
  transformMatrix.set(1,0,transformMatrix.get(1,0) + y);
}

void Shape3d::transformZ(float z){
  transformMatrix.set(2,0,transformMatrix.get(2,0) + z);
}

void Shape3d::scale(float value){
  scaleValueMatrix.set(0,0,scaleValueMatrix.get(0,0) + value);
  scaleValueMatrix.set(1,1,scaleValueMatrix.get(1,1) + value);
  scaleValueMatrix.set(2,2,scaleValueMatrix.get(2,2) + value);
}

void Shape3d::rotateX(float x){
  rotationDegreeMatrix.set(0,0,rotationDegreeMatrix.get(0,0) + x);
  rotationValueXMatrix.set(0,0,cosf(rotationDegreeMatrix.get(0,0)));
  rotationValueXMatrix.set(0,1,-sinf(rotationDegreeMatrix.get(0,0)));
  rotationValueXMatrix.set(1,0,sinf(rotationDegreeMatrix.get(0,0))); 
  rotationValueXMatrix.set(1,1,cosf(rotationDegreeMatrix.get(0,0))); 
}

void Shape3d::rotateY(float y){
  rotationDegreeMatrix.set(1,0,rotationDegreeMatrix.get(1,0) + y);
  rotationValueYMatrix.set(0,0,cosf(rotationDegreeMatrix.get(1,0)));
  rotationValueYMatrix.set(0,2,sinf(rotationDegreeMatrix.get(1,0)));
  rotationValueYMatrix.set(2,0,-sinf(rotationDegreeMatrix.get(1,0))); 
  rotationValueYMatrix.set(2,2,cosf(rotationDegreeMatrix.get(1,0))); 
}

void Shape3d::rotateZ(float z){
  rotationDegreeMatrix.set(2,0,rotationDegreeMatrix.get(2,0) + z);
  rotationValueZMatrix.set(1,1,cosf(rotationDegreeMatrix.get(2,0)));
  rotationValueZMatrix.set(1,2,sinf(rotationDegreeMatrix.get(2,0)));
  rotationValueZMatrix.set(2,1,-sinf(rotationDegreeMatrix.get(2,0))); 
  rotationValueZMatrix.set(2,2,cosf(rotationDegreeMatrix.get(2,0))); 
}