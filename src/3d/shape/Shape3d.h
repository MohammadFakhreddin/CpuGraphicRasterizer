#ifndef Shape3d_class
#define Shape3d_class

#include <vector>
#include <memory>

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
    std::vector<std::unique_ptr<Surface>>& surfaces,
    std::vector<MatrixFloat>& normals,
    const float specularIntensity
  );

  Shape3d(
    std::vector<MatrixFloat>& nodes,
    std::vector<std::unique_ptr<Surface>>& surfaces,
    std::vector<MatrixFloat>& normals,
    const float specularIntensity,
    float initialTransformX,
    float initialTransformY,
    float initialTransformZ
  );

  Shape3d(
    std::vector<MatrixFloat>& nodes,
    std::vector<std::unique_ptr<Surface>>& surfaces,
    std::vector<MatrixFloat>& normals,
    const float specularIntensity,
    float transformX,
    float transformY,
    float transformZ,
    float rotationDegreeX,
    float rotationDegreeY,
    float rotationDegreeZ,
    float scaleValue
  );

  void setSpecularIntensity(const float& value);

  void transformX(float x);

  void transformY(float y);

  void transformZ(float z);

  void scale(float value);

  void rotateXYZ(const float& x, const float& y, const float& z);

  bool checkDataValidation();

  std::vector<MatrixFloat> nodes;
  
  std::vector<std::unique_ptr<Surface>>surfaces;

  std::vector<MatrixFloat> normals;
  
  std::vector<MatrixFloat> worldPoints;

  std::vector<MatrixFloat> worldNormals;

  MatrixFloat transformMatrix;

  MatrixFloat rotationDegreeMatrix;
 
  MatrixFloat rotationXYZMatrix;

  MatrixFloat scaleValueMatrix;

  float specularIntensity;
  
};

#endif