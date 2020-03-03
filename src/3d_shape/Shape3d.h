#ifndef Shape3d_class
#define Shape3d_class

#include "../data_types/MatrixTemplate.h"
#include <vector>
#include <iostream>
#include "../fa_texture/FaTexture.h"
#include "../data_types/VectorTemplate.h"
#include "./edge/base_edge/BaseEdge.h"
#include <memory>

class Shape3d
{
public:
  static std::unique_ptr<Shape3d> generate3DCube(
    float xWidth,
    float yWidth,
    float zWidth,
    float transformX,
    float transformY,
    float transformZ,
    float rotationX,
    float rotationY,
    float rotationZ,
    float scale
  );
  Shape3d(
    std::vector<MatrixFloat> nodes,
    std::vector<BaseEdge*> edges
  );
  Shape3d(
    std::vector<MatrixFloat> nodes,
    std::vector<BaseEdge*> edges,
    float initialTransformX,
    float initialTransformY,
    float initialTransformZ
  );
  Shape3d(
    std::vector<MatrixFloat> nodes,
    std::vector<BaseEdge*> edges,
    float transformX,
    float transformY,
    float transformZ,
    float rotationDegreeX,
    float rotationDegreeY,
    float rotationDegreeZ,
    float scaleValue
  );
  bool checkForNodesValidation();
  void update(float deltaTime);
  void transformX(float x);
  void transformY(float y);
  void transformZ(float z);
  void scale(float value);
  void rotateX(float x);
  void rotateY(float y);
  void rotateZ(float z);
private:
  std::vector<MatrixFloat> nodes;
  std::vector<std::unique_ptr<BaseEdge>>edges;
  std::vector<MatrixFloat> worldPoints;
  MatrixFloat transformMatrix;
  MatrixFloat rotationDegreeMatrix;
  MatrixFloat rotationValueXMatrix;
  MatrixFloat rotationValueYMatrix;
  MatrixFloat rotationValueZMatrix;
  MatrixFloat scaleValueMatrix;
  MatrixFloat zScaleMatrix;
  float zLocation = 0;
  float scaleValue = 0;
};

#endif