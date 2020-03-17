#ifndef SimpleEdge_class
#define SimpleEdge_class

#include <vector>

#include "../base_edge/BaseEdge.h"
#include "../../../data_types/MatrixTemplate.h"

class SimpleEdge : public BaseEdge
{
private:
  float red;
  float green;
  float blue;
  MatrixFloat* firstEdge;
  MatrixFloat* secondEdge;
  MatrixFloat* thirdEdge;
public:
  SimpleEdge(
    int edge1,
    int edge2,
    int edge3,
    float red,
    float green,
    float blue
  );
  void render(
    Camera& cameraInstance,
    std::vector<MatrixFloat>* worldPoints,
    float transformCenterX,
    float transformCenterY,
    float transformCenterZ
  ) override;
  EdgeType getEdgeType() override;
};

#endif