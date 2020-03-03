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
  BaseEdge() = default;
  ~BaseEdge() = default;
  BaseEdge(BaseEdge const& other);
  BaseEdge(BaseEdge && other) = default;
  BaseEdge& operator=(BaseEdge const& other);
  BaseEdge& operator=(BaseEdge && other) = default;
  virtual void render(std::vector<MatrixFloat>* worldPoints);
  virtual EdgeType getEdgeType();
private:
  int edge1;
  int edge2;
  int edge3;
  float red;
  float green;
  float blue;
  void assignToObject(BaseEdge const& other);
protected:
  int getEdgeByIndex(int index);
};

#endif