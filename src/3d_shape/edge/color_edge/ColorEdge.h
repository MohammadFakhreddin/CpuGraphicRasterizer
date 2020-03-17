#ifndef ColorEdge_class
#define ColorEdge_class

#include "../base_edge/BaseEdge.h"

class ColorEdge : public BaseEdge
{
private:
  float red;
  float green;
  float blue;
  static constexpr float stepValue = 1;
  static constexpr float minimumDrawDistance = 0.5;
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
    std::vector<MatrixFloat>* worldPoints,
    Vec3DFloat& cameraLocation,
    unsigned int appScreenWidth,
    unsigned int appScreenHeight,
    float transformX,
    float transformY,
    float transformZ
  ) override;
  EdgeType getEdgeType() override;
};

#endif