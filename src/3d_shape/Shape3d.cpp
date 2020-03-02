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
    ColorEdge(0,1,2,true,245.0f/255.0f, 144.0f/255.0f, 66.0f/255.0f),
    ColorEdge(3,1,2,true,245.0f/255.0f, 144.0f/255.0f, 66.0f/255.0f),
    ColorEdge(3,1,5,true,221.0f/255.0f, 245.0f/255.0f, 66.0f/255.0f),
    ColorEdge(3,7,5,true,221.0f/255.0f, 245.0f/255.0f, 66.0f/255.0f),
    ColorEdge(0,4,5,true,66.0f/255.0f, 245.0f/255.0f, 126.0f/255.0f),
    ColorEdge(5,0,1,true,66.0f/255.0f, 245.0f/255.0f, 126.0f/255.0f),
    ColorEdge(4,0,6,true,66.0f/255.0f, 194.0f/255.0f, 245.0f/255.0f),
    ColorEdge(2,0,6,true,66.0f/255.0f, 194.0f/255.0f, 245.0f/255.0f),
    ColorEdge(2,7,6,true,96.0f/255.0f, 66.0f/255.0f, 245.0f/255.0f), 
    ColorEdge(2,7,3,true,96.0f/255.0f, 66.0f/255.0f, 245.0f/255.0f),
    ColorEdge(4,5,7,true,245.0f/255.0f, 66.0f/255.0f, 123.0f/255.0f),
    ColorEdge(4,6,7,true,245.0f/255.0f, 66.0f/255.0f, 123.0f/255.0f)  
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
  return -1;
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
  return -1;
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

