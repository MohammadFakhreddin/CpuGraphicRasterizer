#ifndef Shape3d_class
#define Shape3d_class

#include <vector>
#include <iostream>
#include <memory>
#include <functional>
#include <thread>

#include "../data_types/MatrixTemplate.h"
#include "../texture/Texture.h"
#include "../data_types/VectorTemplate.h"
#include "./../camera/Camera.h"
#include "../surface/Surface.h"
#include "../utils/thread_pool/ThreadPool.h"

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

  void updateSurfaces(
    const unsigned int& threadNumber
  );

  void updateNodeAndNormals(
    const unsigned int& threadNumber
  );

  std::vector<MatrixFloat> nodes;
  
  std::vector<std::unique_ptr<Surface>>surfaces;

  std::vector<MatrixFloat> normals;
  
  std::vector<MatrixFloat> worldPoints;

  std::vector<MatrixFloat> worldNormals;

  MatrixFloat transformMatrix;

  MatrixFloat rotationDegreeMatrix;
 
  MatrixFloat rotationXYZMatrix;

  MatrixFloat scaleValueMatrix;
  
  std::vector<MatrixFloat> zScaleMatrix;

  unsigned int numberOfSupportedThreads;

  unsigned int threadNumberIndex;

  std::vector<MatrixFloat> currentWorldPoint;

  std::vector<MatrixFloat> currentWorldNormal;

  std::vector<float> zLocation;

  std::vector<float> scaleValue;

  std::vector<unsigned int> nodeIndex;

  std::vector<unsigned int> surfaceIndex;

  std::vector<unsigned int> normalIndex;

  Camera* cameraInstance;
  
  std::vector<std::unique_ptr<Light>>* lightSources;

  std::function<void(const unsigned int&)> updateNodeAndNormalsRefrence = std::bind(&Shape3d::updateNodeAndNormals, this, std::placeholders::_1);

  std::function<void(const unsigned int&)> updatSurfacesRefrence = std::bind(&Shape3d::updateSurfaces, this, std::placeholders::_1);

  ThreadPool& threadPool;

};

#endif