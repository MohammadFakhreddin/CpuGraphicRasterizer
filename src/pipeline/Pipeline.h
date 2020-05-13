#ifndef PipeLine_Class
#define PipeLine_Class

//#define DEBUG_PIPELINE

#include <functional>

#include "../shaders/ambient_light/AmbientLight.h"
#include "../camera/Camera.h"
#include "../shaders/directional_light/DirectionalLight.h"
#include "../shaders/point_light/PointLight.h"
#include "../3d/shape/Shape3d.h"
#include "../utils/thread_pool/ThreadPool.h"
#include "../texture/ImageTexture/ImageTexture.h"

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

  void assignAmbientLight(AmbientLight* ambientLight);

  void assignDirectionalLight(std::vector<DirectionalLight*>& directionalLights);

  void assignPointLight(std::vector<PointLight*>& pointLights);

  void assignShape(std::vector<Shape3d*>& shapes);

  void assignDirectionalLight(DirectionalLight* directionalLights);

  void assignPointLight(PointLight* pointLights);

  void assignShape(Shape3d* shapes);

  void update(double deltaTime);

  void updateSurface(
    Shape3d* shape3d,
    Surface* surface
  );

private:

  void updateShapeNodes(
    const unsigned int& threadNumber,
    Shape3d* shape
  );

  void updateShapeNormals(
    const unsigned int& threadNumber,
    Shape3d* shape
  );

  void assembleTriangles(
    Shape3d* shape3d,
    Surface* surface
  );

  void assembleLines(
    Shape3d* shape,
    Surface* surface,
    const Matrix4X1Float& paramTriangleStart,
    const Matrix4X1Float& paramTriangleEnd,
    const Matrix2X1Float& paramTextureStart,
    const Matrix2X1Float& paramTextureEnd,
    const Matrix4X1Float& paramLightColorStart,
    const Matrix4X1Float& paramLightColorEnd,
    const Matrix4X1Float& paramNormalStart,
    const Matrix4X1Float& paramNormalEnd
  );

  void computeLightIntensityForPoint(
    const Matrix4X1Float& worldPoint,
    const Matrix4X1Float& worldNormal,
    const float& specularIntensity,
    Matrix4X1Float& colorOutputPlaceholder,
    Matrix4X1Float& cameraVectorPlaceholder,
    Matrix4X1Float& cameraVectorHatPlaceholder,
    Matrix4X1Float& lightVectorPlaceholder,
    Matrix4X1Float& lightVectorHatPlaceholder,
    Matrix4X1Float& lightReflectionVectorPlaceholder,
    Matrix4X1Float& lightReflectionVectorHatPlaceholder,
    Matrix4X1Float& output
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

  void updateShapeSurfaces(
    const unsigned int& threadNumber,
    Shape3d* shape
  );

  std::function<void(const unsigned int&, void*)>  updateShapeSurfacesReference = std::bind(
    &PipeLine::updateShapeSurfacesConversionMethod,
    this,
    std::placeholders::_1,
    std::placeholders::_2
  );

  ThreadPool& threadPool;

  std::vector<Shape3d*> shapes;

  Camera& camera;
    
  AmbientLight* ambientLight = nullptr;
    
  std::vector<DirectionalLight*> directionalLights;
    
  std::vector<PointLight*> pointLights;
    
  const unsigned int& numberOfSupportedThreads;

};

#endif // !Light_Class
