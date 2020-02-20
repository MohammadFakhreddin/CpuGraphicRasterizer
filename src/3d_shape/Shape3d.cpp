#include "Shape3d.h"
#include <assert.h>
#include "../OpenGlHeaders.h"
#include <math.h>
#include <iostream>
#include "../application/Application.h"

std::unique_ptr<Shape3d> Shape3d::generate3DCube(
  float h,
  float w,
  float d,
  float transformX,
  float transformY,
  float transformZ,
  float rotationX,
  float rotationY,
  float rotationZ,
  float scale
){
  float x = -w/2;
	float y = -h/2;
	float z = -d/2;
	std::vector<MatrixFloat> nodeList = {
		MatrixFloat(3,1,std::vector<std::vector<float>>{
			std::vector<float>{x},
			std::vector<float>{y},
			std::vector<float>{z}
		}),
		MatrixFloat(3,1,std::vector<std::vector<float>>{
			std::vector<float>{x},
			std::vector<float>{y},
			std::vector<float>{z+d}
		}),
		MatrixFloat(3,1,std::vector<std::vector<float>>{
			std::vector<float>{x},
			std::vector<float>{y+h},
			std::vector<float>{z}
		}),
		MatrixFloat(3,1,std::vector<std::vector<float>>{
			std::vector<float>{x},
			std::vector<float>{y+h},
			std::vector<float>{z+d}
		}),
		MatrixFloat(3,1,std::vector<std::vector<float>>{
			std::vector<float>{x+w},
			std::vector<float>{y},
			std::vector<float>{z}
		}),
		MatrixFloat(3,1,std::vector<std::vector<float>>{
			std::vector<float>{x+w},
			std::vector<float>{y},
			std::vector<float>{z+d}
		}),
		MatrixFloat(3,1,std::vector<std::vector<float>>{
			std::vector<float>{x+w},
			std::vector<float>{y+h},
			std::vector<float>{z}
		}),
		MatrixFloat(3,1,std::vector<std::vector<float>>{
			std::vector<float>{x+w},
			std::vector<float>{y+h},
			std::vector<float>{z+d}
		})
	};
	std::vector<Edge> edgeList = {
		Edge(0,1),
		Edge(1,3),
		Edge(3,2),
		Edge(2,0),
		Edge(4,5),
		Edge(5,7),
		Edge(7,6),
		Edge(6,4),
		Edge(0,4),
		Edge(1,5),
		Edge(2,6),
		Edge(3,7)
	};
	return std::unique_ptr<Shape3d>(new Shape3d(
		nodeList,
		edgeList,
		transformX,
		transformY,
    transformZ,
    rotationX,
    rotationY,
    rotationZ,
    scale
	));
}

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
  Shape3d(nodes,edges,0,0,0,0,0,0,1)
{}

Shape3d::Shape3d(
  std::vector<MatrixFloat> nodes,
  std::vector<Edge> edges,
  float initialTransformX,
  float initialTransformY,
  float initialTransformZ
)
  :
  Shape3d(nodes,edges,initialTransformX,initialTransformY,initialTransformZ,0,0,0,1)
{}

Shape3d::Shape3d(
  std::vector<MatrixFloat> nodes,
  std::vector<Edge> edges,
  float transformX,
  float transformY,
  float transformZ,
  float rotationDegreeX,
  float rotationDegreeY,
  float rotationDegreeZ,
  float scaleValue
)  :
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
  }),
  zScaleMatrix(3,3,std::vector<std::vector<float>>{
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
  transformMatrix.set(0,0,transformX);
  transformMatrix.set(1,0,transformY);
  transformMatrix.set(2,0,transformZ);
  rotationDegreeMatrix.set(0,0,rotationDegreeX);
  rotationDegreeMatrix.set(1,0,rotationDegreeY);
  rotationDegreeMatrix.set(2,0,rotationDegreeZ);
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
    MatrixFloat rotationAndScaleResult(3,1,0);
    MatrixFloat zComparisionMatrix(3,1,0);
    float zLocation = 0;
    float scaleValue = 0;
    for(int i=0;i<nodes.size();i++){
      rotationAndScaleResult = (
        nodes[i] * 
        rotationValueXMatrix *
        rotationValueYMatrix *
        rotationValueZMatrix *
        scaleValueMatrix
      );
      zComparisionMatrix = rotationAndScaleResult + transformMatrix;
      zLocation = zComparisionMatrix.get(2,0);
      scaleValue = (Application::cameraZLocation - zLocation) / Application::maximumFov;
      zScaleMatrix.set(0,0,scaleValue);
      zScaleMatrix.set(1,1,scaleValue);
      worldPoints.at(i) = rotationAndScaleResult *
        zScaleMatrix + 
        transformMatrix;
    }
  }
}

void Shape3d::render(){
  if(nodes.size()>0 && edges.size()>0){
    MatrixFloat* currentWorldPoint;
    for(auto& edge:edges){
      //TODO Avoid rendering objects that are out of FOV
      glBegin(GL_LINES);
      currentWorldPoint = &worldPoints.at(edge.getByIndex(0));
      glVertex2f(currentWorldPoint->get(0,0),currentWorldPoint->get(1,0));
      currentWorldPoint = &worldPoints.at(edge.getByIndex(1));
      glVertex2f(currentWorldPoint->get(0,0),currentWorldPoint->get(1,0));
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