#ifndef PipeLine_Class
#define PipeLine_Class

#include <functional>

#include "../shaders/ambient_light/AmbientLight.h"
#include "../camera/Camera.h"
#include "../shaders/directional_light/DirectionalLight.h"
#include "../shaders/point_light/PointLight.h"
#include "../3d/shape/Shape3d.h"
#include "../utils/thread_pool/ThreadPool.h"

class PipeLine {

public:

  PipeLine(
    Camera& camera
  );

  PipeLine(
    Camera& camera,
    AmbientLight* ambientLight,
    std::vector<DirectionalLight*>& directionalLights,
    std::vector<PointLight*>& pointLights,
    std::vector<Shape3d*>& shapes
  );

  void updateShapeNodes(
    const unsigned int& threadNumber,
    Shape3d* shape
  );

  void updateShapeNormals(
    const unsigned int& threadNumber,
    Shape3d* shape
  );

  void updateShapeSurfaces(
    const unsigned int& threadNumber,
    Shape3d* shape
  );

  void updateSurface(
    Shape3d* shape3d,
    Surface* surface
  );

  void assembleTriangles(
    Shape3d* shape3d,
    Surface* surface
  );

  void assembleLines(
    Surface* surface,
    const MatrixFloat& paramTriangleStart,
    const MatrixFloat& paramTriangleEnd,
    const MatrixFloat& paramTextureStart,
    const MatrixFloat& paramTextureEnd,
    const MatrixFloat& paramLightColorStart,
    const MatrixFloat& paramLightColorEnd,
    const MatrixFloat& paramNormalStart,
    const MatrixFloat& paramNormalEnd
  );

  void computeLightIntensityForPoint(
    const MatrixFloat& worldPoint,
    const MatrixFloat& worldNormal,
    const float& specularIntensity,
    MatrixFloat& colorOutputPlaceholder,
    MatrixFloat& cameraVectorPlaceholder,
    MatrixFloat& cameraVectorHatPlaceholder,
    MatrixFloat& lightVectorPlaceholder,
    MatrixFloat& lightVectorHatPlaceholder,
    MatrixFloat& lightReflectionVectorPlaceholder,
    MatrixFloat& lightReflectionVectorHatPlaceholder,
    MatrixFloat& output
  );

  void updateShapeNodesConversionMethod(const unsigned int& threadNumber, void* shape);

  std::function<void(const unsigned int&, void*)>  updateShapeNodesReference = std::bind(
    &PipeLine::updateShapeNodesConversionMethod,
    this,
    std::placeholders::_1,
    std::placeholders::_2
  );

  void updateShapeNormalsConversionMethod(const unsigned int& threadNumber, void* shape);

  std::function<void(const unsigned int&, void*)>  updateShapeNormalsReference = std::bind(
    &PipeLine::updateShapeNormalsConversionMethod,
    this,
    std::placeholders::_1,
    std::placeholders::_2
  );

  void updateShapeSurfacesConversionMethod(const unsigned int& threadNumber, void* shape);

  std::function<void(const unsigned int&, void*)>  updateShapeSurfacesReference = std::bind(
    &PipeLine::updateShapeSurfacesConversionMethod,
    this,
    std::placeholders::_1,
    std::placeholders::_2
  );

  void assignAmbientLight(AmbientLight* ambientLight);

  void assignDirectionalLight(std::vector<DirectionalLight*>& directionalLights);

  void assignPointLight(std::vector<PointLight*>& pointLights);

  void assignShapes(std::vector<Shape3d*>& shapes);

  void assignDirectionalLight(const DirectionalLight* directionalLights);

  void assignPointLight(const PointLight* pointLights);

  void assignShapes(const Shape3d* shapes);

  void update(double deltaTime);

private:

  ThreadPool& threadPool;

  std::vector<Shape3d*> shapes;

  bool isPointLightsArrayEmpty = true;

  bool isShapesArrayEmpty = true; 

  bool isDirectionalLightsArrayEmpty = false;

  Camera& camera;
    
  AmbientLight* ambientLight;
    
  std::vector<DirectionalLight*> directionalLights;
    
  std::vector<PointLight*> pointLights;
    
  const unsigned int& numberOfSupportedThreads;

};

#endif // !Light_Class
