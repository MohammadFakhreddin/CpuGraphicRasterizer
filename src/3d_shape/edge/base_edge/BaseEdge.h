#ifndef BaseEdge_class
#define BaseEdge_class

#include <vector>

#include "../../../data_types/MatrixTemplate.h"
#include "../../../data_types/VectorTemplate.h"
#include "../../../camera/Camera.h"

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
    Camera& cameraInstance,
    std::vector<MatrixFloat>* worldPoints,
    float shapeCenterX,
    float shapeCenterY,
    float shapeCenterZ
  );
  virtual EdgeType getEdgeType();
  int getEdgeByIndex(int index);
  bool areEdgesDataValid(int nodesSize);
protected:
  int edge1;
  int edge2;
  int edge3;
  bool isVisibleToCamera(
    Camera& cameraInstance,
    std::vector<MatrixFloat>* worldPoints,
    float shapeCenterX,
    float shapeCenterY,
    float shapeCenterZ
  );
private:
  Vec3DFloat cameraVector = Vec3DFloat(0.0f,0.0f,0.0f);
  Vec3DFloat normalVector = Vec3DFloat(0.0f,0.0f,0.0f);
  Vec3DFloat edge1To2Vector = Vec3DFloat(0.0f,0.0f,0.0f);
  Vec3DFloat edge1To3Vector = Vec3DFloat(0.0f,0.0f,0.0f);
  Vec3DFloat triangleCenter = Vec3DFloat(0.0f,0.0f,0.0f);
  Vec3DFloat triangleCenterToPolygonCenterPoint = Vec3DFloat(0.0f,0.0f,0.0f);
  float dotProductValue = 0;
  float normalVectorExtraDirectionFactor = 0;
};

#endif