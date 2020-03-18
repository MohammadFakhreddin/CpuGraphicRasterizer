#ifndef SimpleSurface_class
#define SimpleSurface_class

#include <vector>

#include "../base_surface/BaseSurface.h"
#include "../../../data_types/MatrixTemplate.h"

class SimpleSurface : public BaseSurface
{
private:

  void computePixelMapData(
    Camera& cameraInstance,
    std::vector<MatrixFloat>* worldPoints
  ) override;
  void drawLineBetweenPoints(
    Camera& cameraInstance,
    float startX,
    float startY,
    float startZ,
    float endX,
    float endY,
    float endZ,
    float red,
    float green,
    float blue
  );

  float red;
  float green;
  float blue;
  MatrixFloat* firstEdge;
  MatrixFloat* secondEdge;
  MatrixFloat* thirdEdge;

public:

  SimpleSurface(
    int edge1,
    int edge2,
    int edge3,
    float red,
    float green,
    float blue
  );
  EdgeType getEdgeType() override;

};

#endif