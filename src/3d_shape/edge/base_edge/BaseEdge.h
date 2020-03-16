#ifndef BaseEdge_class
#define BaseEdge_class

#include "../../../data_types/MatrixTemplate.h"
#include <vector>
#include "../../../data_types/VectorTemplate.h"


enum EdgeType{
  color,
  simple,
  texture,
  base
};

class BaseEdge
{
public:
  virtual void render(
    std::vector<MatrixFloat>* worldPoints,
    Vec3DFloat& cameraLocation,
    unsigned int appScreenWidth,
    unsigned int appScreenHeight,
    float shapeTransformX,
    float shapeTransformY,
    float shapeTransformZ
  );
  virtual EdgeType getEdgeType();
  int getEdgeByIndex(int index);
  bool areEdgesDataValid(int nodesSize);
protected:
  int edge1;
  int edge2;
  int edge3;
  bool isVisibleToCamera(
    std::vector<MatrixFloat>* worldPoints,
    Vec3DFloat& cameraLocation,
    unsigned int appScreenWidth,
    unsigned int appScreenHeight,
    float shapeTransformX,
    float shapeTransformY,
    float shapeTransformZ
  );
private:
  Vec3DFloat cameraVector = Vec3DFloat(0.0f,0.0f,0.0f);
  Vec3DFloat normalVector = Vec3DFloat(0.0f,0.0f,0.0f);
  Vec3DFloat edge1To2Vector = Vec3DFloat(0.0f,0.0f,0.0f);
  Vec3DFloat edge1To3Vector = Vec3DFloat(0.0f,0.0f,0.0f);
  Vec3DFloat triangleCenter = Vec3DFloat(0.0f,0.0f,0.0f);
  Vec3DFloat triangleCenterToPolygonCenterPoint = Vec3DFloat(0.0f,0.0f,0.0f);
  float dotProductValue = 0;
  int normalVectorExtraDirectionFactor = 0;
};

#endif