void Shape3d::update(float deltaTime){
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
          MatrixFloat* firstEdge;
          MatrixFloat* secondEdge;
          MatrixFloat* thirdEdge;
          for(int i=0;i<3;i++){
            firstEdge = &worldPoints.at(edge.getEdgeByIndex((i)%3));
            secondEdge = &worldPoints.at(edge.getEdgeByIndex((i+1)%3));
            thirdEdge = &worldPoints.at(edge.getEdgeByIndex((i+2)%3));
            drawLineBetweenPoints(
              firstEdge->get(0,0),
              firstEdge->get(1,0),
              firstEdge->get(2,0),
              secondEdge->get(0,0),
              secondEdge->get(1,0),
              secondEdge->get(2,0),
              edge.getRed(),
              edge.getGreen(),
              edge.getBlue()
            );
            drawLineBetweenPoints(
              firstEdge->get(0,0),
              firstEdge->get(1,0),
              firstEdge->get(2,0),
              thirdEdge->get(0,0),
              thirdEdge->get(1,0),
              thirdEdge->get(2,0),
              edge.getRed(),
              edge.getGreen(),
              edge.getBlue()
            );
            drawLineBetweenPoints(
              thirdEdge->get(0,0),
              thirdEdge->get(1,0),
              thirdEdge->get(2,0),
              secondEdge->get(0,0),
              secondEdge->get(1,0),
              secondEdge->get(2,0),
              edge.getRed(),
              edge.getGreen(),
              edge.getBlue()
            );
          }
        } else{
          std::vector<MatrixFloat*> topEdges;
          std::vector<MatrixFloat*> bottomEdges;
          std::vector<MatrixFloat*> middleEdges;
          MatrixFloat middlePoint = MatrixFloat(3,1,std::vector<std::vector<float>>{
            std::vector<float>{0},
            std::vector<float>{0},
            std::vector<float>{0}
          });
          MatrixFloat* currentWorldPoint;
          for(int i=0;i<3;i++){
            currentWorldPoint = &worldPoints.at(edge.getEdgeByIndex(i));
            if(topEdges.size()==0){
              topEdges.emplace_back(currentWorldPoint);
            }
            else if(currentWorldPoint->get(1,0)>topEdges.at(0)->get(1,0)){
              topEdges.erase(topEdges.begin(),topEdges.end());
              topEdges.emplace_back(currentWorldPoint);
            }
            else if(currentWorldPoint->get(1,0)==topEdges.at(0)->get(1,0)){
              topEdges.emplace_back(currentWorldPoint);
            }
          }
          for(int i=0;i<3;i++){
            currentWorldPoint = &worldPoints.at(edge.getEdgeByIndex(i));
            if(bottomEdges.size()==0){
              bottomEdges.emplace_back(currentWorldPoint);
            }
            else if(currentWorldPoint->get(1,0)<bottomEdges.at(0)->get(1,0)){
              bottomEdges.erase(bottomEdges.begin(),bottomEdges.end());
              bottomEdges.emplace_back(currentWorldPoint);
            }
            else if(currentWorldPoint->get(1,0)==bottomEdges.at(0)->get(1,0)){
              bottomEdges.emplace_back(currentWorldPoint);
            }
          }
          assert(topEdges.size()>=1);
          assert(bottomEdges.size()>=1);
          if(topEdges.size()!=2&&bottomEdges.size()!=2){
            for(int i=0;i<3;i++){
              currentWorldPoint = &worldPoints.at(edge.getEdgeByIndex(i));
              if(currentWorldPoint!=topEdges.at(0) && currentWorldPoint!=bottomEdges.at(0)){
                middleEdges.emplace_back(currentWorldPoint);
                middlePoint.set(0,0,(
                    currentWorldPoint->get(1,0) - topEdges.at(0)->get(1,0)
                  ) * (
                    (
                      topEdges.at(0)->get(0,0) - bottomEdges.at(0)->get(0,0)
                    )/(
                      topEdges.at(0)->get(1,0) - bottomEdges.at(0)->get(1,0)
                    )
                  ) + topEdges.at(0)->get(0,0)
                );
                middlePoint.set(1,0,currentWorldPoint->get(1,0));
                middlePoint.set(2,0,
                  (
                    currentWorldPoint->get(1,0) - topEdges.at(0)->get(1,0)
                  ) * (
                    (
                      topEdges.at(0)->get(2,0) - bottomEdges.at(0)->get(2,0)
                    )/(
                      topEdges.at(0)->get(1,0) - bottomEdges.at(0)->get(1,0)
                    )
                  ) + topEdges.at(0)->get(2,0)
                );
                MatrixFloat* middlePointPointer = &middlePoint;
                middleEdges.emplace_back(middlePointPointer);
                break;
              }
            }
          }
          float stepValue = 1;
          float minimumDrawDistance = 0.5;
          if(topEdges.size()==2){
            float currentY = topEdges.at(0)->get(1,0);
            float startX = topEdges.at(0)->get(0,0);
            float startZ = topEdges.at(0)->get(2,0);
            float endX = topEdges.at(1)->get(0,0);
            float endZ = topEdges.at(1)->get(2,0);
            float finalY = bottomEdges.at(0)->get(1,0);
            float finalX = bottomEdges.at(0)->get(0,0);
            float finalZ = bottomEdges.at(0)->get(2,0);
            float startXM = (startX - finalX)/(currentY - finalY);
            float startZM = (startZ - finalZ)/(currentY - finalY);
            float endXM = (endX - finalX)/(currentY - finalY);
            float endZM = (endZ - finalZ)/(currentY - finalY);
            if(abs(currentY-finalY)>minimumDrawDistance){
              do{
                drawLineBetweenPoints(
                  startX,currentY,startZ,
                  endX,currentY,endZ,
                  edge.getRed(),
                  edge.getGreen(),
                  edge.getBlue()
                );
                currentY-=stepValue;
                startX-=startXM * stepValue;
                startZ-=startZM * stepValue;
                endX-=endXM * stepValue;
                endZ-=endZM * stepValue;
              } while (currentY>finalY+stepValue);
            }
          }else if(bottomEdges.size()==2){
            float currentY = bottomEdges.at(0)->get(1,0);
            float startX = bottomEdges.at(0)->get(0,0);
            float startZ = bottomEdges.at(0)->get(2,0);
            float endX = bottomEdges.at(1)->get(0,0);
            float endZ = bottomEdges.at(1)->get(2,0);
            float finalY = topEdges.at(0)->get(1,0);
            float finalX = topEdges.at(0)->get(0,0);
            float finalZ = topEdges.at(0)->get(2,0);
            float yDifference = (currentY - finalY);
            float startXM = (startX - finalX)/yDifference;
            float startZM = (startZ - finalZ)/yDifference;
            float endXM = (endX - finalX)/yDifference;
            float endZM = (endZ - finalZ)/yDifference;
            if(abs(currentY - finalY)>minimumDrawDistance){
              do{
                drawLineBetweenPoints(
                  startX,currentY,startZ,
                  endX,currentY,endZ,
                  edge.getRed(),
                  edge.getGreen(),
                  edge.getBlue()
                );
                currentY+=stepValue;
                startX+=startXM * stepValue;
                startZ+=startZM * stepValue;
                endX+=endXM * stepValue;
                endZ+=endZM * stepValue;
              } while (currentY<finalY-1);
            }
          }else {
            assert(topEdges.size()==1);
            assert(middleEdges.size()==2);
            assert(bottomEdges.size()==1);
            assert(middleEdges.at(0)->get(1,0)<topEdges.at(0)->get(1,0));
            assert(middleEdges.at(0)->get(1,0)>bottomEdges.at(0)->get(1,0));
            assert(middleEdges.at(1)->get(1,0)<topEdges.at(0)->get(1,0));
            assert(middleEdges.at(1)->get(1,0)>bottomEdges.at(0)->get(1,0));
            assert(middleEdges.at(0)->get(1,0)==middleEdges.at(1)->get(1,0));
            {//From middle to top
              float currentY = middleEdges.at(0)->get(1,0);
              float startX = middleEdges.at(0)->get(0,0);
              float startZ = middleEdges.at(0)->get(2,0);
              float endX = middleEdges.at(1)->get(0,0);
              float endZ = middleEdges.at(1)->get(2,0);
              float finalY = topEdges.at(0)->get(1,0);
              float finalX = topEdges.at(0)->get(0,0);
              float finalZ = topEdges.at(0)->get(2,0);
              float startXM = (startX - finalX)/(currentY - finalY);
              float startZM = (startZ - finalZ)/(currentY - finalY);
              float endXM = (endX - finalX)/(currentY - finalY);
              float endZM = (endZ - finalZ)/(currentY - finalY);
              if(abs(currentY - finalY)>minimumDrawDistance){
                do{
                  drawLineBetweenPoints(
                    startX,currentY,startZ,
                    endX,currentY,endZ,
                    edge.getRed(),
                    edge.getGreen(),
                    edge.getBlue()
                  );
                  currentY+=stepValue;
                  startX+=startXM * stepValue;
                  startZ+=startZM * stepValue;
                  endX+=endXM * stepValue;
                  endZ+=endZM * stepValue;
                } while (currentY<finalY-1);
              }
            }
            {//From middle to bottom
              float currentY = middleEdges.at(0)->get(1,0);
              float startX = middleEdges.at(0)->get(0,0);
              float startZ = middleEdges.at(0)->get(2,0);
              float endX = middleEdges.at(1)->get(0,0);
              float endZ = middleEdges.at(1)->get(2,0);
              float finalY = bottomEdges.at(0)->get(1,0);
              float finalX = bottomEdges.at(0)->get(0,0);
              float finalZ = bottomEdges.at(0)->get(2,0);
              float startXM = (startX - finalX)/(currentY - finalY);
              float startZM = (startZ - finalZ)/(currentY - finalY);
              float endXM = (endX - finalX)/(currentY - finalY);
              float endZM = (endZ - finalZ)/(currentY - finalY);
              if(abs(currentY - finalY) > minimumDrawDistance){
                do{
                  drawLineBetweenPoints(
                    startX,currentY,startZ,
                    endX,currentY,endZ,
                    edge.getRed(),
                    edge.getGreen(),
                    edge.getBlue()
                  );
                  currentY-= stepValue;
                  startX-=startXM * stepValue;
                  startZ-=startZM * stepValue;
                  endX-=endXM * stepValue;
                  endZ-=endZM * stepValue;
                } while (currentY>finalY+1);
              }
            }
          }
        }
      }
    }
  }
}

