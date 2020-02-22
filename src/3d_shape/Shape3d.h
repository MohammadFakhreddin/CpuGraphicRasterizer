#pragma once

#include "../data_types/MatrixTemplate.h"
#include <vector>
#include <unordered_map>

class ColorEdge
{
private:
  int edge1;
  int edge2;
  int edge3;
  bool fillSpaceBetween;
  float red;
  float green;
  float blue;
public:
  ColorEdge(
    int edge1,
    int edge2,
    int edge3,
    bool fillSpaceBetween,
    float red,
    float green,
    float blue
  );
  int getEdgeByIndex(int index);
  float getColorByIndex(int index);
  float getRed();
  float getGreen();
  float getBlue();
  int getEdge1();
  int getEdge2();
  int getEdge3();
  bool getFillSpaceBetween();
};

class Shape3d
{
private:
  typedef std::pair<int,int> pixelPair;
  struct DrawPixel{
    int x;
    int y;
    float zValue;
    float red;
    float green;
    float blue;
  };
  struct hash_pair { 
    size_t operator()(const pixelPair& p) const
    { 
        auto hash1 = p.first; 
        auto hash2 = p.second; 
        return hash1 ^ hash2; 
    } 
  }; 
  std::vector<MatrixFloat> nodes;
  std::vector<ColorEdge>edges;
  std::vector<MatrixFloat> worldPoints;
  MatrixFloat transformMatrix;
  MatrixFloat rotationDegreeMatrix;
  MatrixFloat rotationValueXMatrix;
  MatrixFloat rotationValueYMatrix;
  MatrixFloat rotationValueZMatrix;
  MatrixFloat scaleValueMatrix;
  MatrixFloat zScaleMatrix;
  std::unordered_map<pixelPair,Shape3d::DrawPixel,hash_pair> pixelMap;
  void drawLineBetweenPoints(
    MatrixFloat point1,
    MatrixFloat point2,
    float red,
    float green,
    float blue
  );
  void putPixelInMapIfPossible(
    int x,
    int y,
    float zValue,
    float red,
    float green,
    float blue
  );
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
    std::vector<ColorEdge> colorEdges
  );
  Shape3d(
    std::vector<MatrixFloat> nodes,
    std::vector<ColorEdge> colorEdges,
    float initialTransformX,
    float initialTransformY,
    float initialTransformZ
  );
  Shape3d(
    std::vector<MatrixFloat> nodes,
    std::vector<ColorEdge> colorEdges,
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

