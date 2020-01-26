#pragma once

#include "../date_types/MatrixTemplate.h"
#include <vector>

class Shape3d
{
private:
  std::vector<MatrixFloat> nodes;
  std::vector<int[2]>edges;
public:
  Shape3d(std::vector<MatrixFloat> nodes,std::vector<int[2]> edges);
};

