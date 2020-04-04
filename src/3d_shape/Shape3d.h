#ifndef Shape3d_class
#define Shape3d_class

#include <vector>
#include <iostream>
#include <memory>

#include "../data_types/MatrixTemplate.h"
#include "../texture/Texture.h"
#include "../data_types/VectorTemplate.h"
#include "./../camera/Camera.h"
#include "../surface/Surface.h"

class Shape3d
{

public:

  //TODO Rename node and edge to vertices and indices
  Shape3d(
    std::vector<MatrixFloat>& nodes,
    std::vector<std::unique_ptr<Surface>>& surfaces,
    std::vector<MatrixFloat>& normals
  );

  Shape3d(
    std::vector<MatrixFloat>& nodes,
    std::vector<std::unique_ptr<Surface>>&  surfaces,
    std::vector<MatrixFloat>& normals,
    float initialTransformX,
    float initialTransformY,
    float initialTransformZ
  );

  Shape3d(
    std::vector<MatrixFloat>& nodes,
    std::vector<std::unique_ptr<Surface>>& surfaces,
    std::vector<MatrixFloat>& normals,
    float transformX,
    float transformY,
    float transformZ,
    float rotationDegreeX,
    float rotationDegreeY,
    float rotationDegreeZ,
    float scaleValue
  );

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
  
  std::vector<std::unique_ptr<Surface>>surfaces;

  std::vector<MatrixFloat> normals;
  
  std::vector<MatrixFloat> worldPoints;

  std::vector<MatrixFloat> worldNormals;

  MatrixFloat transformMatrix;

  MatrixFloat rotationDegreeMatrix;
  
  MatrixFloat rotationValueXMatrix;

  MatrixFloat rotationValueYMatrix;
  
  MatrixFloat rotationValueZMatrix;

  MatrixFloat scaleValueMatrix;
  
  MatrixFloat zScaleMatrix;

  /*Temporary variables*/

  MatrixFloat currentWorldPoint = MatrixFloat(3,1,0.0f);

  MatrixFloat currentWorldNormal = MatrixFloat(3,1,0.0f);

  unsigned int i = 0;

  float zLocation = 0;

  float scaleValue = 0;

};

#endif