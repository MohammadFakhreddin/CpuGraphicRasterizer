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

class Shape3d
{

public:

  //TODO Rename node and edge to vertices and indices
  Shape3d(
    std::vector<MatrixFloat>& nodes,
    std::vector<std::unique_ptr<BaseSurface>>& edges
  );

  Shape3d(
    std::vector<MatrixFloat>& nodes,
    std::vector<std::unique_ptr<BaseSurface>>&  edges,
    float initialTransformX,
    float initialTransformY,
    float initialTransformZ
  );

  Shape3d(
    std::vector<MatrixFloat>& nodes,
    std::vector<std::unique_ptr<BaseSurface>>& edges,
    float transformX,
    float transformY,
    float transformZ,
    float rotationDegreeX,
    float rotationDegreeY,
    float rotationDegreeZ,
    float scaleValue
  );

  bool checkForNodesValidation();

  bool areAllEdgesDataValid();

  void update(
    double deltaTime,
    Camera& cameraInstance,
    std::vector<std::unique_ptr<Light>>& lightSources
  );

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

  MatrixFloat transformResultMatrix = MatrixFloat(3,1,0.0f);

};

#endif