void Shape3d::drawLineBetweenPoints(
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
  if( moveByX == true ){
    float xDifference = endX - startX;
    float yM = (endY - startY)/xDifference;
    float zM = (endZ - startZ)/xDifference;
    Application::getInstance()->putPixelInMap(ceil(startX),ceil(startY),startZ,red,green,blue);
    float stepMoveValue = startX - endX > 0 ? -1 : +1;
    for(
      int i = startX;
      (stepMoveValue>0 && startX<endX) || (stepMoveValue<0 && startX > endX); 
      i+=stepMoveValue
    )
    {
      startX += stepMoveValue;
      startY += yM * stepMoveValue;
      startZ += zM * stepMoveValue;
      Application::getInstance()->putPixelInMap(int(ceil(startX)),int(ceil(startY)),startZ,red,green,blue);
    }
  } else {
    float yDifference = endY - startY;
    float xM = (endX - startX)/yDifference;
    float zM = (endZ - startZ)/yDifference;
    Application::getInstance()->putPixelInMap(int(ceil(startX)),int(ceil(startY)),startZ,red,green,blue);
    float stepMoveValue = startY - endY > 0 ? -1 : +1;
    for(
      int i=startY;
      (stepMoveValue>0 && startY<endY) || (stepMoveValue<0 && startY > endY); 
      i+=stepMoveValue
    ){
      startY += stepMoveValue;
      startX += xM * stepMoveValue;
      startZ += zM * stepMoveValue;
      Application::getInstance()->putPixelInMap(int(ceil(startX)),int(ceil(startY)),startZ,red,green,blue);
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