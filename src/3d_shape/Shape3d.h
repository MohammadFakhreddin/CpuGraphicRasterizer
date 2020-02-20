#pragma once

#include "../date_types/MatrixTemplate.h"
#include <vector>

class Edge
{
private:
  int first;
  int second;
public:
  Edge(int first,int second);
  int getByIndex(int index);
  int getFirst();
  int getSecond();
};

class Shape3d
{
private:
  std::vector<MatrixFloat> nodes;
  std::vector<Edge>edges;
  std::vector<MatrixFloat> worldPoints;
  MatrixFloat transformMatrix;
  MatrixFloat rotationDegreeMatrix;
  MatrixFloat rotationValueXMatrix;
  MatrixFloat rotationValueYMatrix;
  MatrixFloat rotationValueZMatrix;
  MatrixFloat scaleValueMatrix;
  MatrixFloat zScaleMatrix;
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
  Shape3d(std::vector<MatrixFloat> nodes,std::vector<Edge> edges);
  Shape3d(
    std::vector<MatrixFloat> nodes,
    std::vector<Edge> edges,
    float initialTransformX,
    float initialTransformY,
    float initialTransformZ
  );
  Shape3d(
    std::vector<MatrixFloat> nodes,
    std::vector<Edge> edges,
    float transformX,
    float transformY,
    float transformZ,
    float rotationDegreeX,
    float rotationDegreeY,
    float rotationDegreeZ,
    float scaleValue
  );
  bool checkForNodesValidation();
  void update();
  void render();
  void transformX(float x);
  void transformY(float y);
  void transformZ(float z);
  void scale(float value);
  void rotateX(float x);
  void rotateY(float y);
  void rotateZ(float z);
};

