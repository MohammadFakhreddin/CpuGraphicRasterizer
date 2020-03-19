#ifndef Shape3d_class
#define Shape3d_class

#include <vector>
#include <iostream>
#include <memory>

#include "../data_types/MatrixTemplate.h"
#include "../fa_texture/FaTexture.h"
#include "../data_types/VectorTemplate.h"
#include "./surface/base_surface/BaseSurface.h"
#include "./../camera/Camera.h"
// tiny_obj_loader.cpp
// MiniBall.h
class Shape3d
{
public:
  static std::unique_ptr<Shape3d> generateColored3DCube(
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
  static std::unique_ptr<Shape3d> generateTextured3DCube(
    std::unique_ptr<FaTexture>& texture,
    std::vector<BaseSurface*> edgeList,
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
    std::vector<BaseSurface*> edges
  );
  Shape3d(
    std::vector<MatrixFloat> nodes,
    std::vector<BaseSurface*> edges,
    float initialTransformX,
    float initialTransformY,
    float initialTransformZ
  );
  Shape3d(
    std::vector<MatrixFloat> nodes,
    std::vector<BaseSurface*> edges,
    float transformX,
    float transformY,
    float transformZ,
    float rotationDegreeX,
    float rotationDegreeY,
    float rotationDegreeZ,
    float scaleValue
  );
  bool checkForNodesValidation();
  void update(double deltaTime,Camera& cameraInstance);
  void transformX(float x);
  void transformY(float y);
  void transformZ(float z);
  void scale(float value);
  void rotateX(float x);
  void rotateY(float y);
  void rotateZ(float z);
private:
  std::vector<MatrixFloat> nodes;
  std::vector<std::unique_ptr<BaseSurface>>edges;
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
private:
  MatrixFloat rotationAndScaleResult = MatrixFloat(0.0f,0.0f,0.0f);
  MatrixFloat zComparisionMatrix = MatrixFloat(0.0f,0.0f,0.0f);
};

#endif