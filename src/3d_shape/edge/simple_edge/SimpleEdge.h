#ifndef SimpleEdge_class
#define SimpleEdge_class

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
  void render(std::vector<MatrixFloat>* worldPoints) override;
  EdgeType getEdgeType() override;
};

#endif