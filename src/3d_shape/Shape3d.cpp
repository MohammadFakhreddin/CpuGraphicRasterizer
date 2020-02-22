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
	std::vector<ColorEdge> edgeList = {
		ColorEdge(0,1,3,false,1,1,1),
		ColorEdge(1,3,2,false,1,1,1),
		ColorEdge(3,2,0,false,1,1,1),
		ColorEdge(2,0,1,false,1,1,1),
		ColorEdge(4,5,7,false,1,1,1),
		ColorEdge(5,7,6,false,1,1,1),
		ColorEdge(7,6,4,false,1,1,1),
		ColorEdge(6,4,5,false,1,1,1),
		ColorEdge(0,4,5,false,1,1,1),
		ColorEdge(1,5,7,false,1,1,1),
		ColorEdge(2,6,4,false,1,1,1),
		ColorEdge(3,7,6,false,1,1,1)
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

ColorEdge::ColorEdge(
  int edge1,
  int edge2,
  int edge3,
  bool fillSpaceBetween,
  float red,
  float green,
  float blue
)
:
  edge1(edge1),
  edge2(edge2),
  edge3(edge3),
  fillSpaceBetween(fillSpaceBetween),
  red(red),
  green(green),
  blue(blue)
{}

int ColorEdge::getEdgeByIndex(int index){
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
}

int ColorEdge::getEdge1(){
  return edge1;
}

int ColorEdge::getEdge2(){
  return edge2;
}

int ColorEdge::getEdge3(){
  return edge3;
}

float ColorEdge::getColorByIndex(int index){
  assert(index>-1);
  assert(index<3);
  switch (index)
  {
  case 0:
    return red;
  case 1:
    return green;
  case 2:
    return blue;
  }
}

float ColorEdge::getRed(){
  return red;
}

float ColorEdge::getGreen(){
  return green;
}

float ColorEdge::getBlue(){
  return blue;
}

bool ColorEdge::getFillSpaceBetween(){
  return fillSpaceBetween;
}

Shape3d::Shape3d(std::vector<MatrixFloat> nodes,std::vector<ColorEdge> edges)
  :
  Shape3d(nodes,edges,0,0,0,0,0,0,1)
{}

Shape3d::Shape3d(
  std::vector<MatrixFloat> nodes,
  std::vector<ColorEdge> edges,
  float initialTransformX,
  float initialTransformY,
  float initialTransformZ
)
  :
  Shape3d(nodes,edges,initialTransformX,initialTransformY,initialTransformZ,0,0,0,1)
{}

Shape3d::Shape3d(
  std::vector<MatrixFloat> nodes,
  std::vector<ColorEdge> edges,
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
  pixelMap.erase(pixelMap.begin(),pixelMap.end());
  if(nodes.size()>0){
    {
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
    if(edges.size()>0){
      for(auto& edge:edges){
        //TODO Avoid rendering objects that are out of FOV
        if(edge.getFillSpaceBetween()==false){
          for(int i=0;i<3;i++){
            drawLineBetweenPoints(
              worldPoints.at(edge.getEdgeByIndex((i)%3)),
              worldPoints.at(edge.getEdgeByIndex((i+1)%3)),
              edge.getRed(),
              edge.getGreen(),
              edge.getBlue()
            );
            drawLineBetweenPoints(
              worldPoints.at(edge.getEdgeByIndex((i)%3)),
              worldPoints.at(edge.getEdgeByIndex((i+2)%3)),
              edge.getRed(),
              edge.getGreen(),
              edge.getBlue()
            );
            drawLineBetweenPoints(
              worldPoints.at(edge.getEdgeByIndex((i+2)%3)),
              worldPoints.at(edge.getEdgeByIndex((i+1)%3)),
              edge.getRed(),
              edge.getGreen(),
              edge.getBlue()
            );
          }
        } else{
          //TODO
        }
      }
    }
  }
}

void Shape3d::drawLineBetweenPoints(
  MatrixFloat point1,
  MatrixFloat point2,
  float red,
  float green,
  float blue
){
  float startX = point1.get(0,0);
  float startY = point1.get(1,0);
  float startZ = point1.get(2,0);
  float endX = point2.get(0,0);
  float endY = point2.get(1,0);
  float endZ = point2.get(2,0);
  bool moveByX = true;
  if(abs(startX-endX)<abs(startY-endY)){
    moveByX = false;
  }
  if( moveByX == true ){
    float xDifference = endX - startX;
    float yM = (endY - startY)/xDifference;
    float zM = (endZ - startZ)/xDifference;
    int drawY = roundf(startY);
    putPixelInMapIfPossible(startX,drawY,startZ,red,green,blue);
    float stepMoveValue = startX - endX > 0 ? -1 : +1;
    for(int i = startX;i <= endX; i++)
    {
      startX += stepMoveValue;
      startY += yM;
      startZ += zM;
      drawY = roundf(startY);
      putPixelInMapIfPossible(startX,drawY,startZ,red,green,blue);
    }
  } else {
    float yDifference = endY - startY;
    float xM = (endX - startX)/yDifference;
    float zM = (endZ - startZ)/yDifference;
    int drawX = roundf(startX);
    putPixelInMapIfPossible(drawX,startY,startZ,red,green,blue);
    float stepMoveValue = startY - endY > 0 ? -1 : +1;
    for(int i=startY;i<=endY;i++){
      startY += stepMoveValue;
      startX += xM;
      startZ += zM;
      drawX = roundf(startX);
      putPixelInMapIfPossible(drawX,startY,startZ,red,green,blue);
    }
  }
}

void Shape3d::putPixelInMapIfPossible(
  int x,
  int y,
  float zValue,
  float red,
  float green,
  float blue
){
  auto existingPixel = pixelMap.find({x,y});
  if(existingPixel==pixelMap.end()){
    DrawPixel pixel;
    pixel.x = x;
    pixel.y = y;
    pixel.zValue = zValue;
    pixel.red = red;
    pixel.green = green;
    pixel.blue = blue;
    pixelMap[{x,y}] = {
      x,
      y,
      zValue,
      red,
      green,
      blue
    };
  }else if(existingPixel->second.zValue < zValue)
  {
    existingPixel->second.x = x;
    existingPixel->second.y = y;
    existingPixel->second.red = red;
    existingPixel->second.green = green;
    existingPixel->second.blue = blue;
    existingPixel->second.zValue = zValue;
  }
}

void Shape3d::render(){
  if(pixelMap.size()>0){
    glBegin(GL_POINTS);
    for(auto pixel:pixelMap){
      glColor3f(pixel.second.red,pixel.second.green,pixel.second.blue);
      glVertex2f(pixel.second.x,pixel.second.y);
    }
    glEnd();
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