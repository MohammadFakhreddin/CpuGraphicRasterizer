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

  enum class NormalType {
    smooth,
    sharp,
    fileDefault
  };

  static std::vector<MatrixFloat> generateNormals(
    std::vector<std::unique_ptr<Surface>>& surfaceList,
    std::vector<MatrixFloat>& nodes,
    NormalType normalType
  );

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

  void rotateXYZ(const float& x, const float& y, const float& z);

private:

  bool checkDataValidation();

  std::vector<MatrixFloat> nodes;
  
  std::vector<std::unique_ptr<Surface>>surfaces;

  std::vector<MatrixFloat> normals;
  
  std::vector<MatrixFloat> worldPoints;

  std::vector<MatrixFloat> worldNormals;

  MatrixFloat transformMatrix;

  MatrixFloat rotationDegreeMatrix;
  
  /*MatrixFloat rotationValueXMatrix;

  MatrixFloat rotationValueYMatrix;
  
  MatrixFloat rotationValueZMatrix;*/

  MatrixFloat rotationXYZMatrix;

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