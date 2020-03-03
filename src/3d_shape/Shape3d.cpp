#include "./Shape3d.h"
#include <assert.h>
#include "../OpenGlHeaders.h"
#include <math.h>
#include <iostream>
#include "../application/Application.h"
#include "./edge/base_edge/BaseEdge.h"
#include "./edge/color_edge/ColorEdge.h"

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
	std::vector<BaseEdge*> edgeList = {
    new ColorEdge(0,1,2,245.0f/255.0f, 144.0f/255.0f, 66.0f/255.0f),
    new ColorEdge(3,1,2,245.0f/255.0f, 144.0f/255.0f, 66.0f/255.0f),
    new ColorEdge(3,1,5,221.0f/255.0f, 245.0f/255.0f, 66.0f/255.0f),
    new ColorEdge(3,7,5,221.0f/255.0f, 245.0f/255.0f, 66.0f/255.0f),
    new ColorEdge(0,4,5,66.0f/255.0f, 245.0f/255.0f, 126.0f/255.0f),
    new ColorEdge(5,0,1,66.0f/255.0f, 245.0f/255.0f, 126.0f/255.0f),
    new ColorEdge(4,0,6,66.0f/255.0f, 194.0f/255.0f, 245.0f/255.0f),
    new ColorEdge(2,0,6,66.0f/255.0f, 194.0f/255.0f, 245.0f/255.0f),
    new ColorEdge(2,7,6,96.0f/255.0f, 66.0f/255.0f, 245.0f/255.0f), 
    new ColorEdge(2,7,3,96.0f/255.0f, 66.0f/255.0f, 245.0f/255.0f),
    new ColorEdge(4,5,7,245.0f/255.0f, 66.0f/255.0f, 123.0f/255.0f),
    new ColorEdge(4,6,7,245.0f/255.0f, 66.0f/255.0f, 123.0f/255.0f)  
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

Shape3d::Shape3d(
  std::vector<MatrixFloat> nodes,
  std::vector<BaseEdge*> edges
)
  :
  Shape3d(nodes,edges,0,0,0,0,0,0,1)
{}

Shape3d::Shape3d(
  std::vector<MatrixFloat> nodes,
  std::vector<BaseEdge*> edges,
  float initialTransformX,
  float initialTransformY,
  float initialTransformZ
)
  :
  Shape3d(nodes,edges,initialTransformX,initialTransformY,initialTransformZ,0,0,0,1)
{}

Shape3d::Shape3d(
  std::vector<MatrixFloat> nodes,
  std::vector<BaseEdge*> edges,
  float transformX,
  float transformY,
  float transformZ,
  float rotationDegreeX,
  float rotationDegreeY,
  float rotationDegreeZ,
  float scaleValue
)  :
  nodes(nodes),
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
  if(edges.size()>0){
    for(auto edge:edges){
      this->edges.emplace_back(std::unique_ptr<BaseEdge>(edge));
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

void Shape3d::update(float deltaTime){
  if(nodes.size()>0){
    {
      MatrixFloat rotationAndScaleResult(3,1,0);
      MatrixFloat zComparisionMatrix(3,1,0);
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
    if(edges.size()>0){
      for(auto& edge:edges){
        edge->render(&worldPoints);
      }
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