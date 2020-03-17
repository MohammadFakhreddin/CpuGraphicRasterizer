#ifndef ColorEdge_class
#define ColorEdge_class

#include "../base_edge/BaseEdge.h"
#include "./../../../camera/Camera.h"

class ColorEdge : public BaseEdge
{
private:
  static constexpr float stepValue = 1;
  static constexpr float minimumDrawDistance = 0.5;
  float red;
  float green;
  float blue;
  MatrixFloat* currentWorldPoint;
  std::vector<MatrixFloat*> topEdges;
  std::vector<MatrixFloat*> bottomEdges;
  std::vector<MatrixFloat*> middleEdges;
public:
  ColorEdge(
    int edge1,
    int edge2,
    int edge3,
    float red,
    float green,
    float blue
  );
  MatrixFloat middlePoint = MatrixFloat(3,1,std::vector<std::vector<float>>{
    std::vector<float>{0},
    std::vector<float>{0},
    std::vector<float>{0}
  });
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